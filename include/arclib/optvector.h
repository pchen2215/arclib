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

#ifndef __ARCLIB_OPTVECTOR_H
#define __ARCLIB_OPTVECTOR_H

#include "int.h"
#include "concepts.h"
#include "memory.h"
#include <cassert>
#include <utility>

namespace arcl {

// =======================================================================================
// optvector CLASS DEFINITION
// =======================================================================================

    template <typename T>
    class optvector {
        static constexpr double GROW_FACTOR = 1.5;
        static constexpr uint64 INITIAL_SIZE = 8;
    public:  // ==================================================================
        // MEMBER TYPES

        struct optval {
            T& val;
            const bool has_val;
        };

        struct const_optval {
            const T& val;
            const bool has_val;
        };

        template <bool Const>
        class basic_iterator;

        using iterator = basic_iterator<false>;
        using const_iterator = basic_iterator<true>;

        // =======================================================================
        // CONSTRUCTION AND DESTRUCTION

        // Default constructor
        optvector():
            _data(nullptr), _mask(nullptr), _alloc(0), _size(0) { }

        // Copy constructor
        optvector(const optvector& ov):
            _data(memalloc<T>(ov._alloc)),
            _mask(memalloc<uint64>(ov._alloc)),
            _alloc(ov._alloc), _size(ov._size) {
            if (ov.empty()) { return; }

            // Copy elements
            for (uint64 i = 0; i < _size; i++) {
                if (ov[i].has_val) { new (_data + i) T(ov[i].val); }
            }

            // Copy bitmask
            memcopy(_mask, ov._mask, masklen(_size));
        }

        // Move constructor
        optvector(optvector&& ov):
            _data(ov._data), _mask(ov._mask), _alloc(ov._alloc), _size(ov._size) {
            ov._data = nullptr;
            ov._mask = nullptr;
            ov._alloc = ov._size = 0;
        }

        // Destructor
        ~optvector() {
            dealloc();
        }

        // =======================================================================
        // ELEMENT ACCESS

        /// <summary>
        /// Accesses the optval at the specified index.
        /// </summary>
        /// <param name="idx">The index.</param>
        /// <returns>The optval at the specified index.</returns>
        optval at(const uint64 idx) {
            assert(idx < _size);
            return { _data[idx], maskval(idx) };
        }

        /// <summary>
        /// Accesses the optval at the specified index.
        /// </summary>
        /// <param name="idx">The index.</param>
        /// <returns>The optval at the specified index.</returns>
        const_optval at(const uint64 idx) const {
            assert(idx < _size);
            return { _data[idx], maskval(idx) };
        }

        // =======================================================================
        // ITERATORS

        /// <summary>
        /// Returns an iterator to the beginning of the container.
        /// </summary>
        /// <returns>An iterator to the beginning of the container.</returns>
        iterator begin() {
            return iterator(*this, 0);
        }

        /// <summary>
        /// Returns an iterator to the beginning of the container.
        /// </summary>
        /// <returns>An iterator to the beginning of the container.</returns>
        const_iterator begin() const {
            return cbegin();
        }

        /// <summary>
        /// Returns an iterator to the beginning of the container.
        /// </summary>
        /// <returns>An iterator to the beginning of the container.</returns>
        const_iterator cbegin() const {
            return const_iterator(*this, 0);
        }

        /// <summary>
        /// Returns an iterator to the end of the container.
        /// </summary>
        /// <returns>An iterator to the end of the container.</returns>
        iterator end() {
            return iterator(*this, _size);
        }

        /// <summary>
        /// Returns an iterator to the end of the container.
        /// </summary>
        /// <returns>An iterator to the end of the container.</returns>
        const_iterator end() const {
            return cend();
        }

        /// <summary>
        /// Returns an iterator to the end of the container.
        /// </summary>
        /// <returns>An iterator to the end of the container.</returns>
        const_iterator cend() const {
            return const_iterator(*this, _size);
        }

        // =======================================================================
        // CAPACITY

        /// <summary>
        /// Checks if the container is empty.
        /// </summary>
        /// <returns>true if the container contains no elements, else
        /// false.</returns>
        bool empty() const {
            return _size == 0;
        }

        /// <summary>
        /// Returns the number of elements in the container.
        /// </summary>
        /// <returns>The number of elements.</returns>
        uint64 size() const {
            return _size;
        }

        /// <summary>
        /// Reserves sufficient storage to hold the specified number of elements
        /// without reallocation. If the requested capacity is less than or equal
        /// to the current capacity, this function does nothing.
        /// </summary>
        /// <param name="cap">The requested capacity.</param>
        void reserve(const uint64 cap) {
            if (_alloc < cap) { realloc(cap); }
        }

