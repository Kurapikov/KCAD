#include "ui/ui.hpp"
#include "context.hpp"

void ui_update()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (g_ctxt.p_ui_stat->show_demo_window)
        ImGui::ShowDemoWindow(&(g_ctxt.p_ui_stat->show_demo_window));

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &(g_ctxt.p_ui_stat->show_demo_window));      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &(g_ctxt.p_ui_stat->show_another_window));

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&(g_ctxt.p_ui_stat->clear_color)); // Edit 3 floats representing a color
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, ImGui::ColorConvertFloat4ToU32(
            ImVec4(g_ctxt.p_ui_stat->clear_color.w, g_ctxt.p_ui_stat->clear_color.z, g_ctxt.p_ui_stat->clear_color.y, g_ctxt.p_ui_stat->clear_color.x)), 1.0f, 0);

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / g_ctxt.p_imgui_io->Framerate, g_ctxt.p_imgui_io->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (g_ctxt.p_ui_stat->show_another_window)
    {
        ImGui::Begin("Another Window", &(g_ctxt.p_ui_stat->show_another_window));   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            (g_ctxt.p_ui_stat->show_another_window) = false;
        ImGui::End();
    }
}