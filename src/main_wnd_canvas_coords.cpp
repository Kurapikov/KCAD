#include "context.hpp"
#include "main_wnd_canvas_coords.hpp"

void setup_main_wnd_canvas_vertices() {
    main_wnd_canvas_coords[0].x = -g_ctxt.canvas_x_max / 2;
    main_wnd_canvas_coords[0].y =  g_ctxt.canvas_y_max / 2;
    main_wnd_canvas_coords[1].x = -g_ctxt.canvas_x_max / 2;
    main_wnd_canvas_coords[1].y = -g_ctxt.canvas_y_max / 2;
    main_wnd_canvas_coords[2].x =  g_ctxt.canvas_x_max / 2;
    main_wnd_canvas_coords[2].y =  g_ctxt.canvas_y_max / 2;
    main_wnd_canvas_coords[3].x =  g_ctxt.canvas_x_max / 2;
    main_wnd_canvas_coords[3].y = -g_ctxt.canvas_y_max / 2;
    main_wnd_canvas_coords[4].x =  g_ctxt.canvas_x_max / 2;
    main_wnd_canvas_coords[4].y =  g_ctxt.canvas_y_max / 2;
    main_wnd_canvas_coords[5].x = -g_ctxt.canvas_x_max / 2;
    main_wnd_canvas_coords[5].y = -g_ctxt.canvas_y_max / 2;
}