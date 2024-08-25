#include <cassert>
#include <algorithm>
#include <chrono>

#include "raster.hpp"

static inline void fill_patch_color(Framebuffer *color_dst, i32 min_x_i, i32 min_y_i, i32 max_x_i, i32 max_y_i, u32 color32) {
    u32 width = color_dst->width;
    for (i32 y = min_y_i; y < max_y_i; ++y) {
        for (i32 x = min_x_i; x < max_x_i; ++x) {
            color_dst->data[y * width + x] = color32;
        }
    }
}
static inline void fill_patch_depth(Framebuffer *depth_dst, i32 min_x_i, i32 min_y_i, i32 max_x_i, i32 max_y_i, u32 depth32) {
    u32 width = depth_dst->width;
    u32 idx;
    for (i32 y = min_y_i; y < max_y_i; ++y) {
        for (i32 x = min_x_i; x < max_x_i; ++x) {
            idx = y * width + x;

            if (depth32 < depth_dst->data[idx]) {
                depth_dst->data[idx] = depth32;
            }
        }
    }
}
static inline void fill_patch_color_depth(Framebuffer *color_dst, Framebuffer *depth_dst, i32 min_x_i, i32 min_y_i, i32 max_x_i, i32 max_y_i, u32 color32, u32 depth32) {
    assert(color_dst->width == depth_dst->width);
    assert(color_dst->height == depth_dst->height);

    u32 width = color_dst->width;

    u32 idx;
    for (i32 y = min_y_i; y < max_y_i; ++y) {
        for (i32 x = min_x_i; x < max_x_i; ++x) {
            idx = y * width + x;

            if (depth32 < depth_dst->data[idx]) {
                depth_dst->data[idx] = depth32;
                color_dst->data[idx] = color32;
            }
        }
    }
}

static WindingOrder get_winding_order(const vec4 &ndc0, const vec4 &ndc1, const vec4 &ndc2) {
    /// Determinant of such matrix:
    // | ndc0.x ndc0.y 1.0 |
    // | ndc1.x ndc1.y 1.0 |
    // | ndc2.x ndc2.y 1.0 |

    return ((
        ndc0.x * ndc1.y + ndc0.y * ndc2.x + ndc1.x * ndc2.y - ndc1.y * ndc2.x - ndc0.y * ndc1.x - ndc0.x * ndc2.y
    ) < 0.0f) ? WindingOrder::CW : WindingOrder::CCW;
}

void raster::draw_patch(const Patch &patch, const DrawPatchesConfig &cfg) {
    vec4 v0_ndc = cfg.vertex_shader_fn(patch.pos[0]);
    vec4 v1_ndc = cfg.vertex_shader_fn(patch.pos[1]);
    vec4 v2_ndc = cfg.vertex_shader_fn(patch.pos[2]);

    WindingOrder tri_winding = get_winding_order(v0_ndc, v1_ndc, v2_ndc);
    if (tri_winding != cfg.front_winding && cfg.enable_back_cull) {
        return;
    }

    f32 min_x = std::min(v0_ndc.x, std::min(v1_ndc.x, v2_ndc.x));
    f32 min_y = std::min(v0_ndc.y, std::min(v1_ndc.y, v2_ndc.y));
    f32 min_z = std::min(v0_ndc.z, std::min(v1_ndc.z, v2_ndc.z));
    f32 max_x = std::max(v0_ndc.x, std::max(v1_ndc.x, v2_ndc.x));
    f32 max_y = std::max(v0_ndc.y, std::max(v1_ndc.y, v2_ndc.y));
    f32 max_z = std::max(v0_ndc.z, std::max(v1_ndc.z, v2_ndc.z));

    if (max_x <= -1.0f || min_x >= 1.0f || max_y <= -1.0f || min_y >= 1.0f || min_z <= 0.0f || max_z >= 1.0f) {
        return;
    }

    u32 width{}, height{};
    if (cfg.color_buffer != nullptr) {
        width = cfg.color_buffer->width;
        height = cfg.color_buffer->height;
    } else if (cfg.depth_buffer != nullptr) {
        width = cfg.depth_buffer->width;
        height = cfg.depth_buffer->height;
    }

    i32 min_x_i = std::max(static_cast<i32>(std::floor((min_x * 0.5f + 0.5f) * static_cast<f32>(width))), 0);
    i32 min_y_i = std::max(static_cast<i32>(std::floor((min_y * 0.5f + 0.5f) * static_cast<f32>(height))), 0);
    i32 max_x_i = std::min(static_cast<i32>(std::ceil((max_x * 0.5f + 0.5f) * static_cast<f32>(width))), static_cast<i32>(width));
    i32 max_y_i = std::min(static_cast<i32>(std::ceil((max_y * 0.5f + 0.5f) * static_cast<f32>(height))), static_cast<i32>(height));

    vec4 ndc_avg = (v0_ndc + v1_ndc + v2_ndc) / 3.0f;

    constexpr const f32 max_depth_f = static_cast<f32>(UINT32_MAX);

    u32 depth32 = static_cast<u32>(ndc_avg.z * max_depth_f);

    if (cfg.color_buffer != nullptr) {
        vec4 color = cfg.patch_shader_fn(patch, ndc_avg).max(0.0f).min(1.0f);

        u32 color32 = rgba_to_u32(color);

        if (cfg.depth_buffer != nullptr) {
            fill_patch_color_depth(cfg.color_buffer, cfg.depth_buffer, min_x_i, min_y_i, max_x_i, max_y_i, color32, depth32);
        } else {
            fill_patch_color(cfg.color_buffer, min_x_i, min_y_i, max_x_i, max_y_i, color32);
        }
    } else {
        if (cfg.depth_buffer != nullptr) {
            fill_patch_depth(cfg.depth_buffer, min_x_i, min_y_i, max_x_i, max_y_i, depth32);
        }
    }
}
void raster::draw_patches(const std::vector<Patch> &patches, const DrawPatchesConfig &cfg) {
    if (cfg.color_buffer == nullptr && cfg.depth_buffer == nullptr) {
        return;
    }

    for (const auto &patch : patches) {
        draw_patch(patch, cfg);
    }
}