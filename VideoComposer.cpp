#include "VideoComposer.h"
#include "KeyframeManager.h"
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <QtConcurrent/QtConcurrent>

VideoComposer::VideoComposer(QObject *parent)
    : QObject(parent)
{
    qDebug() << "VideoComposer initialized";
    setStatus("VideoComposer 已初始化");
}

VideoComposer::~VideoComposer()
{
    qDebug() << "VideoComposer destroyed";
}

void VideoComposer::testFunction()
{
    qDebug() << "VideoComposer test function called";
    m_videoCount++;
    emit videoCountChanged();
    
    setStatus("測試功能已執行 " + QString::number(m_videoCount) + " 次");
    emit testCompleted("測試成功！執行次數: " + QString::number(m_videoCount));
}

QStringList VideoComposer::getTestData()
{
    QStringList testData;
    testData << "測試項目1" << "測試項目2" << "測試項目3";
    
    qDebug() << "Returning test data:" << testData;
    return testData;
}

void VideoComposer::loadVideos(const QStringList &filePaths)
{
    setStatus("載入影片中...");
    
    for (const QString &path : filePaths) {
        QFileInfo fileInfo(path);
        if (!fileInfo.exists()) {
            emit videoLoadError("檔案不存在: " + path);
            continue;
        }
        
        VideoInfo video;
        video.filePath = path;
        video.name = fileInfo.baseName();
        
        // 使用OpenCV載入影片
        bool opened = false;
        
        // 如果是 h264 格式，使用 GStreamer pipeline
        if (path.endsWith(".h264", Qt::CaseInsensitive)) {
            // 嘗試硬體解碼
            QString gstPipeline = QString(
                "filesrc location=\"%1\" ! "
                "h264parse ! "
                "v4l2h264dec ! "
                "videoconvert ! "
                "video/x-raw,format=BGR ! "
                "appsink"
            ).arg(path);
            
            qDebug() << "嘗試 GStreamer 硬體解碼 h264:" << path;
            opened = video.capture.open(gstPipeline.toStdString(), cv::CAP_GSTREAMER);
            
            if (!opened) {
                // 備用：軟體解碼
                gstPipeline = QString(
                    "filesrc location=\"%1\" ! "
                    "h264parse ! "
                    "avdec_h264 ! "
                    "videoconvert ! "
                    "video/x-raw,format=BGR ! "
                    "appsink"
                ).arg(path);
                
                qDebug() << "嘗試 GStreamer 軟體解碼 h264";
                opened = video.capture.open(gstPipeline.toStdString(), cv::CAP_GSTREAMER);
            }
        } else {
            // MP4 等其他格式直接打開
            opened = video.capture.open(path.toStdString());
        }
        
        if (!opened || !video.capture.isOpened()) {
            emit videoLoadError("無法開啟影片: " + path);
            continue;
        }
        
        // 獲取影片資訊
        video.frameCount = static_cast<int>(video.capture.get(cv::CAP_PROP_FRAME_COUNT));
        video.fps = video.capture.get(cv::CAP_PROP_FPS);
        video.width = static_cast<int>(video.capture.get(cv::CAP_PROP_FRAME_WIDTH));
        video.height = static_cast<int>(video.capture.get(cv::CAP_PROP_FRAME_HEIGHT));
        
        m_videos.push_back(video);
        m_loadedVideos.append(video.name);
        m_videoCount++;
        
        // 添加到QVariantList供QML使用
        QVariantMap videoMap;
        videoMap["name"] = video.name;
        videoMap["filePath"] = video.filePath;
        videoMap["selected"] = video.selected;
        videoMap["info"] = QString("%1x%2, %3 fps, %4 frames")
                          .arg(video.width).arg(video.height)
                          .arg(video.fps).arg(video.frameCount);
        m_videoList.append(videoMap);
        
        emit videoLoaded(video.name);
        emit videoListChanged();
        qDebug() << "Loaded video:" << video.name 
                 << "Size:" << video.width << "x" << video.height 
                 << "FPS:" << video.fps 
                 << "Frames:" << video.frameCount;
    }
    
    // 重新計算選擇數量
    m_selectedVideoCount = 0;
    for (const auto& video : m_videos) {
        if (video.selected) m_selectedVideoCount++;
    }
    
    emit videoCountChanged();
    emit loadedVideosChanged();
    emit selectedVideoCountChanged();
    
    if (m_videoCount > 0) {
        setStatus(QString("已載入 %1 個影片").arg(m_videoCount));
    } else {
        setStatus("沒有載入任何影片");
    }
}

void VideoComposer::clearVideos()
{
    // 釋放所有OpenCV VideoCapture
    for (auto &video : m_videos) {
        if (video.capture.isOpened()) {
            video.capture.release();
        }
    }
    
    m_videos.clear();
    m_loadedVideos.clear();
    m_videoList.clear();
    m_videoCount = 0;
    m_selectedVideoCount = 0;
    m_composedFrames.clear();
    
    emit videoCountChanged();
    emit loadedVideosChanged();
    emit videoListChanged();
    emit selectedVideoCountChanged();
    emit hasComposedFramesChanged();
    setStatus("已清除所有影片");
    
    qDebug() << "All videos cleared";
}

QString VideoComposer::getVideoInfo(int index)
{
    if (index < 0 || index >= static_cast<int>(m_videos.size())) {
        return "無效的索引";
    }
    
    const VideoInfo &video = m_videos[index];
    return QString("%1 (%2x%3, %4 fps, %5 frames)")
           .arg(video.name)
           .arg(video.width)
           .arg(video.height)
           .arg(video.fps)
           .arg(video.frameCount);
}

QStringList VideoComposer::getSelectedVideoPaths() const
{
    QStringList paths;
    for (int i = 0; i < static_cast<int>(m_videoList.size()); i++) {
        QVariantMap videoItem = m_videoList[i].toMap();
        if (videoItem.value("selected", false).toBool()) {
            paths.append(videoItem.value("filePath").toString());
        }
    }
    qDebug() << "Selected video paths:" << paths;
    return paths;
}

void VideoComposer::setStatus(const QString &status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
        qDebug() << "VideoComposer status:" << status;
    }
}

// 設定合成參數
void VideoComposer::setCompositionParams(double fgWeight, double bgWeight, int blurSize)
{
    setForegroundWeight(fgWeight);
    setBackgroundWeight(bgWeight);
    setBlurSize(blurSize);
}

void VideoComposer::setForegroundWeight(double weight)
{
    if (qAbs(m_fgWeight - weight) > 0.01) {
        m_fgWeight = weight;
        emit foregroundWeightChanged();
        qDebug() << "Foreground weight changed to:" << m_fgWeight;
    }
}

void VideoComposer::setBackgroundWeight(double weight)
{
    if (qAbs(m_bgWeight - weight) > 0.01) {
        m_bgWeight = weight;
        emit backgroundWeightChanged();
        qDebug() << "Background weight changed to:" << m_bgWeight;
    }
}

void VideoComposer::setBlurSize(int size)
{
    int actualSize = (size % 2 == 0) ? size + 1 : size;  // 確保模糊核為奇數
    if (m_blurSize != actualSize) {
        m_blurSize = actualSize;
        emit blurSizeChanged();
        qDebug() << "Blur size changed to:" << m_blurSize;
    }
}

// 新增算法參數設定函數
void VideoComposer::setBackgroundWindowSize(int size)
{
    if (m_backgroundWindowSize != size && size > 0) {
        m_backgroundWindowSize = size;
        emit backgroundWindowSizeChanged();
        qDebug() << "Background window size changed to:" << m_backgroundWindowSize;
    }
}

void VideoComposer::setEdgeEnhancement(bool enabled)
{
    if (m_edgeEnhancement != enabled) {
        m_edgeEnhancement = enabled;
        emit edgeEnhancementChanged();
        qDebug() << "Edge enhancement:" << (enabled ? "enabled" : "disabled");
    }
}

void VideoComposer::setCenterWeighting(bool enabled)
{
    if (m_centerWeighting != enabled) {
        m_centerWeighting = enabled;
        emit centerWeightingChanged();
        qDebug() << "Center weighting:" << (enabled ? "enabled" : "disabled");
    }
}

void VideoComposer::setThreshold(double threshold)
{
    if (qAbs(m_threshold - threshold) > 0.1) {
        m_threshold = threshold;
        emit thresholdChanged();
        qDebug() << "Threshold changed to:" << m_threshold;
    }
}

