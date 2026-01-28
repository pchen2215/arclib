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
            _field(nullptr), _alloc(b._alloc) {
            if (b.size() == 0) { return; }
            _field = memalloc<uint8>(_alloc);
            memcopy(_field, b._field, _alloc);
        }

        // Move constructor
        bitfield(bitfield&& b):
            _field(b._field), _alloc(b._alloc) {
            b._field = nullptr;
            b._alloc = 0;
        }

        /// <summary>
        /// Value constructor.
        /// </summary>
        /// <param name="bytes">The number of bytes to allocate to the
        /// bitfield.</param>
        bitfield(const uint64 bytes):
            _field(memalloc<uint8>(bytes)), _alloc(bytes) {
            memfill(_field, 0, _alloc);
        }

        // Destructor
        ~bitfield() {
            memfree(_field);
        }

        // =======================================================================
        // CAPACITY

        /// <summary>
        /// Returns the number of bytes contained in the bitfield.
        /// </summary>
        /// <returns>The number of bytes.</returns>
        uint64 size() const {
            return _alloc;
        }

        /// <summary>
        /// Resizes the bitfield to contain the requested number of bits. Bits
        /// added in excess of the current allocation will be default-initialized
        /// to zero.
        /// </summary>
        /// <param name="bits">The number of bits to allocate. It must be a
        /// multiple of 8.</param>
        void resize(const uint64 bits) {
            assert(bits % 8 == 0);
            const uint64 bytes = bits / 8;

            // Handle special case
            if (bytes == 0) {
                memfree(_field);
                _field = nullptr;
                _alloc = 0;
                return;
            }

            // Handle general case
            uint8* new_field = memalloc<uint8>(bytes);
            memcopy(new_field, _field, std::min(bytes, _alloc));
            if (bytes > _alloc) {
                memfill(new_field + _alloc, 0, bytes - _alloc);
            }
            memfree(_field);
            _field = new_field;
            _alloc = bytes;
        }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Copy assignment operator
        bitfield& operator=(const bitfield& b) {
            if (&b == this) { return *this; }
            memfree(_field);
            _field = nullptr;
            _alloc = 0;
            if (b.size() == 0) { return *this; }
            _alloc = b._alloc;

            // Allocate and copy field
            _field = memalloc<uint8>(_alloc);
            memcopy(_field, b._field, _alloc);

            // Return self
            return *this;
        }

        // Move assignment operator
        bitfield& operator=(bitfield&& b) {
            if (&b == this) { return *this; }
            memfree(_field);

            // Transfer ownership
            _field = b._field;
            _alloc = b._alloc;

            // Reset state of other bitfield
            b._field = nullptr;
            b._alloc = 0;

            // Return self
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

        uint8* _field;
        uint64 _alloc;

    };

// =======================================================================================
// bitfield::basic_bitref CLASS DEFINITION
// =======================================================================================

    template <bool Const>
    class bitfield::basic_bitref { // ============================================
        // FRIEND AND MEMBER TYPE DECLARATIONS

        friend class bitfield;
        using byte_t = std::conditional_t<Const, const uint8, uint8>;

    public: // ===================================================================
        // CONSTRUCTION

        // Delete default constructor
        basic_bitref() = delete;

        // Copy constructor
        basic_bitref(const basic_bitref&) = default;

        // Conversion constructor
        basic_bitref(const basic_bitref<false>& b) requires (Const):
            _byte(b._byte), _offset(b._offset) { }

        // =======================================================================
        // OPERATORS

        // Delete copy assignment operator
        basic_bitref& operator=(const basic_bitref&) = delete;

        // Assignment operator
        bitref& operator=(bool b) requires (!Const) {
            b ? _byte |= ((uint8)1 << _offset) : _byte &= ~((uint8)1 << _offset);
            return *this;
        }

        // Bool cast operator
        operator bool() const {
            return (_byte >> _offset) & (uint8)1;
        }

    private: // ==================================================================
        // PRIVATE MEMBERS

        // Private value constructor
        basic_bitref(byte_t& byte, const uint8 offset):
            _byte(byte), _offset(offset) { }

        // Reference to bit
        byte_t& _byte;
        uint8 _offset;
    
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
