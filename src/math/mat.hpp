#ifndef SIMD_EXPERIMENT_MAT_HPP
#define SIMD_EXPERIMENT_MAT_HPP

#include <types.hpp>

#include "math_config.hpp"
#include "vec.hpp"

// Column major like GLSL by default
namespace math {
    struct alignas(64) mat4 {
        mat4();

        explicit mat4(f32 a);

        mat4(
            f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33
        );

        float m[4][4]{};

        mat4 operator+(f32 a) const;
        mat4 operator-(f32 a) const;
        mat4 operator*(f32 a) const;
        mat4 operator/(f32 a) const;

        mat4 operator+=(f32 a);
        mat4 operator-=(f32 a);
        mat4 operator*=(f32 a);
        mat4 operator/=(f32 a);

        mat4 operator*(const mat4 &v) const;
        vec4 operator*(const vec4 &v) const;

        static mat4 perspective(f32 fov_y, f32 aspect, f32 near_plane, f32 far_plane);
        static mat4 orthogonal(f32 w, f32 h, f32 near_plane, f32 far_plane);
        static mat4 look_at(const vec3 &position, const vec3 &target);
    };
}


#endif