void VideoComposer::setAdaptiveThreshold(bool enabled)
{
    if (m_adaptiveThreshold != enabled) {
        m_adaptiveThreshold = enabled;
        emit adaptiveThresholdChanged();
        qDebug() << "Adaptive threshold:" << (enabled ? "enabled" : "disabled");
    }
}

// 獲取合成後的幀路徑
QStringList VideoComposer::getComposedFramePaths()
{
    QStringList paths;
    for (int i = 0; i < static_cast<int>(m_composedFrames.size()); ++i) {
        paths << QString("frame_%1").arg(i);
    }
    return paths;
}

// 載入影片的所有幀到記憶體
std::vector<cv::Mat> VideoComposer::loadVideoFrames(const QString& filePath)
{
    std::vector<cv::Mat> frames;
    cv::VideoCapture cap(filePath.toStdString());
    
    if (!cap.isOpened()) {
        qDebug() << "Cannot open video:" << filePath;
        return frames;
    }
    
    cv::Mat frame;
    while (cap.read(frame)) {
        frames.push_back(frame.clone());
    }
    
    cap.release();
    qDebug() << "Loaded" << frames.size() << "frames from" << filePath;
    return frames;
}

// 計算背景（使用滑動窗口中位數）
cv::Mat VideoComposer::computeBackground(const std::vector<cv::Mat>& frames, int currentIndex, int windowSize)
{
    if (frames.empty()) {
        return cv::Mat();
    }
    
    int halfWindow = windowSize / 2;
    int startIdx = std::max(0, currentIndex - halfWindow);
    int endIdx = std::min(static_cast<int>(frames.size()), currentIndex + halfWindow + 1);
    
    if (endIdx - startIdx <= 1) {
        return frames[currentIndex].clone();
    }
    
    // 收集窗口內的幀
    std::vector<cv::Mat> windowFrames;
    for (int i = startIdx; i < endIdx; ++i) {
        windowFrames.push_back(frames[i]);
    }
    
    // 計算中位數背景
    cv::Mat background = cv::Mat::zeros(frames[0].size(), frames[0].type());
    for (int row = 0; row < frames[0].rows; ++row) {
        for (int col = 0; col < frames[0].cols; ++col) {
            std::vector<cv::Vec3b> pixels;
            for (const auto& frame : windowFrames) {
                pixels.push_back(frame.at<cv::Vec3b>(row, col));
            }
            
            // 對每個通道計算中位數
            std::vector<uchar> b_values, g_values, r_values;
            for (const auto& pixel : pixels) {
                b_values.push_back(pixel[0]);
                g_values.push_back(pixel[1]);
                r_values.push_back(pixel[2]);
            }
            
            std::sort(b_values.begin(), b_values.end());
            std::sort(g_values.begin(), g_values.end());
            std::sort(r_values.begin(), r_values.end());
            
            int mid = b_values.size() / 2;
            background.at<cv::Vec3b>(row, col) = cv::Vec3b(
                b_values[mid], g_values[mid], r_values[mid]
            );
        }
    }
    
    return background;
}

// 增強遮罩
cv::Mat VideoComposer::enhanceMask(const cv::Mat& mask, const cv::Mat& grayFrame)
{
    cv::Mat enhanced = mask.clone();
    
    // 可以在此添加邊緣檢測和中心加權等增強功能
    // 目前保持簡單實現
    
    return enhanced;
}

// 核心合成算法
bool VideoComposer::composeVideos(const QStringList& selectedPaths)
{
    // Start composition in background and return immediately.
    if (selectedPaths.size() < 2) {
        emit compositionError("需要選擇至少兩個影片進行合成");
        return false;
    }

    // Reset cancel flag and emit started immediately so UI can update.
    m_cancelComposeRequested.store(false);
    emit compositionStarted();

    // Use QtConcurrent to run the heavy composition on a worker thread and store the future.
    m_composeFuture = QtConcurrent::run([this, selectedPaths]() -> bool {
        return this->composeVideosInternal(selectedPaths);
    });
    return true;
}

// 真正的同步實作，會在背景執行緒中被呼叫
bool VideoComposer::composeVideosInternal(const QStringList& selectedPaths)
{
    if (selectedPaths.size() < 2) {
        emit compositionError("需要選擇至少兩個影片進行合成");
        return false;
    }

    emit compositionStarted();
    setStatus(QString::fromUtf8("Loading video frames..."));

    // 載入所有選擇的影片幀
    std::vector<std::vector<cv::Mat>> allVideoFrames;
    int maxFrames = 0;
    cv::Size targetSize;

    for (const QString& path : selectedPaths) {
        std::vector<cv::Mat> frames = loadVideoFrames(path);
        if (frames.empty()) {
            emit compositionError(QString("Cannot load video: %1").arg(path));
            return false;
        }

        allVideoFrames.push_back(frames);
        maxFrames = std::max(maxFrames, static_cast<int>(frames.size()));

        if (targetSize.width == 0) {
            targetSize = frames[0].size();
        }
    }
    
    // 保存合成元數據供導出使用
    m_compositionFrameSize = targetSize;
    // 假設使用常見的幀率，或從 m_videos 中獲取（如果可用）
    m_compositionFps = 30.0;  // 默認 30fps
    if (!m_videos.empty() && m_videos[0].fps > 0 && m_videos[0].fps < 1000) {
        m_compositionFps = m_videos[0].fps;
    }
    
    qDebug() << "Composition metadata - Size:" << m_compositionFrameSize.width << "x" << m_compositionFrameSize.height 
             << "FPS:" << m_compositionFps;

    setStatus("Computing backgrounds...");

    // 為每個影片計算靜態背景
    std::vector<cv::Mat> backgrounds;
    for (const auto& videoFrames : allVideoFrames) {
        if (m_cancelComposeRequested.load()) {
            emit compositionError("Composition cancelled");
            setStatus("Composition cancelled");
            return false;
        }
        int referenceFrame = videoFrames.size() / 2;
        cv::Mat background = computeDynamicBackground(videoFrames, referenceFrame);

        if (background.empty()) {
            int framesToUse = std::min(30, static_cast<int>(videoFrames.size()));
            cv::Mat accumulator = cv::Mat::zeros(targetSize, CV_32FC3);
            for (int i = 0; i < framesToUse; ++i) {
                cv::Mat floatFrame;
                videoFrames[i].convertTo(floatFrame, CV_32FC3);
                accumulator += floatFrame;
            }
            accumulator /= framesToUse;
            accumulator.convertTo(background, CV_8UC3);
        }

        backgrounds.push_back(background);
        qDebug() << "Dynamic background computed for video with" << videoFrames.size() << "frames";
    }

    setStatus("Compositing videos...");
    m_composedFrames.clear();

    // 合成每一幀
    for (int frameIdx = 0; frameIdx < maxFrames; ++frameIdx) {
        // 協作式取消檢查
        if (m_cancelComposeRequested.load()) {
            emit compositionError("Composition cancelled");
            setStatus("Composition cancelled");
            return false;
        }

        emit compositionProgress(frameIdx + 1, maxFrames);

        cv::Mat blended = cv::Mat::zeros(targetSize, CV_32FC3);
        int validVideos = 0;

        for (int videoIdx = 0; videoIdx < static_cast<int>(allVideoFrames.size()); ++videoIdx) {
            const auto& videoFrames = allVideoFrames[videoIdx];
            const cv::Mat& background = backgrounds[videoIdx];

            cv::Mat currentFrame;
            if (frameIdx < static_cast<int>(videoFrames.size())) {
                currentFrame = videoFrames[frameIdx];
            } else {
                currentFrame = videoFrames.back();
            }

            cv::Mat mask = extractForeground(currentFrame, background);
            cv::Mat enhancedMask = enhanceForegroundMask(mask, currentFrame);

            cv::Mat blurredMask;
            if (m_blurSize > 1) {
                cv::GaussianBlur(enhancedMask, blurredMask, cv::Size(m_blurSize, m_blurSize), 0);
            } else {
                blurredMask = enhancedMask;
            }

            blurredMask.convertTo(blurredMask, CV_32F, 1.0/255.0);
            cv::Mat mask3Channel;
            cv::merge(std::vector<cv::Mat>{blurredMask, blurredMask, blurredMask}, mask3Channel);

            cv::Mat floatFrame, floatBackground;
            currentFrame.convertTo(floatFrame, CV_32FC3);
            background.convertTo(floatBackground, CV_32FC3);

            cv::Mat foreground = floatFrame.mul(mask3Channel) * m_fgWeight;
            cv::Mat bg = floatBackground.mul(cv::Scalar::all(1.0) - mask3Channel) * m_bgWeight;

            blended += (foreground + bg);
            validVideos++;
        }

        if (validVideos > 0) {
            blended /= validVideos;
            cv::Mat finalFrame;
            blended.convertTo(finalFrame, CV_8UC3);
            m_composedFrames.push_back(finalFrame);
        }
    }

    emit compositionCompleted(static_cast<int>(m_composedFrames.size()));
    emit hasComposedFramesChanged();
    setStatus(QString("Composition completed: %1 frames").arg(m_composedFrames.size()));

    return true;
}

