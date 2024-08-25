#include <iostream>
#include <vector>
#include <chrono>

#include <MiniFB.h>

#include "types.hpp"
#include "math/math.hpp"
#include "raster.hpp"
#include "ply_importer.hpp"

constexpr const char *WINDOW_TITLE = "SIMD Rasterizer";
constexpr u32 FRAMEBUFFER_WIDTH = 960u;
constexpr u32 FRAMEBUFFER_HEIGHT = 540u;

/// TODO:
// +=, -=, *=, /= operators

static mat4 _shader_view_proj_matrix{};
static mat4 _shader_shadow_proj_view_matrix{};
static vec4 _shader_object_position{};
static vec3 _shader_sun_direction = vec3(1.0f).normalized();
static vec3 _shader_sun_color{};

static Framebuffer color_buffer{
    .width = FRAMEBUFFER_WIDTH,
    .height = FRAMEBUFFER_HEIGHT
};
static Framebuffer depth_buffer{
    .width = FRAMEBUFFER_WIDTH,
    .height = FRAMEBUFFER_HEIGHT
};
static Framebuffer shadow_map{
    .width = 256u,
    .height = 256u
};

static vec4 _vertex_shader(const vec4 &v_in) {
    vec4 w_pos = v_in + _shader_object_position;
    vec4 ndc = _shader_view_proj_matrix * w_pos;

    // Faster than creating a new vec4, it won't need ndc.w later anyway
    return ndc / ndc.w;
}
static vec4 _shadow_vertex_shader(const vec4 &v_in) {
    vec4 w_pos = v_in + _shader_object_position;
    vec4 ndc = _shader_shadow_proj_view_matrix * w_pos;

    // Faster than creating a new vec4, it won't need ndc.w later anyway
    return ndc / ndc.w;
}
static vec4 _lit_patch_shader(const Patch &patch, const vec4 &avg_ndc) {
    vec3 light = _shader_sun_color * std::max(patch.normal.dot(_shader_sun_direction), 0.0f);

    vec3 ambient = vec3(0.6f, 0.8f, 1.0f) * 0.25f;

    return vec4(patch.color * (light + ambient), 1.0f);
}
static vec4 _lit_shadow_patch_shader(const Patch &patch, const vec4 &avg_ndc) {
    vec3 light = _shader_sun_color * std::max(patch.normal.dot(_shader_sun_direction), 0.0f);

    vec4 avg_vpos = (patch.pos[0] + patch.pos[1] + patch.pos[2]) / 3.0f;

    vec4 w_pos = avg_vpos + _shader_object_position;
    vec4 shadow_ndc = _shader_shadow_proj_view_matrix * w_pos;
    shadow_ndc = shadow_ndc / shadow_ndc.w;
    u32 shadow32 = static_cast<u32>(std::max(std::min(shadow_ndc.z - 0.01f, 1.0f), 0.0f) * static_cast<f32>(UINT32_MAX));

    u32 y = static_cast<u32>((shadow_ndc.y * 0.5f + 0.5f) * static_cast<f32>(shadow_map.height));
    u32 x = static_cast<u32>((shadow_ndc.x * 0.5f + 0.5f) * static_cast<f32>(shadow_map.width));
    if (shadow_map.data[y * shadow_map.width + x] < shadow32) {
        light = vec3(0.0f);
    }

    vec3 ambient = vec3(0.6f, 0.8f, 1.0f) * 0.25f;

    return vec4(patch.color * (light + ambient), 1.0f);
}
static vec4 _unlit_patch_shader(const Patch &patch, const vec4 &avg_ndc) {
    return vec4(patch.color, 1.0f);
}

