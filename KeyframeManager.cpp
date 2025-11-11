#include "KeyframeManager.h"
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

KeyframeManager::KeyframeManager(QObject *parent)
    : QObject(parent)
    , m_keyframeDirectory("/home/mxpt2/coachui/keyframes")
{
    ensureKeyframeDirectoryExists();
    qDebug() << "KeyframeManager initialized, directory:" << m_keyframeDirectory;
}

KeyframeManager::~KeyframeManager()
{
}

void KeyframeManager::ensureKeyframeDirectoryExists()
{
    QDir dir(m_keyframeDirectory);
    if (!dir.exists()) {
        if (dir.mkpath(".")) {
            qDebug() << "Created keyframe directory:" << m_keyframeDirectory;
        } else {
            qWarning() << "Failed to create keyframe directory:" << m_keyframeDirectory;
        }
    }
}

void KeyframeManager::setCurrentVideo(const QString &videoPath)
{
    if (m_currentVideoFile == videoPath) {
        return;
    }
    
    // 儲存舊的關鍵幀
    if (!m_currentVideoFile.isEmpty() && !m_keyframes.isEmpty()) {
        saveKeyframes();
    }
    
    m_currentVideoFile = videoPath;
    emit currentVideoFileChanged();
    
    // 載入新影片的關鍵幀
    loadKeyframes(videoPath);
}

void KeyframeManager::addKeyframe(qint64 timestamp, const QString &name, const QString &description)
{
    Keyframe kf;
    kf.timestamp = timestamp;
    kf.name = name.isEmpty() ? QString("Keyframe %1").arg(m_keyframes.size() + 1) : name;
    kf.description = description;
    
    // 按時間順序插入
    int insertPos = 0;
    for (int i = 0; i < m_keyframes.size(); ++i) {
        if (m_keyframes[i].timestamp > timestamp) {
            break;
        }
        insertPos = i + 1;
    }
    
    m_keyframes.insert(insertPos, kf);
    
    qDebug() << "Added keyframe:" << kf.name << "at" << timestamp << "ms";
    
    emit keyframesChanged();
    emit keyframeAdded(timestamp, kf.name);
}

void KeyframeManager::removeKeyframe(int index)
{
    if (index < 0 || index >= m_keyframes.size()) {
        qWarning() << "Invalid keyframe index:" << index;
        return;
    }
    
    QString removedName = m_keyframes[index].name;
    m_keyframes.removeAt(index);
    
    qDebug() << "Removed keyframe:" << removedName;
    
    emit keyframesChanged();
    emit keyframeRemoved(index);
}

void KeyframeManager::updateKeyframe(int index, const QString &name, const QString &description)
{
    if (index < 0 || index >= m_keyframes.size()) {
        qWarning() << "Invalid keyframe index:" << index;
        return;
    }
    
    m_keyframes[index].name = name;
    m_keyframes[index].description = description;
    
    qDebug() << "Updated keyframe at index" << index;
    
    emit keyframesChanged();
}

QVariantList KeyframeManager::getKeyframes() const
{
    QVariantList list;
    for (int i = 0; i < m_keyframes.size(); ++i) {
        const Keyframe &kf = m_keyframes[i];
        QVariantMap map;
        map["index"] = i;
        map["timestamp"] = kf.timestamp;
        map["name"] = kf.name;
        map["description"] = kf.description;
        list.append(map);
    }
    return list;
}

bool KeyframeManager::saveKeyframes()
{
    if (m_currentVideoFile.isEmpty()) {
        emit saveFailed("No video file loaded");
        return false;
    }
    
    QString filePath = getKeyframeFilePath(m_currentVideoFile);
    
    QJsonObject root;
    root["videoFile"] = m_currentVideoFile;
    root["videoFileName"] = QFileInfo(m_currentVideoFile).fileName();
    root["keyframeCount"] = m_keyframes.size();
    
    QJsonArray keyframesArray;
    for (const Keyframe &kf : m_keyframes) {
        keyframesArray.append(kf.toJson());
    }
    root["keyframes"] = keyframesArray;
    
    QJsonDocument doc(root);
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly)) {
        QString error = QString("Failed to open file for writing: %1").arg(filePath);
        qWarning() << error;
        emit saveFailed(error);
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Saved" << m_keyframes.size() << "keyframes to" << filePath;
    emit saveSuccess(filePath);
    
    return true;
}

bool KeyframeManager::loadKeyframes(const QString &videoPath)
{
    clearKeyframes();
    
    if (videoPath.isEmpty()) {
        return false;
    }
    
    QString filePath = getKeyframeFilePath(videoPath);
    QFile file(filePath);
    
    if (!file.exists()) {
        qDebug() << "No keyframe file exists for this video:" << filePath;
        emit loadSuccess(0);
        return true;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        QString error = QString("Failed to open keyframe file: %1").arg(filePath);
        qWarning() << error;
        emit loadFailed(error);
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        QString error = "Invalid JSON format";
        qWarning() << error << "in" << filePath;
        emit loadFailed(error);
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray keyframesArray = root["keyframes"].toArray();
    
    for (const QJsonValue &value : keyframesArray) {
        if (value.isObject()) {
            Keyframe kf = Keyframe::fromJson(value.toObject());
            m_keyframes.append(kf);
        }
    }
    
    qDebug() << "Loaded" << m_keyframes.size() << "keyframes from" << filePath;
    emit keyframesChanged();
    emit loadSuccess(m_keyframes.size());
    
    return true;
}

void KeyframeManager::clearKeyframes()
{
    if (!m_keyframes.isEmpty()) {
        m_keyframes.clear();
        emit keyframesChanged();
        qDebug() << "Cleared all keyframes";
    }
}

QString KeyframeManager::getKeyframeFilePath(const QString &videoPath) const
{
    if (videoPath.isEmpty()) {
        return QString();
    }
    
    QFileInfo fileInfo(videoPath);
    QString baseName = fileInfo.completeBaseName();
    QString jsonFileName = baseName + "_keyframes.json";
    
    return QDir(m_keyframeDirectory).filePath(jsonFileName);
}
