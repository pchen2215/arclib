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

#ifndef __ARCLIB_BYTE_H
#define __ARCLIB_BYTE_H

#include "int.h"
#include <cassert>
#include <type_traits>

namespace arcl {

// =======================================================================================
// base_bitref CLASS DEFINITION
// =======================================================================================

    template <bool Const>
    class base_bitref { // ======================================================
        // FRIEND DECLARATIONS AND PRIVATE MEMBER TYPES

        template <bool C>
        friend class base_bitref;

        friend class byte;
        using byte_t = std::conditional_t<Const, const byte, byte>;

    public: // ===================================================================
        // CONSTRUCTORS

        // Delete default constructor
        constexpr base_bitref() = delete;

        // Copy constructor
        constexpr base_bitref(const base_bitref& b) = default;

        // Conversion constructor
        constexpr base_bitref(const base_bitref<false>& b) requires (Const):
            _byte(b._byte), _mask(b._mask) { }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Delete copy assignment operator
        constexpr base_bitref& operator=(const base_bitref& b) requires (!Const) {
            return *this = (bool)b;
        }

        // Value assignment operator
        constexpr base_bitref& operator=(bool b) requires (!Const) {
            _byte._val ^= (-(uint8)b ^ _byte._val) & _mask;
            return *this;
        }

        // =======================================================================
        // TYPE CAST OPERATORS

        // bool cast operator
        constexpr operator bool() const {
            return _byte._val & _mask;
        }

    private: // ==================================================================
        // PRIVATE CONSTRUCTOR

        // Value constructor
        constexpr base_bitref(byte_t& byte, const uint8 pos):
            _byte(byte), _mask(1 << pos) { }

        // =======================================================================
        // PRIVATE DATA MEMBERS

        byte_t& _byte;
        const uint8 _mask;

    };

    using bitref = base_bitref<false>;
    using const_bitref = base_bitref<true>;

// =======================================================================================
// byte CLASS DEFINITION
// =======================================================================================

    class byte { // ==============================================================
        // FRIEND DECLARATIONS

        template <bool Const>
        friend class base_bitref;

    public: // ===================================================================
        // CONSTRUCTORS

        // Default constructor
        constexpr byte():
            _val(0) { }

        // Copy constructor
        constexpr byte(const byte& b) = default;

        // Value constructor
        constexpr byte(const uint8 val):
            _val(val) { }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Copy assignment operator
        constexpr byte& operator=(const byte b) {
            _val = b._val;
            return *this;
        }

        // Bitwise AND assignment operator
        constexpr byte& operator&=(const byte b) {
            _val &= b._val;
            return *this;
        }

        // Bitwise OR assignment operator
        constexpr byte& operator|=(const byte b) {
            _val |= b._val;
            return *this;
        }

        // Bitwise XOR assignment operator
        constexpr byte& operator^=(const byte b) {
            _val ^= b._val;
            return *this;
        }

        // Bitwise left-shift assignment operator
        constexpr byte& operator<<=(const byte b) {
            _val <<= b._val;
            return *this;
        }

        // Bitwise right-shift assignment operator
        constexpr byte& operator>>=(const byte b) {
            _val >>= b._val;
            return *this;
        }

        // =======================================================================
        // ARITHMETIC OPERATORS

        // Bitwise NOT operator
        friend constexpr byte operator~(byte a) {
            a._val = ~a._val;
            return a;
        }

        // Bitwise AND operator
        friend constexpr byte operator&(byte a, const byte b) {
            return a &= b;
        }

        // Bitwise OR operator
        friend constexpr byte operator|(byte a, const byte b) {
            return a |= b;
        }

        // Bitwise XOR operator
        friend constexpr byte operator^(byte a, const byte b) {
            return a ^= b;
        }

        // Bitwise left-shift operator
        friend constexpr byte operator<<(byte a, const byte b) {
            return a <<= b;
        }

        // Bitwise right-shift operator
        friend constexpr byte operator>>(byte a, const byte b) {
            return a >>= b;
        }

        // =======================================================================
        // COMPARISON OPERATORS

        // Equality operator
        constexpr bool operator==(const byte b) const {
            return _val == b._val;
        }

        // Inequality operator
        constexpr bool operator!=(const byte b) const {
            return _val != b._val;
        }

        // =======================================================================
        // ACCESS OPERATORS

        // Subscript operator
        constexpr bitref operator[](const uint8 pos) {
            assert(pos < 8);
            return bitref(*this, pos);
        }

        // Subscript operator
        constexpr const_bitref operator[](const uint8 pos) const {
            assert(pos < 8);
            return const_bitref(*this, pos);
        }

        // =======================================================================
        // TYPE CAST OPERATORS

        // uint8 cast operator
        explicit constexpr operator uint8() const {
            return _val;
        }

    private: // ==================================================================
        // PRIVATE DATA MEMBERS
        
        // Underlying uint8
        uint8 _val;

    };

}

// =======================================================================================
#endif
