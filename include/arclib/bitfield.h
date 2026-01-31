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

#ifndef __ARCLIB_BITFIELD_H
#define __ARCLIB_BITFIELD_H

#include "int.h"
#include "concepts.h"
#include "memory.h"
#include <algorithm>
#include <cassert>

namespace arcl {

// =======================================================================================
// bitfield CLASS DEFINITION
// =======================================================================================

    class bitfield {
    public: // ===================================================================
        // PUBLIC MEMBER TYPES

        template <bool Const>
        class basic_bitref;

        using bitref = basic_bitref<false>;
        using const_bitref = basic_bitref<true>;

        // =======================================================================
        // CONSTRUCTION AND DESTRUCTION

        // Default constructor
        bitfield():
            _field(nullptr), _alloc(0) { }

        // Copy constructor
        bitfield(const bitfield& b):
            _field(nullptr), _alloc(0) {
            copy_into(*this, b);
        }

        // Move constructor
        bitfield(bitfield&& b):
            _field(nullptr), _alloc(0) {
            move_into(*this, std::forward<bitfield>(b));
        }

        /// <summary>
        /// Value constructor.
        /// </summary>
        /// <param name="bytes">The number of bytes to allocate to the
        /// bitfield.</param>
        bitfield(const uint64 bytes):
            _field(nullptr), _alloc(0) {
            realloc(bytes);
        }

        // Destructor
        ~bitfield() {
            dealloc();
        }

        // =======================================================================
        // CAPACITY

        /// <summary>
        /// Returns the number of bits contained in the bitfield.
        /// </summary>
        /// <returns>The number of bits.</returns>
        uint64 size() const {
            return _alloc * 8;
        }

        /// <summary>
        /// Resizes the bitfield to contain the requested number of bits. Bits
        /// added in excess of the current allocation will be default-initialized
        /// to zero.
        /// </summary>
        /// <param name="bits">The number of bits to allocate. It must be an
        /// integer multiple of 8.</param>
        void resize(const uint64 bits) {
            assert(bits % 8 == 0);
            const uint64 bytes = bits / 8;
            realloc(bytes);
        }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Copy assignment operator
        bitfield& operator=(const bitfield& b) {
            if (&b == this) { return *this; }
            dealloc();
            copy_into(*this, b);
            return *this;
        }

        // Move assignment operator
        bitfield& operator=(bitfield&& b) {
            if (&b == this) { return *this; }
            dealloc();
            move_into(*this, std::forward<bitfield>(b));
            return *this;
        }

        // =======================================================================
        // ACCESS OPERATORS

        // Subscript operator
        bitref operator[](const uint64 bit);

        // Subscript operator
        const_bitref operator[](const uint64 bit) const;

    private: // ==================================================================
        // PRIVATE MEMBERS

        byte* _field;
        uint64 _alloc;

        /// <summary>
        /// Performs a value copy of one object of the type into another. The
        /// destination object must not have ownership responsibility over any
        /// heap memory allocations upon calling this function. The destination
        /// and source must not be the same object.
        /// </summary>
        /// <param name="dst">The destination object.</param>
        /// <param name="src">The source object.</param>
        static void copy_into(bitfield& dst, const bitfield& src) {
            assert(&dst != &src);
            assert(dst._field == nullptr);

            // Copy trivial members
            dst._alloc = src._alloc;

            // Copy allocated resources
            if (src._field == nullptr) {
                assert(dst._alloc == 0);
                dst._field = nullptr;
            } else {
                dst._field = bytealloc(dst._alloc);
                std::memcpy(dst._field, src._field, dst._alloc);
            }
        }

        /// <summary>
        /// Performs a move copy of one object of the type into another. The
        /// destination object must not have ownership responsibility over any
        /// heap allocations upon calling this function. Upon returning, the
        /// source object will have no ownership responsibility over any heap
        /// memory allocations. The destination and source must not be the same
        /// object.
        /// </summary>
        /// <param name="dst">The destination object.</param>
        /// <param name="src">The source object.</param>
        static void move_into(bitfield& dst, bitfield&& src) {
            assert(&dst != &src);
            assert(dst._field == nullptr);

            // Copy trivial members
            dst._alloc = src._alloc;
            src._alloc = 0;

            // Move allocated resources
            dst._field = src._field;
            src._field = nullptr;
            assert(dst._field == nullptr ? dst._alloc == 0 : true);
        }

        /// <summary>
        /// Reallocates any heap memory allocations owned by this object and
        /// performs any necessary copies to preserve object invariance.
        /// </summary>
        /// <param name="new_alloc">The size of the reallocation in bytes.</param>
        void realloc(const uint64 new_alloc) {
            // Handle realloc to zero
            if (new_alloc == 0) {
                dealloc();
                return;
            }

            // Make new allocation and copy
            byte* new_field = bytealloc(new_alloc); {
                std::memcpy(new_field, _field, std::min(_alloc, new_alloc));
                if (new_alloc > _alloc) {
                    std::memset(new_field + _alloc, 0, new_alloc - _alloc);
                }
            }

            // Replace allocation
            memfree(_field, _alloc);
            _field = new_field;
            _alloc = new_alloc;
        }

        /// <summary>
        /// Frees all heap memory allocations this object is responsible for. Upon
        /// returning, this object will have no responsibility over any heap
        /// memory allocations.
        /// </summary>
        void dealloc() {
            memfree(_field, _alloc);
            _alloc = 0;
        }

    };

// =======================================================================================
// bitfield::basic_bitref CLASS DEFINITION
// =======================================================================================

    template <bool Const>
    class bitfield::basic_bitref { // ============================================
        // FRIEND AND MEMBER TYPE DECLARATIONS

        friend class bitfield;
        using byte_t = std::conditional_t<Const, const byte, byte>;

    public: // ===================================================================
        // CONSTRUCTION

        // Delete default constructor
        basic_bitref() = delete;

        // Copy constructor
        basic_bitref(const basic_bitref&) = default;

        // Conversion constructor
        basic_bitref(const basic_bitref<false>& b) requires (Const):
            _byte(b._byte), _mask(b._mask) { }

        // =======================================================================
        // OPERATORS

        // Copy assignment operator
        basic_bitref& operator=(const basic_bitref& b)
            requires (!Const) {
            return *this = (bool)b;
        }

        // Assignment operator
        bitref& operator=(bool b) requires (!Const) {
            b ? _byte |= _mask : _byte &= ~_mask;
            return *this;
        }

        // Logical NOT operator
        bool operator!() const {
            return !(bool)(*this);
        }

        // Bool cast operator
        operator bool() const {
            return (_byte & _mask) != 0;
        }

    private: // ==================================================================
        // PRIVATE MEMBERS

        // Private value constructor
        basic_bitref(byte_t& byte, const uint8 idx):
            _byte(byte), _mask(1 << idx) { }

        // Reference to bit
        byte_t& _byte;
        const uint8 _mask;
    
    };

// =======================================================================================
// bitfield MEMBER FUNCTION DEFINITIONS
// =======================================================================================

    bitfield::bitref bitfield::operator[](const uint64 bit) {
        assert(bit < _alloc * 8);
        return bitref(_field[bit / 8], bit % 8);
    }

    bitfield::const_bitref bitfield::operator[](const uint64 bit) const {
        assert(bit < _alloc * 8);
        return const_bitref(_field[bit / 8], bit % 8);
    }

}

// =======================================================================================
#endif
