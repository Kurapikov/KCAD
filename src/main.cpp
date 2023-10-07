#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTimer>

#include "bgfx/bgfx.h"

int init_bgfx(WId nwh, uint32_t width, uint32_t height, bgfx::RendererType::Enum renderer_type) {
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

class MyTimer : public QObject
{
    Q_OBJECT
public:
    MyTimer();
    QTimer *timer;

public slots:
    void MyTimerSlot();
};
MyTimer::MyTimer()
{
    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(MyTimerSlot()));

    // msec
    timer->start(1000);
}

void MyTimer::MyTimerSlot()
{
    qDebug() << "In";
// Inside the main loop after glfwPollEvents
// Check if window size changed and update the view respectively
bgfx::reset(100, 100, BGFX_RESET_VSYNC);
bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

// Ensure the view is redrawn even if no graphic commands are called
bgfx::touch(0);

// End the frame
bgfx::frame();
qDebug() << "Out";
}

int main(int argc, char *argv[])
{
    // Initialize main window
    QGuiApplication app(argc, argv);
    qDebug() << "Welcome to KCAD";
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    // Initialize BGFX
    WId main_wid = qobject_cast<QQuickWindow*>( engine.rootObjects().at(0) ) -> winId();
    // TODO: RendererType should read from .ini file.
    // FIXME: Ubuntu in VMWare will report it supports Vulkan, but actually doesn't.
    if (init_bgfx(main_wid, 100, 100, bgfx::RendererType::OpenGL) != 0)
    {
        return -1;
    }
    // TODO: These codes are test code
    // We will use this to reference where we're drawing
    const bgfx::ViewId main_view_id = 0;

    // This is set once to determine the clear color to use on starting a new frame
    bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x88888888);

    // This is set to determine the size of the drawable surface
    bgfx::setViewRect(main_view_id, 0, 0, static_cast<std::uint16_t>(100), static_cast<std::uint16_t>(100));

    MyTimer timer;
    return app.exec();
}

#include "main.moc"