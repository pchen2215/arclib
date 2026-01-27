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

#ifndef __ARCLIB_RECT_H
#define __ARCLIB_RECT_H

#include "concepts.h"
#include "vec2.h"
#include <algorithm>
#include <array>

namespace arcl {

// =======================================================================================
// rect STRUCT DEFINITION
// =======================================================================================

    /// <summary>
    /// arcl::rect holds four values representing an axis-aligned rectangle.
    /// arclib operations treat it as canonically oriented to x-right and y-up. The width
    /// and height values must also be positive. However, the user is free to use it as a
    /// POD type with no invariant.
    /// </summary>
    template <floating_t T>
    struct rect {
        T x = 0;
        T y = 0;
        T w = 0;
        T h = 0;
    };

    using rectf = rect<float>;
    using rectd = rect<double>;

// =======================================================================================
// RECTANGLE OPERATIONS
// =======================================================================================

    /// <summary>
    /// Returns the x-value of the left side of a rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The x-value of the left side.</returns>
    template <floating_t T>
    constexpr T rect_left(const rect<T>& r) {
        return r.x;
    }

    /// <summary>
    /// Returns the x-value of the right side of a rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The x-value of the right side.</returns>
    template <floating_t T>
    constexpr T rect_right(const rect<T>& r) {
        return r.x + r.w;
    }

    /// <summary>
    /// Returns the y-value of the top side of a rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The y-value of the top side.</returns>
    template <floating_t T>
    constexpr T rect_top(const rect<T>& r) {
        return r.y + r.h;
    }

    /// <summary>
    /// Returns the y-value of the bottom side of a rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The y-value of the bottom side.</returns>
    template <floating_t T>
    constexpr T rect_bot(const rect<T>& r) {
        return r.y;
    }

    /// <summary>
    /// Returns the center point of the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The center point.</returns>
    template <floating_t T>
    constexpr vec2<T> rect_center(const rect<T>& r) {
        return { r.x + r.w / 2, r.y + r.h / 2 };
    }

    /// <summary>
    /// Returns the top-left point of the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The top-left point.</returns>
    template <floating_t T>
    constexpr vec2<T> rect_topleft(const rect<T>& r) {
        return { rect_left(r), rect_top(r) };
    }

    /// <summary>
    /// Returns the top-right point of the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The top-right point.</returns>
    template <floating_t T>
    constexpr vec2<T> rect_topright(const rect<T>& r) {
        return { rect_right(r), rect_top(r) };
    }

    /// <summary>
    /// Returns the bottom-right point of the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The bottom-right point.</returns>
    template <floating_t T>
    constexpr vec2<T> rect_botright(const rect<T>& r) {
        return { rect_right(r), rect_bot(r) };
    }

    /// <summary>
    /// Returns the bottom-left point of the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>The bottom-left point.</returns>
    template <floating_t T>
    constexpr vec2<T> rect_botleft(const rect<T>& r) {
        return { rect_left(r), rect_bot(r) };
    }

    /// <summary>
    /// Splits the rectangle into its 4 quadrants.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <returns>An array containing the quadrants in clockwise order starting from the
    /// top-left quadrant.</returns>
    template <floating_t T>
    constexpr std::array<rect<T>, 4> rect_split4(const rect<T>& r) {
        std::array<rect<T>, 4> result;
        const T half_w = r.w / 2;
        const T half_h = r.h / 2;
        const T mid_x = r.x + half_w;
        const T mid_y = r.y + half_h;
        result[0] = { r.x, mid_y, half_w, half_h };
        result[1] = { mid_x, mid_y, half_w, half_h };
        result[2] = { mid_x, r.y, half_w, half_h };
        result[3] = { r.x, r.y, half_w, half_h };
        return result;
    }

    /// <summary>
    /// Checks if a point is contained within the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <param name="x">The x-value of the point.</param>
    /// <param name="y">The y-value of the point.</param>
    /// <returns>true if the point lies within the rectangle, else false.</returns>
    template <floating_t T>
    constexpr bool contains(const rect<T>& r, T x, T y) {
        return rect_left(r) <= x && x <= rect_right(r) && rect_bot(r) <= y && y <= rect_top(r);
    }

    /// <summary>
    /// Checks if a point is contained within the rectangle.
    /// </summary>
    /// <param name="r">The rectangle.</param>
    /// <param name="pt">The point.</param>
    /// <returns>true if the point lies within the rectangle, else false.</returns>
    template <floating_t T>
    constexpr bool contains(const rect<T>& r, const vec2<T>& pt) {
        return contains(r, pt.x, pt.y);
    }

    /// <summary>
    /// Checks if a rectangle is contained completely within another.
    /// </summary>
    /// <param name="out">The outer rectangle.</param>
    /// <param name="in">The inner rectangle.</param>
    /// <returns>true if the inner rectangle lies completely within the outer rectangle,
    /// else false.</returns>
    template <floating_t T>
    constexpr bool contains(const rect<T>& out, const rect<T>& in) {
        return rect_left(out) <= rect_left(in) && rect_right(in) <= rect_right(out)
               && rect_bot(out) <= rect_bot(in) && rect_top(in) <= rect_top(out);
    }

    /// <summary>
    /// Checks if two rectangles intersect.
    /// </summary>
    /// <param name="r1">The first rectangle.</param>
    /// <param name="r2">The second rectangle.</param>
    /// <returns>true if the rectangles intersect, else false.</returns>
    template <floating_t T>
    constexpr bool intersects(const rect<T>& r1, const rect<T>& r2) {
        return rect_left(r1) <= rect_right(r2) && rect_left(r2) <= rect_right(r1)
               && rect_bot(r1) <= rect_top(r2) && rect_bot(r2) <= rect_top(r1);
    }

// =======================================================================================
// UTILITY FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Casts a rect of one type to another.
    /// </summary>
    /// <param name="r">The rect.</param>
    /// <returns>The casted rect.</returns>
    template <floating_t T, floating_t U>
    constexpr rect<T> rect_cast(const rect<U>& r) {
        return { (T)r.x, (T)r.y, (T)r.w, (T)r.h };
    }

    /// <summary>
    /// Constructs a rect from two points.
    /// </summary>
    /// <param name="a">The first point.</param>
    /// <param name="b">The second point.</param>
    /// <returns>The constructed rect.</returns>
    template <floating_t T>
    constexpr rect<T> rect_construct(const vec2<T>& a, const vec2<T>& b) {
        const T x = std::min(a.x, b.x);
        const T y = std::min(a.y, b.y);
        return { x, y, std::max(a.x, b.x) - x, std::max(a.y, b.y) - y };
    }

}

// =======================================================================================
#endif
