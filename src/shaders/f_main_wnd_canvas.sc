$input v_TexCoord

#include <../../3rdparty/bgfx/include/bgfx/bgfx_shader.sh>

SAMPLER2D(s_texture, 0);

void main()
{
    gl_FragColor = texture2D(s_texture, v_TexCoord);
}