// 添加單一影片
void VideoComposer::addVideo(const QString &filePath)
{
    QStringList singleFile;
    singleFile << filePath;
    loadVideos(singleFile);
}

// 移除影片
void VideoComposer::removeVideo(int index)
{
    if (index < 0 || index >= static_cast<int>(m_videos.size())) {
        return;
    }
    
    // 釋放OpenCV資源
    if (m_videos[index].capture.isOpened()) {
        m_videos[index].capture.release();
    }
    
    m_videos.erase(m_videos.begin() + index);
    m_loadedVideos.removeAt(index);
    m_videoList.removeAt(index);
    m_videoCount--;
    
    // 重新計算選擇數量
    m_selectedVideoCount = 0;
    for (const auto& video : m_videos) {
        if (video.selected) m_selectedVideoCount++;
    }
    
    emit videoCountChanged();
    emit loadedVideosChanged();
    emit videoListChanged();
    emit selectedVideoCountChanged();
    
    qDebug() << "Video removed at index:" << index;
}

// 切換影片選擇狀態
void VideoComposer::toggleVideoSelection(int index)
{
    if (index < 0 || index >= static_cast<int>(m_videos.size())) {
        return;
    }
    
    m_videos[index].selected = !m_videos[index].selected;
    
    // 更新QVariantList
    QVariantMap videoMap = m_videoList[index].toMap();
    videoMap["selected"] = m_videos[index].selected;
    m_videoList[index] = videoMap;
    
    // 重新計算選擇數量
    m_selectedVideoCount = 0;
    for (const auto& video : m_videos) {
        if (video.selected) m_selectedVideoCount++;
    }
    
    emit videoListChanged();
    emit selectedVideoCountChanged();
    
    qDebug() << "Video selection toggled for:" << m_videos[index].name 
             << "Now selected:" << m_videos[index].selected;
}

// 合成選中的影片
bool VideoComposer::composeSelectedVideos()
{
    QStringList selectedPaths;
    for (const auto& video : m_videos) {
        if (video.selected) {
            selectedPaths.append(video.filePath);
        }
    }
    
    if (selectedPaths.size() < 2) {
        emit compositionError("需要選擇至少兩個影片進行合成");
        return false;
    }
    
    return composeVideos(selectedPaths);
}

// 從對話框載入影片（預留功能）
void VideoComposer::loadVideosFromDialog()
{
    // 這個功能需要在有GUI的環境下實現
    qDebug() << "loadVideosFromDialog called (需要GUI環境)";
    setStatus("File dialog requires GUI environment");
}

// 導出合成影片（啟動於背景執行緒）
bool VideoComposer::exportComposedVideo(const QString& outputPath)
{
    if (m_composedFrames.empty()) {
        emit exportError("No composed frames to export");
        return false;
    }

    // Reset cancel flag and emit started immediately
    m_cancelExportRequested.store(false);
    emit exportStarted();

    // Run export in background and keep future
    m_exportFuture = QtConcurrent::run([this, outputPath]() -> bool {
        return this->exportComposedVideoInternal(outputPath);
    });
    return true;
}

bool VideoComposer::exportComposedVideoInternal(const QString& outputPath)
{
    if (m_composedFrames.empty()) {
        emit exportError("No composed frames to export");
        return false;
    }

    emit exportStarted();
    setStatus("Exporting video...");

    // 使用合成時保存的元數據
    double fps = m_compositionFps;
    cv::Size frameSize = m_compositionFrameSize;
    
    // 驗證元數據
    if (frameSize.width <= 0 || frameSize.height <= 0) {
        frameSize = m_composedFrames[0].size();
        qDebug() << "Using frame size from composed frames:" << frameSize.width << "x" << frameSize.height;
    }
    
    if (fps <= 0 || fps > 1000) {
        fps = 30.0;  // 默認 30fps
        qDebug() << "Invalid fps, using default: 30.0";
    }
    
    qDebug() << "Export parameters - FPS:" << fps << "Size:" << frameSize.width << "x" << frameSize.height;

    cv::VideoWriter writer;
    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');  // MP4 codec

    if (!writer.open(outputPath.toStdString(), codec, fps, frameSize, true)) {
        emit exportError(QString("Cannot create video writer: %1").arg(outputPath));
        return false;
    }

    for (size_t i = 0; i < m_composedFrames.size(); ++i) {
        if (m_cancelExportRequested.load()) {
            writer.release();
            emit exportError("Export cancelled");
            setStatus("Export cancelled");
            return false;
        }

        writer.write(m_composedFrames[i]);
        emit exportProgress(static_cast<int>(i + 1), static_cast<int>(m_composedFrames.size()));

        if (i % 100 == 0 || i == m_composedFrames.size() - 1) {
            setStatus(QString("Exporting: %1/%2 frames").arg(i + 1).arg(m_composedFrames.size()));
        }
    }

    writer.release();

    emit exportCompleted(outputPath);
    setStatus(QString("Video exported: %1").arg(outputPath));

    qDebug() << "Video exported successfully to:" << outputPath;
    return true;
}

// 取消合成 / 導出 的協作式取消 API
void VideoComposer::cancelCompose()
{
    m_cancelComposeRequested.store(true);
    if (m_composeFuture.isRunning()) {
        m_composeFuture.cancel(); // cooperative
    }
}

void VideoComposer::cancelExport()
{
    m_cancelExportRequested.store(true);
    if (m_exportFuture.isRunning()) {
        m_exportFuture.cancel();
    }
}

bool VideoComposer::isComposing() const
{
    return m_composeFuture.isRunning();
}

bool VideoComposer::isExporting() const
{
    return m_exportFuture.isRunning();
}

// =============== 新增的進階算法函數 ===============

// 計算動態背景（使用滑動窗口和更智慧的中位數計算）
cv::Mat VideoComposer::computeDynamicBackground(const std::vector<cv::Mat>& frames, int currentIndex)
{
    if (frames.empty() || currentIndex < 0 || currentIndex >= static_cast<int>(frames.size())) {
        return cv::Mat();
    }
    
    int halfWindow = m_backgroundWindowSize / 2;
    int startIdx = std::max(0, currentIndex - halfWindow);
    int endIdx = std::min(static_cast<int>(frames.size()), currentIndex + halfWindow + 1);
    
    // 如果窗口太小，擴大窗口範圍
    if (endIdx - startIdx < 3) {
        startIdx = std::max(0, currentIndex - 5);
        endIdx = std::min(static_cast<int>(frames.size()), currentIndex + 6);
    }
    
    if (endIdx - startIdx <= 1) {
        return frames[currentIndex].clone();
    }
    
    cv::Mat background = cv::Mat::zeros(frames[0].size(), frames[0].type());
    
    // 使用更高效的分塊處理
    int blockSize = 16;
    for (int row = 0; row < frames[0].rows; row += blockSize) {
        for (int col = 0; col < frames[0].cols; col += blockSize) {
            int endRow = std::min(row + blockSize, frames[0].rows);
            int endCol = std::min(col + blockSize, frames[0].cols);
            
            for (int r = row; r < endRow; ++r) {
                for (int c = col; c < endCol; ++c) {
                    std::vector<cv::Vec3b> pixels;
                    for (int i = startIdx; i < endIdx; ++i) {
                        pixels.push_back(frames[i].at<cv::Vec3b>(r, c));
                    }
                    
                    // 對每個通道計算中位數
                    std::vector<uchar> b_values, g_values, r_values;
                    for (const auto& pixel : pixels) {
                        b_values.push_back(pixel[0]);
                        g_values.push_back(pixel[1]);
                        r_values.push_back(pixel[2]);
                    }
                    
                    std::nth_element(b_values.begin(), b_values.begin() + b_values.size()/2, b_values.end());
                    std::nth_element(g_values.begin(), g_values.begin() + g_values.size()/2, g_values.end());
                    std::nth_element(r_values.begin(), r_values.begin() + r_values.size()/2, r_values.end());
                    
                    background.at<cv::Vec3b>(r, c) = cv::Vec3b(
                        b_values[b_values.size()/2],
                        g_values[g_values.size()/2], 
                        r_values[r_values.size()/2]
                    );
                }
            }
        }
    }
    
    return background;
}

