#ifndef SIMD_EXPERIMENT_VEC_HPP
#define SIMD_EXPERIMENT_VEC_HPP

#include "math_config.hpp"
#include <types.hpp>

#ifdef MATH_ENABLE_SIMD
#include <intrin.h>
#endif

namespace math {
    struct alignas(16) vec2 {
        constexpr vec2() : x(0.0f), y(0.0f) {}

        constexpr explicit vec2(f32 a) : x(a), y(a) {}

        constexpr vec2(f32 _x, f32 _y) : x(_x), y(_y) {}

        f32 x{}, y{}, _p0{}, _p1{};

        vec2 operator+(f32 a) const;
        vec2 operator-(f32 a) const;
        vec2 operator*(f32 a) const;
        vec2 operator/(f32 a) const;

        vec2 operator+(const vec2 &v) const;
        vec2 operator-(const vec2 &v) const;
        vec2 operator*(const vec2 &v) const;
        vec2 operator/(const vec2 &v) const;

        vec2 min(const vec2 &v) const;
        vec2 max(const vec2 &v) const;

        f32 dot(const vec2 &v) const;
        f32 magnitude() const;

        vec2 normalized() const;
    };

    struct alignas(16) vec3 {
        constexpr vec3() : x(0.0f), y(0.0f), z(0.0f) {};

        constexpr explicit vec3(f32 a) : x(a), y(a), z(a) {};

        constexpr vec3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {};

        constexpr vec3(const vec2& v, f32 z) : x(v.x), y(v.y), z(z) {}

        f32 x{}, y{}, z{}, _p0{};

        vec2 xy() const;

        vec3 operator+(f32 a) const;
        vec3 operator-(f32 a) const;
        vec3 operator*(f32 a) const;
        vec3 operator/(f32 a) const;

        vec3 operator+(const vec3 &v) const;
        vec3 operator-(const vec3 &v) const;
        vec3 operator*(const vec3 &v) const;
        vec3 operator/(const vec3 &v) const;

        vec3 min(const vec3 &v) const;
        vec3 max(const vec3 &v) const;

        f32 dot(const vec3 &v) const;
        f32 magnitude() const;

        vec3 cross(const vec3 &v) const;

        vec3 normalized() const;
    };

    struct alignas(16) vec4 {
        constexpr vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

        constexpr vec4(f32 a) : x(a), y(a), z(a), w(a) {}

        constexpr vec4(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}

        constexpr vec4(const vec3& v, f32 w) : x(v.x), y(v.y), z(v.z), w(w) {}
        constexpr vec4(const vec2& a, const vec2& b) : x(a.x), y(a.y), z(b.x), w(b.y) {}

        f32 x{}, y{}, z{}, w{};

        vec2 xy() const;
        vec3 xyz() const;

        vec4 operator+(f32 a) const;
        vec4 operator-(f32 a) const;
        vec4 operator*(f32 a) const;
        vec4 operator/(f32 a) const;

        vec4 operator+(const vec4 &v) const;
        vec4 operator-(const vec4 &v) const;
        vec4 operator*(const vec4 &v) const;
        vec4 operator/(const vec4 &v) const;

        vec4 min(const vec4 &v) const;
        vec4 max(const vec4 &v) const;

        f32 dot(const vec4 &v) const;
        f32 magnitude() const;

        vec4 normalized() const;
    };

    static constexpr vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
}

#endif