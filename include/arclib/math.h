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

#ifndef __ARCLIB_MATH_H
#define __ARCLIB_MATH_H

#include <concepts>
#include <cmath>

namespace arcl {

    template <typename T>
    concept integral = std::integral<T>;

    template <typename T>
    concept floating_point = std::floating_point<T>;

    template <typename T>
    concept arithmetic = integral<T> || floating_point<T>;

    // ===================================================================================
    // 2d vector math
    // ===================================================================================

    template <floating_point T>
    struct vec2;

    // Vector addition operator
    template <floating_point T>
    constexpr vec2<T> operator+(const vec2<T>& a, const vec2<T>& b) {
        return { a.x + b.x, a.y + b.y };
    }

    // Vector addition assignment operator
    template <floating_point T>
    constexpr vec2<T>& operator+=(vec2<T>& a, const vec2<T>& b) { a = a + b; return a; }

    // Vector subtraction operator
    template <floating_point T>
    constexpr vec2<T> operator-(const vec2<T>& a, const vec2<T>& b) { return a + -b; }

    // Vector subtraction assignment operator
    template <floating_point T>
    constexpr vec2<T>& operator-=(vec2<T>& a, const vec2<T>& b) { a = a - b; return a; }

    // Scalar multiplication operator
    template <floating_point T>
    constexpr vec2<T> operator*(T k, const vec2<T>& v) { return { k * v.x, k * v.y }; }

    // Scalar multiplication operator
    template <floating_point T>
    constexpr vec2<T> operator*(const vec2<T>& v, T k) { return k * v; }

    // Scalar multiplication assignment operator
    template <floating_point T>
    constexpr vec2<T>& operator*=(vec2<T>& v, T k) { v = v * k; return v; }

    // Scalar division operator
    template <floating_point T>
    constexpr vec2<T> operator/(const vec2<T>& v, T k) { return { v.x / k, v.y / k }; }

    // Scalar division assignment operator
    template <floating_point T>
    constexpr vec2<T>& operator/=(vec2<T>& v, T k) { v = v / k; return v; }
    
    // Negation operator
    template <floating_point T>
    constexpr vec2<T> operator-(const vec2<T>& v) { return { -v.x, -v.y }; }

    /// <summary>
    /// Computes the magnitude of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude of the vector.</returns>
    template <floating_point T>
    T magnitude(const vec2<T>& v) { return std::sqrt(magnitude2(v)); }

    /// <summary>
    /// Computes the magnitude squared of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude squared of the vector.</returns>
    template <floating_point T>
    constexpr T magnitude2(const vec2<T>& v) { return dot(v, v); }

    /// <summary>
    /// Computes a unit vector in the direction of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>A unit vector in the direction of the vector.</returns>
    template <floating_point T>
    vec2<T> normalize(const vec2<T>& v) {
        if (v.x == 0 && v.y == 0) { return v; }
        T m = magnitude(v);
        return { v.x / m, v.y / m };
    }

    /// <summary>
    /// Computes the dot produce of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The dot product.</returns>
    template <floating_point T>
    constexpr T dot(const vec2<T>& a, const vec2<T>& b) { return a.x * b.x + a.y * b.y; }

    /// <summary>
    /// Computes the cross produce of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The cross product.</returns>
    template <floating_point T>
    constexpr T cross(const vec2<T>& a, const vec2<T>& b) {
        return a.x * b.y - a.y * b.x;
    }

    /// <summary>
    /// Computes the distance between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance.</returns>
    template <floating_point T>
    T distance(const vec2<T>& a, const vec2<T>& b) { return magnitude(a - b); }

    /// <summary>
    /// Computes the distance squared between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance squared.</returns>
    template <floating_point T>
    constexpr T distance2(const vec2<T>& a, const vec2<T>& b) {
        return magnitude2(a - b);
    }
    
    template <floating_point T>
    struct vec2 {
        T x = 0;
        T y = 0;

        /// <summary>
        /// Computes the magnitude of the vector.
        /// </summary>
        /// <returns>The magnitude of the vector.</returns>
        T magnitude() const { return arcl::magnitude(*this); }

        /// <summary>
        /// Computes the magnitude squared of the vector.
        /// </summary>
        /// <returns>The magnitude squared of the vector.</returns>
        constexpr T magnitude2() const { return arcl::magnitude2(*this); }

        /// <summary>
        /// Computes a unit vector in the direction of the vector.
        /// </summary>
        /// <returns>A unit vector in the direction of the vector.</returns>
        vec2 normalize() const { return arcl::normalize(*this); }

        /// <summary>
        /// Computes the dot product with another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The dot product.</returns>
        constexpr T dot(const vec2<T> v) const { return arcl::dot(*this, v); }

        /// <summary>
        /// Computes the cross product with another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The cross product.</returns>
        constexpr T cross(const vec2<T> v) const { return arcl::cross(*this, v); }

        /// <summary>
        /// Computes the distance to another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The distance.</returns>
        T distance(const vec2& v) { return arcl::distance(*this, v); }

        /// <summary>
        /// Computes the distance squared to another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The distance squared.</returns>
        constexpr T distance2(const vec2<T>& v) { return arcl::distance2(*this, v); }
    };

