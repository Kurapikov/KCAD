#include <QQuickWindow>

#include "bgfx/bgfx.h"
#include "bgfx_bridge.hpp"

int bgfx_bridge::init_bgfx(WId nwh, uint32_t width, uint32_t height, bgfx::RendererType::Enum renderer_type) {
    bgfx::Init init_object;
    init_object.platformData.nwh = reinterpret_cast<void *>(nwh);
    init_object.resolution.width = static_cast<uint32_t>(width);
    init_object.resolution.height = static_cast<uint32_t>(height);
    init_object.resolution.reset = BGFX_RESET_VSYNC;
    init_object.type = bgfx::RendererType::OpenGL;
    if (!bgfx::init(init_object))
    {
        return 1;
    }
    return 0;
}

bgfx_bridge::bgfx_bridge(QQuickWindow *win) : m_p_window(nullptr), m_is_bgfx_initialized(false)
{
    m_p_window = win;
    connect(win, &QQuickWindow::beforeSynchronizing, this, &bgfx_bridge::sync, Qt::DirectConnection);
    connect(win, &QQuickWindow::sceneGraphInvalidated, this, &bgfx_bridge::cleanup, Qt::DirectConnection);
    // Ensure we start with cleared to black. The bgfx_bridge's blend mode relies on this.
    win->setColor(Qt::black);
}

void bgfx_bridge::paint()
{
    // Check if window size changed and update the view respectively
    bgfx::reset(100, 100, BGFX_RESET_VSYNC);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

    // Ensure the view is redrawn even if no graphic commands are called
    bgfx::touch(0);

    // End the frame
    bgfx::frame();
    qDebug() << "Signal: beforeRenderPassRecording";
}

void bgfx_bridge::paint2()
{
    qDebug() << "Signal: beforeRendering";
}


void bgfx_bridge::sync()
{
    if (!m_is_bgfx_initialized) {
        // Initialize BGFX
        // TODO: RendererType should read from .ini file.
        // FIXME: Ubuntu in VMWare will report it supports Vulkan, but actually doesn't.
        if (init_bgfx(m_wid, 100, 100, bgfx::RendererType::OpenGL) != 0)
        {
            return;
        }
        // TODO: These codes are test code
        const bgfx::ViewId main_view_id = 0;
        bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x88888888);
        bgfx::setViewRect(main_view_id, 0, 0, static_cast<std::uint16_t>(100), static_cast<std::uint16_t>(100));
        connect(m_p_window, &QQuickWindow::beforeRenderPassRecording, this, &bgfx_bridge::paint, Qt::DirectConnection);
        connect(m_p_window, &QQuickWindow::beforeRendering, this, &bgfx_bridge::paint2, Qt::DirectConnection);
        m_is_bgfx_initialized = true;
    }
}
void bgfx_bridge::cleanup()
{
    return;
}

void bgfx_bridge::releaseResources()
{
    m_is_bgfx_initialized = false;
}

void bgfx_bridge::set_wid(WId wid)
{
    m_wid = wid;
}

#include "bgfx_bridge.moc"
