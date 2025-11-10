#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QVideoSink>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <vector>

class VideoPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString currentFile READ currentFile NOTIFY currentFileChanged)
    Q_PROPERTY(QStringList recentFiles READ recentFiles NOTIFY recentFilesChanged)
    Q_PROPERTY(double playbackRate READ playbackRate WRITE setPlaybackRate NOTIFY playbackRateChanged)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(bool isSideBySideMode READ isSideBySideMode NOTIFY sideBySideModeChanged)
    Q_PROPERTY(QStringList sideBySideVideos READ sideBySideVideos NOTIFY sideBySideVideosChanged)
    Q_PROPERTY(bool sideBySideHorizontal READ sideBySideHorizontal NOTIFY sideBySideHorizontalChanged)

public:
    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();
    
    bool isPlaying() const { return m_isPlaying; }
    qint64 position() const { return m_position; }
    qint64 duration() const { return m_duration; }
    QString currentFile() const { return m_currentFile; }
    QStringList recentFiles() const { return m_recentFiles; }
    double playbackRate() const { return m_playbackRate; }
    QVideoSink* videoSink() const { return m_videoSink; }
    
    // 並排播放相關屬性
    bool isSideBySideMode() const { return m_isSideBySideMode; }
    QStringList sideBySideVideos() const { return m_sideBySideVideos; }
    bool sideBySideHorizontal() const { return m_sideBySideHorizontal; }

public slots:
    void loadVideo(const QString &filePath);
    void play();
    void pause();
    void stop();
    void setPosition(qint64 position);
    void playH264Direct(const QString &h264FilePath);
    void seekForward();
    void seekBackward();
    void setPlaybackRate(double rate);
    void setVideoSink(QVideoSink* sink);
    QStringList getVideoFiles(const QString &directory = "/home/mxpt2/Videos");
    
    // 簡化的並排播放方法
    void enableSideBySideMode(const QStringList &filePaths, bool horizontal = true);
    void disableSideBySideMode();
    
    // 並排模式的狀態同步
    Q_INVOKABLE void updateSideBySideDuration(qint64 duration);
    Q_INVOKABLE void updateSideBySidePosition(qint64 position);
    
    // 預覽模式控制
    Q_INVOKABLE void setPreviewPosition(qint64 position);

signals:
    void isPlayingChanged();
    void positionChanged();
    void durationChanged();
    void currentFileChanged();
    void recentFilesChanged();
    void playbackRateChanged();
    void videoSinkChanged();
    void videoLoaded();
    void playbackError(const QString &error);
    
    // 並排播放信號
    void sideBySideModeChanged();
    void sideBySideVideosChanged();
    void sideBySideHorizontalChanged();
    void sideBySidePlayersReady();

private slots:
    void updatePosition();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void updateRecentFiles(const QString &filePath);
    void refreshRecentFiles();
    void startGStreamerPlayback(const QString &filePath);
    void loadVideoEmbedded(const QString &filePath);
    void showLoadingFrame(const QString &filePath);
    void simulateVideoPlayback(const QString &filePath);
    void generateSimulatedFrame(const QString &filePath);
    void showVideoPlaybackInfo(const QString &filePath);
    void startEmbeddedMediaPlayer(const QString &filePath);
    QString formatTime(qint64 milliseconds) const;
    void clearSideBySideMediaPlayers();
    
    // 狀態管理
    bool m_isPlaying;
    qint64 m_position;
    qint64 m_duration;
    QString m_currentFile;
    QStringList m_recentFiles;
    double m_playbackRate;
    
    // 定時器更新進度
    QTimer *m_positionTimer;
    QTimer *m_simulationTimer;
    
    // GStreamer 進程播放器
    QProcess *m_gstProcess;
    
    // 嵌入式視訊輸出
    QVideoSink *m_videoSink;
    bool m_useEmbedded;
    
    // Qt MediaPlayer 用於真正的嵌入式播放
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    bool m_autoplayPending;
    
    // 並排播放私有成員
    bool m_isSideBySideMode;
    QStringList m_sideBySideVideos;
    bool m_sideBySideHorizontal;
    
    // 主從視頻同步
    int m_masterVideoIndex;           // 主控視頻索引 (通常是0)
    qint64 m_masterDuration;          // 主控視頻的duration
    bool m_isSeekingInProgress;       // 防止seek循環的標志
    
    // 多播放器支援並排播放 (QML創建的MediaPlayer引用)
    std::vector<QMediaPlayer*> m_sideBySideMediaPlayers;
};

#endif // VIDEOPLAYER_H