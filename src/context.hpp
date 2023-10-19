#pragma once

#include <filesystem>

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

    bgfx::ProgramHandle main_wnd_canvas_program = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle main_wnd_canvas_vbh = BGFX_INVALID_HANDLE;
    //bgfx::IndexBufferHandle ibh = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle main_wnd_canvas_texture_handle = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle main_wnd_canvas_texture_sampler_handle = BGFX_INVALID_HANDLE;
    float cam_pitch = 0.0f;
    float cam_yaw = 0.0f;
    float rot_scale = 0.01f;

    int prev_mouse_x = 0;
    int prev_mouse_y = 0;

    struct ui_status *p_ui_stat;
    ImGuiIO *p_imgui_io;

    float canvas_x_max = 1;
    float canvas_y_max = 1;
    float canvas_ratio = 1;
};

inline struct kcad_context g_ctxt;

void init_app_context();

void deinit_app_context();
