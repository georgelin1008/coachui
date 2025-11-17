#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QString>

struct VideoItem {
    QString fileName;
    QString filePath;
    QString displayName;
    QString info;
    bool selected = false;
};

class VideoListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { FileNameRole = Qt::UserRole + 1, FilePathRole, DisplayNameRole, InfoRole, SelectedRole };
    explicit VideoListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void append(const QString &fileName, const QString &filePath, const QString &info = QString());
    // C++ API to append with an explicit display name (used by AppController)
    Q_INVOKABLE void appendDetailed(const QString &displayName, const QString &fileName, const QString &filePath, const QString &info = QString());
    Q_INVOKABLE void removeAt(int index);
    Q_INVOKABLE void toggleSelection(int index);
    Q_INVOKABLE int count() const;

signals:
    void selectionChanged(int selectedCount);

private:
    QVector<VideoItem> m_items;
};
