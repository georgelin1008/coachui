#include "VideoListModel.h"

VideoListModel::VideoListModel(QObject *parent) : QAbstractListModel(parent) {}

int VideoListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant VideoListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) return {};
    const VideoItem &it = m_items.at(index.row());
    switch (role) {
        case FileNameRole: return it.fileName;
        case FilePathRole: return it.filePath;
        case InfoRole: return it.info;
        case SelectedRole: return it.selected;
        default: return {};
    }
}

QHash<int, QByteArray> VideoListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    roles[InfoRole] = "info";
    roles[SelectedRole] = "selected";
    return roles;
}

void VideoListModel::clear() {
    beginResetModel();
    m_items.clear();
    endResetModel();
    emit selectionChanged(0);
}

void VideoListModel::append(const QString &fileName, const QString &filePath, const QString &info) {
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back({fileName, filePath, info, false});
    endInsertRows();
}

void VideoListModel::removeAt(int index) {
    if (index < 0 || index >= m_items.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    m_items.removeAt(index);
    endRemoveRows();
    int selectedCount = 0;
    for (const auto &v : m_items) if (v.selected) ++selectedCount;
    emit selectionChanged(selectedCount);
}

void VideoListModel::toggleSelection(int index) {
    if (index < 0 || index >= m_items.size()) return;
    m_items[index].selected = !m_items[index].selected;
    QModelIndex idx = createIndex(index, 0);
    emit dataChanged(idx, idx, {SelectedRole});
    int selectedCount = 0;
    for (const auto &v : m_items) if (v.selected) ++selectedCount;
    emit selectionChanged(selectedCount);
}

int VideoListModel::count() const { return m_items.size(); }
