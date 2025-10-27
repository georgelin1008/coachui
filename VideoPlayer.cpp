#include "VideoPlayer.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QVideoFrame>
#include <QVideoSink>
#include <QVideoFrameFormat>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <cstring>

VideoPlayer::VideoPlayer(QObject *parent)
    : QObject(parent)
    , m_isPlaying(false)
    , m_position(0)
    , m_duration(0)
    , m_playbackRate(1.0)
    , m_positionTimer(new QTimer(this))
    , m_simulationTimer(new QTimer(this))
    , m_isSideBySideMode(false)
    , m_sideBySideHorizontal(true)
    , m_masterVideoIndex(0)
    , m_masterDuration(0)
    , m_gstProcess(nullptr)
    , m_videoSink(nullptr)
    , m_useEmbedded(true)
    , m_mediaPlayer(nullptr)
    , m_audioOutput(nullptr)
{
    // 設定進度更新定時器
    m_positionTimer->setInterval(100); // 100ms 更新一次
    connect(m_positionTimer, &QTimer::timeout, this, &VideoPlayer::updatePosition);
    
    // 初始化 MediaPlayer
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    
    // 連接 MediaPlayer 信號
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        bool wasPlaying = m_isPlaying;
        m_isPlaying = (state == QMediaPlayer::PlayingState);
        if (wasPlaying != m_isPlaying) {
            emit isPlayingChanged();
        }
    });
    
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        m_position = position;
        emit positionChanged();
    });
    
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        m_duration = duration;
        emit durationChanged();
    });
    
    // 連接播放狀態變化
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        bool wasPlaying = m_isPlaying;
        m_isPlaying = (state == QMediaPlayer::PlayingState);
        
        if (m_isPlaying && !wasPlaying) {
            m_positionTimer->start();
        } else if (!m_isPlaying && wasPlaying) {
            m_positionTimer->stop();
        }
        
        emit isPlayingChanged();
        qDebug() << "MediaPlayer state changed to:" << state << "isPlaying:" << m_isPlaying;
    });
    
    // 載入最近播放的檔案
    refreshRecentFiles();
    
    qDebug() << "Embedded VideoPlayer with QMediaPlayer initialized";
}

VideoPlayer::~VideoPlayer()
{
    if (m_gstProcess) {
        m_gstProcess->kill();
        m_gstProcess->deleteLater();
    }
    
    // 清理並排播放器
    clearSideBySideMediaPlayers();
}

void VideoPlayer::loadVideo(const QString &filePath)
{
    if (filePath.isEmpty()) {
        return;
    }
    
    // 檢查檔案是否存在
    QFile file(filePath);
    if (!file.exists()) {
        qWarning() << "Video file does not exist:" << filePath;
        emit playbackError("檔案不存在");
        return;
    }
    
    qDebug() << "Loading video:" << filePath;
    qDebug() << "File size:" << file.size() << "bytes";
    
    // 停止當前播放
    stop();
    
    m_currentFile = filePath;
    updateRecentFiles(filePath);
    emit currentFileChanged();
    
    // 重置位置和持續時間
    m_position = 0;
    // 估算持續時間（簡化實作，可以後續改用 ffprobe）
    m_duration = 120000; // 預設 2 分鐘，可以根據實際需求調整
    emit positionChanged();
    emit durationChanged();
    
    if (m_useEmbedded && m_videoSink) {
        // 使用嵌入式播放（簡化實作：直接顯示測試畫面）
        loadVideoEmbedded(filePath);
    } else {
        // 使用外部 GStreamer 進程播放
        startGStreamerPlayback(filePath);
    }
    
    emit videoLoaded();
    qDebug() << "Video loaded successfully, embedded mode:" << m_useEmbedded;
}

void VideoPlayer::playH264Direct(const QString &h264FilePath)
{
    qDebug() << "Playing H.264 file directly:" << h264FilePath;
    loadVideo(h264FilePath);
}

