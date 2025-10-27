#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QCamera>
#include <QVideoSink>
#include <QMediaCaptureSession>
#include <QVideoFrame>
#include <QQuickItem>
#include <QProcess>

class CameraController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(QStringList availableCameras READ availableCameras NOTIFY availableCamerasChanged)
    Q_PROPERTY(QString currentCamera READ currentCamera WRITE setCurrentCamera NOTIFY currentCameraChanged)
    Q_PROPERTY(QObject* videoOutput READ videoOutput WRITE setVideoOutput NOTIFY videoOutputChanged)
    Q_PROPERTY(bool isPreviewActive READ isPreviewActive NOTIFY isPreviewActiveChanged)
    Q_PROPERTY(QQuickItem* parentItem READ parentItem WRITE setParentItem NOTIFY parentItemChanged)
    Q_PROPERTY(int exposureValue READ exposureValue WRITE setExposureValue NOTIFY exposureValueChanged)
    Q_PROPERTY(int frameRate READ frameRate WRITE setFrameRate NOTIFY frameRateChanged)

public:
    explicit CameraController(QObject *parent = nullptr);
    ~CameraController();

    bool isActive() const { return m_isActive; }
    QStringList availableCameras() const { return m_availableCameras; }
    QString currentCamera() const { return m_currentCamera; }
    QObject* videoOutput() const { return m_videoOutput; }
    bool isPreviewActive() const { return m_isPreviewActive; }
    QQuickItem* parentItem() const { return m_parentItem; }
    int exposureValue() const { return m_exposureValue; }
    int frameRate() const { return m_frameRate; }

public slots:
    void startPreview();
    void stopPreview();
    void setCurrentCamera(const QString &cameraId);
    void setVideoOutput(QObject* output);
    void setParentItem(QQuickItem* item);
    void refreshCameras();
    void setExposureValue(int value);
    void setFrameRate(int fps);

signals:
    void isActiveChanged();
    void availableCamerasChanged();
    void currentCameraChanged();
    void videoOutputChanged();
    void isPreviewActiveChanged();
    void parentItemChanged();
    void exposureValueChanged();
    void frameRateChanged();
    void cameraError(const QString &error);
    void previewStarted();
    void previewStopped();

private slots:
    void onCameraError(QCamera::Error error);
    void onCameraActiveChanged(bool active);

private:
    QCamera *m_camera;
    QMediaCaptureSession *m_captureSession;
    QVideoSink *m_videoSink;
    QObject *m_videoOutput;
    QProcess *m_previewProcess;
    QQuickItem *m_parentItem;
    
    bool m_isActive;
    bool m_isPreviewActive;
    QStringList m_availableCameras;
    QString m_currentCamera;
    int m_exposureValue;
    int m_frameRate;

    void setupCamera();
    void updateAvailableCameras();
};

#endif // CAMERACONTROLLER_H