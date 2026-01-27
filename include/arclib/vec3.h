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

#ifndef __ARCLIB_VEC3_H
#define __ARCLIB_VEC3_H

#include "concepts.h"
#include <cmath>

namespace arcl {

// =======================================================================================
// vec3 STRUCT DEFINITION
// =======================================================================================

    template <floating_t T>
    struct vec3 {
        T x = 0;
        T y = 0;
        T z = 0;
    };

    using vec3f = vec3<float>;
    using vec3d = vec3<double>;

// =======================================================================================
// ASSIGNMENT OPERATORS
// =======================================================================================

    // Vector addition assignment operator
    template <floating_t T>
    constexpr vec3<T>& operator+=(vec3<T>& a, const vec3<T>& b) {
        a = a + b; return a;
    }

    // Vector subtraction assignment operator
    template <floating_t T>
    constexpr vec3<T>& operator-=(vec3<T>& a, const vec3<T>& b) {
        a = a - b; return a;
    }

    // Scalar multiplication assignment operator
    template <floating_t T>
    constexpr vec3<T>& operator*=(vec3<T>& v, T k) {
        v = v * k; return v;
    }

    // Scalar division assignment operator
    template <floating_t T>
    constexpr vec3<T>& operator/=(vec3<T>& v, T k) {
        v = v / k; return v;
    }

// =======================================================================================
// ARITHMETIC OPERATORS
// =======================================================================================

    // Vector addition operator
    template <floating_t T>
    constexpr vec3<T> operator+(const vec3<T>& a, const vec3<T>& b) {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    // Vector subtraction operator
    template <floating_t T>
    constexpr vec3<T> operator-(const vec3<T>& a, const vec3<T>& b) {
        return a + -b;
    }

    // Scalar multiplication operator
    template <floating_t T>
    constexpr vec3<T> operator*(T k, const vec3<T>& v) {
        return { k * v.x, k * v.y, k * v.z };
    }

    // Scalar multiplication operator
    template <floating_t T>
    constexpr vec3<T> operator*(const vec3<T>& v, T k) {
        return k * v;
    }

    // Scalar division operator
    template <floating_t T>
    constexpr vec3<T> operator/(const vec3<T>& v, T k) {
        return { v.x / k, v.y / k, v.z / k };
    }
    
    // Negation operator
    template <floating_t T>
    constexpr vec3<T> operator-(const vec3<T>& v) {
        return { -v.x, -v.y, -v.z };
    }

// =======================================================================================
// COMPARISON OPERATORS
// =======================================================================================

    // Equality operator
    template <floating_t T>
    constexpr bool operator==(const vec3<T>& a, const vec3<T>& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    // Inequality operator
    template <floating_t T>
    constexpr bool operator!=(const vec3<T>& a, const vec3<T>& b) {
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
    template <floating_t T>
    T magnitude(const vec3<T>& v) {
        return std::sqrt(magnitude2(v));
    }

    /// <summary>
    /// Computes the magnitude squared of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude squared of the vector.</returns>
    template <floating_t T>
    constexpr T magnitude2(const vec3<T>& v) {
        return dot(v, v);
    }

    /// <summary>
    /// Computes a unit vector in the direction of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>A unit vector in the direction of the vector.</returns>
    template <floating_t T>
    vec3<T> normalize(const vec3<T>& v) {
        return v / magnitude(v);
    }

    /// <summary>
    /// Computes the dot product of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The dot product.</returns>
    template <floating_t T>
    constexpr T dot(const vec3<T>& a, const vec3<T>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    /// <summary>
    /// Computes the cross product of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The cross product.</returns>
    template <floating_t T>
    constexpr vec3<T> cross(const vec3<T>& a, const vec3<T>& b) {
        T x = a.y * b.z - a.z * b.y;
        T y = a.z * b.x - a.x * b.z;
        T z = a.x * b.y - a.y * b.x;
        return { x, y, z };
    }

    /// <summary>
    /// Computes the distance between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance.</returns>
    template <floating_t T>
    T distance(const vec3<T>& a, const vec3<T>& b) {
        return magnitude(a - b);
    }

    /// <summary>
    /// Computes the distance squared between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance squared.</returns>
    template <floating_t T>
    constexpr T distance2(const vec3<T>& a, const vec3<T>& b) {
        return magnitude2(a - b);
    }

// =======================================================================================
// UTILITY FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Casts a vec3 of one type to another.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The casted vector.</returns>
    template <floating_t T, floating_t U>
    constexpr vec3<T> vec3_cast(const vec3<U>& v) {
        return { (T)v.x, (T)v.y, (T)v.z };
    }

}

// =======================================================================================
#endif