void VideoPlayer::play()
{
    // 並排模式 - 只更新狀態，讓QML處理實際播放
    if (m_isSideBySideMode) {
        m_isPlaying = true;
        emit isPlayingChanged();
        qDebug() << "Side-by-side mode: play signal emitted";
        return;
    }
    
    // 單一影片模式
    if (m_currentFile.isEmpty()) {
        qDebug() << "No video loaded";
        return;
    }
    
    if (m_useEmbedded && m_videoSink && m_mediaPlayer) {
        // 使用嵌入式 MediaPlayer
        m_mediaPlayer->play();
        qDebug() << "Embedded MediaPlayer play";
    } else if (m_simulationTimer && !m_simulationTimer->isActive()) {
        // 使用模擬播放
        m_isPlaying = true;
        m_simulationTimer->start();
        m_positionTimer->start();
        emit isPlayingChanged();
        qDebug() << "Simulation playback started";
    } else if (!m_useEmbedded || !m_videoSink) {
        // 外部進程播放
        startGStreamerPlayback(m_currentFile);
    }
}

void VideoPlayer::pause()
{
    // 並排模式 - 只更新狀態，讓QML處理實際暫停
    if (m_isSideBySideMode) {
        m_isPlaying = false;
        emit isPlayingChanged();
        qDebug() << "Side-by-side mode: pause signal emitted";
        return;
    }
    
    // 單一影片模式
    if (!m_useEmbedded || !m_mediaPlayer) {
        // GStreamer 模式或沒有媒體播放器
        return;
    }
    
    m_mediaPlayer->pause();
    qDebug() << "Video paused via QMediaPlayer";
}

void VideoPlayer::stop()
{
    // 並排模式 - 更新狀態並重置位置，讓QML處理實際停止
    if (m_isSideBySideMode) {
        m_isPlaying = false;
        m_position = 0;
        emit isPlayingChanged();
        emit positionChanged();
        qDebug() << "Side-by-side mode: stop signal emitted";
        return;
    }
    
    // 單一影片模式
    if (m_useEmbedded && m_videoSink && m_mediaPlayer) {
        // 使用嵌入式 MediaPlayer
        m_mediaPlayer->stop();
        qDebug() << "Embedded MediaPlayer stop";
    } else if (m_simulationTimer) {
        // 停止模擬播放
        m_isPlaying = false;
        m_position = 0;
        m_simulationTimer->stop();
        m_positionTimer->stop();
        emit isPlayingChanged();
        emit positionChanged();
        
        // 清空 VideoSink
        if (m_videoSink) {
            m_videoSink->setVideoFrame(QVideoFrame());
        }
        qDebug() << "Simulation playback stopped";
    } else if (m_gstProcess) {
        // 外部進程停止
        qDebug() << "Stopping GStreamer playback";
        m_gstProcess->terminate();
        if (!m_gstProcess->waitForFinished(3000)) {
            m_gstProcess->kill();
        }
        m_gstProcess->deleteLater();
        m_gstProcess = nullptr;
        
        m_isPlaying = false;
        m_position = 0;
        m_positionTimer->stop();
        emit isPlayingChanged();
        emit positionChanged();
    }
}

void VideoPlayer::seekForward()
{
    if (!m_currentFile.isEmpty()) {
        qDebug() << "Seeking forward 10 seconds";
        
        qint64 currentPos = m_position;
        if (m_useEmbedded && m_mediaPlayer) {
            currentPos = m_mediaPlayer->position();
        }
        
        qint64 newPosition = currentPos + 10000; // 10 秒 = 10000ms
        
        if (newPosition > m_duration && m_duration > 0) {
            newPosition = m_duration;
        }
        
        setPosition(newPosition);
        qDebug() << "Seek forward completed, new position:" << newPosition;
    }
}

void VideoPlayer::seekBackward()
{
    if (!m_currentFile.isEmpty()) {
        qDebug() << "Seeking backward 10 seconds";
        
        qint64 currentPos = m_position;
        if (m_useEmbedded && m_mediaPlayer) {
            currentPos = m_mediaPlayer->position();
        }
        
        qint64 newPosition = currentPos - 10000; // 10 秒 = 10000ms
        
        if (newPosition < 0) {
            newPosition = 0;
        }
        
        setPosition(newPosition);
        qDebug() << "Seek backward completed, new position:" << newPosition;
    }
}

