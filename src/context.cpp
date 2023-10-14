#include "context.hpp"
#include "utils.hpp"
#include "ui/ui.hpp"

void init_app_context()
{
    // Setup structs
    g_ctxt.p_ui_stat = &g_ui_stat;

    // Setup paths
    g_ctxt.exe_file_path = get_exe_file_path();

    // TODO: Init INI

    // Init logger
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("KCAD.log"));
    g_ctxt.p_logger = new spdlog::logger("name", begin(sinks), end(sinks));
    g_ctxt.p_logger->set_pattern("[%H:%M:%S.%e] [%^-%L-%$] [%t] %v");
    g_ctxt.p_logger->info("Welcome to spdlog!");

    // Init runtime contents
    ImGui::CreateContext();
    g_ctxt.p_imgui_io = &(ImGui::GetIO());// (void)g_ctxt.imgui_io;
    g_ctxt.p_imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    g_ctxt.p_imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // TODO: this is temp code
    g_ctxt.width=1440;
    g_ctxt.height=900;
}

void deinit_app_context()
{
    delete (g_ctxt.p_logger);
}