        /// <summary>
        /// Returns the number of elements that can fit in currently allocated
        /// memory.
        /// </summary>
        uint64 capacity() const {
            return _alloc;
        }

        // =======================================================================
        // MODIFIERS

        /// <summary>
        /// Destructs and removes all elements from the container.
        /// </summary>
        void clear() {
            for (uint64 i = 0; i < _size; i++) {
                if (maskval(i)) { _data[i].~T(); }
                // No need for maskset() since mask values are set by push/emplace
            }
            _size = 0;
        }

        /// <summary>
        /// Inserts an element into the container at the specified position. If an
        /// optval containing a value currently exists at that position, it will
        /// be destructed and overwritten.
        /// </summary>
        /// <param name="pos">An iterator to the position of insertion.</param>
        /// <param name="val">The value to insert.</param>
        void insert(const const_iterator pos, const T& val) {
            // Special case
            if (pos == end()) {
                push_back(val);
                return;
            }

            // General case
            assert(pos._idx < _size);
            const uint64 idx = pos._idx;
            if (maskval(idx)) { _data[idx].~T(); }
            new (_data + idx) T(val);
            maskset(idx, true);
        }

        /// <summary>
        /// Inserts an element into the container at the specified position. If an
        /// optval containing a value currently exists at that position, it will
        /// be destructed and overwritten.
        /// </summary>
        /// <param name="pos">An iterator to the position of insertion.</param>
        /// <param name="val">The value to insert.</param>
        void insert(const const_iterator pos, T&& val) {
            // Special case
            if (pos == end()) {
                push_back(std::move(val));
                return;
            }

            // General case
            assert(pos._idx < _size);
            const uint64 idx = pos._idx;
            if (maskval(idx)) { _data[idx].~T(); }
            new (_data + idx) T(std::move(val));
            maskset(idx, true);
        }

        /// <summary>
        /// Constructs an element in-place at the specified position. If an optval
        /// containing a value currently exists at that position, it will be
        /// destructed and overwritten.
        /// </summary>
        /// <param name="pos">An iterator to the position of emplacement.</param>
        /// <param name="args">The constructor arguments.</param>
        template <typename... Args>
        void emplace(const const_iterator pos, Args&&... args) {
            // Special case
            if (pos == end()) {
                emplace_back(std::forward<Args>(args)...);
                return;
            }

            // General case
            assert(pos._idx < _size);
            const uint64 idx = pos._idx;
            if (maskval(idx)) { _data[idx].~T(); }
            new (_data + idx) T(std::forward<Args>(args)...);
            maskset(idx, true);
        }

        /// <summary>
        /// Erases the element contained in an optval from the container, if it
        /// exists. The destructor will be called on the element and the vacant
        /// optval will remain in the container at the position without reordering
        /// any other element.
        /// </summary>
        /// <param name="pos">An iterator to the element to erase.</param>
        void erase(const const_iterator pos) {
            destruct_elem(pos._idx);
        }

        /// <summary>
        /// Erases a range of elements contained in optvals from the container, if
        /// they exist. The destructor will be called on the elements and the
        /// vacant optvals will remain in the container at their position without
        /// reordering any other elements.
        /// </summary>
        /// <param name="start">An iterator to the first element to erase.</param>
        /// <param name="stop">An iterator to the element to stop erasing
        /// at.</param>
        void erase(const_iterator start, const const_iterator stop) {
            while (start != stop) { erase(start++); }
        }

        /// <summary>
        /// Adds an element to the end of the container.
        /// </summary>
        /// <param name="val">The element to add.</param>
        void push_back(const T& val) {
            // Reallocate if necessary
            if (_size == _alloc) {
                uint64 new_alloc = _alloc * GROW_FACTOR;
                if (new_alloc == 0) { new_alloc = INITIAL_SIZE; }
                realloc(new_alloc);
            }

            // Insert at final position
            _size++;
            new (_data + _size - 1) T(val);
            maskset(_size - 1, true);
        }

        /// <summary>
        /// Adds an element to the end of the container.
        /// </summary>
        /// <param name="val">The element to add.</param>
        void push_back(T&& val) {
            // Reallocate if necessary
            if (_size == _alloc) {
                uint64 new_alloc = _alloc * GROW_FACTOR;
                if (new_alloc == 0) { new_alloc = INITIAL_SIZE; }
                realloc(new_alloc);
            }

            // Insert at final position
            _size++;
            new (_data + _size - 1) T(std::move(val));
            maskset(_size - 1, true);
        }

