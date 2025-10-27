/****************************************************************************
** Meta object code from reading C++ file 'AppController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/controllers/AppController.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppController.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_AppController_t {
    uint offsetsAndSizes[26];
    char stringdata0[14];
    char stringdata1[26];
    char stringdata2[1];
    char stringdata3[19];
    char stringdata4[6];
    char stringdata5[17];
    char stringdata6[8];
    char stringdata7[22];
    char stringdata8[11];
    char stringdata9[6];
    char stringdata10[15];
    char stringdata11[16];
    char stringdata12[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AppController_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_AppController_t qt_meta_stringdata_AppController = {
    {
        QT_MOC_LITERAL(0, 13),  // "AppController"
        QT_MOC_LITERAL(14, 25),  // "selectedVideoCountChanged"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 18),  // "onSelectionChanged"
        QT_MOC_LITERAL(60, 5),  // "count"
        QT_MOC_LITERAL(66, 16),  // "refreshVideoList"
        QT_MOC_LITERAL(83, 7),  // "dirPath"
        QT_MOC_LITERAL(91, 21),  // "getSelectedVideoPaths"
        QT_MOC_LITERAL(113, 10),  // "loadVideos"
        QT_MOC_LITERAL(124, 5),  // "paths"
        QT_MOC_LITERAL(130, 14),  // "videoListModel"
        QT_MOC_LITERAL(145, 15),  // "VideoListModel*"
        QT_MOC_LITERAL(161, 18)   // "selectedVideoCount"
    },
    "AppController",
    "selectedVideoCountChanged",
    "",
    "onSelectionChanged",
    "count",
    "refreshVideoList",
    "dirPath",
    "getSelectedVideoPaths",
    "loadVideos",
    "paths",
    "videoListModel",
    "VideoListModel*",
    "selectedVideoCount"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_AppController[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       2,   64, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   53,    2, 0x08,    5 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   56,    2, 0x02,    7 /* Public */,
       5,    0,   59,    2, 0x22,    9 /* Public | MethodCloned */,
       7,    0,   60,    2, 0x102,   10 /* Public | MethodIsConst  */,
       8,    1,   61,    2, 0x02,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::QStringList,
    QMetaType::Void, QMetaType::QStringList,    9,

 // properties: name, type, flags
      10, 0x80000000 | 11, 0x00015409, uint(-1), 0,
      12, QMetaType::Int, 0x00015001, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject AppController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_AppController.offsetsAndSizes,
    qt_meta_data_AppController,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_AppController_t,
        // property 'videoListModel'
        QtPrivate::TypeAndForceComplete<VideoListModel*, std::true_type>,
        // property 'selectedVideoCount'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AppController, std::true_type>,
        // method 'selectedVideoCountChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'refreshVideoList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'refreshVideoList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getSelectedVideoPaths'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'loadVideos'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>
    >,
    nullptr
} };

void AppController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AppController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->selectedVideoCountChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->onSelectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->refreshVideoList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->refreshVideoList(); break;
        case 4: { QStringList _r = _t->getSelectedVideoPaths();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->loadVideos((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AppController::*)(int );
            if (_t _q_method = &AppController::selectedVideoCountChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< VideoListModel* >(); break;
        }
    }
else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<AppController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< VideoListModel**>(_v) = _t->videoListModel(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->selectedVideoCount(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *AppController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AppController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AppController::selectedVideoCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