// 提取前景（改進的差分算法）
cv::Mat VideoComposer::extractForeground(const cv::Mat& frame, const cv::Mat& background)
{
    if (frame.empty() || background.empty()) {
        return cv::Mat();
    }
    
    cv::Mat diff, grayDiff, mask;
    cv::absdiff(frame, background, diff);
    cv::cvtColor(diff, grayDiff, cv::COLOR_BGR2GRAY);
    
    // 使用自適應或固定閾值
    double thresholdValue = m_threshold;
    if (m_adaptiveThreshold) {
        thresholdValue = computeAdaptiveThreshold(grayDiff);
    }
    
    cv::threshold(grayDiff, mask, thresholdValue, 255, cv::THRESH_BINARY);
    
    // 形態學運算去除雜訊
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);  // 開運算去除雜訊
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel); // 閉運算填補空洞
    
    return mask;
}

// 增強前景遮罩（邊緣增強和中心加權）
cv::Mat VideoComposer::enhanceForegroundMask(const cv::Mat& mask, const cv::Mat& frame)
{
    if (mask.empty()) {
        return cv::Mat();
    }
    
    cv::Mat enhancedMask = mask.clone();
    
    // 邊緣增強
    if (m_edgeEnhancement && !frame.empty()) {
        cv::Mat gray, edges;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, 50, 150);
        
        // 膨脹邊緣以增強效果
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
        cv::dilate(edges, edges, kernel);
        
        // 將邊緣信息融合到遮罩中
        cv::bitwise_or(enhancedMask, edges, enhancedMask);
    }
    
    // 中心加權
    if (m_centerWeighting) {
        enhancedMask = applyCenterWeighting(enhancedMask, frame.size());
    }
    
    // 高斯模糊平滑遮罩邊緣
    if (m_blurSize > 1) {
        cv::GaussianBlur(enhancedMask, enhancedMask, cv::Size(m_blurSize, m_blurSize), 0);
    }
    
    return enhancedMask;
}

// 應用中心加權
cv::Mat VideoComposer::applyCenterWeighting(const cv::Mat& mask, const cv::Size& frameSize)
{
    if (mask.empty()) {
        return cv::Mat();
    }
    
    cv::Mat weightedMask = mask.clone();
    cv::Point2f center(frameSize.width / 2.0f, frameSize.height / 2.0f);
    double maxDistance = std::sqrt(center.x * center.x + center.y * center.y);
    
    for (int row = 0; row < weightedMask.rows; ++row) {
        for (int col = 0; col < weightedMask.cols; ++col) {
            double distance = std::sqrt(std::pow(col - center.x, 2) + std::pow(row - center.y, 2));
            double weight = 1.0 - (distance / maxDistance) * 0.5; // 邊緣權重減至50%
            
            uchar originalValue = weightedMask.at<uchar>(row, col);
            if (originalValue > 0) {
                weightedMask.at<uchar>(row, col) = static_cast<uchar>(originalValue * weight);
            }
        }
    }
    
    return weightedMask;
}

// 計算自適應閾值
double VideoComposer::computeAdaptiveThreshold(const cv::Mat& diffImage)
{
    if (diffImage.empty()) {
        return m_threshold;
    }
    
    cv::Scalar meanVal, stdDevVal;
    cv::meanStdDev(diffImage, meanVal, stdDevVal);
    
    // 使用平均值加上標準差的倍數作為自適應閾值
    double adaptiveThresh = meanVal[0] + 2.0 * stdDevVal[0];
    
    // 限制閾值範圍
    adaptiveThresh = std::max(10.0, std::min(100.0, adaptiveThresh));
    
    return adaptiveThresh;
}

// 輔助函數：使用 GStreamer 硬體加速打開影片（Raspberry Pi 5 優化）
static bool openVideoWithHWAccel(cv::VideoCapture& cap, const QString& path)
{
    // 判斷檔案類型
    bool isRawH264 = path.endsWith(".h264");
    
    if (isRawH264) {
        // 對於原始 h264 檔案，使用特殊的 pipeline
        QString gstPipeline = QString(
            "filesrc location=\"%1\" ! "
            "h264parse ! "  // 直接解析 h264 流
            "v4l2h264dec ! "  // 硬體解碼器
            "videoconvert ! "
            "video/x-raw,format=BGR ! "
            "appsink"
        ).arg(path);
        
        qDebug() << "   嘗試使用 GStreamer 硬體加速 (raw h264):" << path;
        if (cap.open(gstPipeline.toStdString(), cv::CAP_GSTREAMER)) {
            qDebug() << "   ✅ 使用 GStreamer 硬體加速成功";
            return true;
        }
        
        // 備用：軟體解碼
        gstPipeline = QString(
            "filesrc location=\"%1\" ! "
            "h264parse ! "
            "avdec_h264 ! "  // 軟體解碼器
            "videoconvert ! "
            "video/x-raw,format=BGR ! "
            "appsink"
        ).arg(path);
        
        qDebug() << "   硬體加速失敗，嘗試 GStreamer 軟體解碼";
        if (cap.open(gstPipeline.toStdString(), cv::CAP_GSTREAMER)) {
            qDebug() << "   ✅ 使用 GStreamer 軟體解碼";
            return true;
        }
    } else {
        // 對於 MP4 等容器格式
        QString gstPipeline = QString(
            "filesrc location=\"%1\" ! "
            "qtdemux ! h264parse ! "
            "v4l2h264dec ! "  // 硬體解碼器
            "videoconvert ! "
            "video/x-raw,format=BGR ! "
            "appsink"
        ).arg(path);
        
        qDebug() << "   嘗試使用 GStreamer 硬體加速:" << path;
        if (cap.open(gstPipeline.toStdString(), cv::CAP_GSTREAMER)) {
            qDebug() << "   ✅ 使用 GStreamer 硬體加速成功";
            return true;
        }
        
        // 嘗試通用解碼
        gstPipeline = QString(
            "filesrc location=\"%1\" ! "
            "decodebin ! "
            "videoconvert ! "
            "video/x-raw,format=BGR ! "
            "appsink"
        ).arg(path);
        
        qDebug() << "   硬體加速失敗，嘗試 GStreamer 軟體解碼";
        if (cap.open(gstPipeline.toStdString(), cv::CAP_GSTREAMER)) {
            qDebug() << "   ✅ 使用 GStreamer 軟體解碼";
            return true;
        }
    }
    
    // 最後嘗試標準 OpenCV
    qDebug() << "   GStreamer 失敗，使用標準 OpenCV";
    if (cap.open(path.toStdString())) {
        qDebug() << "   ⚠️ 使用標準 OpenCV（較慢）";
        return true;
    }
    
    qWarning() << "   ❌ 所有方法都失敗";
    return false;
}

// 輔助函數：使用硬體編碼創建輸出（Raspberry Pi 5 優化）
static bool createVideoWriterWithHWAccel(cv::VideoWriter& writer, const QString& outputPath, 
                                         double fps, int width, int height)
{
    // 修正 FPS 異常值
    if (fps <= 0 || fps > 1000) {
        qWarning() << "   ⚠️ FPS 異常:" << fps << "，使用預設 30 FPS";
        fps = 30.0;
    }
    
    qDebug() << "   目標 FPS:" << fps;
    
    // MP4 容器使用標準編碼器
    qDebug() << "   嘗試編碼器: H.264 (mp4v)";
    if (writer.open(outputPath.toStdString(), 
                   cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                   fps, cv::Size(width, height))) {
        qDebug() << "   ✅ 使用 mp4v 編碼器";
        return true;
    }
    
    qDebug() << "   ❌ mp4v 編碼器失敗，嘗試 X264";
    if (writer.open(outputPath.toStdString(), 
                   cv::VideoWriter::fourcc('X', '2', '6', '4'),
                   fps, cv::Size(width, height))) {
        qDebug() << "   ✅ 使用 X264 編碼器";
        return true;
    }
    
    qDebug() << "   ❌ X264 編碼器失敗，嘗試 H264";
    if (writer.open(outputPath.toStdString(), 
                   cv::VideoWriter::fourcc('H', '2', '6', '4'),
                   fps, cv::Size(width, height))) {
        qDebug() << "   ✅ 使用 H264 編碼器";
        return true;
    }
    
    qDebug() << "   ❌ H264 編碼器失敗，嘗試 MJPEG";
    if (writer.open(outputPath.toStdString(), 
                   cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                   fps, cv::Size(width, height))) {
        qDebug() << "   ✅ 使用 MJPEG 編碼器";
        return true;
    }
    
    qWarning() << "   ❌ 所有編碼器都失敗";
    return false;
}

