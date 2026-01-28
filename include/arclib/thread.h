// =======================================================================================
// 
// A Really Cool Library - arclib
// Copyright (C) 2026 Patrick Chen <pchen.2215@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// =======================================================================================

#ifndef __ARCLIB_THREAD_H
#define __ARCLIB_THREAD_H

#include "int.h"
#include "concepts.h"
#include <algorithm>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace arcl {

// =======================================================================================
// thread_pool CLASS DEFINITION
// =======================================================================================

    class thread_pool {
    public: // ===================================================================
        // CONSTRUCTION AND DESTRUCTION

        // Default constructor
        thread_pool():
            _num_workers(0), _num_jobs(0), _workers(nullptr), _running(false) { }

        // Delete copy constructor
        thread_pool(const thread_pool&) = delete;

        // Delete move constructor
        thread_pool(thread_pool&&) = delete;

        // Destructor
        ~thread_pool() {
            terminate();
        }

        // =======================================================================
        // CONCURRENCY FUNCTIONALITY

        /// <summary>
        /// Starts running the thread pool. Has no effect if the thread pool is
        /// already running. This function should only be called once in the
        /// lifetime of the thread pool.
        /// </summary>
        /// <param name="num_workers">The number of worker threads that the pool
        /// should use.</param>
        void start(const uint32 num_workers) {
            std::unique_lock<std::mutex> lock(_mtx);

            // Spin off worker threads
            _num_workers = num_workers;
            _workers = new std::thread[_num_workers];
            for (uint32 i = 0; i < num_workers; i++) {
                _workers[i] = std::thread(&thread_pool::work_loop, this);
            }
            _running = true;
        }

        /// <summary>
        /// Finishes running all queued jobs before shutting down the thread pool.
        /// </summary>
        void finish() {
            wait();
            terminate();
        }

        /// <summary>
        /// Finishes running all currently running jobs before shutting down the
        /// thread pool.
        /// </summary>
        void terminate() {
            { // Disable running flag
                std::unique_lock<std::mutex> lock(_mtx);
                _running = false;
            }

            // Notify all workers and wait for current jobs to finish
            _cv_workers.notify_all();
            for (uint32 i = 0; i < _num_workers; i++) { _workers[i].join(); }

            // Free worker threads
            delete[] _workers;
            _workers = nullptr;
        }

        /// <summary>
        /// Adds a function call to the end of the job queue. This function should
        /// only be called from the main thread.
        /// </summary>
        /// <param name="func">The function.</param>
        /// <param name="...args">The arguments to pass to the function.</param>
        template <typename Func, typename... Args>
        void enqueue_job(Func&& func, Args&&... args) {
            // Wrap function call in lambda
            auto job = [func = std::forward<Func>(func),
                        ...args = std::forward<Args>(args)]() mutable {
                std::invoke(std::move(func), std::move(args)...);
            };

            { // Add job to queue
                std::unique_lock<std::mutex> lock(_mtx);
                _job_queue.emplace(std::move(job));
                _num_jobs++;
            }
            _cv_workers.notify_one();
        }

        /// <summary>
        /// Sleeps the calling thread until all jobs in the thread pool have been
        /// executed. This function should only be called from the main thread.
        /// </summary>
        void wait() {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv_wait.wait(lock, [&]() { return _num_jobs == 0; });
        }

        // =======================================================================
        // UTILITY FUNCTIONS

        /// <summary>
        /// Returns the number of worker threads currently in use by the thread
        /// pool.
        /// </summary>
        /// <returns>The number of worker threads in use.</returns>
        uint32 num_workers() const {
            return _num_workers;
        }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Delete copy assignment operator
        thread_pool& operator=(const thread_pool&) = delete;

        // Delete move assignment operator
        thread_pool& operator=(thread_pool&&) = delete;

    private: // ==================================================================
        // PRIVATE MEMBERS

        // Counters
        std::atomic<uint32> _num_workers;
        uint32 _num_jobs;

        // Workers
        std::thread* _workers;
        std::queue<std::function<void()>> _job_queue;

        // Synchronization
        std::mutex _mtx;
        std::condition_variable _cv_workers;
        std::condition_variable _cv_wait;
        bool _running;

        // =======================================================================
        // WORKER THREAD EXECUTION LOOP

        void work_loop() {
            std::unique_lock<std::mutex> lock(_mtx);
            while (true) {
                // Wait for pool termination or avaliable job
                _cv_workers.wait(lock, [&]() {
                    return !_running || _num_jobs != 0;
                });

                // Only execute jobs if pool is still running
                if (_running) {
                    assert(!_job_queue.empty());

                    // Fetch and run job from queue
                    std::function<void()> job = std::move(_job_queue.front());
                    _job_queue.pop();
                    lock.unlock();
                    job();

                    // Notify wait() that a job finished running
                    lock.lock();
                    _num_jobs--;
                    _cv_wait.notify_one();
                } else break;
            }
        }
    };

