#include "vec.hpp"
#include "scalar.hpp"

namespace math {
#ifndef MATH_ENABLE_SIMD
    vec2 vec2::operator+(f32 a) const {
        return vec2{x + a, y + a};
    }
    vec2 vec2::operator-(f32 a) const {
        return vec2{x - a, y - a};
    }
    vec2 vec2::operator*(f32 a) const {
        return vec2{x * a, y * a};
    }
    vec2 vec2::operator/(f32 a) const {
        return vec2{x / a, y / a};
    }

    vec2 vec2::operator+(const vec2 &v) const {
        return vec2{x + v.x, y + v.y};
    }
    vec2 vec2::operator-(const vec2 &v) const {
        return vec2{x - v.x, y - v.y};
    }
    vec2 vec2::operator*(const vec2 &v) const {
        return vec2{x * v.x, y * v.y};
    }
    vec2 vec2::operator/(const vec2 &v) const {
        return vec2{x / v.x, y / v.y};
    }

    vec2 vec2::min(const vec2 &v) const {
        return vec2{ std::min(x, v.x), std::min(y, v.y) };
    }
    vec2 vec2::max(const vec2 &v) const {
        return vec2{ std::max(x, v.x), std::max(y, v.y) };
    }

    f32 vec2::dot(const vec2 &v) const {
        return x * v.x + y * v.y;
    }
    f32 vec2::magnitude() const {
        return math::length(x, y);
    }
#else
    vec2 vec2::operator+(f32 a) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_add_ps(vec, scalar));
#endif

        return result;
    }
    vec2 vec2::operator-(f32 a) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_sub_ps(vec, scalar));
#endif

        return result;
    }
    vec2 vec2::operator*(f32 a) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_mul_ps(vec, scalar));
#endif

        return result;
    }
    vec2 vec2::operator/(f32 a) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_div_ps(vec, scalar));
#endif

        return result;
    }

    vec2 vec2::operator+(const vec2 &v) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_add_ps(a, b));
#endif

        return result;
    }
    vec2 vec2::operator-(const vec2 &v) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_sub_ps(a, b));
#endif

        return result;
    }
    vec2 vec2::operator*(const vec2 &v) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_mul_ps(a, b));
#endif

        return result;
    }
    vec2 vec2::operator/(const vec2 &v) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_div_ps(a, b));
#endif

        return result;
    }

    vec2 vec2::min(const vec2 &v) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_min_ps(a, b));
#endif

        return result;
    }
    vec2 vec2::max(const vec2 &v) const {
        vec2 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_max_ps(a, b));
#endif

        return result;
    }

    f32 vec2::dot(const vec2 &v) const {
        f32 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);

        __m128 mul = _mm_mul_ps(a, b);
        mul = _mm_hadd_ps(mul, mul);
        mul = _mm_hadd_ps(mul, mul);
        _mm_store_ss(&result, mul);
#endif

        return result;
    }
    f32 vec2::magnitude() const {
        f32 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);

        __m128 sqr = _mm_mul_ps(vec, vec);
        sqr = _mm_hadd_ps(sqr, sqr);
        sqr = _mm_hadd_ps(sqr, sqr);
        _mm_store_ss(&result, _mm_sqrt_ss(sqr));
#endif

        return result;
    }
#endif

    vec2 vec2::normalized() const {
        return *this / magnitude();
    }
}