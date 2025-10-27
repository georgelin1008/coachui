#ifndef RECORDERCONTROLLER_H
#define RECORDERCONTROLLER_H

#include <QObject>
#include <QProcess>

class RecorderController : public QObject
{
    Q_OBJECT
public:
    explicit RecorderController(QObject *parent = nullptr);

    Q_INVOKABLE void startRecording(const QString &filePath);
    Q_INVOKABLE void startRecording(const QString &filePath, int frameRate);
    Q_INVOKABLE void stopRecording();

signals:
    void recordingFinished(const QString &filePath);

private slots:
    void convertToMp4(const QString &h264FilePath);

private:
    QProcess *m_process;
    QString m_currentOutputFile;
};

#endif // RECORDERCONTROLLER_H
