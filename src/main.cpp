#include <stdio.h>

#include "SDL.h"
#include "SDL_syswm.h"

#include "bx/platform.h"
#include <bx/math.h>
#include "bgfx/platform.h"
#include "bgfx/bgfx.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"

#include "imgui_impl_bgfx/imgui_impl_bgfx.hpp"

#include "utils.hpp"
#include "context.hpp"
#include "ui/ui.hpp"
#include "workaround_macos_bgfx_mt.h"

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cube_vertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t cube_tri_list[] = {
    0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6,
    1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};

static bgfx::ShaderHandle create_shader(
    const std::string& shader, const char* name)
{
    const bgfx::Memory* mem = bgfx::copy(shader.data(), shader.size());
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}

void main_loop()
{
        // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(g_ctxt.window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_Implbgfx_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ui_update();

        // Rendering
        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
        if (!ImGui::GetIO().WantCaptureMouse) {
            // simple input code for orbit camera
            int mouse_x, mouse_y;
            const int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
            if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
                int delta_x = mouse_x - g_ctxt.prev_mouse_x;
                int delta_y = mouse_y - g_ctxt.prev_mouse_y;
                g_ctxt.cam_yaw += float(-delta_x) * g_ctxt.rot_scale;
                g_ctxt.cam_pitch += float(-delta_y) * g_ctxt.rot_scale;
            }
            g_ctxt.prev_mouse_x = mouse_x;
            g_ctxt.prev_mouse_y = mouse_y;
        }

        float cam_rotation[16];
        bx::mtxRotateXYZ(cam_rotation, g_ctxt.cam_pitch, g_ctxt.cam_yaw, 0.0f);

        float cam_translation[16];
        bx::mtxTranslate(cam_translation, 0.0f, 0.0f, -5.0f);

        float cam_transform[16];
        bx::mtxMul(cam_transform, cam_translation, cam_rotation);

        float view[16];
        bx::mtxInverse(view, cam_transform);

        float proj[16];
        bx::mtxProj(
            proj, 60.0f, float(g_ctxt.width) / float(g_ctxt.height), 0.1f,
            100.0f, bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(0, view, proj);

        float model[16];
        bx::mtxIdentity(model);
        bgfx::setTransform(model);

        bgfx::setVertexBuffer(0, g_ctxt.vbh);
        bgfx::setIndexBuffer(g_ctxt.ibh);

        bgfx::submit(0, g_ctxt.program);
        bgfx::touch(0);
        bgfx::frame();
    }
}

int main(int, char**)
{
    init_app_context();

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        g_ctxt.p_logger->error("SDL_Init() Error: {}", SDL_GetError());
        return -1;
    }
    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
#if BX_PLATFORM_WINDOWS
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#elif BX_PLATFORM_OSX
    //SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    //WORKAROUND: https://github.com/ocornut/imgui/issues/5931
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE);
#elif BX_PLATFORM_LINUX
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX
    g_ctxt.window = SDL_CreateWindow("KCAD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_ctxt.width, g_ctxt.height, window_flags);

    // Setup BGFX
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(g_ctxt.window, &wmi)) {
        printf(
            "SDL_SysWMinfo could not be retrieved. SDL_Error: %s\n",
            SDL_GetError());
        return 1;
    }
    bgfx::PlatformData pd{};
#if BX_PLATFORM_WINDOWS
    pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
    // WORKAROUND: https://github.com/bkaradzic/bgfx/issues/2036
    pd.nwh = workaround_cbSetupMetalLayer(wmi.info.cocoa.window);
#elif BX_PLATFORM_LINUX
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX

    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
    bgfx_init.resolution.width = g_ctxt.width;
    bgfx_init.resolution.height = g_ctxt.height;
    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx_init.platformData = pd;
    bgfx::init(bgfx_init);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, g_ctxt.width, g_ctxt.height);

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_Implbgfx_Init(255);
#if BX_PLATFORM_WINDOWS
    ImGui_ImplSDL2_InitForD3D(g_ctxt.window);
#elif BX_PLATFORM_OSX
    ImGui_ImplSDL2_InitForMetal(g_ctxt.window);
#elif BX_PLATFORM_LINUX
    ImGui_ImplSDL2_InitForOpenGL(g_ctxt.window, nullptr);
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX

    // TODO: Load Fonts

    // bgfx draw something
    bgfx::VertexLayout pos_col_vert_layout;
    pos_col_vert_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(cube_vertices, sizeof(cube_vertices)),
        pos_col_vert_layout);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(cube_tri_list, sizeof(cube_tri_list)));
    const std::string shader_root = g_ctxt.exe_file_path / "shaders/";
    std::string vshader;
    if (!fileops::read_file(shader_root + "v_simple.bin", vshader)) {
        printf("Could not find shader vertex shader (ensure shaders have been "
               "compiled).\n"
               "Run compile-shaders-<platform>.sh/bat\n");
        return 1;
    }
    std::string fshader;
    if (!fileops::read_file(shader_root + "f_simple.bin", fshader)) {
        printf("Could not find shader fragment shader (ensure shaders have "
               "been compiled).\n"
               "Run compile-shaders-<platform>.sh/bat\n");
        return 1;
    }
    bgfx::ShaderHandle vsh = create_shader(vshader, "vshader");
    bgfx::ShaderHandle fsh = create_shader(fshader, "fshader");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);
    g_ctxt.program = program;
    g_ctxt.vbh = vbh;
    g_ctxt.ibh = ibh;

    main_loop();

    // Cleanup
    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
    bgfx::destroy(program);
    ImGui_ImplSDL2_Shutdown();
    ImGui_Implbgfx_Shutdown();
    ImGui::DestroyContext();
    bgfx::shutdown();
    SDL_DestroyWindow(g_ctxt.window);
    SDL_Quit();
    deinit_app_context();
    return 0;
}