void VideoPlayer::setPosition(qint64 position)
{
    if (m_isSideBySideMode) {
        // 使用主視頻的duration作為限制
        if (m_masterDuration > 0 && position > m_masterDuration) {
            position = m_masterDuration;
        }
        if (position < 0) {
            position = 0;
        }
        
        // 立即更新位置狀態，實現實時預覽
        m_position = position;
        emit positionChanged();
        
        qDebug() << "Side-by-side real-time seek to:" << position << "ms";
        return;
    }
    
    // 單一播放模式 - 支持實時預覽
    if (position < 0) {
        position = 0;
    }
    if (m_duration > 0 && position > m_duration) {
        position = m_duration;
    }
    
    if (m_useEmbedded && m_mediaPlayer) {
        // 使用嵌入式 MediaPlayer - 實時seek
        m_mediaPlayer->setPosition(position);
        qDebug() << "Real-time seek to:" << position << "ms";
    } else {
        // 用於模擬播放
        m_position = position;
        emit positionChanged();
    }
}

void VideoPlayer::startGStreamerPlayback(const QString &filePath)
{
    if (m_gstProcess) {
        stop();
    }
    
    m_gstProcess = new QProcess(this);
    connect(m_gstProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &VideoPlayer::onProcessFinished);
    
    QStringList arguments;
    
    // 根據檔案類型選擇不同的 GStreamer 管線，支援播放速度控制
    if (filePath.endsWith(".h264")) {
        qDebug() << "Playing H.264 raw stream with rate:" << m_playbackRate;
        arguments << "filesrc" << QString("location=%1").arg(filePath)
                  << "!" << "h264parse"
                  << "!" << "avdec_h264"
                  << "!" << "videoconvert"
                  << "!" << "videorate"
                  << "!" << QString("video/x-raw,framerate=%1/1").arg(static_cast<int>(30 * m_playbackRate))
                  << "!" << "autovideosink";
    } else if (filePath.endsWith(".mp4")) {
        qDebug() << "Playing MP4 file with rate:" << m_playbackRate;
        arguments << "filesrc" << QString("location=%1").arg(filePath)
                  << "!" << "qtdemux"
                  << "!" << "h264parse"
                  << "!" << "avdec_h264"
                  << "!" << "videoconvert"
                  << "!" << "videorate"
                  << "!" << QString("video/x-raw,framerate=%1/1").arg(static_cast<int>(30 * m_playbackRate))
                  << "!" << "autovideosink";
    } else {
        qDebug() << "Playing generic video file with playbin";
        arguments << "playbin" << QString("uri=file://%1").arg(filePath);
        // playbin 播放速度將在啟動後透過 GStreamer 命令設定
    }
    
    qDebug() << "Starting GStreamer with arguments:" << arguments;
    
    m_gstProcess->start("gst-launch-1.0", arguments);
    
    if (m_gstProcess->waitForStarted()) {
        m_isPlaying = true;
        m_positionTimer->start();
        emit isPlayingChanged();
        qDebug() << "GStreamer playback started";
    } else {
        qWarning() << "Failed to start GStreamer process";
        emit playbackError("無法啟動 GStreamer 播放器");
    }
}

void VideoPlayer::setPlaybackRate(double rate)
{
    // 限制播放速度範圍 (0.1x 到 10x)
    double clampedRate = qBound(0.1, rate, 10.0);
    
    if (qAbs(m_playbackRate - clampedRate) > 0.01) {
        double oldRate = m_playbackRate;
        m_playbackRate = clampedRate;
        emit playbackRateChanged();
        
        if (m_useEmbedded && m_mediaPlayer) {
            // 使用嵌入式 MediaPlayer 設定播放速度
            if (m_mediaPlayer->playbackState() != QMediaPlayer::StoppedState) {
                m_mediaPlayer->setPlaybackRate(clampedRate);
                qDebug() << "MediaPlayer playback rate changed from" << oldRate << "to" << clampedRate;
            } else {
                qDebug() << "MediaPlayer not active, rate will be applied on next play:" << clampedRate;
            }
        } else if (m_simulationTimer && m_simulationTimer->isActive()) {
            // 對於模擬播放，調整計時器間隔
            int newInterval = qMax(50, (int)(100 / clampedRate)); // 基本間隔100ms，根據速率調整
            m_simulationTimer->setInterval(newInterval);
            qDebug() << "Simulation timer interval adjusted to:" << newInterval << "ms for rate:" << clampedRate;
        }
        
        // 更新嵌入式顯示（如果正在使用）
        if (m_useEmbedded && m_videoSink && !m_currentFile.isEmpty()) {
            if (m_mediaPlayer) {
                // MediaPlayer 模式不需要額外更新
            } else {
                // 模擬播放模式，立即更新顯示
                generateSimulatedFrame(m_currentFile);
            }
        }
        
        qDebug() << "Playback rate successfully set to:" << m_playbackRate << "x";
    }
}

