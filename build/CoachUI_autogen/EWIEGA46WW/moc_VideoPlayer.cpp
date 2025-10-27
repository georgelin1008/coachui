/****************************************************************************
** Meta object code from reading C++ file 'VideoPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../VideoPlayer.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_VideoPlayer_t {
    uint offsetsAndSizes[110];
    char stringdata0[12];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[16];
    char stringdata5[19];
    char stringdata6[19];
    char stringdata7[20];
    char stringdata8[17];
    char stringdata9[12];
    char stringdata10[14];
    char stringdata11[6];
    char stringdata12[22];
    char stringdata13[24];
    char stringdata14[28];
    char stringdata15[23];
    char stringdata16[10];
    char stringdata17[9];
    char stringdata18[5];
    char stringdata19[6];
    char stringdata20[5];
    char stringdata21[12];
    char stringdata22[9];
    char stringdata23[15];
    char stringdata24[13];
    char stringdata25[12];
    char stringdata26[13];
    char stringdata27[16];
    char stringdata28[5];
    char stringdata29[13];
    char stringdata30[12];
    char stringdata31[5];
    char stringdata32[14];
    char stringdata33[10];
    char stringdata34[21];
    char stringdata35[10];
    char stringdata36[11];
    char stringdata37[22];
    char stringdata38[25];
    char stringdata39[9];
    char stringdata40[25];
    char stringdata41[19];
    char stringdata42[15];
    char stringdata43[18];
    char stringdata44[9];
    char stringdata45[21];
    char stringdata46[11];
    char stringdata47[10];
    char stringdata48[12];
    char stringdata49[12];
    char stringdata50[13];
    char stringdata51[10];
    char stringdata52[17];
    char stringdata53[17];
    char stringdata54[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VideoPlayer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VideoPlayer_t qt_meta_stringdata_VideoPlayer = {
    {
        QT_MOC_LITERAL(0, 11),  // "VideoPlayer"
        QT_MOC_LITERAL(12, 16),  // "isPlayingChanged"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 15),  // "positionChanged"
        QT_MOC_LITERAL(46, 15),  // "durationChanged"
        QT_MOC_LITERAL(62, 18),  // "currentFileChanged"
        QT_MOC_LITERAL(81, 18),  // "recentFilesChanged"
        QT_MOC_LITERAL(100, 19),  // "playbackRateChanged"
        QT_MOC_LITERAL(120, 16),  // "videoSinkChanged"
        QT_MOC_LITERAL(137, 11),  // "videoLoaded"
        QT_MOC_LITERAL(149, 13),  // "playbackError"
        QT_MOC_LITERAL(163, 5),  // "error"
        QT_MOC_LITERAL(169, 21),  // "sideBySideModeChanged"
        QT_MOC_LITERAL(191, 23),  // "sideBySideVideosChanged"
        QT_MOC_LITERAL(215, 27),  // "sideBySideHorizontalChanged"
        QT_MOC_LITERAL(243, 22),  // "sideBySidePlayersReady"
        QT_MOC_LITERAL(266, 9),  // "loadVideo"
        QT_MOC_LITERAL(276, 8),  // "filePath"
        QT_MOC_LITERAL(285, 4),  // "play"
        QT_MOC_LITERAL(290, 5),  // "pause"
        QT_MOC_LITERAL(296, 4),  // "stop"
        QT_MOC_LITERAL(301, 11),  // "setPosition"
        QT_MOC_LITERAL(313, 8),  // "position"
        QT_MOC_LITERAL(322, 14),  // "playH264Direct"
        QT_MOC_LITERAL(337, 12),  // "h264FilePath"
        QT_MOC_LITERAL(350, 11),  // "seekForward"
        QT_MOC_LITERAL(362, 12),  // "seekBackward"
        QT_MOC_LITERAL(375, 15),  // "setPlaybackRate"
        QT_MOC_LITERAL(391, 4),  // "rate"
        QT_MOC_LITERAL(396, 12),  // "setVideoSink"
        QT_MOC_LITERAL(409, 11),  // "QVideoSink*"
        QT_MOC_LITERAL(421, 4),  // "sink"
        QT_MOC_LITERAL(426, 13),  // "getVideoFiles"
        QT_MOC_LITERAL(440, 9),  // "directory"
        QT_MOC_LITERAL(450, 20),  // "enableSideBySideMode"
        QT_MOC_LITERAL(471, 9),  // "filePaths"
        QT_MOC_LITERAL(481, 10),  // "horizontal"
        QT_MOC_LITERAL(492, 21),  // "disableSideBySideMode"
        QT_MOC_LITERAL(514, 24),  // "updateSideBySideDuration"
        QT_MOC_LITERAL(539, 8),  // "duration"
        QT_MOC_LITERAL(548, 24),  // "updateSideBySidePosition"
        QT_MOC_LITERAL(573, 18),  // "setPreviewPosition"
        QT_MOC_LITERAL(592, 14),  // "updatePosition"
        QT_MOC_LITERAL(607, 17),  // "onProcessFinished"
        QT_MOC_LITERAL(625, 8),  // "exitCode"
        QT_MOC_LITERAL(634, 20),  // "QProcess::ExitStatus"
        QT_MOC_LITERAL(655, 10),  // "exitStatus"
        QT_MOC_LITERAL(666, 9),  // "isPlaying"
        QT_MOC_LITERAL(676, 11),  // "currentFile"
        QT_MOC_LITERAL(688, 11),  // "recentFiles"
        QT_MOC_LITERAL(700, 12),  // "playbackRate"
        QT_MOC_LITERAL(713, 9),  // "videoSink"
        QT_MOC_LITERAL(723, 16),  // "isSideBySideMode"
        QT_MOC_LITERAL(740, 16),  // "sideBySideVideos"
        QT_MOC_LITERAL(757, 20)   // "sideBySideHorizontal"
    },
    "VideoPlayer",
    "isPlayingChanged",
    "",
    "positionChanged",
    "durationChanged",
    "currentFileChanged",
    "recentFilesChanged",
    "playbackRateChanged",
    "videoSinkChanged",
    "videoLoaded",
    "playbackError",
    "error",
    "sideBySideModeChanged",
    "sideBySideVideosChanged",
    "sideBySideHorizontalChanged",
    "sideBySidePlayersReady",
    "loadVideo",
    "filePath",
    "play",
    "pause",
    "stop",
    "setPosition",
    "position",
    "playH264Direct",
    "h264FilePath",
    "seekForward",
    "seekBackward",
    "setPlaybackRate",
    "rate",
    "setVideoSink",
    "QVideoSink*",
    "sink",
    "getVideoFiles",
    "directory",
    "enableSideBySideMode",
    "filePaths",
    "horizontal",
    "disableSideBySideMode",
    "updateSideBySideDuration",
    "duration",
    "updateSideBySidePosition",
    "setPreviewPosition",
    "updatePosition",
    "onProcessFinished",
    "exitCode",
    "QProcess::ExitStatus",
    "exitStatus",
    "isPlaying",
    "currentFile",
    "recentFiles",
    "playbackRate",
    "videoSink",
    "isSideBySideMode",
    "sideBySideVideos",
    "sideBySideHorizontal"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VideoPlayer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
      10,  275, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  212,    2, 0x06,   11 /* Public */,
       3,    0,  213,    2, 0x06,   12 /* Public */,
       4,    0,  214,    2, 0x06,   13 /* Public */,
       5,    0,  215,    2, 0x06,   14 /* Public */,
       6,    0,  216,    2, 0x06,   15 /* Public */,
       7,    0,  217,    2, 0x06,   16 /* Public */,
       8,    0,  218,    2, 0x06,   17 /* Public */,
       9,    0,  219,    2, 0x06,   18 /* Public */,
      10,    1,  220,    2, 0x06,   19 /* Public */,
      12,    0,  223,    2, 0x06,   21 /* Public */,
      13,    0,  224,    2, 0x06,   22 /* Public */,
      14,    0,  225,    2, 0x06,   23 /* Public */,
      15,    0,  226,    2, 0x06,   24 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    1,  227,    2, 0x0a,   25 /* Public */,
      18,    0,  230,    2, 0x0a,   27 /* Public */,
      19,    0,  231,    2, 0x0a,   28 /* Public */,
      20,    0,  232,    2, 0x0a,   29 /* Public */,
      21,    1,  233,    2, 0x0a,   30 /* Public */,
      23,    1,  236,    2, 0x0a,   32 /* Public */,
      25,    0,  239,    2, 0x0a,   34 /* Public */,
      26,    0,  240,    2, 0x0a,   35 /* Public */,
      27,    1,  241,    2, 0x0a,   36 /* Public */,
      29,    1,  244,    2, 0x0a,   38 /* Public */,
      32,    1,  247,    2, 0x0a,   40 /* Public */,
      32,    0,  250,    2, 0x2a,   42 /* Public | MethodCloned */,
      34,    2,  251,    2, 0x0a,   43 /* Public */,
      34,    1,  256,    2, 0x2a,   46 /* Public | MethodCloned */,
      37,    0,  259,    2, 0x0a,   48 /* Public */,
      38,    1,  260,    2, 0x0a,   49 /* Public */,
      40,    1,  263,    2, 0x0a,   51 /* Public */,
      41,    1,  266,    2, 0x0a,   53 /* Public */,
      42,    0,  269,    2, 0x08,   55 /* Private */,
      43,    2,  270,    2, 0x08,   56 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   22,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   28,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::QStringList, QMetaType::QString,   33,
    QMetaType::QStringList,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Bool,   35,   36,
    QMetaType::Void, QMetaType::QStringList,   35,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   39,
    QMetaType::Void, QMetaType::LongLong,   22,
    QMetaType::Void, QMetaType::LongLong,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 45,   44,   46,

 // properties: name, type, flags
      47, QMetaType::Bool, 0x00015001, uint(0), 0,
      22, QMetaType::LongLong, 0x00015001, uint(1), 0,
      39, QMetaType::LongLong, 0x00015001, uint(2), 0,
      48, QMetaType::QString, 0x00015001, uint(3), 0,
      49, QMetaType::QStringList, 0x00015001, uint(4), 0,
      50, QMetaType::Double, 0x00015103, uint(5), 0,
      51, 0x80000000 | 30, 0x0001510b, uint(6), 0,
      52, QMetaType::Bool, 0x00015001, uint(9), 0,
      53, QMetaType::QStringList, 0x00015001, uint(10), 0,
      54, QMetaType::Bool, 0x00015001, uint(11), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoPlayer.offsetsAndSizes,
    qt_meta_data_VideoPlayer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_VideoPlayer_t,
        // property 'isPlaying'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'position'
        QtPrivate::TypeAndForceComplete<qint64, std::true_type>,
        // property 'duration'
        QtPrivate::TypeAndForceComplete<qint64, std::true_type>,
        // property 'currentFile'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'recentFiles'
        QtPrivate::TypeAndForceComplete<QStringList, std::true_type>,
        // property 'playbackRate'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'videoSink'
        QtPrivate::TypeAndForceComplete<QVideoSink*, std::true_type>,
        // property 'isSideBySideMode'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'sideBySideVideos'
        QtPrivate::TypeAndForceComplete<QStringList, std::true_type>,
        // property 'sideBySideHorizontal'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoPlayer, std::true_type>,
        // method 'isPlayingChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'positionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'durationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'currentFileChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'recentFilesChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playbackRateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'videoSinkChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'videoLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playbackError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sideBySideModeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sideBySideVideosChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sideBySideHorizontalChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sideBySidePlayersReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadVideo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'play'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pause'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'playH264Direct'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'seekForward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'seekBackward'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setPlaybackRate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'setVideoSink'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVideoSink *, std::false_type>,
        // method 'getVideoFiles'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getVideoFiles'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'enableSideBySideMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'enableSideBySideMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'disableSideBySideMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSideBySideDuration'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'updateSideBySidePosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'setPreviewPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'updatePosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProcessFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>
    >,
    nullptr
} };

void VideoPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoPlayer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->isPlayingChanged(); break;
        case 1: _t->positionChanged(); break;
        case 2: _t->durationChanged(); break;
        case 3: _t->currentFileChanged(); break;
        case 4: _t->recentFilesChanged(); break;
        case 5: _t->playbackRateChanged(); break;
        case 6: _t->videoSinkChanged(); break;
        case 7: _t->videoLoaded(); break;
        case 8: _t->playbackError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->sideBySideModeChanged(); break;
        case 10: _t->sideBySideVideosChanged(); break;
        case 11: _t->sideBySideHorizontalChanged(); break;
        case 12: _t->sideBySidePlayersReady(); break;
        case 13: _t->loadVideo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->play(); break;
        case 15: _t->pause(); break;
        case 16: _t->stop(); break;
        case 17: _t->setPosition((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 18: _t->playH264Direct((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->seekForward(); break;
        case 20: _t->seekBackward(); break;
        case 21: _t->setPlaybackRate((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 22: _t->setVideoSink((*reinterpret_cast< std::add_pointer_t<QVideoSink*>>(_a[1]))); break;
        case 23: { QStringList _r = _t->getVideoFiles((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 24: { QStringList _r = _t->getVideoFiles();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 25: _t->enableSideBySideMode((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 26: _t->enableSideBySideMode((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 27: _t->disableSideBySideMode(); break;
        case 28: _t->updateSideBySideDuration((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 29: _t->updateSideBySidePosition((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 30: _t->setPreviewPosition((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 31: _t->updatePosition(); break;
        case 32: _t->onProcessFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QVideoSink* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::isPlayingChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::positionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::durationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::currentFileChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::recentFilesChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::playbackRateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::videoSinkChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::videoLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)(const QString & );
            if (_t _q_method = &VideoPlayer::playbackError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::sideBySideModeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::sideBySideVideosChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::sideBySideHorizontalChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (VideoPlayer::*)();
            if (_t _q_method = &VideoPlayer::sideBySidePlayersReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVideoSink* >(); break;
        }
    }
else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VideoPlayer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isPlaying(); break;
        case 1: *reinterpret_cast< qint64*>(_v) = _t->position(); break;
        case 2: *reinterpret_cast< qint64*>(_v) = _t->duration(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->currentFile(); break;
        case 4: *reinterpret_cast< QStringList*>(_v) = _t->recentFiles(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->playbackRate(); break;
        case 6: *reinterpret_cast< QVideoSink**>(_v) = _t->videoSink(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->isSideBySideMode(); break;
        case 8: *reinterpret_cast< QStringList*>(_v) = _t->sideBySideVideos(); break;
        case 9: *reinterpret_cast< bool*>(_v) = _t->sideBySideHorizontal(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VideoPlayer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 5: _t->setPlaybackRate(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setVideoSink(*reinterpret_cast< QVideoSink**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *VideoPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoPlayer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void VideoPlayer::isPlayingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VideoPlayer::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VideoPlayer::durationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VideoPlayer::currentFileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void VideoPlayer::recentFilesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void VideoPlayer::playbackRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void VideoPlayer::videoSinkChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void VideoPlayer::videoLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void VideoPlayer::playbackError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void VideoPlayer::sideBySideModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void VideoPlayer::sideBySideVideosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void VideoPlayer::sideBySideHorizontalChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void VideoPlayer::sideBySidePlayersReady()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
