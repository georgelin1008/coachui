#include "RecorderController.h"
#include <QDebug>
#include <QFile>
#include <QThread>

RecorderController::RecorderController(QObject *parent)
    : QObject(parent), m_process(nullptr)
{
}

void RecorderController::startRecording(const QString &filePath)
{
    // 使用默認幜率120fps
    startRecording(filePath, 120);
}

void RecorderController::startRecording(const QString &filePath, int frameRate)
{
    if (m_process) {
        qWarning() << "Recording already in progress!";
        return;
    }

    // 確保先停止任何正在運行的 rpicam 進程
    QProcess::execute("pkill", QStringList() << "-f" << "rpicam-vid");
    QThread::msleep(500); // 等待進程完全停止

    m_process = new QProcess(this);
    
    // 儲存輸出檔案路徑
    m_currentOutputFile = filePath;

    // 根據幀率調整位元率
    int bitrate = frameRate <= 30 ? 10000000 : 
                 frameRate <= 60 ? 15000000 :
                 frameRate <= 90 ? 18000000 : 20000000;

    QString program = "rpicam-vid";
    QStringList args;
    args << "--width" << "640"          // 使用與測試相同的解析度
         << "--height" << "400"          // OV9281 的原生解析度
         << "--framerate" << QString::number(frameRate)  // 使用動態幀率
         << "--denoise" << "off"
         << "--mode" << "640:400:10:P"   // OV9281 特定模式
         << "--codec" << "h264"          // 明確指定編碼格式
         << "--bitrate" << QString::number(bitrate)  // 根據幀率調整位元率
         << "-t" << "0"
         << "--preview" << "20,200,640,380"  // 與預覽保持相同對齊位置
         << "-o" << filePath;

    qDebug() << "Starting recording:" << program << args;
    m_process->start(program, args);

    if (!m_process->waitForStarted()) {
        qCritical() << "Failed to start rpicam-vid!";
        delete m_process;
        m_process = nullptr;
    } else {
        qDebug() << "Recording started successfully";
    }
}

void RecorderController::stopRecording()
{
    if (!m_process) {
        qWarning() << "No recording process running!";
        return;
    }

    qDebug() << "Stopping recording...";
    
    // 儲存當前輸出檔案路徑，用於後續轉換
    QString currentOutputFile = m_currentOutputFile;
    
    m_process->terminate();   
    if (!m_process->waitForFinished(3000)) {
        qWarning() << "Forcing kill...";
        m_process->kill();    
    }
    m_process->deleteLater();
    m_process = nullptr;
    
    // 如果錄製的是 H.264 檔案，自動轉換為 MP4
    if (currentOutputFile.endsWith(".h264")) {
        convertToMp4(currentOutputFile);
    }
}

void RecorderController::convertToMp4(const QString &h264FilePath)
{
    QString mp4FilePath = h264FilePath;
    mp4FilePath.replace(".h264", ".mp4");
    
    qDebug() << "Converting" << h264FilePath << "to" << mp4FilePath;
    
    QProcess *convertProcess = new QProcess(this);
    
    // 使用 ffmpeg 封裝 H.264 裸碼流為 MP4 容器，修正時間戳
    QStringList args;
    args << "-r" << "30"                  // 輸入幀率
         << "-i" << h264FilePath          // 輸入檔案
         << "-c:v" << "copy"              // 複製視訊串流
         << "-avoid_negative_ts" << "make_zero"  // 修正負時間戳
         << "-movflags" << "+faststart"   // 優化串流播放
         << "-f" << "mp4"                 // 強制 MP4 格式
         << "-y"                          // 覆蓋輸出檔案
         << mp4FilePath;                  // 輸出檔案
    
    qDebug() << "FFmpeg encapsulation args:" << args;
    
    connect(convertProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, h264FilePath, mp4FilePath, convertProcess](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode == 0) {
            qDebug() << "MP4 encapsulation successful:" << mp4FilePath;
            // 保留原始 H.264 檔案以防需要
            // QFile::remove(h264FilePath);  // 暫時不刪除
            // 發送 MP4 檔案路徑
            emit recordingFinished(mp4FilePath);
        } else {
            qWarning() << "MP4 encapsulation failed with exit code:" << exitCode;
            qDebug() << "Fallback to original H.264 file";
            // 轉換失敗時發送原始 H.264 檔案
            emit recordingFinished(h264FilePath);
        }
        convertProcess->deleteLater();
    });
    
    convertProcess->start("ffmpeg", args);
}
