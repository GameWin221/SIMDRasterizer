#ifndef SIMD_EXPERIMENT_TRIGONOMETRY_HPP
#define SIMD_EXPERIMENT_TRIGONOMETRY_HPP

#include "math_config.hpp"
#include <cmath>

#ifdef MATH_ENABLE_SIMD
#include <intrin.h>
#endif

namespace math {
    inline f32 sin(f32 a) {
        return std::sin(a);
    }

    inline f32 cos(f32 a) {
        return std::cos(a);
    }

    inline f32 tan(f32 a) {
        return std::tan(a);
    }
}

#endif
