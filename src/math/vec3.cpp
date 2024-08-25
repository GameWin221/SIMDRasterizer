#include "vec.hpp"
#include "scalar.hpp"

namespace math {
    vec2 vec3::xy() const {
        return vec2{x, y};
    }

#ifndef MATH_ENABLE_SIMD
    vec3 vec3::operator+(f32 a) const {
        return vec3{x + a, y + a, z + a};
    }
    vec3 vec3::operator-(f32 a) const {
        return vec3{x - a, y - a, z - a};
    }
    vec3 vec3::operator*(f32 a) const {
        return vec3{x * a, y * a, z * a};
    }
    vec3 vec3::operator/(f32 a) const {
        return vec3{x / a, y / a, z / a};
    }

    vec3 vec3::operator+(const vec3 &v) const {
        return vec3{x + v.x, y + v.y, z + v.z};
    }
    vec3 vec3::operator-(const vec3 &v) const {
        return vec3{x - v.x, y - v.y, z - v.z};
    }
    vec3 vec3::operator*(const vec3 &v) const {
        return vec3{x * v.x, y * v.y, z * v.z};
    }
    vec3 vec3::operator/(const vec3 &v) const {
        return vec3{x / v.x, y / v.y, z / v.z};
    }

    vec3 vec3::min(const vec3 &v) const {
        return vec3{ std::min(x, v.x), std::min(y, v.y), std::min(z, v.z) };
    }
    vec3 vec3::max(const vec3 &v) const {
        return vec3{ std::max(x, v.x), std::max(y, v.y), std::max(z, v.z) };
    }

    f32 vec3::dot(const vec3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    f32 vec3::magnitude() const {
        return math::length(x, y, z);
    }

    vec3 vec3::cross(const vec3 &v) const {
        return vec3{
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }
#else
    vec3 vec3::operator+(f32 a) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_add_ps(vec, scalar));
#endif

        return result;
    }
    vec3 vec3::operator-(f32 a) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_sub_ps(vec, scalar));
#endif

        return result;
    }
    vec3 vec3::operator*(f32 a) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_mul_ps(vec, scalar));
#endif

        return result;
    }
    vec3 vec3::operator/(f32 a) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);
        __m128 scalar = _mm_set_ps1(a);
        _mm_store_ps((f32*)&result, _mm_div_ps(vec, scalar));
#endif

        return result;
    }

    vec3 vec3::operator+(const vec3 &v) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_add_ps(a, b));
#endif

        return result;
    }
    vec3 vec3::operator-(const vec3 &v) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_sub_ps(a, b));
#endif

        return result;
    }
    vec3 vec3::operator*(const vec3 &v) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_mul_ps(a, b));
#endif

        return result;
    }
    vec3 vec3::operator/(const vec3 &v) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_div_ps(a, b));
#endif

        return result;
    }

    vec3 vec3::min(const vec3 &v) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_min_ps(a, b));
#endif

        return result;
    }
    vec3 vec3::max(const vec3 &v) const {
        vec3 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 a = _mm_load_ps((f32*)this);
        __m128 b = _mm_load_ps((f32*)&v);
        _mm_store_ps((f32*)&result, _mm_max_ps(a, b));
#endif

        return result;
    }

    f32 vec3::dot(const vec3 &v) const {
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
    f32 vec3::magnitude() const {
        f32 result{};

#if defined(MATH_SIMD_AVX) || defined(MATH_SIMD_SSE)
        __m128 vec = _mm_load_ps((f32*)this);

        __m128 sqr = _mm_mul_ps(vec, vec);

        vec4 v; _mm_store_ps((f32*)&v, sqr);
        result = math::sqrt(v.x + v.y + v.z);
        sqr = _mm_hadd_ps(sqr, sqr);
        sqr = _mm_hadd_ps(sqr, sqr);
        _mm_store_ss(&result, _mm_sqrt_ss(sqr));
#endif

        return result;
    }

    vec3 vec3::cross(const vec3 &v) const {
        vec3 result{};

        // vec3 {
        //     x * v.y - v.x * y
        //     y * v.z - v.y * z,
        //     z * v.x - v.z * x
        // };

        //  ||
        //  ||
        // \||/
        //  \/

        // vec3 {
        //     y * v.z - v.y * z,
        //     z * v.x - v.z * x,
        //     x * v.y - v.x * y
        // };

        __m128 this_vec = _mm_load_ps((f32*)this);
        __m128 v_vec = _mm_load_ps((f32*)&v);

        __m128 v_a = _mm_shuffle_ps(v_vec, v_vec, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 this_b = _mm_shuffle_ps(this_vec, this_vec, _MM_SHUFFLE(3, 0, 2, 1));

        __m128 mul0 = _mm_mul_ps(this_vec, v_a);
        __m128 mul1 = _mm_mul_ps(v_vec, this_b);

        __m128 res = _mm_sub_ps(mul0, mul1);

        _mm_store_ps((f32*)&result, _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 0, 2, 1)));

        return result;
    }
#endif

    vec3 vec3::normalized() const {
        return *this / magnitude();
    }
}
