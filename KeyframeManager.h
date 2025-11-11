#ifndef KEYFRAMEMANAGER_H
#define KEYFRAMEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QList>
#include <QVariantMap>

struct Keyframe {
    qint64 timestamp;      // 時間戳記（毫秒）
    QString name;          // 關鍵幀名稱
    QString description;   // 描述（選填）
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["timestamp"] = timestamp;
        obj["name"] = name;
        obj["description"] = description;
        return obj;
    }
    
    static Keyframe fromJson(const QJsonObject &obj) {
        Keyframe kf;
        kf.timestamp = obj["timestamp"].toInteger();
        kf.name = obj["name"].toString();
        kf.description = obj["description"].toString();
        return kf;
    }
};

class KeyframeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentVideoFile READ currentVideoFile NOTIFY currentVideoFileChanged)
    Q_PROPERTY(int keyframeCount READ keyframeCount NOTIFY keyframesChanged)

public:
    explicit KeyframeManager(QObject *parent = nullptr);
    ~KeyframeManager();

    QString currentVideoFile() const { return m_currentVideoFile; }
    int keyframeCount() const { return m_keyframes.size(); }

public slots:
    // 設定當前影片並載入相關的關鍵幀
    void setCurrentVideo(const QString &videoPath);
    
    // 新增關鍵幀
    void addKeyframe(qint64 timestamp, const QString &name, const QString &description = "");
    
    // 移除關鍵幀
    void removeKeyframe(int index);
    
    // 更新關鍵幀
    void updateKeyframe(int index, const QString &name, const QString &description);
    
    // 取得所有關鍵幀（回傳 QVariantList 供 QML 使用）
    QVariantList getKeyframes() const;
    
    // 儲存關鍵幀到 JSON 檔案
    bool saveKeyframes();
    
    // 從 JSON 檔案載入關鍵幀
    bool loadKeyframes(const QString &videoPath);
    
    // 清空所有關鍵幀
    void clearKeyframes();
    
    // 取得關鍵幀儲存路徑
    QString getKeyframeFilePath(const QString &videoPath) const;

signals:
    void currentVideoFileChanged();
    void keyframesChanged();
    void keyframeAdded(qint64 timestamp, const QString &name);
    void keyframeRemoved(int index);
    void saveSuccess(const QString &filePath);
    void saveFailed(const QString &error);
    void loadSuccess(int count);
    void loadFailed(const QString &error);

private:
    QString m_currentVideoFile;
    QList<Keyframe> m_keyframes;
    QString m_keyframeDirectory;
    
    void ensureKeyframeDirectoryExists();
};

#endif // KEYFRAMEMANAGER_H
