#include "context.hpp"
#include "utils.hpp"

void init_app_context()
{
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

    // TODO: this is temp code
    g_ctxt.width=1440;
    g_ctxt.height=900;
}