// 並排合成多個影片
bool VideoComposer::composeSideBySide(const QStringList& videoPaths, const QString& outputPath)
{
    if (videoPaths.size() < 2 || videoPaths.size() > 4) {
        qWarning() << "❌ composeSideBySide: 需要 2-4 個影片，當前提供:" << videoPaths.size();
        return false;
    }
    
    qDebug() << "🎬 開始並排合成" << videoPaths.size() << "個影片";
    qDebug() << "   輸出路徑:" << outputPath;
    
    // 打開所有影片
    std::vector<cv::VideoCapture> captures;
    std::vector<int> frameCounts;
    int minFrames = INT_MAX;
    double fps = 30.0;
    int width = 0, height = 0;
    
    for (const QString& path : videoPaths) {
        cv::VideoCapture cap;
        if (!openVideoWithHWAccel(cap, path)) {
            qWarning() << "❌ 無法打開影片:" << path;
            return false;
        }
        
        int frameCount = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
        double videoFps = cap.get(cv::CAP_PROP_FPS);
        int videoWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
        int videoHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        
        qDebug() << "   ✅" << path << "- 解析度:" << videoWidth << "x" << videoHeight 
                 << "幀數:" << frameCount << "FPS:" << videoFps;
        
        if (width == 0) {
            width = videoWidth;
            height = videoHeight;
            fps = videoFps;
        }
        
        captures.push_back(std::move(cap));
        frameCounts.push_back(frameCount);
        minFrames = std::min(minFrames, frameCount);
    }
    
    // 計算佈局：2個=橫排(1x2)，3個=橫排(1x3)，4個=網格(2x2)
    int rows = (videoPaths.size() <= 2) ? 1 : 2;
    int cols = (videoPaths.size() == 2) ? 2 : (videoPaths.size() == 3 ? 3 : 2);
    int cellWidth = width;
    int cellHeight = height;
    int outputWidth = cellWidth * cols;
    int outputHeight = cellHeight * rows;
    
    qDebug() << "   佈局:" << rows << "x" << cols << "輸出解析度:" << outputWidth << "x" << outputHeight;
    
    // 創建輸出影片（使用硬體加速）
    cv::VideoWriter writer;
    if (!createVideoWriterWithHWAccel(writer, outputPath, fps, outputWidth, outputHeight)) {
        qWarning() << "❌ 無法創建輸出影片:" << outputPath;
        return false;
    }
    
    // 逐幀合成
    for (int frameIdx = 0; frameIdx < minFrames; ++frameIdx) {
        cv::Mat outputFrame(outputHeight, outputWidth, CV_8UC3);
        
        // 讀取所有影片的當前幀並放置到輸出畫面
        for (size_t i = 0; i < captures.size(); ++i) {
            cv::Mat frame;
            if (!captures[i].read(frame) || frame.empty()) {
                qWarning() << "❌ 讀取第" << i << "個影片的幀" << frameIdx << "失敗";
                return false;
            }
            
            // 調整幀大小
            cv::Mat resizedFrame;
            cv::resize(frame, resizedFrame, cv::Size(cellWidth, cellHeight));
            
            // 計算在輸出畫面中的位置
            int row = i / cols;
            int col = i % cols;
            int x = col * cellWidth;
            int y = row * cellHeight;
            
            // 複製到輸出畫面
            cv::Rect roi(x, y, cellWidth, cellHeight);
            resizedFrame.copyTo(outputFrame(roi));
        }
        
        writer.write(outputFrame);
        
        // 每30幀輸出一次進度
        if (frameIdx % 30 == 0 || frameIdx == minFrames - 1) {
            int percent = (frameIdx * 100) / minFrames;
            qDebug() << "   進度:" << frameIdx << "/" << minFrames << "(" << percent << "%)";
        }
    }
    
    // 釋放資源
    for (auto& cap : captures) {
        cap.release();
    }
    writer.release();
    
    // 檢查輸出文件大小
    QFileInfo fileInfo(outputPath);
    qint64 fileSize = fileInfo.size();
    qDebug() << "✅ 並排合成完成:" << outputPath;
    qDebug() << "   檔案大小:" << (fileSize / 1024) << "KB";
    
    if (fileSize < 1024) {
        qWarning() << "❌ 輸出檔案太小，可能合成失敗";
        return false;
    }
    
    return true;
}