void VideoPlayer::updatePosition()
{
    // 根據播放速度更新位置
    if (m_isPlaying) {
        int increment = static_cast<int>(100 * m_playbackRate); // 基礎 100ms * 播放速度
        m_position += increment;
        
        // 檢查是否到達結尾
        if (m_position >= m_duration) {
            m_position = m_duration;
            stop();
        }
        
        emit positionChanged();
        
        // 如果是嵌入式播放，更新畫面
        if (m_useEmbedded && m_videoSink && !m_currentFile.isEmpty()) {
            loadVideoEmbedded(m_currentFile);
        }
    }
}

void VideoPlayer::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "GStreamer process finished with exit code:" << exitCode;
    m_isPlaying = false;
    m_positionTimer->stop();
    emit isPlayingChanged();
    
    if (m_gstProcess) {
        m_gstProcess->deleteLater();
        m_gstProcess = nullptr;
    }
}

void VideoPlayer::updateRecentFiles(const QString &filePath)
{
    m_recentFiles.removeAll(filePath);
    m_recentFiles.prepend(filePath);
    
    if (m_recentFiles.size() > 20) {
        m_recentFiles.removeLast();
    }
    
    emit recentFilesChanged();
}

void VideoPlayer::refreshRecentFiles()
{
    // 這裡可以從設定檔載入最近的檔案
    // 目前先清空
    m_recentFiles.clear();
    emit recentFilesChanged();
}

void VideoPlayer::setVideoSink(QVideoSink* sink)
{
    if (m_videoSink != sink) {
        m_videoSink = sink;
        m_useEmbedded = (sink != nullptr);
        emit videoSinkChanged();
        qDebug() << "VideoSink set, embedded mode:" << m_useEmbedded;
    }
}

void VideoPlayer::loadVideoEmbedded(const QString &filePath)
{
    if (!m_videoSink) {
        qWarning() << "No video sink available for embedded playback";
        return;
    }
    
    qDebug() << "Starting TRUE embedded playback for:" << filePath;
    
    // 停止現有的播放
    if (m_simulationTimer) {
        m_simulationTimer->stop();
    }
    if (m_gstProcess) {
        m_gstProcess->kill();
        m_gstProcess->deleteLater();
        m_gstProcess = nullptr;
    }
    
    // 使用 Qt 的 QMediaPlayer 來實現真正的嵌入式播放
    startEmbeddedMediaPlayer(filePath);
}

void VideoPlayer::showLoadingFrame(const QString &filePath)
{
    if (!m_videoSink) {
        return;
    }
    
    // 創建載入中的畫面
    QSize frameSize(640, 480);
    QImage loadingImage(frameSize, QImage::Format_RGB32);
    loadingImage.fill(QColor(30, 30, 30));
    
    QPainter painter(&loadingImage);
    painter.setPen(QPen(Qt::white, 2));
    painter.setFont(QFont("Arial", 20));
    
    QString fileName = QFileInfo(filePath).baseName();
    QString loadingText = QString("正在載入影片...\n\n📁 %1").arg(fileName);
    
    painter.drawText(loadingImage.rect(), Qt::AlignCenter, loadingText);
    
    // 添加載入動畫效果 (簡單的進度條)
    int barWidth = 300;
    int barHeight = 6;
    int barX = (frameSize.width() - barWidth) / 2;
    int barY = frameSize.height() / 2 + 80;
    
    painter.fillRect(barX, barY, barWidth, barHeight, Qt::darkGray);
    painter.fillRect(barX, barY, barWidth / 3, barHeight, Qt::cyan);
    
    painter.end();
    
    // 創建 VideoFrame 並發送到 VideoSink
    QVideoFrameFormat frameFormat(frameSize, QVideoFrameFormat::Format_BGRA8888);
    QVideoFrame frame(frameFormat);
    
    if (frame.isValid() && frame.map(QVideoFrame::WriteOnly)) {
        QImage convertedImage = loadingImage.convertToFormat(QImage::Format_ARGB32);
        memcpy(frame.bits(0), convertedImage.constBits(), 
               qMin(frame.mappedBytes(0), (int)convertedImage.sizeInBytes()));
        frame.unmap();
        
        if (m_videoSink) {
            m_videoSink->setVideoFrame(frame);
        }
    }
    
    qDebug() << "Loading frame displayed for:" << fileName;
}

