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
    /// Allocates an uninitialized block of memory of sufficient byte length to store the
    /// requested number of elements of type T. This allocated block must be freed by a
    /// matching call to memfree().
    /// </summary>
    /// <param name="size">The number of elements.</param>
    /// <returns>A pointer to the beginning of the allocated block.</returns>
    template <typename T>
    T* memalloc(uint64 size) {
        assert(size != 0);
        return (T*)::operator new(size * sizeof(T));
    }

    /// <summary>
    /// Performs a bitwise copy of the specified number of bytes from one block of memory to
    /// another.
    /// </summary>
    /// <param name="dst">The destination block.</param>
    /// <param name="src">The source block.</param>
    /// <param name="size">The number of bytes to copy.</param>
    void memcopy(void* dst, const void* src, uint64 size) {
        std::memcpy(dst, src, size);
    }

    /// <summary>
    /// Frees a block of memory allocated by memalloc(). No destructors will be called as a
    /// result of calling this function.
    /// </summary>
    /// <param name="pt">A pointer to the beginning of the allocated block.</param>
    void memfree(void* pt) {
        ::operator delete(pt);
    }

}

// =======================================================================================
#endif
