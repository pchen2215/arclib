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

#ifndef __ARCLIB_MEMORY_H
#define __ARCLIB_MEMORY_H

#include "int.h"
#include <cassert>
#include <cstring>

namespace arcl {

// =======================================================================================
// MEMORY MANAGEMENT FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Allocates an uninitialized block of memory of the requested byte length. The block
    /// will be aligned to the default system alignment. The allocated block must be freed
    /// by a matching call to memfree().
    /// </summary>
    /// <param name="bytes">The number of bytes.</param>
    /// <returns>A pointer to the beginning of the allocated block.</returns>
    void* bytealloc(const uint64 bytes) {
        assert(bytes != 0);
        return ::operator new(bytes);
    }

    /// <summary>
    /// Allocates an uninitialized block of memory of the requested byte length. The block
    /// will be aligned to the requested alignment. The allocated block must be freed by a
    /// matching call to memfree().
    /// </summary>
    /// <param name="bytes">The number of bytes.</param>
    /// <param name="align">The alignment in bytes.</param>
    /// <returns>A pointer to the beginning of the allocated block.</returns>
    void* bytealloc(const uint64 bytes, const std::align_val_t align) {
        assert(bytes != 0);
        return ::operator new(bytes, align);
    }

    /// <summary>
    /// Allocates an uninitialized block of memory of sufficient byte length to store the
    /// requested number of elements of type T contiguously. The block is guaranteed to
    /// meet the alignment requirements of any type for which alignof is defined. The
    /// allocated block must be freed by a matching call to memfree().
    /// </summary>
    /// <param name="size">The number of elements.</param>
    /// <returns>A pointer to the beginning of the allocated block.</returns>
    template <typename T>
    T* typealloc(const uint64 size) {
        assert(size != 0);
        return (T*)::operator new(size * sizeof(T), alignof(T));
    }

    /// <summary>
    /// Frees a block of memory allocated by arclib memory allocation functions. No
    /// destructors will be called as a result of calling this function.
    /// </summary>
    /// <param name="pt">A pointer to the beginning of the allocated block.</param>
    void memfree(void* block) {
        ::operator delete(block);
    }

}

// =======================================================================================
#endif
