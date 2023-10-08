#include <QQuickItem>
#include <QQuickWindow>

#include "bgfx/bgfx.h"

class bgfx_bridge : public QQuickItem
{
    Q_OBJECT

public:
    bgfx_bridge(QQuickWindow *win);
    void set_wid(WId wid);
public slots:
    void sync();
    void cleanup();
    void paint();
    void paint2();

private:
    bool m_is_bgfx_initialized;
    WId m_wid;
    QQuickWindow *m_p_window;
    void releaseResources() override;
    int init_bgfx(WId nwh, uint32_t width, uint32_t height, bgfx::RendererType::Enum renderer_type);
};
