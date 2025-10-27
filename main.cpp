#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QQmlContext>
#include <cstdlib>
// GLib logging API used to filter noisy GStreamer/GLib messages
#include <glib.h>

#include "RecorderController.h"
#include "CameraController.h"
#include "VideoPlayer.h"
#include "VideoComposer.h"
#include "src/controllers/AppController.h"

int main(int argc, char *argv[])
{
    // 禁用 GStreamer 警告和調試信息
    qputenv("GST_DEBUG", "0");
    qputenv("GST_DEBUG_NO_COLOR", "1");
    // Install a GLib log handler to quietly ignore known noisy GStreamer warnings
    // (e.g. gst_value_set_int_range_step assertion failures) while letting other
    // logs pass through to the default handler.
    g_log_set_handler(nullptr, (GLogLevelFlags)(G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL),
        [](const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
            if (message) {
                // filter exact noisy substring(s)
                if (g_strstr_len(message, -1, "gst_value_set_int_range_step") != nullptr)
                    return; // drop this message
                if (g_strstr_len(message, -1, "GStreamer-CRITICAL") != nullptr)
                    return; // drop this message
            }
            // otherwise, forward to default handler
            g_log_default_handler(log_domain, log_level, message, user_data);
        }, nullptr);
    
    QGuiApplication app(argc, argv);

    qDebug() << "QRC root entries:";
    for (const auto &e : QDir(":/").entryList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        qDebug() << "  " << e;
    }
    qDebug() << "Exists :/main.qml ?" << QFile(":/main.qml").exists();

    QQmlApplicationEngine engine;

    // Instantiate controllers/players in C++ and expose as context properties to QML
    // This makes C++ the single source of truth for models and high-level logic.
    CameraController cameraController;
    VideoPlayer videoPlayer;
    VideoComposer videoComposer;

    engine.rootContext()->setContextProperty("cameraController", &cameraController);
    engine.rootContext()->setContextProperty("videoPlayer", &videoPlayer);
    engine.rootContext()->setContextProperty("videoComposer", &videoComposer);

    // Expose RecorderController to QML
    RecorderController recorder;
    engine.rootContext()->setContextProperty("recorder", &recorder);

    // Expose AppController (C++ model/controller) to QML
    AppController appController;
    engine.rootContext()->setContextProperty("appController", &appController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "Failed to create root object for" << url;
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
