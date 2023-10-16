#pragma once

#include <filesystem>

#include "bx/allocator.h"
#include "bgfx/bgfx.h"

#include "SDL.h"

#include "simple_ini/SimpleIni.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "ui/ui.hpp"

struct kcad_context {
    //  PART1 system related
    std::filesystem::path exe_file_path;
    //  PART2 ini related
    spdlog::logger *p_logger;
    //  PART3 runtime related
    int width;
    int height;
    SDL_Window * window;

    bx::DefaultAllocator *p_allocator;
    bgfx::UniformHandle texture_sampler;
    bgfx::TextureHandle temp_texture;
    bgfx::ProgramHandle texture_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle ibh = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle texture_vbh = BGFX_INVALID_HANDLE;

    float cam_pitch = 0.0f;
    float cam_yaw = 0.0f;
    float rot_scale = 0.01f;
    float matrix_view[16];

    int prev_mouse_x = 0;
    int prev_mouse_y = 0;

    struct ui_status *p_ui_stat;
    ImGuiIO *p_imgui_io;
};

inline struct kcad_context g_ctxt;

void init_app_context();

void deinit_app_context();
