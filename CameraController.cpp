#include "CameraController.h"
#include <QMediaDevices>
#include <QCameraDevice>
#include <QDebug>
#include <QProcess>
#include <QTimer>

CameraController::CameraController(QObject *parent)
    : QObject(parent), m_camera(nullptr), m_captureSession(nullptr), m_videoSink(nullptr), m_videoOutput(nullptr), m_previewProcess(nullptr), m_parentItem(nullptr), m_isActive(false), m_isPreviewActive(false), m_exposureValue(4000) // 默認曝光值 4ms (適合120fps)
      ,
      m_frameRate(120) // 默認幀率 120fps
{
    // 簡化：直接設定有攝影機可用
    m_availableCameras << "OV9281 Camera";
    m_currentCamera = "OV9281 Camera";

    // 使用定時器延遲啟動預覽，確保所有初始化完成
    QTimer::singleShot(500, this, [this]()
                       { startPreview(); });
}

CameraController::~CameraController()
{
    stopPreview();
    if (m_previewProcess)
    {
        m_previewProcess->kill();
        m_previewProcess->deleteLater();
    }
}

void CameraController::startPreview()
{
    if (m_isPreviewActive)
    {
        qDebug() << "Preview already active";
        return;
    }

    // 清理舊的 process（如果存在但未運行）
    if (m_previewProcess)
    {
        if (m_previewProcess->state() == QProcess::NotRunning)
        {
            qDebug() << "Cleaning up old preview process";
            m_previewProcess->deleteLater();
            m_previewProcess = nullptr;
        }
        else
        {
            qDebug() << "Preview process already exists and running";
            return;
        }
    }

    // 在樹莓派上，使用 rpicam-vid 來預覽
    m_previewProcess = new QProcess(this);

        QString program = "rpicam-vid";
        QStringList args;
        args << "--width" << "640"                            // 使用與測試相同的解析度
             << "--height" << "400"                           // OV9281 原生解析度
             << "--framerate" << QString::number(m_frameRate) // 使用動態幀率
             << "--denoise" << "off"
             << "--mode" << "640:400:10:P"                      // OV9281 特定模式
             << "--shutter" << QString::number(m_exposureValue) // 曝光時間 (微秒)
             << "-t" << "0"                                     // 無限時間，讓預覽持續顯示
             << "--preview" << "20,200,640,380";                // 調整對齊位置: x=20, y=200, 高度調整為380

        connect(m_previewProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int exitCode, QProcess::ExitStatus exitStatus)
                {
            Q_UNUSED(exitCode)
            Q_UNUSED(exitStatus)
            qDebug() << "Preview process finished, cleaning up...";
            if (m_previewProcess) {
                m_previewProcess->deleteLater();
                m_previewProcess = nullptr;
            }
            m_isPreviewActive = false;
            emit isPreviewActiveChanged();
            emit previewStopped(); });

        // 設定環境變數確保顯示正常
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        m_previewProcess->setProcessEnvironment(env);

        qDebug() << "Starting rpicam-vid preview:" << program << args;
        m_previewProcess->start(program, args);

        if (m_previewProcess->waitForStarted())
        {
            m_isPreviewActive = true;
            emit isPreviewActiveChanged();
            emit previewStarted();
            qDebug() << "rpicam-vid preview started";
        }
        else
        {
            emit cameraError("Failed to start rpicam-vid preview");
            m_previewProcess->deleteLater();
            m_previewProcess = nullptr;
        }
}

void CameraController::stopPreview()
{
    if (!m_isPreviewActive || !m_previewProcess)
    {
        return;
    }

    qDebug() << "Stopping rpicam-vid preview...";
    m_previewProcess->terminate();
    if (!m_previewProcess->waitForFinished(3000))
    {
        qWarning() << "Forcing kill preview process...";
        m_previewProcess->kill();
    }

    m_previewProcess->deleteLater();
    m_previewProcess = nullptr;
    m_isPreviewActive = false;
    emit isPreviewActiveChanged();
    emit previewStopped();
    qDebug() << "rpicam-vid preview stopped";
}

