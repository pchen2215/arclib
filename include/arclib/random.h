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

#ifndef __ARCLIB_RANDOM_H
#define __ARCLIB_RANDOM_H

#include "int.h"
#include <random>

namespace arcl {

    enum random_distribution {
        RANDOM_DISTRIBUTION_UNIFORM
    };

// =======================================================================================
// random_engine CLASS DEFINITION
// =======================================================================================

    class random_engine { // =====================================================
        // CLASS FRIENDS

        template <typename It>
        friend void shuffle(It, It, random_engine&);

    public: // ===================================================================
        // CONSTRUCTION
        
        // Default constructor
        random_engine() = default;

        // Value constructor
        random_engine(uint64 seed_val):
            _engine(seed_val) { }

        // Copy constructor
        random_engine(const random_engine&) = default;

        // Move constructor
        random_engine(random_engine&&) = default;

        // =======================================================================
        // SEED AND DISTRIBUTION SETTINGS

        /// <summary>
        /// Reseeds the engine.
        /// </summary>
        /// <param name="seed_val">The new seed.</param>
        void seed(uint64 seed_val) {
            _engine.seed(seed_val);
        }

        // =======================================================================
        // RANDOM VALUE GENERATION

        /// <summary>
        /// Generates the next pseudorandom value as a 64-bit integer on the
        /// requested range according to the requested distribution. By default,
        /// a value on the range [0, INT64_MAX] will be generated according to a
        /// uniform distribution.
        /// </summary>
        /// <param name="min">The minimum value (inclusive) that can be
        /// generated.</param>
        /// <param name="max">The maximum value (inclusive) that can be
        /// generated.</param>
        /// <param name="dis">The distribution to use.</param>
        /// <returns>The value.</returns>
        int64 rand(int64 min, int64 max,
                   random_distribution dis = RANDOM_DISTRIBUTION_UNIFORM) {
            switch (dis) {
            case RANDOM_DISTRIBUTION_UNIFORM:
            default:
                std::uniform_int_distribution<int64>::param_type p(min, max);
                _uniform_int_distribution.param(p);
                return _uniform_int_distribution(_engine);
            }
        }

        /// <summary>
        /// Generates the next pseudorandom value as a 64-bit integer on the
        /// requested range according to the requested distribution. By default,
        /// a value on the range [0, INT64_MAX] will be generated according to a
        /// uniform distribution.
        /// </summary>
        /// <param name="dis">The distribution to use.</param>
        /// <returns>The value.</returns>
        int64 rand(random_distribution dis = RANDOM_DISTRIBUTION_UNIFORM) {
            return rand(0, INT64_MAX, dis);
        }

        /// <summary>
        /// Generates the next pseudorandom value as an double on the requested
        /// range according to the requested distribution. By default, a value on
        /// the range [0.0, 1.0) will be generated according to a uniform
        /// distribution.
        /// </summary>
        /// <param name="min">The minimum value (inclusive) that can be
        /// generated.</param>
        /// <param name="max">The maximum value (exclusive) that can be
        /// generated.</param>
        /// <param name="dis">The distribution to use.</param>
        /// <returns>The value.</returns>
        double randf(double min, double max,
                     random_distribution dis = RANDOM_DISTRIBUTION_UNIFORM) {
            switch (dis) {
            case RANDOM_DISTRIBUTION_UNIFORM:
            default:
                std::uniform_real_distribution<double>::param_type p(min, max);
                _uniform_real_distribution.param(p);
                return _uniform_real_distribution(_engine);
            }
        }

        /// <summary>
        /// Generates the next pseudorandom value as an double on the requested
        /// range according to the requested distribution. By default, a value on
        /// the range [0.0, 1.0) will be generated according to a uniform
        /// distribution.
        /// </summary>
        /// <param name="dis">The distribution to use.</param>
        /// <returns>The value.</returns>
        double randf(random_distribution dis = RANDOM_DISTRIBUTION_UNIFORM) {
            return randf(0.0, 1.0, dis);
        }

        // =======================================================================
        // ASSIGNMENT OPERATORS

        // Copy assignment operator
        random_engine& operator=(const random_engine&) = default;

        // Move assignment operator
        random_engine& operator=(random_engine&&) = default;

    private: // ==================================================================
        // PRIVATE MEMBERS

        std::mt19937_64 _engine;
        std::uniform_int_distribution<int64> _uniform_int_distribution;
        std::uniform_real_distribution<double> _uniform_real_distribution;
    };

// =======================================================================================
// RANDOM FUNCTIONS
// =======================================================================================

    /// <summary>
    /// Outputs true or false based on a specific probability.
    /// </summary>
    /// <param name="c">The probability of returning true.</param>
    /// <param name="e">The random_engine to use.</param>
    /// <returns>true or false depending on the probability.</returns>
    bool chance(double c, random_engine& e);

    /// <summary>
    /// Randomly reorders the elements in the specified range.
    /// </summary>
    /// <param name="begin">The start of the range.</param>
    /// <param name="end">The end of the range.</param>
    /// <param name="e">The random_engine to use.</param>
    template <typename It>
    void shuffle(It begin, It end, random_engine& e) {
        std::shuffle(begin, end, e._engine);
    }

}

// =======================================================================================
#endif