void VideoPlayer::showVideoPlaybackInfo(const QString &filePath)
{
    if (!m_videoSink) {
        return;
    }
    
    // 創建資訊顯示畫面
    QSize frameSize(640, 480);
    QImage infoImage(frameSize, QImage::Format_RGB32);
    infoImage.fill(QColor(20, 20, 40));
    
    QPainter painter(&infoImage);
    painter.setPen(QPen(Qt::white, 2));
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    
    QString fileName = QFileInfo(filePath).baseName();
    
    // 顯示播放資訊
    painter.drawText(QRect(20, 50, frameSize.width()-40, 50), 
                     Qt::AlignCenter, QString("🎬 正在播放"));
    
    painter.setFont(QFont("Arial", 16));
    painter.drawText(QRect(20, 120, frameSize.width()-40, 40), 
                     Qt::AlignCenter, fileName);
    
    // 顯示說明文字
    painter.setFont(QFont("Arial", 14));
    painter.setPen(QPen(Qt::lightGray, 1));
    
    QStringList infoLines;
    infoLines << "影片正在外部 GStreamer 視窗中播放";
    infoLines << "";
    infoLines << "使用播放控制按鈕來操作：";
    infoLines << "• 播放/暫停：控制播放狀態";
    infoLines << "• 停止：停止播放並關閉視窗";
    infoLines << "• 快進/後退：跳轉播放位置";
    infoLines << "• 播放速度：調整播放速率";
    
    int yPos = 220;
    for (const QString &line : infoLines) {
        painter.drawText(QRect(40, yPos, frameSize.width()-80, 25), 
                         Qt::AlignLeft, line);
        yPos += 30;
    }
    
    // 添加狀態指示
    painter.setPen(QPen(Qt::cyan, 2));
    painter.setFont(QFont("Arial", 12));
    QString statusText = m_isPlaying ? "▶ 播放中" : "⏸ 已暫停";
    painter.drawText(QRect(20, 420, frameSize.width()-40, 30), 
                     Qt::AlignCenter, statusText);
    
    painter.end();
    
    // 創建 VideoFrame 並發送到 VideoSink
    QVideoFrameFormat frameFormat(frameSize, QVideoFrameFormat::Format_BGRA8888);
    QVideoFrame frame(frameFormat);
    
    if (frame.isValid() && frame.map(QVideoFrame::WriteOnly)) {
        QImage convertedImage = infoImage.convertToFormat(QImage::Format_ARGB32);
        memcpy(frame.bits(0), convertedImage.constBits(), 
               qMin(frame.mappedBytes(0), (int)convertedImage.sizeInBytes()));
        frame.unmap();
        
        if (m_videoSink) {
            m_videoSink->setVideoFrame(frame);
        }
    }
    
    qDebug() << "Video playback info displayed for:" << fileName;
}

void VideoPlayer::startEmbeddedMediaPlayer(const QString &filePath)
{
    if (!m_mediaPlayer || !m_videoSink) {
        qWarning() << "MediaPlayer or VideoSink not available";
        return;
    }
    
    qDebug() << "Starting embedded MediaPlayer for:" << filePath;
    
    // 設定 VideoSink
    m_mediaPlayer->setVideoSink(m_videoSink);
    
    // 載入影片檔案
    QUrl fileUrl = QUrl::fromLocalFile(filePath);
    m_mediaPlayer->setSource(fileUrl);
    
    qDebug() << "MediaPlayer source set to:" << fileUrl;
    
    // 自動開始播放
    m_mediaPlayer->play();
    
    qDebug() << "Embedded MediaPlayer started";
}

