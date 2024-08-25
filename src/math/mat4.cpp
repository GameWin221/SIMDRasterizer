#include "mat.hpp"
#include "trigonometry.hpp"

#ifdef MATH_ENABLE_SIMD
#include <intrin.h>
#endif

namespace math {
    mat4::mat4() = default;

    mat4::mat4(f32 a) {
        m[0][0] = a;
        m[1][1] = a;
        m[2][2] = a;
        m[3][3] = a;
    }

    mat4::mat4(
        f32 m00, f32 m10, f32 m20, f32 m30,
        f32 m01, f32 m11, f32 m21, f32 m31,
        f32 m02, f32 m12, f32 m22, f32 m32,
        f32 m03, f32 m13, f32 m23, f32 m33
    ) {
        m[0][0] = m00;
        m[1][0] = m01;
        m[2][0] = m02;
        m[3][0] = m03;

        m[0][1] = m10;
        m[1][1] = m11;
        m[2][1] = m12;
        m[3][1] = m13;

        m[0][2] = m20;
        m[1][2] = m21;
        m[2][2] = m22;
        m[3][2] = m23;

        m[0][3] = m30;
        m[1][3] = m31;
        m[2][3] = m32;
        m[3][3] = m33;
    }

#ifndef MATH_ENABLE_SIMD
    mat4 mat4::operator+(f32 a) const {
        return mat4{
            m[0][0] + a, m[0][1] + a, m[0][2] + a, m[0][3] + a,
            m[1][0] + a, m[1][1] + a, m[1][2] + a, m[1][3] + a,
            m[2][0] + a, m[2][1] + a, m[2][2] + a, m[2][3] + a,
            m[3][0] + a, m[3][1] + a, m[3][2] + a, m[3][3] + a
        };
    }
    mat4 mat4::operator-(f32 a) const {
        return mat4{
            m[0][0] - a, m[0][1] - a, m[0][2] - a, m[0][3] - a,
            m[1][0] - a, m[1][1] - a, m[1][2] - a, m[1][3] - a,
            m[2][0] - a, m[2][1] - a, m[2][2] - a, m[2][3] - a,
            m[3][0] - a, m[3][1] - a, m[3][2] - a, m[3][3] - a
        };
    }
    mat4 mat4::operator*(f32 a) const {
        return mat4{
            m[0][0] * a, m[0][1] * a, m[0][2] * a, m[0][3] * a,
            m[1][0] * a, m[1][1] * a, m[1][2] * a, m[1][3] * a,
            m[2][0] * a, m[2][1] * a, m[2][2] * a, m[2][3] * a,
            m[3][0] * a, m[3][1] * a, m[3][2] * a, m[3][3] * a
        };
    }
    mat4 mat4::operator/(f32 a) const {
        return mat4{
            m[0][0] / a, m[0][1] / a, m[0][2] / a, m[0][3] / a,
            m[1][0] / a, m[1][1] / a, m[1][2] / a, m[1][3] / a,
            m[2][0] / a, m[2][1] / a, m[2][2] / a, m[2][3] / a,
            m[3][0] / a, m[3][1] / a, m[3][2] / a, m[3][3] / a
        };
    }

