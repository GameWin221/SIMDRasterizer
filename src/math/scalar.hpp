#ifndef SIMD_EXPERIMENT_SCALAR_HPP
#define SIMD_EXPERIMENT_SCALAR_HPP

#include "math_config.hpp"
#include <numbers>
#include <cmath>
#include <types.hpp>

#ifdef MATH_ENABLE_SIMD
#include <intrin.h>
#endif

namespace math {
    inline constexpr f32 rad_to_deg(f32 radians) {
        return radians / (std::numbers::pi_v<f32> * 180.0f);
    }

    inline constexpr f32 deg_to_rad(f32 degrees) {
        return degrees * (std::numbers::pi_v<f32> / 180.0f);
    }

    inline f32 sqrt(f32 a) {
        return std::sqrt(a);
    }
    inline f32 length(f32 a, f32 b, f32 c) {
        return std::hypot(a, b, c);
    }
    inline f32 length(f32 a, f32 b) {
        return std::hypot(a, b);
    }
}

#endif
