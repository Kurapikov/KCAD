#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "bgfx_bridge.hpp"

int main(int argc, char *argv[])
{
    // Debug output system information
    QLoggingCategory::setFilterRules(QStringLiteral("qt.scenegraph.*=true")); 
    qSetMessagePattern("%{category}: %{message}");

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

    QQuickWindow *p_main_win = qobject_cast<QQuickWindow*>( engine.rootObjects().at(0) );
    bgfx_bridge *p_bgfx_bridge_inst = new bgfx_bridge(p_main_win);
    p_bgfx_bridge_inst->set_wid(p_main_win -> winId());
    return app.exec();
}

#include "main.moc"
