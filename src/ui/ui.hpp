#pragma once

#include "imgui.h"

struct ui_status
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

inline struct ui_status g_ui_stat;

void ui_update();