// =======================================================================================
// MULTITHREADING FUNCTIONALITY
// =======================================================================================

    /// <summary>
    /// Runs a function on each element in a sequential range in parallel. This function
    /// will halt the calling thread until all processing is complete.
    /// </summary>
    /// <param name="pool">The thread pool to use.</param>
    /// <param name="begin">The beginning of the range, inclusive. Must be a random-access
    /// iterator.</param>
    /// <param name="end">The end of the range, exclusive. Must be a random-access
    /// iterator.</param>
    /// <param name="func">The function to run on each element in the range.</param>
    /// <param name="schedule_factor">Multiplied by the number of worker threads to
    /// determine the number of chunks to divide the range into. Lower values may be
    /// preferred when the processing cost of each element in the range is similar, while
    /// higher values may be preferred if the processing cost of each element is more
    /// variable. The range may be divided into fewer chunks than this factor may suggest
    /// to guarantee the minimum chunk size. By default, the processor will attempt to
    /// split the range into only as many chunks as the thread pool has worker
    /// threads.</param>
    /// <param name="min_chunk_size">The minimum number of elements that will be processed
    /// as a chunk by a thread. By default, chunks can be as small as a single element,
    /// but larger minimum sizes may be preferred if the range is small.</param>
    template <std::random_access_iterator It, typename Func>
        requires std::invocable<Func, std::iter_reference_t<It>>
    void run_in_parallel(thread_pool& pool, It begin, It end, Func func,
                         const uint64 schedule_factor, const uint64 min_chunk_size) {
        assert(schedule_factor != 0 && min_chunk_size != 0);

        // Calculate chunk size
        const uint64 range_size = std::distance(begin, end);
        if (range_size == 0) { return; }
        uint64 num_chunks = schedule_factor * pool.num_workers();
        uint64 chunk_size = range_size / num_chunks;

        // Adjust to ensure minimum chunk size
        if (chunk_size < min_chunk_size) {
            chunk_size = min_chunk_size;
            num_chunks = range_size / chunk_size;
        }

        // Run on current thread if range is small
        if (range_size <= chunk_size) {
            while (begin != end) { std::invoke(func, *begin++); }
            return;
        }

        // Split range into chunks
        uint64 idx_begin = 0;
        uint64 idx_end = chunk_size;
        while (idx_begin != range_size) {
            // Create and enqueue job
            It it = begin + idx_begin;
            It it_end = begin + idx_end;
            pool.enqueue_job([it, it_end, func]() mutable {
                while (it != it_end) { std::invoke(func, *it++); }
            });

            // Move to the range of the next chunk
            idx_begin = idx_end;
            idx_end = std::min(idx_end + chunk_size, range_size);
        }

        // Wait for jobs to finish
        pool.wait();
    }

    /// <summary>
    /// Runs a function on each element in a sequential range in parallel. This function
    /// will halt the calling thread until all processing is complete.
    /// </summary>
    /// <param name="pool">The thread pool to use.</param>
    /// <param name="begin">The beginning of the range, inclusive. Must be a random-access
    /// iterator.</param>
    /// <param name="end">The end of the range, exclusive. Must be a random-access
    /// iterator.</param>
    /// <param name="func">The function to run on each element in the range.</param>
    /// <param name="schedule_factor">Multiplied by the number of worker threads to
    /// determine the number of chunks to divide the range into. Lower values may be
    /// preferred when the processing cost of each element in the range is similar, while
    /// higher values may be preferred if the processing cost of each element is more
    /// variable. The range may be divided into fewer chunks than this factor may suggest
    /// to guarantee the minimum chunk size. By default, the processor will attempt to
    /// split the range into only as many chunks as the thread pool has worker
    /// threads.</param>
    /// <param name="min_chunk_size">The minimum number of elements that will be processed
    /// as a chunk by a thread. By default, chunks can be as small as a single element,
    /// but larger minimum sizes may be preferred if the range is small.</param>
    template <std::random_access_iterator It, typename Func>
        requires std::invocable<Func, std::iter_reference_t<It>>
    void run_in_parallel(thread_pool& pool, It begin, It end, Func func) {
        run_in_parallel(pool, begin, end, func, 1, 1);
    }

}

// =======================================================================================
#endif
