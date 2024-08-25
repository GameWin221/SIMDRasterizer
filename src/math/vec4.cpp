#include "vec.hpp"
#include "scalar.hpp"

namespace math {
    vec2 vec4::xy() const {
        return vec2{x, y};
    }
    vec3 vec4::xyz() const {
        return vec3{x, y, z};
    }

#ifndef MATH_ENABLE_SIMD
    vec4 vec4::operator+(f32 a) const {
        return vec4{x + a, y + a, z + a, w + a};
    }
    vec4 vec4::operator-(f32 a) const {
        return vec4{x - a, y - a, z - a, w - a};
    }
    vec4 vec4::operator*(f32 a) const {
        return vec4{x * a, y * a, z * a, w * a};
    }
    vec4 vec4::operator/(f32 a) const {
        return vec4{x / a, y / a, z / a, w / a};
    }

    vec4 vec4::operator+(const vec4 &v) const {
        return vec4{x + v.x, y + v.y, z + v.z, w + v.w};
    }
    vec4 vec4::operator-(const vec4 &v) const {
        return vec4{x - v.x, y - v.y, z - v.z, w + v.w};
    }
    vec4 vec4::operator*(const vec4 &v) const {
        return vec4{x * v.x, y * v.y, z * v.z, w + v.w};
    }
    vec4 vec4::operator/(const vec4 &v) const {
        return vec4{x / v.x, y / v.y, z / v.z, w + v.w};
    }

    vec4 vec4::min(const vec4 &v) const {
        return vec4{ std::min(x, v.x), std::min(y, v.y), std::min(z, v.z), std::min(w, v.w) };
    }
    vec4 vec4::max(const vec4 &v) const {
        return vec4{ std::max(x, v.x), std::max(y, v.y), std::max(z, v.z), std::max(w, v.w) };
    }

    f32 vec4::dot(const vec4 &v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }
    f32 vec4::magnitude() const {
        return math::sqrt(x * x + y * y + z * z + w * w);
    }
#else
    vec4 vec4::operator+(f32 a) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_add_ps(vec, scalar));
#endif

        return result;
    }
    vec4 vec4::operator-(f32 a) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_sub_ps(vec, scalar));
#endif

        return result;
    }
    vec4 vec4::operator*(f32 a) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_mul_ps(vec, scalar));
#endif

        return result;
    }
    vec4 vec4::operator/(f32 a) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_div_ps(vec, scalar));
#endif

        return result;
    }

    vec4 vec4::operator+(const vec4 &v) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_add_ps(a, b));
#endif

        return result;
    }
    vec4 vec4::operator-(const vec4 &v) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_sub_ps(a, b));
#endif

        return result;
    }
    vec4 vec4::operator*(const vec4 &v) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_mul_ps(a, b));
#endif

        return result;
    }
    vec4 vec4::operator/(const vec4 &v) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_div_ps(a, b));
#endif

        return result;
    }

    vec4 vec4::min(const vec4 &v) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_min_ps(a, b));
#endif

        return result;
    }
    vec4 vec4::max(const vec4 &v) const {
        vec4 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_max_ps(a, b));
#endif

        return result;
    }

    f32 vec4::dot(const vec4 &v) const {
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
    f32 vec4::magnitude() const {
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

    vec4 vec4::normalized() const {
        return *this / magnitude();
    }
}