        /// <summary>
        /// Constructs an element in-place at the end of the container.
        /// </summary>
        /// <param name="args">The constructor arguments.</param>
        template <typename... Args>
        void emplace_back(Args&&... args) {
            // Reallocate if necessary
            if (_size == _alloc) {
                uint64 new_alloc = _alloc * GROW_FACTOR;
                if (new_alloc == 0) { new_alloc = INITIAL_SIZE; }
                realloc(new_alloc);
            }

            // Emplace at final position
            _size++;
            new (_data + _size - 1) T(std::forward<Args>(args)...);
            maskset(_size - 1, true);
        }

        /// <summary>
        /// Removes the last optval from the container.
        /// </summary>
        void pop_back() {
            destruct_elem(_size - 1);
            _size--;
        }
        
        /* TODO: resize() functions which allows resizing of container
                 new values could be null optvals or initialized to something */

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Copy assignment operator
        optvector& operator=(const optvector& ov) {
            if (&ov == this) { return *this; }
            dealloc();
            if (ov.empty()) { return *this; }
            _alloc = ov._alloc;
            _size = ov._size;

            // Reallocate bitmask
            _mask = memalloc<uint64>(_alloc);
            memcopy(_mask, ov._mask, masklen(_size));

            // Copy elements
            _data = memalloc<T>(_alloc);
            for (uint64 i = 0; i < _size; i++) {
                if (maskval(i)) { new (_data + i) T(ov[i].val); }
            }

            // Return self
            return *this;
        }

        // Move assignment operator
        optvector& operator=(optvector&& ov) {
            if (&ov == this) { return *this; }
            dealloc();

            // Transfer ownership
            _data = ov._data;
            _mask = ov._mask;
            _alloc = ov._alloc;
            _size = ov._size;

            // Reset state of other optvector
            ov._data = nullptr;
            ov._mask = nullptr;
            ov._alloc = ov._size = 0;

            // Return self
            return *this;
        }

        // =======================================================================
        // SUBSCRIPT OPERATOR

        // Subscript operator
        optval operator[](const uint64 idx) {
            return { _data[idx], maskval(idx) };
        }

        // Subscript operator
        const_optval operator[](const uint64 idx) const {
            return { _data[idx], maskval(idx) };
        }

    private: // ==================================================================
        // ALLOCATION MANAGEMENT

        T* _data;
        uint64* _mask;
        uint64 _alloc;

        /// <summary>
        /// Calls the destructor on all extant elements before freeing all memory
        /// managed by the container.
        /// </summary>
        void dealloc() {
            for (uint64 i = 0; i < _size; i++) {
                if (maskval(i)) { _data[i].~T(); }
            }
            memfree(_data);
            memfree(_mask);
            _data = nullptr;
            _mask = nullptr;
            _alloc = _size = 0;
        }

        /// <summary>
        /// Reallocates memory to fit the requested number of elements without
        /// another allocation. Uses move operations if possible on extant items
        /// in the currently allocated memory. The new allocation must be equal to
        /// or greater than the current allocation.
        /// </summary>
        /// <param name="new_alloc">The new allocation.</param>
        void realloc(const uint64 new_alloc) {
            assert(new_alloc >= _alloc);

            // Allocate new data array and move elements
            T* new_data = memalloc<T>(new_alloc);
            for (uint64 i = 0; i < _size; i++) {
                if (maskval(i)) {
                    new (new_data + i) T(std::move(_data[i]));
                    _data[i].~T();
                }
            }
            memfree(_data);
            _data = new_data;
            _alloc = new_alloc;

            // Reallocate bitmask
            uint64* new_mask = memalloc<uint64>(masklen(_alloc));
            memcopy(new_mask, _mask, masklen(_size));
            memfree(_mask);
            _mask = new_mask;
        }

        /// <summary>
        /// Returns the minimum number of bytes required to store the specified
        /// number of bits in a uint64 byte array.
        /// </summary>
        /// <returns>The minimum number of bytes required to store the specified
        /// number of bits in a uint64 byte array.</returns>
        uint64 masklen(const uint64 bits) const {
            return sizeof(uint64) * ((bits + 63) / 64);
        }

        // =======================================================================
        // ELEMENT MANAGEMENT

        uint64 _size;

        /// <summary>
        /// Destructs an optval if it has a value and marks the index as vacant.
        /// </summary>
        /// <param name="idx">The index of the element to destruct.</param>
        void destruct_elem(const uint64 idx) {
            assert(idx < _size);
            if (maskval(idx)) {
                _data[idx].~T();
                maskset(idx, false);
            }
        }

