#include <cstdint>

#include "bgfx/bgfx.h"

#include "context.hpp"

void generate_border_texture()
{
    uint32_t width = g_ctxt.width;
    uint32_t height = g_ctxt.height;

    // Create an empty texture
    g_ctxt.main_wnd_canvas_texture_handle = bgfx::createTexture2D(
        static_cast<uint16_t>(width), static_cast<uint16_t>(height),
        false, 1, bgfx::TextureFormat::RGBA8,
        BGFX_SAMPLER_U_BORDER | BGFX_SAMPLER_V_BORDER | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT, nullptr);

    std::vector<uint8_t> imageData(width * height * 4); // 32bpp

    uint32_t borderWidth = 10;

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t idx = (y * width + x) * 4;
            if (x < borderWidth || x >= width - borderWidth || y < borderWidth || y >= height - borderWidth) {
                if (y < borderWidth) { // Bottom, Red
                    imageData[idx] = 255;   // R
                    imageData[idx + 1] = 0; // G
                    imageData[idx + 2] = 0; // B
                } else if (y >= height - borderWidth) { // Up, Black
                    imageData[idx] = 0;   // R
                    imageData[idx + 1] = 0; // G
                    imageData[idx + 2] = 0; // B
                } else if (x < borderWidth) { // Left, Green
                    imageData[idx] = 0;   // R
                    imageData[idx + 1] = 255; // G
                    imageData[idx + 2] = 0; // B
                } else if (x > width - borderWidth) { // Right, White
                    imageData[idx] = 255;   // R
                    imageData[idx + 1] = 255; // G
                    imageData[idx + 2] = 255; // B
                }
                imageData[idx + 3] = 255; // A
            } else {
                imageData[idx] = 0;   // R
                imageData[idx + 1] = 0; // G
                imageData[idx + 2] = 255; // B
                imageData[idx + 3] = 255; // A
            }
        }
    }
    const bgfx::Memory* mem = bgfx::copy(imageData.data(), imageData.size());
    bgfx::updateTexture2D(g_ctxt.main_wnd_canvas_texture_handle, 0, 0, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height), mem);
}
