#include "context.hpp"
#include "main_wnd_canvas_vertices.hpp"

void setup_main_wnd_canvas_vertices() {
    main_wnd_canvas_vertices[0].y = g_ctxt.canvas_y_max;
    main_wnd_canvas_vertices[2].x = g_ctxt.canvas_x_max;
    main_wnd_canvas_vertices[2].y = g_ctxt.canvas_y_max;
    main_wnd_canvas_vertices[3].x = g_ctxt.canvas_x_max;
    main_wnd_canvas_vertices[4].x = g_ctxt.canvas_x_max;
    main_wnd_canvas_vertices[4].y = g_ctxt.canvas_y_max;
}