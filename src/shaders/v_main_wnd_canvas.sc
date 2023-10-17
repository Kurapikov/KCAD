$input a_position
$output v_position

#include <../../3rdparty/bgfx/include/bgfx/bgfx_shader.sh>

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_position = a_position;
}