// 對齊關鍵幀的並排合成
bool VideoComposer::composeSideBySideAligned(const QStringList& videoPaths, 
                                               const QString& outputPath, 
                                               const QString& keyframeName)
{
    if (videoPaths.size() < 2 || videoPaths.size() > 4) {
        qWarning() << "❌ composeSideBySideAligned: 需要 2-4 個影片，當前提供:" << videoPaths.size();
        return false;
    }
    
    qDebug() << "🎬 開始對齊關鍵幀並排合成" << videoPaths.size() << "個影片";
    qDebug() << "   對齊關鍵幀:" << keyframeName;
    qDebug() << "   輸出路徑:" << outputPath;
    
    // 創建 KeyframeManager 讀取關鍵幀
    KeyframeManager kfManager;
    
    // 讀取每個影片的關鍵幀時間戳記和總幀數
    struct VideoInfo {
        cv::VideoCapture capture;
        qint64 keyframeTimestamp;  // 關鍵幀時間戳（毫秒）
        int totalFrames;
        double fps;
        int width;
        int height;
        int offsetFrames;  // 需要在開頭添加的黑色幀數
    };
    
    std::vector<VideoInfo> videos;
    int maxTotalFrames = 0;
    qint64 referenceKeyframeTime = -1;  // 以第一個影片的關鍵幀時間為基準
    
    // 步驟 1: 打開所有影片並讀取資訊
    for (const QString& path : videoPaths) {
        VideoInfo info;
        if (!openVideoWithHWAccel(info.capture, path)) {
            qWarning() << "❌ 無法打開影片:" << path;
            return false;
        }
        
        info.totalFrames = static_cast<int>(info.capture.get(cv::CAP_PROP_FRAME_COUNT));
        info.fps = info.capture.get(cv::CAP_PROP_FPS);
        
        // 修正異常的 FPS 值
        if (info.fps <= 0 || info.fps > 1000) {
            qWarning() << "   ⚠️ 偵測到異常 FPS:" << info.fps << "，使用預設 30 FPS";
            info.fps = 30.0;
        }
        
        info.width = static_cast<int>(info.capture.get(cv::CAP_PROP_FRAME_WIDTH));
        info.height = static_cast<int>(info.capture.get(cv::CAP_PROP_FRAME_HEIGHT));
        info.keyframeTimestamp = kfManager.getKeyframeTimestamp(path, keyframeName);
        
        if (info.keyframeTimestamp < 0) {
            qWarning() << "❌ 影片" << path << "沒有關鍵幀" << keyframeName;
            return false;
        }
        
        // 計算關鍵幀所在的幀數
        int keyframeFrame = static_cast<int>((info.keyframeTimestamp / 1000.0) * info.fps);
        
        qDebug() << "   ✅" << path;
        qDebug() << "      解析度:" << info.width << "x" << info.height 
                 << "總幀數:" << info.totalFrames << "FPS:" << info.fps;
        qDebug() << "      關鍵幀時間:" << info.keyframeTimestamp << "ms (幀" << keyframeFrame << ")";
        
        // 第一個影片作為基準
        if (referenceKeyframeTime < 0) {
            referenceKeyframeTime = info.keyframeTimestamp;
        }
        
        videos.push_back(info);
    }
    
    // 步驟 2: 計算每個影片需要的偏移量
    double referenceFps = videos[0].fps;
    int referenceKeyframeFrame = static_cast<int>((referenceKeyframeTime / 1000.0) * referenceFps);
    
    for (size_t i = 0; i < videos.size(); ++i) {
        int thisKeyframeFrame = static_cast<int>((videos[i].keyframeTimestamp / 1000.0) * videos[i].fps);
        
        // 計算需要添加的黑色幀數，使關鍵幀對齊
        videos[i].offsetFrames = referenceKeyframeFrame - thisKeyframeFrame;
        
        // 計算對齊後的總幀數
        int alignedTotalFrames = videos[i].totalFrames + videos[i].offsetFrames;
        maxTotalFrames = std::max(maxTotalFrames, alignedTotalFrames);
        
        qDebug() << "   影片" << i << "偏移:" << videos[i].offsetFrames << "幀"
                 << "對齊後總幀數:" << alignedTotalFrames;
    }
    
    qDebug() << "   最終輸出總幀數:" << maxTotalFrames;
    
    // 步驟 3: 設定輸出參數
    int rows = (videoPaths.size() <= 2) ? 1 : 2;
    int cols = (videoPaths.size() == 2) ? 2 : (videoPaths.size() == 3 ? 3 : 2);
    int cellWidth = videos[0].width;
    int cellHeight = videos[0].height;
    int outputWidth = cellWidth * cols;
    int outputHeight = cellHeight * rows;
    
    qDebug() << "   佈局:" << rows << "x" << cols << "輸出解析度:" << outputWidth << "x" << outputHeight;
    
    // 步驟 4: 創建輸出影片（使用硬體加速）
    cv::VideoWriter writer;
    if (!createVideoWriterWithHWAccel(writer, outputPath, referenceFps, outputWidth, outputHeight)) {
        qWarning() << "❌ 無法創建輸出影片:" << outputPath;
        return false;
    }
    
    // 步驟 5: 創建黑色幀模板
    cv::Mat blackCell(cellHeight, cellWidth, CV_8UC3, cv::Scalar(0, 0, 0));
    
    // 步驟 6: 逐幀合成
    for (int frameIdx = 0; frameIdx < maxTotalFrames; ++frameIdx) {
        cv::Mat outputFrame(outputHeight, outputWidth, CV_8UC3);
        
        for (size_t i = 0; i < videos.size(); ++i) {
            cv::Mat cellFrame;
            int actualFrameIdx = frameIdx - videos[i].offsetFrames;
            
            // 判斷是否需要黑色幀
            if (actualFrameIdx < 0 || actualFrameIdx >= videos[i].totalFrames) {
                // 使用黑色幀
                cellFrame = blackCell.clone();
            } else {
                // 讀取實際幀
                cv::Mat frame;
                videos[i].capture.set(cv::CAP_PROP_POS_FRAMES, actualFrameIdx);
                
                if (!videos[i].capture.read(frame) || frame.empty()) {
                    cellFrame = blackCell.clone();
                } else {
                    cv::resize(frame, cellFrame, cv::Size(cellWidth, cellHeight));
                }
            }
            
            // 計算在輸出畫面中的位置
            int row = i / cols;
            int col = i % cols;
            int x = col * cellWidth;
            int y = row * cellHeight;
            
            // 複製到輸出畫面
            cv::Rect roi(x, y, cellWidth, cellHeight);
            cellFrame.copyTo(outputFrame(roi));
        }
        
        writer.write(outputFrame);
        
        // 每30幀輸出一次進度
        if (frameIdx % 30 == 0 || frameIdx == maxTotalFrames - 1) {
            int percent = (frameIdx * 100) / maxTotalFrames;
            qDebug() << "   進度:" << frameIdx << "/" << maxTotalFrames << "(" << percent << "%)";
        }
    }
    
    // 步驟 7: 釋放資源
    for (auto& video : videos) {
        video.capture.release();
    }
    writer.release();
    
    // 檢查輸出文件大小
    QFileInfo fileInfo(outputPath);
    qint64 fileSize = fileInfo.size();
    qDebug() << "✅ 對齊關鍵幀並排合成完成:" << outputPath;
    qDebug() << "   檔案大小:" << (fileSize / 1024) << "KB";
    
    if (fileSize < 1024) {
        qWarning() << "❌ 輸出檔案太小，可能合成失敗";
        return false;
    }
    
    return true;
}

// 使用 ffmpeg 進行並排合成（支援 h264 原始流和硬體加速）
bool VideoComposer::composeSideBySideFFmpeg(const QStringList& videoPaths, const QString& outputPath) {
    if (videoPaths.size() < 2 || videoPaths.size() > 4) {
        qWarning() << "❌ 只支援 2-4 個影片的並排合成";
        return false;
    }
    
    qDebug() << "🎬 開始 ffmpeg 並排合成" << videoPaths.size() << "個影片";
    qDebug() << "   輸出檔案:" << outputPath;
    
    // 構建 ffmpeg 命令
    QStringList args;
    
    // 輸入檔案（為 h264 原始流偵測並指定幀率）
    for (const QString& path : videoPaths) {
        if (path.endsWith(".h264", Qt::CaseInsensitive)) {
            // 使用 ffprobe 計數幀數
            QProcess probe;
            probe.start("ffprobe", QStringList() 
                        << "-v" << "error"
                        << "-select_streams" << "v:0"
                        << "-count_packets"
                        << "-show_entries" << "stream=nb_read_packets"
                        << "-of" << "csv=p=0"
                        << path);
            probe.waitForFinished(5000);
            int totalFrames = probe.readAllStandardOutput().trimmed().toInt();
            
            // 預設 30 fps，根據幀數調整
            double fps = 30.0;
            if (totalFrames > 1000) {
                fps = 120.0;  // 長影片可能是高幀率
            } else if (totalFrames > 500) {
                fps = 60.0;
            }
            
            qDebug() << "   ⚙️ 偵測到" << totalFrames << "幀，使用幀率:" << fps << "fps";
            args << "-r" << QString::number(fps, 'f', 0);
        }
        args << "-i" << path;
    }
    
    // 根據影片數量決定佈局
    QString filterComplex;
    if (videoPaths.size() == 2) {
        // 1x2 水平並排
        filterComplex = "[0:v]scale=iw/2:-1[v0];[1:v]scale=iw/2:-1[v1];[v0][v1]hstack=inputs=2[v]";
    } else if (videoPaths.size() == 3) {
        // 1x3 水平並排
        filterComplex = "[0:v]scale=iw/3:-1[v0];[1:v]scale=iw/3:-1[v1];[2:v]scale=iw/3:-1[v2];[v0][v1][v2]hstack=inputs=3[v]";
    } else if (videoPaths.size() == 4) {
        // 2x2 網格
        filterComplex = "[0:v]scale=iw/2:-1[v0];[1:v]scale=iw/2:-1[v1];[2:v]scale=iw/2:-1[v2];[3:v]scale=iw/2:-1[v3];"
                       "[v0][v1]hstack=inputs=2[top];[v2][v3]hstack=inputs=2[bot];[top][bot]vstack=inputs=2[v]";
    }
    
    args << "-filter_complex" << filterComplex;
    args << "-map" << "[v]";
    
    // 輸出參數 - 使用 libx264 軟體編碼（最可靠）
    args << "-c:v" << "libx264";
    args << "-preset" << "ultrafast";  // 使用最快預設以加速
    args << "-crf" << "23";  // 品質因子（18-28 為佳）
    args << "-progress" << "pipe:1";  // 輸出進度到 stdout
    args << "-y";  // 覆蓋輸出檔案
    args << outputPath;
    
    // 執行 ffmpeg
    QProcess process;
    process.setProgram("ffmpeg");
    process.setArguments(args);
    process.setProcessChannelMode(QProcess::MergedChannels);  // 合併 stdout 和 stderr
    
    qDebug() << "   執行命令: ffmpeg" << args.join(" ");
    qDebug() << "";
    qDebug() << "📹 開始處理影片...";
    
    process.start();
    
    if (!process.waitForStarted(3000)) {
        qWarning() << "❌ 無法啟動 ffmpeg";
        return false;
    }
    
    // 即時讀取並顯示輸出
    int lastFrame = 0;
    int progressCounter = 0;
    
    while (process.state() == QProcess::Running) {
        if (process.waitForReadyRead(500)) {  // 500ms 超時
            QString output = QString::fromUtf8(process.readAll());
            
            if (output.isEmpty()) {
                continue;
            }
            
            QStringList lines = output.split('\n');
            for (const QString& line : lines) {
                QString trimmedLine = line.trimmed();
                
                if (trimmedLine.isEmpty()) {
                    continue;
                }
                
                if (trimmedLine.startsWith("frame=")) {
                    QRegularExpression frameRe("frame=(\\d+)");
                    QRegularExpressionMatch match = frameRe.match(trimmedLine);
                    if (match.hasMatch()) {
                        int currentFrame = match.captured(1).toInt();
                        if (currentFrame > lastFrame) {
                            lastFrame = currentFrame;
                            qDebug() << "   ⏳ 已處理幀數:" << currentFrame;
                        }
                    }
                } else if (trimmedLine.contains("frame=")) {
                    qDebug().noquote() << "   " << trimmedLine;
                } else if (trimmedLine.contains("error", Qt::CaseInsensitive)) {
                    qWarning() << "⚠️" << trimmedLine;
                }
            }
            
            if (++progressCounter % 10 == 0) {
                qDebug() << "   ⏳ 處理中...";
            }
        }
    }
    
    process.waitForFinished(-1);
    
    int exitCode = process.exitCode();
    
    if (exitCode != 0) {
        qWarning() << "❌ ffmpeg 合成失敗，退出碼:" << exitCode;
        // 顯示最後的錯誤訊息
        QString errorOutput = QString::fromUtf8(process.readAllStandardError());
        if (!errorOutput.isEmpty()) {
            qWarning() << "   錯誤訊息:" << errorOutput;
        }
        return false;
    }
    
    qDebug() << "";
    
    // 檢查輸出檔案
    QFileInfo fileInfo(outputPath);
    if (!fileInfo.exists() || fileInfo.size() < 1024) {
        qWarning() << "❌ 輸出檔案不存在或太小";
        return false;
    }
    
    qDebug() << "✅ ffmpeg 並排合成完成:" << outputPath;
    qDebug() << "   檔案大小:" << (fileInfo.size() / 1024) << "KB";
    
    return true;
}

