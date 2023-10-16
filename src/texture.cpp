#include "bx/file.h"
#include "bimg/decode.h"
#include "bimg/encode.h"
#include "bgfx/bgfx.h"
#include "context.hpp"

bgfx::TextureHandle loadTexture(const char* filePath)
{
    bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;

    // Load file using bx lib
    bx::FileReader fileReader;
    if (bx::open(&fileReader, filePath))
    {
        const bgfx::Memory* mem = nullptr;
        uint32_t size = (uint32_t)bx::getSize(&fileReader);
        void* data = malloc(size);
        bx::Error err;
        bx::read(&fileReader, data, size, &err);
        bx::close(&fileReader);

        // 使用bimg库解码图像
        bimg::ImageContainer* imageContainer = bimg::imageParse(g_ctxt.p_allocator, data, size);
        if (imageContainer)
        {
            // 创建bgfx纹理
            texture = bgfx::createTexture2D(
                uint16_t(imageContainer->m_width),
                uint16_t(imageContainer->m_height),
                1 < imageContainer->m_numMips,
                imageContainer->m_numLayers,
                bgfx::TextureFormat::Enum(imageContainer->m_format),
                BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
                bgfx::makeRef(
                    imageContainer->m_data,
                    imageContainer->m_size
                )
            );

            bimg::imageFree(imageContainer);
        }

        free(data);
    }

    return texture;
}