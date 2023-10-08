#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuickItem>
#include <QQuickWindow>
#include <QRunnable>

#include "bgfx_bridge.hpp"

bgfx_bridge::bgfx_bridge()
    : m_t(0)
    , m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &bgfx_bridge::handleWindowChanged);
}

void bgfx_bridge::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &bgfx_bridge::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &bgfx_bridge::cleanup, Qt::DirectConnection);
        // Ensure we start with cleared to black. The bgfx_bridge's blend mode relies on this.
        win->setColor(Qt::black);
    }
}

void bgfx_bridge::sync()
{
    if (!m_renderer) {
        m_renderer = new bgfx_bridge_renderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &bgfx_bridge_renderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &bgfx_bridge_renderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());
}
void bgfx_bridge::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}



class BGFX_cleanup_job : public QRunnable
{
public:
    BGFX_cleanup_job(bgfx_bridge_renderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    bgfx_bridge_renderer *m_renderer;
};

void bgfx_bridge::releaseResources()
{
    window()->scheduleRenderJob(new BGFX_cleanup_job(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

bgfx_bridge_renderer::~bgfx_bridge_renderer()
{
    delete m_program;
}

void bgfx_bridge::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void bgfx_bridge_renderer::init()
{
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL);

        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "attribute highp vec4 vertices;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_Position = vertices;"
                                                    "    coords = vertices.xy;"
                                                    "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "uniform lowp float t;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
                                                    "    i = smoothstep(t - 0.8, t + 0.8, i);"
                                                    "    i = floor(i * 20.) / 20.;"
                                                    "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                                                    "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->link();

    }
}

void bgfx_bridge_renderer::paint()
{
    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    m_window->beginExternalCommands();

    m_program->bind();

    m_program->enableAttributeArray(0);

    float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    // This example relies on (deprecated) client-side pointers for the vertex
    // input. Therefore, we have to make sure no vertex buffer is bound.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setUniformValue("t", (float) m_t);

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->release();

    m_window->endExternalCommands();
}

#include "bgfx_bridge.moc"