// 使用 ffmpeg 對齊關鍵幀並排合成
bool VideoComposer::composeSideBySideAlignedFFmpeg(const QStringList& videoPaths, 
                                                     const QString& outputPath, 
                                                     const QString& keyframeName,
                                                     QObject* keyframeManagerObj) {
    if (videoPaths.size() < 2 || videoPaths.size() > 4) {
        qWarning() << "❌ 只支援 2-4 個影片的並排合成";
        return false;
    }
    
    if (!keyframeManagerObj) {
        qWarning() << "❌ KeyframeManager 為空";
        return false;
    }
    
    KeyframeManager* kfManager = qobject_cast<KeyframeManager*>(keyframeManagerObj);
    if (!kfManager) {
        qWarning() << "❌ 無法轉換 KeyframeManager";
        return false;
    }
    
    qDebug() << "🎬 開始 ffmpeg 對齊關鍵幀並排合成" << videoPaths.size() << "個影片";
    qDebug() << "   關鍵幀:" << keyframeName;
    qDebug() << "   輸出檔案:" << outputPath;
    
    // 步驟 1: 讀取每個影片的關鍵幀時間戳
    struct VideoInfo {
        QString path;
        qint64 keyframeTimestamp;  // 毫秒
        double duration;  // 秒
        double offsetSeconds;  // 需要延遲的秒數
    };
    
    std::vector<VideoInfo> videos;
    qint64 maxKeyframeTime = 0;
    
    for (const QString& path : videoPaths) {
        VideoInfo info;
        info.path = path;
        
        // 讀取關鍵幀時間戳（可能是從 MP4 記錄的）
        qint64 rawTimestamp = kfManager->getKeyframeTimestamp(path, keyframeName);
        
        if (rawTimestamp < 0) {
            qWarning() << "❌ 影片" << path << "沒有關鍵幀" << keyframeName;
            return false;
        }
        
        // 偵測實際影片長度和幀數
        double actualDuration = 0.0;
        int actualFrames = 0;
        
        // 使用 ffprobe 獲取準確的幀數（不依賴 duration）
        QProcess frameProbe;
        frameProbe.start("ffprobe", QStringList() 
                        << "-v" << "error"
                        << "-select_streams" << "v:0"
                        << "-count_packets"
                        << "-show_entries" << "stream=nb_read_packets"
                        << "-of" << "csv=p=0"
                        << path);
        frameProbe.waitForFinished(5000);
        actualFrames = frameProbe.readAllStandardOutput().trimmed().toInt();
        
        // 嘗試讀取 duration
        QProcess durationProbe;
        durationProbe.start("ffprobe", QStringList() 
                            << "-v" << "error"
                            << "-show_entries" << "format=duration"
                            << "-of" << "default=noprint_wrappers=1:nokey=1"
                            << path);
        durationProbe.waitForFinished();
        QString durationStr = durationProbe.readAllStandardOutput().trimmed();
        actualDuration = durationStr.toDouble();
        
        // 如果無法讀取 duration（h264 原始流），使用幀數推算
        if (actualDuration <= 0 && actualFrames > 0) {
            // 假設常見幀率
            double estimatedFps = 30.0;
            if (actualFrames > 500) estimatedFps = 60.0;
            if (actualFrames > 1000) estimatedFps = 120.0;
            
            actualDuration = actualFrames / estimatedFps;
            qDebug() << "   ⚙️ h264 原始流，推算時長:" << actualDuration << "秒 (" 
                     << actualFrames << "幀 @ " << estimatedFps << "fps)";
        }
        
        info.duration = actualDuration > 0 ? actualDuration : 10.0;
        
        // MP4→h264 時間戳轉換
        // 檢查關鍵幀 JSON 是從 MP4 還是 h264 記錄的
        // 如果當前是 h264 但關鍵幀可能來自 MP4，需要轉換比例
        bool isH264 = path.endsWith(".h264", Qt::CaseInsensitive);
        bool needsConversion = false;
        double conversionRatio = 1.0;
        
        if (isH264) {
            // 檢查是否有對應的 MP4 檔案
            QString mp4Path = path;
            mp4Path.replace(QRegularExpression("\\.h264$", QRegularExpression::CaseInsensitiveOption), ".mp4");
            
            if (QFile::exists(mp4Path)) {
                // 讀取 MP4 的時長
                QProcess mp4Probe;
                mp4Probe.start("ffprobe", QStringList() 
                              << "-v" << "error"
                              << "-show_entries" << "format=duration"
                              << "-of" << "default=noprint_wrappers=1:nokey=1"
                              << mp4Path);
                mp4Probe.waitForFinished();
                double mp4Duration = mp4Probe.readAllStandardOutput().trimmed().toDouble();
                
                if (mp4Duration > 0 && actualDuration > 0) {
                    conversionRatio = actualDuration / mp4Duration;
                    needsConversion = (std::abs(conversionRatio - 1.0) > 0.01);  // 差異超過 1%
                    
                    if (needsConversion) {
                        qDebug() << "   🔄 偵測到 MP4/h264 時間差異";
                        qDebug() << "      MP4 時長:" << mp4Duration << "秒";
                        qDebug() << "      h264 時長:" << actualDuration << "秒";
                        qDebug() << "      轉換比例:" << conversionRatio;
                    }
                }
            }
        }
        
        // 應用時間戳轉換
        if (needsConversion) {
            info.keyframeTimestamp = static_cast<qint64>(rawTimestamp * conversionRatio);
            qDebug() << "      原始關鍵幀時間:" << rawTimestamp << "ms → 轉換後:" << info.keyframeTimestamp << "ms";
        } else {
            info.keyframeTimestamp = rawTimestamp;
        }
        
        if (info.keyframeTimestamp > maxKeyframeTime) {
            maxKeyframeTime = info.keyframeTimestamp;
        }
        
        qDebug() << "   📹" << path;
        qDebug() << "      關鍵幀時間:" << info.keyframeTimestamp << "ms";
        qDebug() << "      影片時長:" << info.duration << "秒";
        qDebug() << "      總幀數:" << actualFrames << "幀";
        
        videos.push_back(info);
    }
    
    // 步驟 2: 計算每個影片的延遲時間（以最晚的關鍵幀為基準）
    for (auto& video : videos) {
        video.offsetSeconds = (maxKeyframeTime - video.keyframeTimestamp) / 1000.0;
        qDebug() << "   影片" << QFileInfo(video.path).fileName() 
                 << "需要延遲" << video.offsetSeconds << "秒";
    }
    
    // 步驟 3: 偵測並統一幀率
    struct VideoFpsInfo {
        double fps;
        int totalFrames;
    };
    std::vector<VideoFpsInfo> videoFpsInfo;
    double maxFps = 30.0;  // 找出最高幀率作為輸出幀率
    
    for (const auto& video : videos) {
        VideoFpsInfo info;
        info.fps = 30.0;  // 預設
        info.totalFrames = 0;
        
        if (video.path.endsWith(".h264", Qt::CaseInsensitive)) {
            // 使用 ffprobe 計數實際幀數
            QProcess probe;
            probe.start("ffprobe", QStringList() 
                        << "-v" << "error"
                        << "-select_streams" << "v:0"
                        << "-count_packets"
                        << "-show_entries" << "stream=nb_read_packets"
                        << "-of" << "csv=p=0"
                        << video.path);
            probe.waitForFinished(5000);
            info.totalFrames = probe.readAllStandardOutput().trimmed().toInt();
            
            // 根據總幀數和時長計算實際幀率
            if (info.totalFrames > 10 && video.duration > 0.1) {
                info.fps = info.totalFrames / video.duration;
                
                // 四捨五入到常見幀率
                if (info.fps > 100) {
                    info.fps = 120.0;
                } else if (info.fps > 50) {
                    info.fps = 60.0;
                } else if (info.fps > 25) {
                    info.fps = 30.0;
                } else {
                    info.fps = 24.0;
                }
            }
            
            qDebug() << "   ⚙️ 偵測到" << info.totalFrames << "幀，幀率:" << info.fps << "fps";
        }
        
        if (info.fps > maxFps) {
            maxFps = info.fps;
        }
        
        videoFpsInfo.push_back(info);
    }
    
    qDebug() << "   📊 統一使用輸出幀率:" << maxFps << "fps";
    
    // 步驟 4: 構建 ffmpeg 命令
    QStringList args;
    
    // 輸入檔案（指定各自的輸入幀率）
    for (size_t i = 0; i < videos.size(); ++i) {
        if (videos[i].path.endsWith(".h264", Qt::CaseInsensitive)) {
            args << "-r" << QString::number(videoFpsInfo[i].fps, 'f', 0);
        }
        args << "-i" << videos[i].path;
    }
    
    // 構建 filter_complex
    // 計算每個影片對齊後的總長度（開頭延遲 + 原始長度）
    int maxTotalFrames = 0;
    std::vector<int> videoStartFrames;
    std::vector<int> videoOriginalFrames;
    
    for (size_t i = 0; i < videos.size(); ++i) {
        int startFrames = static_cast<int>(videos[i].offsetSeconds * maxFps);
        int originalFrames = videoFpsInfo[i].totalFrames;
        
        // 如果輸入幀率與輸出幀率不同，需要調整幀數
        if (videoFpsInfo[i].fps > 0 && videoFpsInfo[i].fps != maxFps) {
            originalFrames = static_cast<int>(originalFrames * maxFps / videoFpsInfo[i].fps);
        }
        
        int totalFrames = startFrames + originalFrames;
        
        videoStartFrames.push_back(startFrames);
        videoOriginalFrames.push_back(originalFrames);
        
        if (totalFrames > maxTotalFrames) {
            maxTotalFrames = totalFrames;
        }
        
        qDebug() << "   📊 影片" << i << "- 開頭延遲:" << startFrames << "幀, 原始:" 
                 << originalFrames << "幀, 總長:" << totalFrames << "幀";
    }
    
    qDebug() << "   📏 最終統一長度:" << maxTotalFrames << "幀 (" 
             << (maxTotalFrames / maxFps) << "秒)";
    
    // 為每個影片添加延遲和填充
    QString filterComplex;
    for (size_t i = 0; i < videos.size(); ++i) {
        if (i > 0) filterComplex += ";";
        
        int startFrames = videoStartFrames[i];
        int endPaddingFrames = maxTotalFrames - startFrames - videoOriginalFrames[i];
        
        qDebug() << "   🎬 影片" << i << "- 前補" << startFrames << "幀, 後補" 
                 << endPaddingFrames << "幀";
        
        // 構建 filter: fps -> tpad(開頭) -> tpad(結尾) -> scale
        QString filter = QString("[%1:v]fps=%2").arg(i).arg(maxFps, 0, 'f', 0);
        
        // 添加開頭黑幀（使用黑色填充）
        if (startFrames > 0) {
            filter += QString(",tpad=start=%1:color=black").arg(startFrames);
        }
        
        // 添加結尾黑幀（使用黑色填充）
        if (endPaddingFrames > 0) {
            filter += QString(",tpad=stop=%1:color=black").arg(endPaddingFrames);
        }
        
        // 縮放
        filter += QString(",scale=iw/%1:-1[v%2]")
                  .arg(videos.size() <= 2 ? 2 : 2)
                  .arg(i);
        
        filterComplex += filter;
    }
    
    // 根據數量決定佈局
    if (videos.size() == 2) {
        filterComplex += ";[v0][v1]hstack=inputs=2[v]";
    } else if (videos.size() == 3) {
        filterComplex += ";[v0][v1][v2]hstack=inputs=3[v]";
    } else if (videos.size() == 4) {
        filterComplex += ";[v0][v1]hstack=inputs=2[top];[v2][v3]hstack=inputs=2[bot];[top][bot]vstack=inputs=2[v]";
    }
    
    args << "-filter_complex" << filterComplex;
    args << "-map" << "[v]";
    
    // 輸出參數 - 使用 libx264 軟體編碼
    args << "-c:v" << "libx264";
    args << "-preset" << "ultrafast";  // 使用最快預設以加速
    args << "-crf" << "23";  // 品質因子
    args << "-progress" << "pipe:1";  // 輸出進度到 stdout
    args << "-y";
    args << outputPath;
    
    // 執行 ffmpeg
    QProcess process;
    process.setProgram("ffmpeg");
    process.setArguments(args);
    process.setProcessChannelMode(QProcess::MergedChannels);  // 合併輸出
    
    qDebug() << "   執行命令: ffmpeg" << args.join(" ");
    qDebug() << "";
    qDebug() << "📹 開始處理影片（對齊關鍵幀）...";
    
    process.start();
    
    if (!process.waitForStarted(3000)) {
        qWarning() << "❌ 無法啟動 ffmpeg";
        return false;
    }
    
    // 即時讀取並顯示輸出
    int lastFrame = 0;
    int progressCounter = 0;
    
    while (process.state() == QProcess::Running) {
        if (process.waitForReadyRead(500)) {  // 增加超時時間到 500ms
            QString output = QString::fromUtf8(process.readAll());
            
            if (output.isEmpty()) {
                continue;
            }
            
            // 解析並顯示進度
            QStringList lines = output.split('\n');
            for (const QString& line : lines) {
                QString trimmedLine = line.trimmed();
                
                if (trimmedLine.isEmpty()) {
                    continue;
                }
                
                // ffmpeg 進度格式：frame=XXX 或 out_time_ms=XXX
                if (trimmedLine.startsWith("frame=")) {
                    QRegularExpression frameRe("frame=(\\d+)");
                    QRegularExpressionMatch match = frameRe.match(trimmedLine);
                    if (match.hasMatch()) {
                        int currentFrame = match.captured(1).toInt();
                        if (currentFrame > lastFrame) {
                            lastFrame = currentFrame;
                            qDebug() << "   ⏳ 已處理幀數:" << currentFrame;
                        }
                    }
                } else if (trimmedLine.contains("frame=")) {
                    // 舊格式的進度行
                    qDebug().noquote() << "   " << trimmedLine;
                } else if (trimmedLine.contains("error", Qt::CaseInsensitive)) {
                    qWarning() << "⚠️" << trimmedLine;
                }
            }
            
            // 顯示處理中的動畫
            if (++progressCounter % 10 == 0) {
                qDebug() << "   ⏳ 處理中...";
            }
        }
    }
    
    process.waitForFinished(-1);
    
    int exitCode = process.exitCode();
    
    if (exitCode != 0) {
        qWarning() << "❌ ffmpeg 合成失敗，退出碼:" << exitCode;
        QString errorOutput = QString::fromUtf8(process.readAllStandardError());
        if (!errorOutput.isEmpty()) {
            qWarning() << "   錯誤訊息:" << errorOutput;
        }
        return false;
    }
    
    qDebug() << "";
    
    // 檢查輸出檔案
    QFileInfo fileInfo(outputPath);
    if (!fileInfo.exists() || fileInfo.size() < 1024) {
        qWarning() << "❌ 輸出檔案不存在或太小";
        return false;
    }
    
    qDebug() << "✅ ffmpeg 對齊關鍵幀並排合成完成:" << outputPath;
    qDebug() << "   檔案大小:" << (fileInfo.size() / 1024) << "KB";
    
    return true;
}