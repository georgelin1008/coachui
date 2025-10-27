#ifndef VIDEOCOMPOSER_H
#define VIDEOCOMPOSER_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QQmlEngine>
#include <QFileInfo>
#include <opencv2/opencv.hpp>

class VideoComposer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    // 基本屬性
    Q_PROPERTY(int videoCount READ videoCount NOTIFY videoCountChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QStringList loadedVideos READ loadedVideos NOTIFY loadedVideosChanged)
    Q_PROPERTY(QVariantList videoList READ videoList NOTIFY videoListChanged)
    Q_PROPERTY(int selectedVideoCount READ selectedVideoCount NOTIFY selectedVideoCountChanged)
    Q_PROPERTY(double foregroundWeight READ foregroundWeight WRITE setForegroundWeight NOTIFY foregroundWeightChanged)
    Q_PROPERTY(double backgroundWeight READ backgroundWeight WRITE setBackgroundWeight NOTIFY backgroundWeightChanged)
    Q_PROPERTY(int blurSize READ blurSize WRITE setBlurSize NOTIFY blurSizeChanged)
    Q_PROPERTY(bool hasComposedFrames READ hasComposedFrames NOTIFY hasComposedFramesChanged)
    
    // 新增的算法參數
    Q_PROPERTY(int backgroundWindowSize READ backgroundWindowSize WRITE setBackgroundWindowSize NOTIFY backgroundWindowSizeChanged)
    Q_PROPERTY(bool edgeEnhancement READ edgeEnhancement WRITE setEdgeEnhancement NOTIFY edgeEnhancementChanged)
    Q_PROPERTY(bool centerWeighting READ centerWeighting WRITE setCenterWeighting NOTIFY centerWeightingChanged)
    Q_PROPERTY(double threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(bool adaptiveThreshold READ adaptiveThreshold WRITE setAdaptiveThreshold NOTIFY adaptiveThresholdChanged)

public:
    explicit VideoComposer(QObject *parent = nullptr);
    ~VideoComposer();

    // 屬性getter
    int videoCount() const { return m_videoCount; }
    QString status() const { return m_status; }
    QStringList loadedVideos() const { return m_loadedVideos; }
    QVariantList videoList() const { return m_videoList; }
    int selectedVideoCount() const { return m_selectedVideoCount; }
    double foregroundWeight() const { return m_fgWeight; }
    double backgroundWeight() const { return m_bgWeight; }
    int blurSize() const { return m_blurSize; }
    bool hasComposedFrames() const { return !m_composedFrames.empty(); }
    
    // 新增算法參數的getter
    int backgroundWindowSize() const { return m_backgroundWindowSize; }
    bool edgeEnhancement() const { return m_edgeEnhancement; }
    bool centerWeighting() const { return m_centerWeighting; }
    double threshold() const { return m_threshold; }
    bool adaptiveThreshold() const { return m_adaptiveThreshold; }

    // 基本方法
    Q_INVOKABLE void testFunction();
    Q_INVOKABLE QStringList getTestData();
    
    // 影片載入功能
    Q_INVOKABLE void loadVideos(const QStringList &filePaths);
    Q_INVOKABLE void addVideo(const QString &filePath);
    Q_INVOKABLE void removeVideo(int index);
    Q_INVOKABLE void clearVideos();
    Q_INVOKABLE QString getVideoInfo(int index);
    Q_INVOKABLE void toggleVideoSelection(int index);
    Q_INVOKABLE void loadVideosFromDialog();
    Q_INVOKABLE QStringList getSelectedVideoPaths() const;
    
    // 影片合成算法
    Q_INVOKABLE bool composeVideos(const QStringList& selectedPaths);
    Q_INVOKABLE bool composeSelectedVideos();
    Q_INVOKABLE void setCompositionParams(double fgWeight, double bgWeight, int blurSize);
    Q_INVOKABLE QStringList getComposedFramePaths();
    Q_INVOKABLE bool exportComposedVideo(const QString& outputPath);
    
public slots:
    // 參數設定函數
    void setForegroundWeight(double weight);
    void setBackgroundWeight(double weight);
    void setBlurSize(int size);
    
    // 新增算法參數設定函數
    void setBackgroundWindowSize(int size);
    void setEdgeEnhancement(bool enabled);
    void setCenterWeighting(bool enabled);
    void setThreshold(double threshold);
    void setAdaptiveThreshold(bool enabled);

signals:
    void videoCountChanged();
    void statusChanged();
    void loadedVideosChanged();
    void videoListChanged();
    void selectedVideoCountChanged();
    void foregroundWeightChanged();
    void backgroundWeightChanged();
    void blurSizeChanged();
    void hasComposedFramesChanged();
    
    // 新增算法參數信號
    void backgroundWindowSizeChanged();
    void edgeEnhancementChanged();
    void centerWeightingChanged();
    void thresholdChanged();
    void adaptiveThresholdChanged();
    void testCompleted(const QString &message);
    void videoLoaded(const QString &filename);
    void videoLoadError(const QString &error);
    void compositionStarted();
    void compositionProgress(int current, int total);
    void compositionCompleted(int frameCount);
    void compositionError(const QString &error);
    void exportStarted();
    void exportProgress(int current, int total);
    void exportCompleted(const QString &filePath);
    void exportError(const QString &error);

private:
    struct VideoInfo {
        QString filePath;
        QString name;
        int frameCount = 0;
        double fps = 0.0;
        int width = 0;
        int height = 0;
        bool selected = false;
        cv::VideoCapture capture;
    };
    
    int m_videoCount = 0;
    QString m_status = "準備就緒";
    QStringList m_loadedVideos;
    QVariantList m_videoList;
    int m_selectedVideoCount = 0;
    std::vector<VideoInfo> m_videos;
    
    // 合成參數
    double m_fgWeight = 1.0;
    double m_bgWeight = 1.0;
    int m_blurSize = 1;
    std::vector<cv::Mat> m_composedFrames;
    
    // 新增算法參數
    int m_backgroundWindowSize = 15;  // 背景計算滑動窗口大小
    bool m_edgeEnhancement = true;    // 邊緣增強
    bool m_centerWeighting = false;   // 中心加權
    double m_threshold = 30.0;        // 前景分離閾值
    bool m_adaptiveThreshold = true;  // 自適應閾值
    
    void setStatus(const QString &status);
    
    // 內部算法函數
    cv::Mat computeBackground(const std::vector<cv::Mat>& frames, int currentIndex, int windowSize = 15);
    cv::Mat enhanceMask(const cv::Mat& mask, const cv::Mat& grayFrame);
    std::vector<cv::Mat> loadVideoFrames(const QString& filePath);
    
    // 新增的算法函數
    cv::Mat computeDynamicBackground(const std::vector<cv::Mat>& frames, int currentIndex);
    cv::Mat extractForeground(const cv::Mat& frame, const cv::Mat& background);
    cv::Mat enhanceForegroundMask(const cv::Mat& mask, const cv::Mat& frame);
    cv::Mat applyCenterWeighting(const cv::Mat& mask, const cv::Size& frameSize);
    double computeAdaptiveThreshold(const cv::Mat& diffImage);
};

#endif // VIDEOCOMPOSER_H