void VideoPlayer::simulateVideoPlayback(const QString &filePath)
{
    if (!m_videoSink) {
        return;
    }
    
    qDebug() << "Starting simulated video playback for:" << filePath;
    
    // 設定模擬參數
    m_duration = 60000; // 60秒模擬影片
    m_position = 0;
    emit durationChanged();
    emit positionChanged();
    
    // 設定模擬計時器 - 每100ms更新一次畫面
    m_simulationTimer->setInterval(100);
    
    connect(m_simulationTimer, &QTimer::timeout, this, [this, filePath]() {
        if (!m_isPlaying) {
            return;
        }
        
        // 更新播放位置
        m_position += 100 * m_playbackRate;
        if (m_position >= m_duration) {
            m_position = 0; // 循環播放
        }
        emit positionChanged();
        
        // 生成模擬的視頻幀
        generateSimulatedFrame(filePath);
    });
    
    // 自動開始播放
    m_isPlaying = true;
    m_simulationTimer->start();
    emit isPlayingChanged();
    
    qDebug() << "Simulated playback started, embedded mode active";
}

void VideoPlayer::generateSimulatedFrame(const QString &filePath)
{
    if (!m_videoSink) {
        return;
    }
    
    QSize frameSize(640, 480);
    QImage frame(frameSize, QImage::Format_RGB32);
    
    // 創建動畫背景
    int timeMs = (int)(m_position / m_playbackRate);
    QColor bgColor = QColor::fromHsv((timeMs / 100) % 360, 100, 50);
    frame.fill(bgColor);
    
    QPainter painter(&frame);
    painter.setPen(QPen(Qt::white, 2));
    
    // 顯示檔案資訊
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    QString fileName = QFileInfo(filePath).baseName();
    painter.drawText(20, 30, QString("📹 %1").arg(fileName));
    
    // 顯示播放狀態
    QString statusText = m_isPlaying ? "▶ 播放中" : "⏸ 已暫停";
    painter.drawText(20, 60, statusText);
    
    // 顯示時間資訊
    painter.setFont(QFont("Arial", 14));
    QString timeText = QString("時間: %1 / %2").arg(formatTime(m_position)).arg(formatTime(m_duration));
    painter.drawText(20, 90, timeText);
    
    // 顯示播放速度
    painter.drawText(20, 120, QString("速度: %1x").arg(m_playbackRate, 0, 'f', 1));
    
    // 繪製進度條
    int progressWidth = frameSize.width() - 40;
    int progressHeight = 15;
    int progressX = 20;
    int progressY = frameSize.height() - 50;
    
    painter.fillRect(progressX, progressY, progressWidth, progressHeight, Qt::darkGray);
    
    if (m_duration > 0) {
        double progress = (double)m_position / m_duration;
        int fillWidth = (int)(progressWidth * progress);
        painter.fillRect(progressX, progressY, fillWidth, progressHeight, Qt::cyan);
    }
    
    // 添加動畫效果 - 旋轉的圓圈
    painter.setPen(QPen(Qt::yellow, 3));
    int centerX = frameSize.width() / 2;
    int centerY = frameSize.height() / 2 + 50;
    int radius = 30;
    int angle = (timeMs / 10) % 360;
    
    painter.translate(centerX, centerY);
    painter.rotate(angle);
    painter.drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter.drawLine(-radius, 0, radius, 0);
    painter.drawLine(0, -radius, 0, radius);
    
    painter.end();
    
    // 創建 VideoFrame 並發送
    QVideoFrameFormat frameFormat(frameSize, QVideoFrameFormat::Format_BGRA8888);
    QVideoFrame videoFrame(frameFormat);
    
    if (videoFrame.isValid() && videoFrame.map(QVideoFrame::WriteOnly)) {
        QImage convertedImage = frame.convertToFormat(QImage::Format_ARGB32);
        memcpy(videoFrame.bits(0), convertedImage.constBits(), 
               qMin(videoFrame.mappedBytes(0), (int)convertedImage.sizeInBytes()));
        videoFrame.unmap();
        
        m_videoSink->setVideoFrame(videoFrame);
    }
}

