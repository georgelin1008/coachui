/****************************************************************************
** Meta object code from reading C++ file 'VideoListModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/models/VideoListModel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoListModel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_VideoListModel_t {
    uint offsetsAndSizes[30];
    char stringdata0[15];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[6];
    char stringdata5[7];
    char stringdata6[9];
    char stringdata7[9];
    char stringdata8[5];
    char stringdata9[15];
    char stringdata10[12];
    char stringdata11[9];
    char stringdata12[6];
    char stringdata13[16];
    char stringdata14[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VideoListModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VideoListModel_t qt_meta_stringdata_VideoListModel = {
    {
        QT_MOC_LITERAL(0, 14),  // "VideoListModel"
        QT_MOC_LITERAL(15, 16),  // "selectionChanged"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 13),  // "selectedCount"
        QT_MOC_LITERAL(47, 5),  // "clear"
        QT_MOC_LITERAL(53, 6),  // "append"
        QT_MOC_LITERAL(60, 8),  // "fileName"
        QT_MOC_LITERAL(69, 8),  // "filePath"
        QT_MOC_LITERAL(78, 4),  // "info"
        QT_MOC_LITERAL(83, 14),  // "appendDetailed"
        QT_MOC_LITERAL(98, 11),  // "displayName"
        QT_MOC_LITERAL(110, 8),  // "removeAt"
        QT_MOC_LITERAL(119, 5),  // "index"
        QT_MOC_LITERAL(125, 15),  // "toggleSelection"
        QT_MOC_LITERAL(141, 5)   // "count"
    },
    "VideoListModel",
    "selectionChanged",
    "",
    "selectedCount",
    "clear",
    "append",
    "fileName",
    "filePath",
    "info",
    "appendDetailed",
    "displayName",
    "removeAt",
    "index",
    "toggleSelection",
    "count"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VideoListModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   71,    2, 0x02,    3 /* Public */,
       5,    3,   72,    2, 0x02,    4 /* Public */,
       5,    2,   79,    2, 0x22,    8 /* Public | MethodCloned */,
       9,    4,   84,    2, 0x02,   11 /* Public */,
       9,    3,   93,    2, 0x22,   16 /* Public | MethodCloned */,
      11,    1,  100,    2, 0x02,   20 /* Public */,
      13,    1,  103,    2, 0x02,   22 /* Public */,
      14,    0,  106,    2, 0x102,   24 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,    6,    7,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Int,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoListModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_meta_stringdata_VideoListModel.offsetsAndSizes,
    qt_meta_data_VideoListModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_VideoListModel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoListModel, std::true_type>,
        // method 'selectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'clear'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'append'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'append'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'appendDetailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'appendDetailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'removeAt'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'toggleSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'count'
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void VideoListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoListModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->clear(); break;
        case 2: _t->append((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 3: _t->append((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->appendDetailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 5: _t->appendDetailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 6: _t->removeAt((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->toggleSelection((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: { int _r = _t->count();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoListModel::*)(int );
            if (_t _q_method = &VideoListModel::selectionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *VideoListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoListModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int VideoListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void VideoListModel::selectionChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