    mat4 mat4::operator+=(f32 a) {
        m[0][0] += a; m[0][1] += a; m[0][2] += a; m[0][3] += a;
        m[1][0] += a; m[1][1] += a; m[1][2] += a; m[1][3] += a;
        m[2][0] += a; m[2][1] += a; m[2][2] += a; m[2][3] += a;
        m[3][0] += a; m[3][1] += a; m[3][2] += a; m[3][3] += a;
        return *this;
    }
    mat4 mat4::operator-=(f32 a) {
        m[0][0] -= a; m[0][1] -= a; m[0][2] -= a; m[0][3] -= a;
        m[1][0] -= a; m[1][1] -= a; m[1][2] -= a; m[1][3] -= a;
        m[2][0] -= a; m[2][1] -= a; m[2][2] -= a; m[2][3] -= a;
        m[3][0] -= a; m[3][1] -= a; m[3][2] -= a; m[3][3] -= a;
        return *this;
    }
    mat4 mat4::operator*=(f32 a) {
        m[0][0] *= a; m[0][1] *= a; m[0][2] *= a; m[0][3] *= a;
        m[1][0] *= a; m[1][1] *= a; m[1][2] *= a; m[1][3] *= a;
        m[2][0] *= a; m[2][1] *= a; m[2][2] *= a; m[2][3] *= a;
        m[3][0] *= a; m[3][1] *= a; m[3][2] *= a; m[3][3] *= a;
        return *this;
    }
    mat4 mat4::operator/=(f32 a) {
        m[0][0] /= a; m[0][1] /= a; m[0][2] /= a; m[0][3] /= a;
        m[1][0] /= a; m[1][1] /= a; m[1][2] /= a; m[1][3] /= a;
        m[2][0] /= a; m[2][1] /= a; m[2][2] /= a; m[2][3] /= a;
        m[3][0] /= a; m[3][1] /= a; m[3][2] /= a; m[3][3] /= a;
        return *this;
    }

