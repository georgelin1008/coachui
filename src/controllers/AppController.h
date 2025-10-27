#pragma once

#include <QObject>
#include <QStringList>
#include "../models/VideoListModel.h"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(VideoListModel* videoListModel READ videoListModel CONSTANT)
    Q_PROPERTY(int selectedVideoCount READ selectedVideoCount NOTIFY selectedVideoCountChanged)
public:
    explicit AppController(QObject *parent = nullptr);

    VideoListModel* videoListModel() const;
    Q_INVOKABLE void refreshVideoList(const QString &dirPath = QString());
    Q_INVOKABLE QStringList getSelectedVideoPaths() const;
    Q_INVOKABLE void loadVideos(const QStringList &paths);

    int selectedVideoCount() const;

signals:
    void selectedVideoCountChanged(int);

private slots:
    void onSelectionChanged(int count);

private:
    VideoListModel *m_videoListModel;
};