    using vec2f = vec2<float>;
    using vec2d = vec2<double>;

    // ===================================================================================
    // 3d vector math
    // ===================================================================================

    template <floating_point T>
    struct vec3;

    // Vector addition operator
    template <floating_point T>
    constexpr vec3<T> operator+(const vec3<T>& a, const vec3<T>& b) {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    // Vector addition assignment operator
    template <floating_point T>
    constexpr vec3<T>& operator+=(vec3<T>& a, const vec3<T>& b) { a = a + b; return a; }

    // Vector subtraction operator
    template <floating_point T>
    constexpr vec3<T> operator-(const vec3<T>& a, const vec3<T>& b) { return a + -b; }

    // Vector subtraction assignment operator
    template <floating_point T>
    constexpr vec3<T>& operator-=(vec3<T>& a, const vec3<T>& b) { a = a - b; return a; }

    // Scalar multiplication operator
    template <floating_point T>
    constexpr vec3<T> operator*(T k, const vec3<T>& v) {
        return { k * v.x, k * v.y, k * v.z };
    }

    // Scalar multiplication operator
    template <floating_point T>
    constexpr vec3<T> operator*(const vec3<T>& v, T k) { return k * v; }

    // Scalar multiplication assignment operator
    template <floating_point T>
    constexpr vec3<T>& operator*=(vec3<T>& v, T k) { v = v * k; return v; }

    // Scalar division operator
    template <floating_point T>
    constexpr vec3<T> operator/(const vec3<T>& v, T k) {
        return { v.x / k, v.y / k, v.z / k };
    }

    // Scalar division assignment operator
    template <floating_point T>
    constexpr vec3<T>& operator/=(vec3<T>& v, T k) { v = v / k; return v; }
    
    // Negation operator
    template <floating_point T>
    constexpr vec3<T> operator-(const vec3<T>& v) { return { -v.x, -v.y, -v.z }; }

    /// <summary>
    /// Computes the magnitude of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude of the vector.</returns>
    template <floating_point T>
    T magnitude(const vec3<T>& v) { return std::sqrt(magnitude2(v)); }

    /// <summary>
    /// Computes the magnitude squared of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>The magnitude squared of the vector.</returns>
    template <floating_point T>
    constexpr T magnitude2(const vec3<T>& v) { return dot(v, v); }

    /// <summary>
    /// Computes a unit vector in the direction of the vector.
    /// </summary>
    /// <param name="v">The vector.</param>
    /// <returns>A unit vector in the direction of the vector.</returns>
    template <floating_point T>
    vec3<T> normalize(const vec3<T>& v) {
        if (v.x == 0 && v.y == 0 && v.z == 0) { return v; }
        T m = magnitude(v);
        return { v.x / m, v.y / m, v.z / m };
    }

    /// <summary>
    /// Computes the dot produce of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The dot product.</returns>
    template <floating_point T>
    constexpr T dot(const vec3<T>& a, const vec3<T>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    /// <summary>
    /// Computes the cross produce of two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The cross product.</returns>
    template <floating_point T>
    constexpr vec3<T> cross(const vec3<T>& a, const vec3<T>& b) {
        return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
    }

    /// <summary>
    /// Computes the distance between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance.</returns>
    template <floating_point T>
    T distance(const vec3<T>& a, const vec3<T>& b) { return magnitude(a - b); }

    /// <summary>
    /// Computes the distance squared between two vectors.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The distance squared.</returns>
    template <floating_point T>
    constexpr T distance2(const vec3<T>& a, const vec3<T>& b) {
        return magnitude2(a - b);
    }

    template <floating_point T>
    struct vec3 {
        T x = 0;
        T y = 0;
        T z = 0;

        /// <summary>
        /// Computes the magnitude of the vector.
        /// </summary>
        /// <returns>The magnitude of the vector.</returns>
        T magnitude() const { return arcl::magnitude(*this); }

        /// <summary>
        /// Computes the magnitude squared of the vector.
        /// </summary>
        /// <returns>The magnitude squared of the vector.</returns>
        constexpr T magnitude2() const { return arcl::magnitude2(*this); }

        /// <summary>
        /// Computes a unit vector in the direction of the vector.
        /// </summary>
        /// <returns>A unit vector in the direction of the vector.</returns>
        vec3 normalize() const { return arcl::normalize(*this); }

        /// <summary>
        /// Computes the dot product with another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The dot product.</returns>
        constexpr T dot(const vec3<T> v) const { return arcl::dot(*this, v); }

        /// <summary>
        /// Computes the cross product with another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The cross product.</returns>
        constexpr vec3<T> cross(const vec3<T> v) const { return arcl::cross(*this, v); }

        /// <summary>
        /// Computes the distance to another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The distance.</returns>
        T distance(const vec3& v) { return arcl::distance(*this, v); }

        /// <summary>
        /// Computes the distance squared to another vector.
        /// </summary>
        /// <param name="v">The vector.</param>
        /// <returns>The distance squared.</returns>
        constexpr T distance2(const vec3<T>& v) { return arcl::distance2(*this, v); }
    };

    using vec3f = vec3<float>;
    using vec3d = vec3<double>;

}

#endif