i32 main() {
    mfb_window *window = mfb_open_ex(WINDOW_TITLE, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, WF_RESIZABLE);

    static_assert(FRAMEBUFFER_WIDTH <= RASTER_MAX_FB_WIDTH);
    static_assert(FRAMEBUFFER_HEIGHT <= RASTER_MAX_FB_HEIGHT);

    u32 clear_depth = UINT32_MAX;
    u32 clear_color = raster::rgba_to_u32(vec4(0.6f, 0.8f, 1.0f, 0.0f));

    vec3 sun_direction = vec3(0.55f, 1.5f, -1.1f).normalized();
    vec3 sun_color = vec3(0.95f, 0.9f, 0.7f);

    std::vector<Patch> main_mesh_patches{};
    if (!ply_import("res/tree.ply", main_mesh_patches)) {
        std::cout << "Failed to import a mesh!\n";
        return 1;
    }

    std::vector<Patch> sun_mesh_patches{};
    if (!ply_import("res/sun.ply", sun_mesh_patches)) {
        std::cout << "Failed to import a mesh!\n";
        return 1;
    }

    std::cout << "Loaded everything\n";

    auto last_frame_time = std::chrono::high_resolution_clock::now();

    f32 time = std::numbers::pi * 0.9f;
    do {
        auto now = std::chrono::high_resolution_clock::now();
        f32 delta_time = std::chrono::duration<f32>(now - last_frame_time).count();
        last_frame_time = now;

        time += delta_time;

        // Main Update
        auto update_start = std::chrono::high_resolution_clock::now();
            color_buffer.fill(clear_color);
            shadow_map.fill(clear_depth);
            depth_buffer.fill(clear_depth);

            mat4 view = mat4::look_at(vec3(math::sin(time * 0.5f), math::sin(time) * 0.20f + 0.3f, math::cos(time * 0.5f)) * 5.5f, vec3(0.0f));
            mat4 proj = mat4::perspective(math::deg_to_rad(60.0f), static_cast<f32>(color_buffer.width) / static_cast<f32>(color_buffer.height), 0.1f, 80.0f);

            vec3 sun_pos = sun_direction * 40.0f;
            mat4 shadow_view = mat4::look_at(sun_pos, sun_pos - sun_direction);
            mat4 shadow_proj = mat4::orthogonal(10.0f, 10.0f, 0.1f, 80.0f);
        auto update_end = std::chrono::high_resolution_clock::now();

        // Main Render
        auto draw_start = std::chrono::high_resolution_clock::now();
            _shader_shadow_proj_view_matrix = shadow_proj * shadow_view;
            _shader_view_proj_matrix = proj * view;
            _shader_sun_direction = sun_direction;
            _shader_sun_color = sun_color;

            // Shadows
            _shader_object_position = vec4(0.0f);
            raster::draw_patches(main_mesh_patches, DrawPatchesConfig {
                .front_winding = WindingOrder::CW,
                .vertex_shader_fn = _shadow_vertex_shader,
                .depth_buffer = &shadow_map
            });

            // Geometry
            _shader_object_position = vec4(0.0f);
            raster::draw_patches(main_mesh_patches, DrawPatchesConfig {
                .vertex_shader_fn = _vertex_shader,
                .patch_shader_fn = _lit_shadow_patch_shader,
                .color_buffer = &color_buffer,
                .depth_buffer = &depth_buffer
            });

            // Sun
            _shader_object_position = vec4(sun_direction * 14.0f, 0.0f);
            raster::draw_patches(sun_mesh_patches, DrawPatchesConfig {
                .vertex_shader_fn = _vertex_shader,
                .patch_shader_fn = _unlit_patch_shader,
                .color_buffer = &color_buffer,
                .depth_buffer = &depth_buffer
            });
        auto draw_end = std::chrono::high_resolution_clock::now();

        std::cout << "Frametime:\n";
        std::cout << "\tupdate: " << std::chrono::duration<f32>(update_end - update_start).count() * 1000.0f << "ms\n";
        std::cout << "\tdraw: " << std::chrono::duration<f32>(draw_end - draw_start).count() * 1000.0f << "ms\n\n";

        if (mfb_update_ex(window, color_buffer.data, color_buffer.width, color_buffer.height) < 0) {
            window = nullptr;
            std::cout << "Failed to update the window!\n";
            break;
        }
    } while (mfb_wait_sync(window));
}