QString VideoPlayer::formatTime(qint64 milliseconds) const
{
    int seconds = (int)(milliseconds / 1000);
    int minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

QStringList VideoPlayer::getVideoFiles(const QString &directory)
{
    QStringList videoFiles;
    QDir dir(directory);
    
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << directory;
        return videoFiles;
    }
    
    // 設定檔案過濾器，只顯示MP4檔案
    QStringList filters;
    filters << "*.mp4";
    
    // 按修改時間排序，最新的在前面
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files, QDir::Time);
    
    foreach (const QFileInfo &fileInfo, fileInfoList) {
        videoFiles.append(fileInfo.absoluteFilePath());
    }
    
    // 添加測試檔案（如果存在）
    QString testVideoPath = "/home/mxpt2/coachui/test_video.mp4";
    QFileInfo testFileInfo(testVideoPath);
    if (testFileInfo.exists() && !videoFiles.contains(testVideoPath)) {
        videoFiles.append(testVideoPath);
    }
    
    qDebug() << "Found" << videoFiles.count() << "MP4 files in" << directory;
    return videoFiles;
}

// =============== 並排播放功能實現 ===============

void VideoPlayer::enableSideBySideMode(const QStringList &filePaths, bool horizontal)
{
    if (filePaths.size() < 2) {
        emit playbackError("需要至少兩個影片檔案");
        return;
    }

    m_sideBySideVideos = filePaths.mid(0, 2); // 只取前兩個
    m_sideBySideHorizontal = horizontal;
    m_isSideBySideMode = true;
    m_masterVideoIndex = 0;           // 第一個視頻為主控
    m_masterDuration = 0;             // 待QML更新
    m_isSeekingInProgress = false;
    
    emit sideBySideModeChanged();
    emit sideBySideVideosChanged();
    emit sideBySideHorizontalChanged();
    
    qDebug() << "Side-by-side mode enabled with master video index:" << m_masterVideoIndex;
}

void VideoPlayer::disableSideBySideMode()
{
    m_isSideBySideMode = false;
    m_sideBySideVideos.clear();
    
    emit sideBySideModeChanged();
    emit sideBySideVideosChanged();
    
    qDebug() << "Side-by-side mode cleared, back to single video mode";
}

void VideoPlayer::updateSideBySideDuration(qint64 duration)
{
    if (!m_isSideBySideMode) return;
    
    // 只接受主視頻(索引0)的duration
    m_masterDuration = duration;
    m_duration = duration;  
    emit durationChanged();
    
    qDebug() << "Master video duration set to:" << duration << "ms";
}

void VideoPlayer::updateSideBySidePosition(qint64 position)
{
    if (!m_isSideBySideMode || m_isSeekingInProgress) return;
    
    // 範圍檢查
    if (m_masterDuration > 0 && position > m_masterDuration) {
        return; // 忽略超出範圍的position更新
    }
    
    m_position = position;
    emit positionChanged();
}

void VideoPlayer::setPreviewPosition(qint64 position)
{
    // 預覽模式 - 只移動畫面，不觸發播放狀態變化
    if (m_isSideBySideMode) {
        // 並排模式預覽
        if (m_masterDuration > 0 && position > m_masterDuration) {
            position = m_masterDuration;
        }
        if (position < 0) {
            position = 0;
        }
        
        // 只更新位置，不觸發其他狀態
        m_position = position;
        emit positionChanged();
        
    } else {
        // 單一視頻模式預覽
        if (position < 0) {
            position = 0;
        }
        if (m_duration > 0 && position > m_duration) {
            position = m_duration;
        }
        
        if (m_useEmbedded && m_mediaPlayer) {
            // 直接設置MediaPlayer位置，實現靜默預覽
            m_mediaPlayer->setPosition(position);
        } else {
            m_position = position;
            emit positionChanged();
        }
    }
    
    qDebug() << "Preview position set to:" << position << "ms";
}

void VideoPlayer::clearSideBySideMediaPlayers()
{
    // MediaPlayer現在在QML中管理，我們只清理引用
    for (auto *player : m_sideBySideMediaPlayers) {
        if (player) {
            player->stop();
            // 不delete，因為是QML創建的
        }
    }
    
    m_sideBySideMediaPlayers.clear();
}

