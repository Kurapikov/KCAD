#pragma once
// For main window texture
struct main_wnd_canvas_coord
{
    float x, y, z; // vertex
    float u, v; // texture

};

inline main_wnd_canvas_coord main_wnd_canvas_coords[] = {
    {-1.0f,  1.0f, 0.0f,  0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f,  0.0f, 0.0f},
    { 1.0f,  1.0f, 0.0f,  1.0f, 1.0f},

    { 1.0f, -1.0f, 0.0f,  1.0f, 0.0f},
    { 1.0f,  1.0f, 0.0f,  1.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f,  0.0f, 0.0f}
};
