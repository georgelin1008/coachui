/****************************************************************************
** Meta object code from reading C++ file 'VideoComposer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../VideoComposer.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoComposer.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_VideoComposer_t {
    uint offsetsAndSizes[164];
    char stringdata0[14];
    char stringdata1[12];
    char stringdata2[5];
    char stringdata3[18];
    char stringdata4[1];
    char stringdata5[14];
    char stringdata6[20];
    char stringdata7[17];
    char stringdata8[26];
    char stringdata9[24];
    char stringdata10[24];
    char stringdata11[16];
    char stringdata12[25];
    char stringdata13[28];
    char stringdata14[23];
    char stringdata15[23];
    char stringdata16[17];
    char stringdata17[25];
    char stringdata18[14];
    char stringdata19[8];
    char stringdata20[12];
    char stringdata21[9];
    char stringdata22[15];
    char stringdata23[6];
    char stringdata24[19];
    char stringdata25[20];
    char stringdata26[8];
    char stringdata27[6];
    char stringdata28[21];
    char stringdata29[11];
    char stringdata30[17];
    char stringdata31[14];
    char stringdata32[15];
    char stringdata33[16];
    char stringdata34[9];
    char stringdata35[12];
    char stringdata36[20];
    char stringdata37[7];
    char stringdata38[20];
    char stringdata39[12];
    char stringdata40[5];
    char stringdata41[24];
    char stringdata42[19];
    char stringdata43[8];
    char stringdata44[19];
    char stringdata45[13];
    char stringdata46[10];
    char stringdata47[21];
    char stringdata48[13];
    char stringdata49[12];
    char stringdata50[11];
    char stringdata51[10];
    char stringdata52[9];
    char stringdata53[12];
    char stringdata54[6];
    char stringdata55[12];
    char stringdata56[13];
    char stringdata57[21];
    char stringdata58[21];
    char stringdata59[22];
    char stringdata60[14];
    char stringdata61[14];
    char stringdata62[22];
    char stringdata63[21];
    char stringdata64[9];
    char stringdata65[9];
    char stringdata66[9];
    char stringdata67[22];
    char stringdata68[20];
    char stringdata69[11];
    char stringdata70[11];
    char stringdata71[7];
    char stringdata72[13];
    char stringdata73[10];
    char stringdata74[19];
    char stringdata75[17];
    char stringdata76[17];
    char stringdata77[18];
    char stringdata78[21];
    char stringdata79[16];
    char stringdata80[16];
    char stringdata81[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VideoComposer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VideoComposer_t qt_meta_stringdata_VideoComposer = {
    {
        QT_MOC_LITERAL(0, 13),  // "VideoComposer"
        QT_MOC_LITERAL(14, 11),  // "QML.Element"
        QT_MOC_LITERAL(26, 4),  // "auto"
        QT_MOC_LITERAL(31, 17),  // "videoCountChanged"
        QT_MOC_LITERAL(49, 0),  // ""
        QT_MOC_LITERAL(50, 13),  // "statusChanged"
        QT_MOC_LITERAL(64, 19),  // "loadedVideosChanged"
        QT_MOC_LITERAL(84, 16),  // "videoListChanged"
        QT_MOC_LITERAL(101, 25),  // "selectedVideoCountChanged"
        QT_MOC_LITERAL(127, 23),  // "foregroundWeightChanged"
        QT_MOC_LITERAL(151, 23),  // "backgroundWeightChanged"
        QT_MOC_LITERAL(175, 15),  // "blurSizeChanged"
        QT_MOC_LITERAL(191, 24),  // "hasComposedFramesChanged"
        QT_MOC_LITERAL(216, 27),  // "backgroundWindowSizeChanged"
        QT_MOC_LITERAL(244, 22),  // "edgeEnhancementChanged"
        QT_MOC_LITERAL(267, 22),  // "centerWeightingChanged"
        QT_MOC_LITERAL(290, 16),  // "thresholdChanged"
        QT_MOC_LITERAL(307, 24),  // "adaptiveThresholdChanged"
        QT_MOC_LITERAL(332, 13),  // "testCompleted"
        QT_MOC_LITERAL(346, 7),  // "message"
        QT_MOC_LITERAL(354, 11),  // "videoLoaded"
        QT_MOC_LITERAL(366, 8),  // "filename"
        QT_MOC_LITERAL(375, 14),  // "videoLoadError"
        QT_MOC_LITERAL(390, 5),  // "error"
        QT_MOC_LITERAL(396, 18),  // "compositionStarted"
        QT_MOC_LITERAL(415, 19),  // "compositionProgress"
        QT_MOC_LITERAL(435, 7),  // "current"
        QT_MOC_LITERAL(443, 5),  // "total"
        QT_MOC_LITERAL(449, 20),  // "compositionCompleted"
        QT_MOC_LITERAL(470, 10),  // "frameCount"
        QT_MOC_LITERAL(481, 16),  // "compositionError"
        QT_MOC_LITERAL(498, 13),  // "exportStarted"
        QT_MOC_LITERAL(512, 14),  // "exportProgress"
        QT_MOC_LITERAL(527, 15),  // "exportCompleted"
        QT_MOC_LITERAL(543, 8),  // "filePath"
        QT_MOC_LITERAL(552, 11),  // "exportError"
        QT_MOC_LITERAL(564, 19),  // "setForegroundWeight"
        QT_MOC_LITERAL(584, 6),  // "weight"
        QT_MOC_LITERAL(591, 19),  // "setBackgroundWeight"
        QT_MOC_LITERAL(611, 11),  // "setBlurSize"
        QT_MOC_LITERAL(623, 4),  // "size"
        QT_MOC_LITERAL(628, 23),  // "setBackgroundWindowSize"
        QT_MOC_LITERAL(652, 18),  // "setEdgeEnhancement"
        QT_MOC_LITERAL(671, 7),  // "enabled"
        QT_MOC_LITERAL(679, 18),  // "setCenterWeighting"
        QT_MOC_LITERAL(698, 12),  // "setThreshold"
        QT_MOC_LITERAL(711, 9),  // "threshold"
        QT_MOC_LITERAL(721, 20),  // "setAdaptiveThreshold"
        QT_MOC_LITERAL(742, 12),  // "testFunction"
        QT_MOC_LITERAL(755, 11),  // "getTestData"
        QT_MOC_LITERAL(767, 10),  // "loadVideos"
        QT_MOC_LITERAL(778, 9),  // "filePaths"
        QT_MOC_LITERAL(788, 8),  // "addVideo"
        QT_MOC_LITERAL(797, 11),  // "removeVideo"
        QT_MOC_LITERAL(809, 5),  // "index"
        QT_MOC_LITERAL(815, 11),  // "clearVideos"
        QT_MOC_LITERAL(827, 12),  // "getVideoInfo"
        QT_MOC_LITERAL(840, 20),  // "toggleVideoSelection"
        QT_MOC_LITERAL(861, 20),  // "loadVideosFromDialog"
        QT_MOC_LITERAL(882, 21),  // "getSelectedVideoPaths"
        QT_MOC_LITERAL(904, 13),  // "composeVideos"
        QT_MOC_LITERAL(918, 13),  // "selectedPaths"
        QT_MOC_LITERAL(932, 21),  // "composeSelectedVideos"
        QT_MOC_LITERAL(954, 20),  // "setCompositionParams"
        QT_MOC_LITERAL(975, 8),  // "fgWeight"
        QT_MOC_LITERAL(984, 8),  // "bgWeight"
        QT_MOC_LITERAL(993, 8),  // "blurSize"
        QT_MOC_LITERAL(1002, 21),  // "getComposedFramePaths"
        QT_MOC_LITERAL(1024, 19),  // "exportComposedVideo"
        QT_MOC_LITERAL(1044, 10),  // "outputPath"
        QT_MOC_LITERAL(1055, 10),  // "videoCount"
        QT_MOC_LITERAL(1066, 6),  // "status"
        QT_MOC_LITERAL(1073, 12),  // "loadedVideos"
        QT_MOC_LITERAL(1086, 9),  // "videoList"
        QT_MOC_LITERAL(1096, 18),  // "selectedVideoCount"
        QT_MOC_LITERAL(1115, 16),  // "foregroundWeight"
        QT_MOC_LITERAL(1132, 16),  // "backgroundWeight"
        QT_MOC_LITERAL(1149, 17),  // "hasComposedFrames"
        QT_MOC_LITERAL(1167, 20),  // "backgroundWindowSize"
        QT_MOC_LITERAL(1188, 15),  // "edgeEnhancement"
        QT_MOC_LITERAL(1204, 15),  // "centerWeighting"
        QT_MOC_LITERAL(1220, 17)   // "adaptiveThreshold"
    },
    "VideoComposer",
    "QML.Element",
    "auto",
    "videoCountChanged",
    "",
    "statusChanged",
    "loadedVideosChanged",
    "videoListChanged",
    "selectedVideoCountChanged",
    "foregroundWeightChanged",
    "backgroundWeightChanged",
    "blurSizeChanged",
    "hasComposedFramesChanged",
    "backgroundWindowSizeChanged",
    "edgeEnhancementChanged",
    "centerWeightingChanged",
    "thresholdChanged",
    "adaptiveThresholdChanged",
    "testCompleted",
    "message",
    "videoLoaded",
    "filename",
    "videoLoadError",
    "error",
    "compositionStarted",
    "compositionProgress",
    "current",
    "total",
    "compositionCompleted",
    "frameCount",
    "compositionError",
    "exportStarted",
    "exportProgress",
    "exportCompleted",
    "filePath",
    "exportError",
    "setForegroundWeight",
    "weight",
    "setBackgroundWeight",
    "setBlurSize",
    "size",
    "setBackgroundWindowSize",
    "setEdgeEnhancement",
    "enabled",
    "setCenterWeighting",
    "setThreshold",
    "threshold",
    "setAdaptiveThreshold",
    "testFunction",
    "getTestData",
    "loadVideos",
    "filePaths",
    "addVideo",
    "removeVideo",
    "index",
    "clearVideos",
    "getVideoInfo",
    "toggleVideoSelection",
    "loadVideosFromDialog",
    "getSelectedVideoPaths",
    "composeVideos",
    "selectedPaths",
    "composeSelectedVideos",
    "setCompositionParams",
    "fgWeight",
    "bgWeight",
    "blurSize",
    "getComposedFramePaths",
    "exportComposedVideo",
    "outputPath",
    "videoCount",
    "status",
    "loadedVideos",
    "videoList",
    "selectedVideoCount",
    "foregroundWeight",
    "backgroundWeight",
    "hasComposedFrames",
    "backgroundWindowSize",
    "edgeEnhancement",
    "centerWeighting",
    "adaptiveThreshold"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VideoComposer[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
      48,   16, // methods
      14,  410, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      25,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  304,    4, 0x06,   15 /* Public */,
       5,    0,  305,    4, 0x06,   16 /* Public */,
       6,    0,  306,    4, 0x06,   17 /* Public */,
       7,    0,  307,    4, 0x06,   18 /* Public */,
       8,    0,  308,    4, 0x06,   19 /* Public */,
       9,    0,  309,    4, 0x06,   20 /* Public */,
      10,    0,  310,    4, 0x06,   21 /* Public */,
      11,    0,  311,    4, 0x06,   22 /* Public */,
      12,    0,  312,    4, 0x06,   23 /* Public */,
      13,    0,  313,    4, 0x06,   24 /* Public */,
      14,    0,  314,    4, 0x06,   25 /* Public */,
      15,    0,  315,    4, 0x06,   26 /* Public */,
      16,    0,  316,    4, 0x06,   27 /* Public */,
      17,    0,  317,    4, 0x06,   28 /* Public */,
      18,    1,  318,    4, 0x06,   29 /* Public */,
      20,    1,  321,    4, 0x06,   31 /* Public */,
      22,    1,  324,    4, 0x06,   33 /* Public */,
      24,    0,  327,    4, 0x06,   35 /* Public */,
      25,    2,  328,    4, 0x06,   36 /* Public */,
      28,    1,  333,    4, 0x06,   39 /* Public */,
      30,    1,  336,    4, 0x06,   41 /* Public */,
      31,    0,  339,    4, 0x06,   43 /* Public */,
      32,    2,  340,    4, 0x06,   44 /* Public */,
      33,    1,  345,    4, 0x06,   47 /* Public */,
      35,    1,  348,    4, 0x06,   49 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      36,    1,  351,    4, 0x0a,   51 /* Public */,
      38,    1,  354,    4, 0x0a,   53 /* Public */,
      39,    1,  357,    4, 0x0a,   55 /* Public */,
      41,    1,  360,    4, 0x0a,   57 /* Public */,
      42,    1,  363,    4, 0x0a,   59 /* Public */,
      44,    1,  366,    4, 0x0a,   61 /* Public */,
      45,    1,  369,    4, 0x0a,   63 /* Public */,
      47,    1,  372,    4, 0x0a,   65 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      48,    0,  375,    4, 0x02,   67 /* Public */,
      49,    0,  376,    4, 0x02,   68 /* Public */,
      50,    1,  377,    4, 0x02,   69 /* Public */,
      52,    1,  380,    4, 0x02,   71 /* Public */,
      53,    1,  383,    4, 0x02,   73 /* Public */,
      55,    0,  386,    4, 0x02,   75 /* Public */,
      56,    1,  387,    4, 0x02,   76 /* Public */,
      57,    1,  390,    4, 0x02,   78 /* Public */,
      58,    0,  393,    4, 0x02,   80 /* Public */,
      59,    0,  394,    4, 0x102,   81 /* Public | MethodIsConst  */,
      60,    1,  395,    4, 0x02,   82 /* Public */,
      62,    0,  398,    4, 0x02,   84 /* Public */,
      63,    3,  399,    4, 0x02,   85 /* Public */,
      67,    0,  406,    4, 0x02,   89 /* Public */,
      68,    1,  407,    4, 0x02,   90 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   26,   27,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   26,   27,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void, QMetaType::QString,   23,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,   37,
    QMetaType::Void, QMetaType::Double,   37,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void, QMetaType::Bool,   43,
    QMetaType::Void, QMetaType::Bool,   43,
    QMetaType::Void, QMetaType::Double,   46,
    QMetaType::Void, QMetaType::Bool,   43,

 // methods: parameters
    QMetaType::Void,
    QMetaType::QStringList,
    QMetaType::Void, QMetaType::QStringList,   51,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void, QMetaType::Int,   54,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   54,
    QMetaType::Void, QMetaType::Int,   54,
    QMetaType::Void,
    QMetaType::QStringList,
    QMetaType::Bool, QMetaType::QStringList,   61,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int,   64,   65,   66,
    QMetaType::QStringList,
    QMetaType::Bool, QMetaType::QString,   69,

 // properties: name, type, flags
      70, QMetaType::Int, 0x00015001, uint(0), 0,
      71, QMetaType::QString, 0x00015001, uint(1), 0,
      72, QMetaType::QStringList, 0x00015001, uint(2), 0,
      73, QMetaType::QVariantList, 0x00015001, uint(3), 0,
      74, QMetaType::Int, 0x00015001, uint(4), 0,
      75, QMetaType::Double, 0x00015103, uint(5), 0,
      76, QMetaType::Double, 0x00015103, uint(6), 0,
      66, QMetaType::Int, 0x00015103, uint(7), 0,
      77, QMetaType::Bool, 0x00015001, uint(8), 0,
      78, QMetaType::Int, 0x00015103, uint(9), 0,
      79, QMetaType::Bool, 0x00015103, uint(10), 0,
      80, QMetaType::Bool, 0x00015103, uint(11), 0,
      46, QMetaType::Double, 0x00015103, uint(12), 0,
      81, QMetaType::Bool, 0x00015103, uint(13), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoComposer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoComposer.offsetsAndSizes,
    qt_meta_data_VideoComposer,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'videoCount'
        int,
        // property 'status'
        QString,
        // property 'loadedVideos'
        QStringList,
        // property 'videoList'
        QVariantList,
        // property 'selectedVideoCount'
        int,
        // property 'foregroundWeight'
        double,
        // property 'backgroundWeight'
        double,
        // property 'blurSize'
        int,
        // property 'hasComposedFrames'
        bool,
        // property 'backgroundWindowSize'
        int,
        // property 'edgeEnhancement'
        bool,
        // property 'centerWeighting'
        bool,
        // property 'threshold'
        double,
        // property 'adaptiveThreshold'
        bool,
        // Q_OBJECT / Q_GADGET
        VideoComposer,
        // method 'videoCountChanged'
        void,
        // method 'statusChanged'
        void,
        // method 'loadedVideosChanged'
        void,
        // method 'videoListChanged'
        void,
        // method 'selectedVideoCountChanged'
        void,
        // method 'foregroundWeightChanged'
        void,
        // method 'backgroundWeightChanged'
        void,
        // method 'blurSizeChanged'
        void,
        // method 'hasComposedFramesChanged'
        void,
        // method 'backgroundWindowSizeChanged'
        void,
        // method 'edgeEnhancementChanged'
        void,
        // method 'centerWeightingChanged'
        void,
        // method 'thresholdChanged'
        void,
        // method 'adaptiveThresholdChanged'
        void,
        // method 'testCompleted'
        void,
        const QString &,
        // method 'videoLoaded'
        void,
        const QString &,
        // method 'videoLoadError'
        void,
        const QString &,
        // method 'compositionStarted'
        void,
        // method 'compositionProgress'
        void,
        int,
        int,
        // method 'compositionCompleted'
        void,
        int,
        // method 'compositionError'
        void,
        const QString &,
        // method 'exportStarted'
        void,
        // method 'exportProgress'
        void,
        int,
        int,
        // method 'exportCompleted'
        void,
        const QString &,
        // method 'exportError'
        void,
        const QString &,
        // method 'setForegroundWeight'
        void,
        double,
        // method 'setBackgroundWeight'
        void,
        double,
        // method 'setBlurSize'
        void,
        int,
        // method 'setBackgroundWindowSize'
        void,
        int,
        // method 'setEdgeEnhancement'
        void,
        bool,
        // method 'setCenterWeighting'
        void,
        bool,
        // method 'setThreshold'
        void,
        double,
        // method 'setAdaptiveThreshold'
        void,
        bool,
        // method 'testFunction'
        void,
        // method 'getTestData'
        QStringList,
        // method 'loadVideos'
        void,
        const QStringList &,
        // method 'addVideo'
        void,
        const QString &,
        // method 'removeVideo'
        void,
        int,
        // method 'clearVideos'
        void,
        // method 'getVideoInfo'
        QString,
        int,
        // method 'toggleVideoSelection'
        void,
        int,
        // method 'loadVideosFromDialog'
        void,
        // method 'getSelectedVideoPaths'
        QStringList,
        // method 'composeVideos'
        bool,
        const QStringList &,
        // method 'composeSelectedVideos'
        bool,
        // method 'setCompositionParams'
        void,
        double,
        double,
        int,
        // method 'getComposedFramePaths'
        QStringList,
        // method 'exportComposedVideo'
        bool,
        const QString &
    >,
    nullptr
} };

