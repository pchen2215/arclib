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
#include <new>
#include <type_traits>

namespace arcl {

// =======================================================================================
// MEMORY MANAGEMENT FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Allocates an uninitialized block of memory of sufficient byte length to store the
    /// requested number of elements of type T contiguously. The block will meet the
    /// alignment requirements of any type for which alignas is defined. The allocated
    /// block should only be freed by a matching call to memfree().
    /// </summary>
    /// <param name="size">The number of elements.</param>
    /// <returns>A pointer to the beginning of the allocated block.</returns>
    template <typename T>
    T* typealloc(const uint64 size) noexcept {
        assert(size != 0);
        const uint64 allocation = size * sizeof(T);
        const std::align_val_t alignment = (std::align_val_t)alignof(T);
        return (T*)::operator new(allocation, alignment, std::nothrow);
    }

    /// <summary>
    /// Allocates an uninitialized block of memory of the requested byte length. The
    /// allocated block should only be freed by a matching call to memfree().
    /// </summary>
    /// <param name="bytes">The number of bytes.</param>
    /// <returns>A pointer to the beginning of the allocated block.</returns>
    byte* bytealloc(const uint64 bytes) noexcept {
        return typealloc<byte>(bytes);
    }

    /// <summary>
    /// Frees a block of memory allocated by arclib memory allocation functions. The
    /// pointer must have a static type exactly equal to the return value of the
    /// allocation function that allocated it.
    /// </summary>
    /// <param name="block">A pointer to the beginning of the allocated block.</param>
    template <typename T>
    void memfree(T* block) noexcept requires (!std::is_void_v<T>) {
        const std::align_val_t alignment = (std::align_val_t)alignof(T);
        return ::operator delete(block, alignment);
    }

    /// <summary>
    /// Frees a block of memory of specific size allocated by arclib memory allocation
    /// functions. The provided size must be exactly equal to the size that was
    /// requested when the block was allocated. The pointer must have a static type
    /// exactly equal to the return value of the allocation function that allocated it.
    /// </summary>
    /// <param name="block">A pointer to the beginning of the allocated block.</param>
    /// <param name="size">The size of the allocation.</param>
    template <typename T>
    void memfree(T* block, const uint64 size) noexcept requires (!std::is_void_v<T>) {
        const uint64 allocation = size * sizeof(T);
        const std::align_val_t alignment = (std::align_val_t)alignof(T);
        return ::operator delete(block, allocation, alignment);
    }

}

// =======================================================================================
#endif
