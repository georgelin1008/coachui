#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QQmlContext>
#include <cstdlib>

#include <glib.h>
#include <execinfo.h>
#include <cstdio>

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
    
    // Install a temporary GLib log handler to capture backtraces for
    // the specific GStreamer assertion we're seeing at runtime.
    // It appends a short stack trace to /tmp/gst_assert_backtrace.log
    // when the message contains "gst_value_set_int_range_step".
    g_log_set_handler("GStreamer", G_LOG_LEVEL_CRITICAL, [](const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
        if (message && g_strrstr(message, "gst_value_set_int_range_step")) {
            FILE *f = fopen("/tmp/gst_assert_backtrace.log", "a");
            if (f) {
                fprintf(f, "GStreamer assert: %s\n", message);
                void *bt[64];
                int nframes = backtrace(bt, 64);
                char **symbols = backtrace_symbols(bt, nframes);
                if (symbols) {
                    for (int i = 0; i < nframes; ++i) {
                        fprintf(f, "%s\n", symbols[i]);
                    }
                    free(symbols);
                }
                fprintf(f, "----\n");
                fclose(f);
            }
        }
        // Chain to default handler so normal logging still occurs.
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