void VideoComposer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoComposer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->videoCountChanged(); break;
        case 1: _t->statusChanged(); break;
        case 2: _t->loadedVideosChanged(); break;
        case 3: _t->videoListChanged(); break;
        case 4: _t->selectedVideoCountChanged(); break;
        case 5: _t->foregroundWeightChanged(); break;
        case 6: _t->backgroundWeightChanged(); break;
        case 7: _t->blurSizeChanged(); break;
        case 8: _t->hasComposedFramesChanged(); break;
        case 9: _t->backgroundWindowSizeChanged(); break;
        case 10: _t->edgeEnhancementChanged(); break;
        case 11: _t->centerWeightingChanged(); break;
        case 12: _t->thresholdChanged(); break;
        case 13: _t->adaptiveThresholdChanged(); break;
        case 14: _t->testCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->videoLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->videoLoadError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->compositionStarted(); break;
        case 18: _t->compositionProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 19: _t->compositionCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->compositionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->exportStarted(); break;
        case 22: _t->exportProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 23: _t->exportCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->exportError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->setForegroundWeight((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 26: _t->setBackgroundWeight((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 27: _t->setBlurSize((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->setBackgroundWindowSize((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->setEdgeEnhancement((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 30: _t->setCenterWeighting((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 31: _t->setThreshold((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 32: _t->setAdaptiveThreshold((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 33: _t->testFunction(); break;
        case 34: { QStringList _r = _t->getTestData();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 35: _t->loadVideos((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 36: _t->addVideo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 37: _t->removeVideo((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 38: _t->clearVideos(); break;
        case 39: { QString _r = _t->getVideoInfo((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 40: _t->toggleVideoSelection((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 41: _t->loadVideosFromDialog(); break;
        case 42: { QStringList _r = _t->getSelectedVideoPaths();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 43: { bool _r = _t->composeVideos((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 44: { bool _r = _t->composeSelectedVideos();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 45: _t->setCompositionParams((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 46: { QStringList _r = _t->getComposedFramePaths();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 47: { bool _r = _t->exportComposedVideo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::videoCountChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::loadedVideosChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::videoListChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::selectedVideoCountChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::foregroundWeightChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::backgroundWeightChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::blurSizeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::hasComposedFramesChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::backgroundWindowSizeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::edgeEnhancementChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::centerWeightingChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::thresholdChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::adaptiveThresholdChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(const QString & );
            if (_t _q_method = &VideoComposer::testCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(const QString & );
            if (_t _q_method = &VideoComposer::videoLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(const QString & );
            if (_t _q_method = &VideoComposer::videoLoadError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::compositionStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(int , int );
            if (_t _q_method = &VideoComposer::compositionProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(int );
            if (_t _q_method = &VideoComposer::compositionCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(const QString & );
            if (_t _q_method = &VideoComposer::compositionError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)();
            if (_t _q_method = &VideoComposer::exportStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(int , int );
            if (_t _q_method = &VideoComposer::exportProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(const QString & );
            if (_t _q_method = &VideoComposer::exportCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (VideoComposer::*)(const QString & );
            if (_t _q_method = &VideoComposer::exportError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 24;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VideoComposer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->videoCount(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->status(); break;
        case 2: *reinterpret_cast< QStringList*>(_v) = _t->loadedVideos(); break;
        case 3: *reinterpret_cast< QVariantList*>(_v) = _t->videoList(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->selectedVideoCount(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->foregroundWeight(); break;
        case 6: *reinterpret_cast< double*>(_v) = _t->backgroundWeight(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->blurSize(); break;
        case 8: *reinterpret_cast< bool*>(_v) = _t->hasComposedFrames(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->backgroundWindowSize(); break;
        case 10: *reinterpret_cast< bool*>(_v) = _t->edgeEnhancement(); break;
        case 11: *reinterpret_cast< bool*>(_v) = _t->centerWeighting(); break;
        case 12: *reinterpret_cast< double*>(_v) = _t->threshold(); break;
        case 13: *reinterpret_cast< bool*>(_v) = _t->adaptiveThreshold(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VideoComposer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 5: _t->setForegroundWeight(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setBackgroundWeight(*reinterpret_cast< double*>(_v)); break;
        case 7: _t->setBlurSize(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setBackgroundWindowSize(*reinterpret_cast< int*>(_v)); break;
        case 10: _t->setEdgeEnhancement(*reinterpret_cast< bool*>(_v)); break;
        case 11: _t->setCenterWeighting(*reinterpret_cast< bool*>(_v)); break;
        case 12: _t->setThreshold(*reinterpret_cast< double*>(_v)); break;
        case 13: _t->setAdaptiveThreshold(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *VideoComposer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoComposer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoComposer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoComposer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 48)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 48)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 48;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void VideoComposer::videoCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VideoComposer::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VideoComposer::loadedVideosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VideoComposer::videoListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void VideoComposer::selectedVideoCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void VideoComposer::foregroundWeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void VideoComposer::backgroundWeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void VideoComposer::blurSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void VideoComposer::hasComposedFramesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void VideoComposer::backgroundWindowSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void VideoComposer::edgeEnhancementChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void VideoComposer::centerWeightingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void VideoComposer::thresholdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void VideoComposer::adaptiveThresholdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void VideoComposer::testCompleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void VideoComposer::videoLoaded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void VideoComposer::videoLoadError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void VideoComposer::compositionStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void VideoComposer::compositionProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void VideoComposer::compositionCompleted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void VideoComposer::compositionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void VideoComposer::exportStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void VideoComposer::exportProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void VideoComposer::exportCompleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void VideoComposer::exportError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
