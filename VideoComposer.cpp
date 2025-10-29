#include "VideoComposer.h"
#include <QDebug>
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
        video.capture.open(path.toStdString());
        if (!video.capture.isOpened()) {
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

    if (m_videos.empty()) {
        emit exportError("No original video info available");
        return false;
    }

    double fps = m_videos[0].fps;
    cv::Size frameSize = m_composedFrames[0].size();

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