/*
 * Copyright (c) 2023 - 2024 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "lottie_interpretor.h"
#include <Corrade/Containers/Pointer.h>
#include <mutex>
#include <unordered_map>
/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

#define NEWTON_MIN_SLOPE 0.02f
#define NEWTON_ITERATIONS 4
#define SUBDIVISION_PRECISION 0.0000001f
#define SUBDIVISION_MAX_ITERATIONS 10

namespace {
std::unordered_map<std::string, Corrade::Containers::Pointer<LottieInterpolator>> interpolators;
std::mutex g_interpolatror_mutex;
}
static constexpr float k_sample_step_size = 1.0f / float(SPLINE_TABLE_SIZE - 1);

static inline float const_a(float a_a1, float a_a2)
{
    return 1.0f - 3.0f * a_a2 + 3.0f * a_a1;
}
static inline float const_b(float a_a1, float a_a2)
{
    return 3.0f * a_a2 - 6.0f * a_a1;
}
static inline float const_c(float a_a1)
{
    return 3.0f * a_a1;
}

static inline float get_slope(float t, float a_a1, float a_a2)
{
    return 3.0f * const_a(a_a1, a_a2) * t * t + 2.0f * const_b(a_a1, a_a2) * t + const_c(a_a1);
}

static inline float calc_bezier(float t, float a_a1, float a_a2)
{
    return ((const_a(a_a1, a_a2) * t + const_b(a_a1, a_a2)) * t + const_c(a_a1)) * t;
}

float LottieInterpolator::get_t_for_x(float a_x) const
{
    //Find interval where t lies
    auto interval_start = 0.0f;
    auto current_sample = &samples[1];
    auto last_sample = &samples[SPLINE_TABLE_SIZE - 1];

    for (; current_sample != last_sample && *current_sample <= a_x; ++current_sample) {
        interval_start += k_sample_step_size;
    }

    --current_sample; // t now lies between *currentSample and *currentSample+1

    // Interpolate to provide an initial guess for t
    auto dist = (a_x - *current_sample) / (*(current_sample + 1) - *current_sample);
    auto guess_for_t = interval_start + dist * k_sample_step_size;

    // Check the slope to see what strategy to use. If the slope is too small
    // Newton-Raphson iteration won't converge on a root so we use bisection
    // instead.
    auto initial_slope = get_slope(guess_for_t, m_out_tangent.x(), m_in_tangent.x());
    if (initial_slope >= NEWTON_MIN_SLOPE)
        return newton_raphson_iterate(a_x, guess_for_t);
    else if (initial_slope == 0.0)
        return guess_for_t;
    else
        return binary_subdivide(a_x, interval_start, interval_start + k_sample_step_size);
}

float LottieInterpolator::binary_subdivide(float a_x, float a_a, float a_b) const
{
    float x, t;
    int i = 0;

    do {
        t = a_a + (a_b - a_a) / 2.0f;
        x = calc_bezier(t, m_out_tangent.x(), m_in_tangent.x()) - a_x;
        if (x > 0.0f)
            a_b = t;
        else
            a_a = t;
    } while (fabsf(x) > SUBDIVISION_PRECISION && ++i < SUBDIVISION_MAX_ITERATIONS);
    return t;
}

float LottieInterpolator::newton_raphson_iterate(float a_x, float a_guess_t) const
{
    // Refine guess with Newton-Raphson iteration
    for (int i = 0; i < NEWTON_ITERATIONS; ++i) {
        // We're trying to find where f(t) = aX,
        // so we're actually looking for a root for: CalcBezier(t) - aX
        auto current_x = calc_bezier(a_guess_t, m_out_tangent.x(), m_in_tangent.x()) - a_x;
        auto current_slope = get_slope(a_guess_t, m_out_tangent.x(), m_in_tangent.x());
        if (current_slope == 0.0f)
            return a_guess_t;
        a_guess_t -= current_x / current_slope;
    }
    return a_guess_t;
}

/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

float LottieInterpolator::progress(float t) const
{
    if (m_out_tangent.x() == m_out_tangent.y() && m_in_tangent.x() == m_in_tangent.y())
        return t;
    return calc_bezier(get_t_for_x(t), m_out_tangent.y(), m_in_tangent.y());
}

void LottieInterpolator::set(const char *key,
                             const alive::Point2D &in_tangent,
                             const alive::Point2D &out_tangent)
{
    this->m_key = key;
    this->m_in_tangent = in_tangent;
    this->m_out_tangent = out_tangent;

    if (out_tangent.x() == out_tangent.y() && in_tangent.x() == in_tangent.y())
        return;

    //calculates sample values
    for (int i = 0; i < SPLINE_TABLE_SIZE; ++i) {
        samples[i] = calc_bezier(float(i) * k_sample_step_size, out_tangent.x(), in_tangent.x());
    }
}

#include <iostream>
LottieInterpolator *get_interpolator(const alive::Point2D &in, const alive::Point2D &out)
{
    char buf[20];
    snprintf(buf, sizeof(buf), "%.2f_%.2f_%.2f_%.2f", in.x(), in.y(), out.x(), out.y());
    LottieInterpolator *result = nullptr;
    {
        std::lock_guard<std::mutex> lg(g_interpolatror_mutex);
        auto it = interpolators.find(std::string(buf));
        if (it != interpolators.end()) {
            result = it->second.get();
        } else {
            auto ptr = Corrade::Containers::pointer<LottieInterpolator>();
            ptr->set(buf, in, out);
            result = ptr.release();
            interpolators.emplace(buf, result);
        }
    }
    return result;
}
