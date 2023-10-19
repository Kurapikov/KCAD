// For main window texture
struct main_wnd_canvas_vertex
{
    float x, y, z;
};

inline main_wnd_canvas_vertex main_wnd_canvas_vertices[] = {
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},

    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}
};

void setup_main_wnd_canvas_vertices();