void CameraController::setCurrentCamera(const QString &cameraId)
{
    if (m_currentCamera == cameraId)
    {
        return;
    }

    // 停止當前預覽
    if (m_isPreviewActive)
    {
        stopPreview();
    }

    m_currentCamera = cameraId;
    setupCamera();
    emit currentCameraChanged();
}

void CameraController::setVideoOutput(QObject *output)
{
    if (m_videoOutput == output)
    {
        return;
    }

    m_videoOutput = output;

    // 將 video sink 連接到 video output
    if (m_videoOutput)
    {
        QQuickItem *videoItem = qobject_cast<QQuickItem *>(m_videoOutput);
        if (videoItem)
        {
            videoItem->setProperty("videoSink", QVariant::fromValue(m_videoSink));
        }
    }

    emit videoOutputChanged();
}

void CameraController::refreshCameras()
{
    updateAvailableCameras();
}

void CameraController::onCameraError(QCamera::Error error)
{
    QString errorString;
    switch (error)
    {
    case QCamera::NoError:
        return;
    case QCamera::CameraError:
        errorString = "Camera error occurred";
        break;
    default:
        errorString = "Unknown camera error";
        break;
    }

    emit cameraError(errorString);
    qDebug() << "Camera error:" << errorString;
}

void CameraController::onCameraActiveChanged(bool active)
{
    if (m_isActive != active)
    {
        m_isActive = active;
        emit isActiveChanged();
    }

    qDebug() << "Camera active changed:" << active;
}

void CameraController::setupCamera()
{
    // 簡化：不使用 Qt 攝影機 API
    qDebug() << "Camera setup completed:" << m_currentCamera;
}

void CameraController::setParentItem(QQuickItem *item)
{
    if (m_parentItem != item)
    {
        m_parentItem = item;
        emit parentItemChanged();
        qDebug() << "Parent item set:" << item;
    }
}

void CameraController::updateAvailableCameras()
{
    // 簡化：不使用 Qt 攝影機 API，直接假設有攝影機
    m_availableCameras.clear();
    m_availableCameras << "OV9281 Camera";
    emit availableCamerasChanged();
    qDebug() << "Available cameras:" << m_availableCameras;
}

void CameraController::setExposureValue(int value)
{
    // 根據當前幀率動態調整曝光值範圍: 50μs 到 1/frameRate 秒
    int maxExposure = 1000000 / m_frameRate; // 1/fps 秒轉換為微秒
    int clampedValue = qBound(50, value, maxExposure);

    if (m_exposureValue != clampedValue)
    {
        m_exposureValue = clampedValue;
        emit exposureValueChanged();

        // 如果預覽正在進行，重新啟動以應用新的曝光設定
        if (m_isPreviewActive)
        {
            qDebug() << "Restarting preview with new exposure for 120fps:" << m_exposureValue;
            stopPreview();
            // 使用定時器稍等一下再重啟
            QTimer::singleShot(200, this, &CameraController::startPreview);
        }

        qDebug() << "Exposure value set to:" << m_exposureValue << "μs (" << m_frameRate << "fps optimized)";
    }
}

void CameraController::setFrameRate(int fps)
{
    // 限制幀率在支援的範圍內
    QList<int> supportedFps = {30, 60, 90, 120};
    if (!supportedFps.contains(fps))
    {
        qWarning() << "Unsupported frame rate:" << fps << ", using default 120";
        fps = 120;
    }

    if (m_frameRate != fps)
    {
        m_frameRate = fps;
        emit frameRateChanged();

        // 根據幀率調整曝光範圍
        int maxExposure = 1000000 / fps; // 1/fps 秒轉換為微秒
        if (m_exposureValue > maxExposure)
        {
            setExposureValue(maxExposure);
        }

        // 如果預覽正在進行，重新啟動以套用新的幀率
        if (m_isPreviewActive)
        {
            qDebug() << "Restarting preview with new frame rate:" << m_frameRate << "fps";
            stopPreview();
            QTimer::singleShot(200, this, &CameraController::startPreview);
        }

        qDebug() << "Frame rate set to:" << m_frameRate << "fps";
    }
}