#ifndef SIMD_EXPERIMENT_RASTER_HPP
#define SIMD_EXPERIMENT_RASTER_HPP

#include <vector>
#include <intrin.h>

#include "types.hpp"
#include "math/math.hpp"

// Support 4k res max
static constexpr u32 RASTER_MAX_FB_WIDTH = 2560u;
static constexpr u32 RASTER_MAX_FB_HEIGHT = 2160u;

enum struct WindingOrder : u32 {
    CW = 1u,
    CCW = 2u,
};

struct Patch {
    vec4 pos[3]{};
    vec3 normal{};
    vec3 color{};
};

typedef vec4 (*VertexShaderFn)(const vec4 &v_in);
typedef vec4 (*PatchShaderFn)(const Patch &patch, const vec4 &avg_ndc);

// Do not allocate on stack or heap manually! Use it as a global static object.
// It is much faster than using runtime memory.
struct alignas(64) Framebuffer {
    u32 data[RASTER_MAX_FB_WIDTH * RASTER_MAX_FB_HEIGHT]{};
    u32 width{};
    u32 height{};

    inline void fill(u32 value) {
        for (u32 y{}; y < height; ++y) {
            for (u32 x{}; x < width; ++x) {
                data[y * width + x] = value;
            }
        }
    }
};

struct DrawPatchesConfig {
    WindingOrder front_winding = WindingOrder::CCW;
    bool enable_back_cull = true;

    VertexShaderFn vertex_shader_fn{};
    PatchShaderFn patch_shader_fn{};

    Framebuffer *color_buffer{};
    Framebuffer *depth_buffer{};
};

namespace raster {
    constexpr inline u32 rgba_to_u32(const vec4 &rgba) {
        return ((((u32)(rgba.w * 255.0f) & 0xff) << 24) | ((u32)(rgba.x * 255.0f) & 0xff) << 16) | (((u32)(rgba.y * 255.0f) & 0xff) << 8) | ((u32)(rgba.z * 255.0f) & 0xff);
    }

    void draw_patch(const Patch &patch, const DrawPatchesConfig &cfg);
    void draw_patches(const std::vector<Patch> &patches, const DrawPatchesConfig &cfg);
}

#endif