    mat4 mat4::operator*(const mat4 &v) const {
        mat4 result{};

        for (u32 i{}; i < 4u; ++i) {
            for (u32 j{}; j < 4u; ++j) {
                for (u32 k{}; k < 4u; ++k) {
                    result.m[i][j] += m[k][j] * v.m[i][k];
                }
            }
        }

        return result;
    }
    vec4 mat4::operator*(const vec4 &v) const {
        return vec4{
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
            m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w
        };
    }
#else
    mat4 mat4::operator+(f32 a) const {
        mat4 result = *this;

#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 srcrow01 = _mm256_load_ps(result.m[0]);
        __m256 srcrow23 = _mm256_load_ps(result.m[2]);

        __m256 dstrow01 = _mm256_add_ps(srcrow01, scalar);
        __m256 dstrow23 = _mm256_add_ps(srcrow23, scalar);

        _mm256_store_ps(result.m[0], dstrow01);
        _mm256_store_ps(result.m[2], dstrow23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 scalar = _mm_set1_ps(a);

        __m128 srcrow0 = _mm_load_ps(result.m[0]);
        __m128 srcrow1 = _mm_load_ps(result.m[1]);
        __m128 srcrow2 = _mm_load_ps(result.m[2]);
        __m128 srcrow3 = _mm_load_ps(result.m[3]);

        __m128 dstrow0 = _mm_add_ps(srcrow0, scalar);
        __m128 dstrow1 = _mm_add_ps(srcrow1, scalar);
        __m128 dstrow2 = _mm_add_ps(srcrow2, scalar);
        __m128 dstrow3 = _mm_add_ps(srcrow3, scalar);

        _mm_store_ps(result.m[0], dstrow0);
        _mm_store_ps(result.m[1], dstrow1);
        _mm_store_ps(result.m[2], dstrow2);
        _mm_store_ps(result.m[3], dstrow3);
#endif

        return result;
    }
    mat4 mat4::operator-(f32 a) const {
        mat4 result = *this;

#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 srcrow01 = _mm256_load_ps(result.m[0]);
        __m256 srcrow23 = _mm256_load_ps(result.m[2]);

        __m256 dstrow01 = _mm256_sub_ps(srcrow01, scalar);
        __m256 dstrow23 = _mm256_sub_ps(srcrow23, scalar);

        _mm256_store_ps(result.m[0], dstrow01);
        _mm256_store_ps(result.m[2], dstrow23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 scalar = _mm_set1_ps(a);

        __m128 srcrow0 = _mm_load_ps(result.m[0]);
        __m128 srcrow1 = _mm_load_ps(result.m[1]);
        __m128 srcrow2 = _mm_load_ps(result.m[2]);
        __m128 srcrow3 = _mm_load_ps(result.m[3]);

        __m128 dstrow0 = _mm_sub_ps(srcrow0, scalar);
        __m128 dstrow1 = _mm_sub_ps(srcrow1, scalar);
        __m128 dstrow2 = _mm_sub_ps(srcrow2, scalar);
        __m128 dstrow3 = _mm_sub_ps(srcrow3, scalar);

        _mm_store_ps(result.m[0], dstrow0);
        _mm_store_ps(result.m[1], dstrow1);
        _mm_store_ps(result.m[2], dstrow2);
        _mm_store_ps(result.m[3], dstrow3);
#endif

        return result;
    }
    mat4 mat4::operator*(f32 a) const {
        mat4 result = *this;

#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 srcrow01 = _mm256_load_ps(result.m[0]);
        __m256 srcrow23 = _mm256_load_ps(result.m[2]);

        __m256 dstrow01 = _mm256_mul_ps(srcrow01, scalar);
        __m256 dstrow23 = _mm256_mul_ps(srcrow23, scalar);

        _mm256_store_ps(result.m[0], dstrow01);
        _mm256_store_ps(result.m[2], dstrow23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 scalar = _mm_set1_ps(a);

        __m128 srcrow0 = _mm_load_ps(result.m[0]);
        __m128 srcrow1 = _mm_load_ps(result.m[1]);
        __m128 srcrow2 = _mm_load_ps(result.m[2]);
        __m128 srcrow3 = _mm_load_ps(result.m[3]);

        __m128 dstrow0 = _mm_mul_ps(srcrow0, scalar);
        __m128 dstrow1 = _mm_mul_ps(srcrow1, scalar);
        __m128 dstrow2 = _mm_mul_ps(srcrow2, scalar);
        __m128 dstrow3 = _mm_mul_ps(srcrow3, scalar);

        _mm_store_ps(result.m[0], dstrow0);
        _mm_store_ps(result.m[1], dstrow1);
        _mm_store_ps(result.m[2], dstrow2);
        _mm_store_ps(result.m[3], dstrow3);
#endif

        return result;
    }
    mat4 mat4::operator/(f32 a) const {
        mat4 result = *this;

#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 srcrow01 = _mm256_load_ps(result.m[0]);
        __m256 srcrow23 = _mm256_load_ps(result.m[2]);

        __m256 dstrow01 = _mm256_div_ps(srcrow01, scalar);
        __m256 dstrow23 = _mm256_div_ps(srcrow23, scalar);

        _mm256_store_ps(result.m[0], dstrow01);
        _mm256_store_ps(result.m[2], dstrow23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 scalar = _mm_set1_ps(a);

        __m128 srcrow0 = _mm_load_ps(result.m[0]);
        __m128 srcrow1 = _mm_load_ps(result.m[1]);
        __m128 srcrow2 = _mm_load_ps(result.m[2]);
        __m128 srcrow3 = _mm_load_ps(result.m[3]);

        __m128 dstrow0 = _mm_div_ps(srcrow0, scalar);
        __m128 dstrow1 = _mm_div_ps(srcrow1, scalar);
        __m128 dstrow2 = _mm_div_ps(srcrow2, scalar);
        __m128 dstrow3 = _mm_div_ps(srcrow3, scalar);

        _mm_store_ps(result.m[0], dstrow0);
        _mm_store_ps(result.m[1], dstrow1);
        _mm_store_ps(result.m[2], dstrow2);
        _mm_store_ps(result.m[3], dstrow3);
#endif

        return result;
    }

    mat4 mat4::operator+=(f32 a) {
#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 src01 = _mm256_load_ps(m[0]);
        __m256 src23 = _mm256_load_ps(m[2]);

        __m256 dst01 = _mm256_add_ps(src01, scalar);
        __m256 dst23 = _mm256_add_ps(src23, scalar);

        _mm256_store_ps(m[0], dst01);
        _mm256_store_ps(m[2], dst23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 src0 = _mm_load_ps(m[0]);
        __m128 src1 = _mm_load_ps(m[1]);
        __m128 src2 = _mm_load_ps(m[2]);
        __m128 src3 = _mm_load_ps(m[3]);

        __m128 scalar = _mm_set_ps1(a);

        __m128 dst = _mm_add_ps(src0, scalar);
        __m128 dst = _mm_add_ps(src1, scalar);
        __m128 dst = _mm_add_ps(src2, scalar);
        __m128 dst = _mm_add_ps(src3, scalar);

        _mm_store_ps(m[0], dst0);
        _mm_store_ps(m[1], dst1);
        _mm_store_ps(m[2], dst2);
        _mm_store_ps(m[3], dst3);
#endif

        return *this;
    }
    mat4 mat4::operator-=(f32 a) {
#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 src01 = _mm256_load_ps(m[0]);
        __m256 src23 = _mm256_load_ps(m[2]);

        __m256 dst01 = _mm256_sub_ps(src01, scalar);
        __m256 dst23 = _mm256_sub_ps(src23, scalar);

        _mm256_store_ps(m[0], dst01);
        _mm256_store_ps(m[2], dst23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 src0 = _mm_load_ps(m[0]);
        __m128 src1 = _mm_load_ps(m[1]);
        __m128 src2 = _mm_load_ps(m[2]);
        __m128 src3 = _mm_load_ps(m[3]);

        __m128 scalar = _mm_set_ps1(a);

        __m128 dst = _mm_sub_ps(src0, scalar);
        __m128 dst = _mm_sub_ps(src1, scalar);
        __m128 dst = _mm_sub_ps(src2, scalar);
        __m128 dst = _mm_sub_ps(src3, scalar);

        _mm_store_ps(m[0], dst0);
        _mm_store_ps(m[1], dst1);
        _mm_store_ps(m[2], dst2);
        _mm_store_ps(m[3], dst3);
#endif

        return *this;
    }
    mat4 mat4::operator*=(f32 a) {
#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 src01 = _mm256_load_ps(m[0]);
        __m256 src23 = _mm256_load_ps(m[2]);

        __m256 dst01 = _mm256_mul_ps(src01, scalar);
        __m256 dst23 = _mm256_mul_ps(src23, scalar);

        _mm256_store_ps(m[0], dst01);
        _mm256_store_ps(m[2], dst23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 src0 = _mm_load_ps(m[0]);
        __m128 src1 = _mm_load_ps(m[1]);
        __m128 src2 = _mm_load_ps(m[2]);
        __m128 src3 = _mm_load_ps(m[3]);

        __m128 scalar = _mm_set_ps1(a);

        __m128 dst = _mm_mul_ps(src0, scalar);
        __m128 dst = _mm_mul_ps(src1, scalar);
        __m128 dst = _mm_mul_ps(src2, scalar);
        __m128 dst = _mm_mul_ps(src3, scalar);

        _mm_store_ps(m[0], dst0);
        _mm_store_ps(m[1], dst1);
        _mm_store_ps(m[2], dst2);
        _mm_store_ps(m[3], dst3);
#endif
        return *this;
    }
    mat4 mat4::operator/=(f32 a) {
#ifdef MATH_SIMD_AVX
        __m256 scalar = _mm256_broadcast_ss(&a);

        __m256 src01 = _mm256_load_ps(m[0]);
        __m256 src23 = _mm256_load_ps(m[2]);

        __m256 dst01 = _mm256_div_ps(src01, scalar);
        __m256 dst23 = _mm256_div_ps(src23, scalar);

        _mm256_store_ps(m[0], dst01);
        _mm256_store_ps(m[2], dst23);
#endif

#ifdef MATH_SIMD_SSE
        __m128 src0 = _mm_load_ps(m[0]);
        __m128 src1 = _mm_load_ps(m[1]);
        __m128 src2 = _mm_load_ps(m[2]);
        __m128 src3 = _mm_load_ps(m[3]);

        __m128 scalar = _mm_set_ps1(a);

        __m128 dst = _mm_div_ps(src0, scalar);
        __m128 dst = _mm_div_ps(src1, scalar);
        __m128 dst = _mm_div_ps(src2, scalar);
        __m128 dst = _mm_div_ps(src3, scalar);

        _mm_store_ps(m[0], dst0);
        _mm_store_ps(m[1], dst1);
        _mm_store_ps(m[2], dst2);
        _mm_store_ps(m[3], dst3);
#endif
        return *this;
    }

    mat4 mat4::operator*(const mat4 &v) const {
        mat4 result{};

#if defined(MATH_SIMD_SSE) || defined(MATH_SIMD_AVX)
        // Use as many registers as the compiler ever needs to improve pipelining
        __m128 e0, e1, e2, e3, m0, m1, m2, m3, r0, r1, r2, r3;

#if defined(__clang__)
#pragma clang loop unroll(full)
#elif defined(__GNUC__)
#pragma GCC unroll 4
#endif
        for (uint32_t i{}; i < 4u; ++i) {
            e0 = _mm_set_ps1(v.m[i][0]);
            e1 = _mm_set_ps1(v.m[i][1]);
            e2 = _mm_set_ps1(v.m[i][2]);
            e3 = _mm_set_ps1(v.m[i][3]);

            r0 = _mm_load_ps(m[0]);
            r1 = _mm_load_ps(m[1]);
            r2 = _mm_load_ps(m[2]);
            r3 = _mm_load_ps(m[3]);

            m0 = _mm_mul_ps(r0, e0);
            m1 = _mm_mul_ps(r1, e1);
            m2 = _mm_mul_ps(r2, e2);
            m3 = _mm_mul_ps(r3, e3);

            _mm_store_ps(result.m[i], _mm_add_ps(_mm_add_ps(m0, m1), _mm_add_ps(m2, m3)));
        }
#endif

        return result;
    }
    vec4 mat4::operator*(const vec4 &v) const {
        __m128 result{};

#if defined(MATH_SIMD_SSE) || defined(MATH_SIMD_AVX)
        // Use as many registers as the compiler ever needs to improve pipelining
        __m128 r0, r1, r2, r3, x, y, z, w, a, b, c, d;
        r0 = _mm_load_ps(m[0]);
        r1 = _mm_load_ps(m[1]);
        r2 = _mm_load_ps(m[2]);
        r3 = _mm_load_ps(m[3]);

        x = _mm_set1_ps(v.x);
        y = _mm_set1_ps(v.y);
        z = _mm_set1_ps(v.z);
        w = _mm_set1_ps(v.w);

        a = _mm_mul_ps(r0, x);
        b = _mm_mul_ps(r1, y);
        c = _mm_mul_ps(r2, z);
        d = _mm_mul_ps(r3, w);

        result = _mm_add_ps(_mm_add_ps(c, d), _mm_add_ps(a,b));
#endif

        return *(vec4*)&result;
    }
#endif

    mat4 mat4::perspective(f32 fov_y, f32 aspect, f32 near_plane, f32 far_plane) {
        mat4 p{};

        f32 half_tan = math::tan(fov_y / 2.0f);

        p.m[0][0] = 1.0f / (half_tan * aspect);
        p.m[1][1] = -1.0f / half_tan;
        p.m[2][2] = far_plane / (near_plane - far_plane);
        p.m[2][3] = -1.0f;
        p.m[3][2] = -(far_plane * near_plane) / (far_plane - near_plane);

        return p;
    }

    mat4 mat4::orthogonal(f32 w, f32 h, f32 near_plane, f32 far_plane) {
        mat4 o(1.0f);

        o.m[0][0] = 2.0f / w;
        o.m[1][1] = -2.0f / h;
        o.m[2][2] = -1.0f / (far_plane - near_plane);
        o.m[3][2] = -near_plane / (far_plane - near_plane);

        return o;
    }

    mat4 mat4::look_at(const vec3 &position, const vec3 &target) {
        vec3 forward = (target - position).normalized();
        vec3 right = (WORLD_UP.cross(forward)).normalized();
        vec3 up = forward.cross(right);

        return mat4{
            right.x, up.x, -forward.x, 0.0f,
            right.y, up.y, -forward.y, 0.0f,
            right.z, up.z, -forward.z, 0.0f,
            right.dot(position), up.dot(position), forward.dot(position), 1.0f
        };
    }
}
