#include "bgfx/bgfx.h"
#include "bx/math.h"

void setup_main_wnd_view_proj()
{
    float cam_rotation[16];
    bx::mtxRotateXYZ(cam_rotation, 0.0f, 0.0f, 0.0f);

    float cam_translation[16];
    bx::mtxTranslate(cam_translation, 0.0f, 0.0f, -1.0f);

    float cam_transform[16];
    bx::mtxMul(cam_transform, cam_translation, cam_rotation);

    float view[16];
    bx::mtxInverse(view, cam_transform);

    float proj[16];
    bx::mtxOrtho(proj, -1, 1, -1, 1, 0.1f, 10.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);
}