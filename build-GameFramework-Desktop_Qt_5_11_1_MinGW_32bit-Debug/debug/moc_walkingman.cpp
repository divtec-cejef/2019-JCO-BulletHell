/****************************************************************************
** Meta object code from reading C++ file 'walkingman.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/walkingman.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'walkingman.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WalkingMan_t {
    QByteArrayData data[5];
    char stringdata0[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WalkingMan_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WalkingMan_t qt_meta_stringdata_WalkingMan = {
    {
QT_MOC_LITERAL(0, 0, 10), // "WalkingMan"
QT_MOC_LITERAL(1, 11, 12), // "onKeyPressed"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "key"
QT_MOC_LITERAL(4, 29, 13) // "onKeyReleased"

    },
    "WalkingMan\0onKeyPressed\0\0key\0onKeyReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WalkingMan[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void WalkingMan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WalkingMan *_t = static_cast<WalkingMan *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onKeyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onKeyReleased((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WalkingMan::staticMetaObject = {
    { &Sprite::staticMetaObject, qt_meta_stringdata_WalkingMan.data,
      qt_meta_data_WalkingMan,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WalkingMan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WalkingMan::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WalkingMan.stringdata0))
        return static_cast<void*>(this);
    return Sprite::qt_metacast(_clname);
}

int WalkingMan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Sprite::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