        /// <summary>
        /// Checks whether or not the element at the specified index exists.
        /// </summary>
        /// <param name="idx">The index.</param>
        /// <returns>true if the element at the index exists, else
        /// false.</returns>
        bool maskval(const uint64 idx) const {
            assert(idx < _size);
            return (_mask[idx >> (uint64)6] >> (idx & (uint64)63)) & (uint64)1;
        }

        /// <summary>
        /// Sets a bit in the mask.
        /// </summary>
        /// <param name="idx">The index of the bit.</param>
        /// <param name="val">The value to set to.</param>
        void maskset(const uint64 idx, const bool val) {
            assert(idx < _size);
            val ? _mask[idx >> (uint64)6] |= ((uint64)1 << (idx & (uint64)63))
                : _mask[idx >> (uint64)6] &= ~((uint64)1 << (idx & (uint64)63));
        }

    };

// =======================================================================================
// optvector::basic_iterator CLASS DEFINITION
// =======================================================================================

#define OPTVECTOR_IT_STATE_CHECK assert(_idx <= _ov->_size)
#define OPTVECTOR_IT_DEREF_CHECK assert(_idx < _ov->_size)

    template <typename T>
    template <bool Const>
    class optvector<T>::basic_iterator { // ======================================
        // FRIEND DECLARATIONS

        friend class optvector;

        // =======================================================================
        // PRIVATE MEMBER TYPES

        using optvec_t = std::conditional_t<Const, const optvector, optvector>;
        using optval_t = std::conditional_t<Const, const_optval, optval>;

    public: // ===================================================================
        // CONSTRUCTION

        // Default constructor
        basic_iterator():
            _ov(nullptr), _idx(-1) { }

        // Copy constructor
        basic_iterator(const basic_iterator&) = default;

        // Conversion constructor
        basic_iterator(const basic_iterator<false>& it) requires (Const):
            _ov(it._ov), _idx(it._idx) { }

        // Value constructor
        basic_iterator(optvec_t& ov, const uint64 idx):
            _ov(&ov), _idx(idx) {
            OPTVECTOR_IT_STATE_CHECK;
        }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Copy assignment operator
        basic_iterator& operator=(const basic_iterator&) = default;

        // Addition assignment operator
        basic_iterator& operator+=(const uint64 val) {
            _idx += val;
            OPTVECTOR_IT_STATE_CHECK;
            return *this;
        }

        // Subtraction assignment operator
        basic_iterator& operator-=(const uint64 val) {
            _idx -= val;
            OPTVECTOR_IT_STATE_CHECK;
            return *this;
        }

        // Pre-increment operator
        basic_iterator& operator++() {
            _idx++;
            OPTVECTOR_IT_STATE_CHECK;
            return *this;
        }

        // Post-increment operator
        basic_iterator operator++(int) {
            const iterator it = *this;
            ++(*this);
            return it;
        }

        // Pre-decrement operator
        basic_iterator& operator--() {
            _idx--;
            OPTVECTOR_IT_STATE_CHECK;
            return *this;
        }

        // Post-ecrement operator
        basic_iterator operator--(int) {
            const iterator it = *this;
            --(*this);
            return it;
        }

        // =======================================================================
        // ARITHMETIC OPERATORS

        // Addition operator
        friend basic_iterator operator+(basic_iterator it, const uint64 val) {
            return it += val;
        }

        // Subtraction operator
        friend basic_iterator operator-(basic_iterator it, const uint64 val) {
            return it -= val;
        }

        // =======================================================================
        // COMPARISON OPERATORS

        // Equality operator
        bool operator==(const basic_iterator&) const = default;

        // Inequality operator
        bool operator!=(const basic_iterator&) const = default;

        /* TODO: random-access comparison operations */

        // =======================================================================
        // ACCESS OPERATORS

        // Dereference operator
        optval operator*() requires (!Const) {
            OPTVECTOR_IT_DEREF_CHECK;
            return _ov->at(_idx);
        }

        // Dereference operator
        const_optval operator*() const {
            OPTVECTOR_IT_DEREF_CHECK;
            return _ov->at(_idx);
        }

        // Arrow operator
        T* operator->() requires (!Const) {
            const optval val = **this;
            assert(val.has_val);
            return &val.val;
        }

        // Arrow operator
        const T* operator->() const {
            const const_optval val = **this;
            assert(val.has_val);
            return &val.val;
        }

    private: // ==================================================================
        // PRIVATE MEMBERS

        optvec_t* _ov;
        uint64 _idx;

    };

#undef OPTVECTOR_IT_STATE_CHECK
#undef OPTVECTOR_IT_DEREF_CHECK

}

// =======================================================================================
#endif
