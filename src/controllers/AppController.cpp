#include "AppController.h"
#include "../models/VideoListModel.h"
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDateTime>
#include <QDebug>

AppController::AppController(QObject *parent) : QObject(parent) {
    m_videoListModel = new VideoListModel(this);
    connect(m_videoListModel, &VideoListModel::selectionChanged, this, &AppController::onSelectionChanged);
}

VideoListModel* AppController::videoListModel() const { return m_videoListModel; }

void AppController::refreshVideoList(const QString &dirPath) {
    QString path = dirPath.isEmpty() ? QDir::homePath() + "/Videos" : dirPath;
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.mp4";  // 只顯示 mp4 檔案
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files, QDir::Time);
    m_videoListModel->clear();
    for (const QFileInfo &fi : files) {
        QString info;
        qint64 mtime = fi.lastModified().toSecsSinceEpoch();
        qint64 now = QDateTime::currentSecsSinceEpoch();
        qint64 diff = now - mtime;
        if (diff < 3600) info = " (new)";
        else if (diff < 86400) info = " (today)";
        QString displayName = fi.fileName() + info;
        QString fileName = fi.fileName();
        qDebug() << "AppController: adding video->" << fi.absoluteFilePath() << "name:" << fileName << "displayName:" << displayName;
        m_videoListModel->appendDetailed(displayName, fileName, fi.absoluteFilePath(), info);
    }
}

QStringList AppController::getSelectedVideoPaths() const {
    QStringList out;
    int n = m_videoListModel->count();
    for (int i = 0; i < n; ++i) {
        QModelIndex idx = m_videoListModel->index(i,0);
        bool sel = m_videoListModel->data(idx, VideoListModel::SelectedRole).toBool();
        if (sel) out << m_videoListModel->data(idx, VideoListModel::FilePathRole).toString();
    }
    return out;
}

void AppController::loadVideos(const QStringList &paths) {
    qDebug() << "AppController::loadVideos paths:" << paths;
    // Placeholder: integrate with VideoComposer / backend as needed
}

int AppController::selectedVideoCount() const {
    int n = m_videoListModel->count(), c = 0;
    for (int i = 0; i < n; ++i) {
        QModelIndex idx = m_videoListModel->index(i,0);
        if (m_videoListModel->data(idx, VideoListModel::SelectedRole).toBool()) ++c;
    }
    return c;
}

void AppController::onSelectionChanged(int count) {
    emit selectedVideoCountChanged(count);
}
