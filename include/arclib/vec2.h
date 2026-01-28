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

#ifndef __ARCLIB_VEC2_H
#define __ARCLIB_VEC2_H

#include "concepts.h"
#include <cmath>

namespace arcl {

// =======================================================================================
// vec2 STRUCT DEFINITION
// =======================================================================================

    template <floating T>
    struct vec2 {
        T x = 0;
        T y = 0;
    };

    using vec2f = vec2<float>;
    using vec2d = vec2<double>;

// =======================================================================================
// ASSIGNMENT OPERATORS
// =======================================================================================

    // Vector addition assignment operator
    template <floating T>
    constexpr vec2<T>& operator+=(vec2<T>& a, const vec2<T>& b) {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    // Vector subtraction assignment operator
    template <floating T>
    constexpr vec2<T>& operator-=(vec2<T>& a, const vec2<T>& b) {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }

    // Scalar multiplication assignment operator
    template <floating T, arithmetic U>
    constexpr vec2<T>& operator*=(vec2<T>& v, U k) {
        v.x *= k;
        v.y *= k;
        return v;
    }

    // Scalar division assignment operator
    template <floating T, arithmetic U>
    constexpr vec2<T>& operator/=(vec2<T>& v, U k) {
        v.x /= k;
        v.y /= k;
        return v;
    }

// =======================================================================================
// ARITHMETIC OPERATORS
// =======================================================================================

    // Vector addition operator
    template <floating T>
    constexpr vec2<T> operator+(vec2<T> a, const vec2<T>& b) {
        return a += b;
    }

    // Vector subtraction operator
    template <floating T>
    constexpr vec2<T> operator-(vec2<T> a, const vec2<T>& b) {
        return a -= b;
    }

    // Scalar multiplication operator
    template <floating T, arithmetic U>
    constexpr vec2<T> operator*(vec2<T> v, U k) {
        return v *= k;
    }

    // Scalar multiplication operator
    template <floating T, arithmetic U>
    constexpr vec2<T> operator*(U k, vec2<T> v) {
        return v *= k;
    }

    // Scalar division operator
    template <floating T, arithmetic U>
    constexpr vec2<T> operator/(vec2<T> v, U k) {
        return v /= k;
    }
    
    // Negation operator
    template <floating T>
    constexpr vec2<T> operator-(const vec2<T>& v) {
        return { -v.x, -v.y };
    }

// =======================================================================================
// COMPARISON OPERATORS
// =======================================================================================

    // Equality operator
    template <floating T>
    constexpr bool operator==(const vec2<T>& a, const vec2<T>& b) {
        return a.x == b.x && a.y == b.y;
    }

    // Inequality operator
    template <floating T>
    constexpr bool operator!=(const vec2<T>& a, const vec2<T>& b) {
        return !(a == b);
    }

// =======================================================================================
// VECTOR MATH FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Computes the magnitude of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude of the vector.</returns>
    template <floating T>
    T magnitude(const vec2<T>& v) {
        return std::sqrt(magnitude2(v));
    }

    /// <summary>
    /// Computes the magnitude squared of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude squared of the vector.</returns>
    template <floating T>
    constexpr T magnitude2(const vec2<T>& v) {
        return dot(v, v);
    }

    /// <summary>
    /// Computes a unit vector in the direction of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>A unit vector in the direction of the vector.</returns>
    template <floating T>
    vec2<T> normalize(const vec2<T>& v) {
        return v / magnitude(v);
    }

    /// <summary>
    /// Computes the dot product of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The dot product.</returns>
    template <floating T>
    constexpr T dot(const vec2<T>& a, const vec2<T>& b) {
        return a.x * b.x + a.y * b.y;
    }

    /// <summary>
    /// Computes the cross product of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The cross product.</returns>
    template <floating T>
    constexpr T cross(const vec2<T>& a, const vec2<T>& b) {
        return a.x * b.y - a.y * b.x;
    }

    /// <summary>
    /// Computes the distance between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance.</returns>
    template <floating T>
    T distance(const vec2<T>& a, const vec2<T>& b) {
        return magnitude(a - b);
    }

    /// <summary>
    /// Computes the distance squared between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance squared.</returns>
    template <floating T>
    constexpr T distance2(const vec2<T>& a, const vec2<T>& b) {
        return magnitude2(a - b);
    }

// =======================================================================================
// UTILITY FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Casts a vec2 of one type to another.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The casted vector.</returns>
    template <floating T, floating U>
    constexpr vec2<T> vec2_cast(const vec2<U>& v) {
        return { (T)v.x, (T)v.y };
    }

}

// =======================================================================================
#endif
