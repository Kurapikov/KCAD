$input a_position
$input a_texcoord0

$output v_texcoord0

#include <../../3rdparty/bgfx/include/bgfx/bgfx_shader.sh>

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_texcoord0 = a_texcoord0;
}
