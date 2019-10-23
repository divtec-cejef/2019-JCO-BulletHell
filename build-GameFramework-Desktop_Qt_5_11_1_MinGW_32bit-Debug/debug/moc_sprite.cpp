/****************************************************************************
** Meta object code from reading C++ file 'sprite.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/sprite.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sprite.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Sprite_t {
    QByteArrayData data[23];
    char stringdata0[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Sprite_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Sprite_t qt_meta_stringdata_Sprite = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Sprite"
QT_MOC_LITERAL(1, 7, 17), // "animationFinished"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 14), // "opacityChanged"
QT_MOC_LITERAL(4, 41, 14), // "visibleChanged"
QT_MOC_LITERAL(5, 56, 14), // "enabledChanged"
QT_MOC_LITERAL(6, 71, 8), // "xChanged"
QT_MOC_LITERAL(7, 80, 8), // "yChanged"
QT_MOC_LITERAL(8, 89, 8), // "zChanged"
QT_MOC_LITERAL(9, 98, 15), // "rotationChanged"
QT_MOC_LITERAL(10, 114, 12), // "scaleChanged"
QT_MOC_LITERAL(11, 127, 20), // "onNextAnimationFrame"
QT_MOC_LITERAL(12, 148, 7), // "opacity"
QT_MOC_LITERAL(13, 156, 7), // "enabled"
QT_MOC_LITERAL(14, 164, 7), // "visible"
QT_MOC_LITERAL(15, 172, 3), // "pos"
QT_MOC_LITERAL(16, 176, 1), // "x"
QT_MOC_LITERAL(17, 178, 1), // "y"
QT_MOC_LITERAL(18, 180, 1), // "z"
QT_MOC_LITERAL(19, 182, 8), // "rotation"
QT_MOC_LITERAL(20, 191, 5), // "scale"
QT_MOC_LITERAL(21, 197, 20), // "transformOriginPoint"
QT_MOC_LITERAL(22, 218, 21) // "currentAnimationFrame"

    },
    "Sprite\0animationFinished\0\0opacityChanged\0"
    "visibleChanged\0enabledChanged\0xChanged\0"
    "yChanged\0zChanged\0rotationChanged\0"
    "scaleChanged\0onNextAnimationFrame\0"
    "opacity\0enabled\0visible\0pos\0x\0y\0z\0"
    "rotation\0scale\0transformOriginPoint\0"
    "currentAnimationFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Sprite[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
      11,   74, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,
       7,    0,   69,    2, 0x06 /* Public */,
       8,    0,   70,    2, 0x06 /* Public */,
       9,    0,   71,    2, 0x06 /* Public */,
      10,    0,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   73,    2, 0x08 /* Private */,

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

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::QReal, 0x00495903,
      13, QMetaType::Bool, 0x00495103,
      14, QMetaType::Bool, 0x00495903,
      15, QMetaType::QPointF, 0x00095903,
      16, QMetaType::QReal, 0x00495903,
      17, QMetaType::QReal, 0x00495903,
      18, QMetaType::QReal, 0x00495803,
      19, QMetaType::QReal, 0x00495103,
      20, QMetaType::QReal, 0x00495103,
      21, QMetaType::QPointF, 0x00095103,
      22, QMetaType::Int, 0x00095103,

 // properties: notify_signal_id
       1,
       3,
       2,
       0,
       4,
       5,
       6,
       7,
       8,
       0,
       0,

       0        // eod
};

void Sprite::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Sprite *_t = static_cast<Sprite *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->animationFinished(); break;
        case 1: _t->opacityChanged(); break;
        case 2: _t->visibleChanged(); break;
        case 3: _t->enabledChanged(); break;
        case 4: _t->xChanged(); break;
        case 5: _t->yChanged(); break;
        case 6: _t->zChanged(); break;
        case 7: _t->rotationChanged(); break;
        case 8: _t->scaleChanged(); break;
        case 9: _t->onNextAnimationFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::animationFinished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::opacityChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::visibleChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::enabledChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::xChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::yChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::zChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::rotationChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Sprite::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sprite::scaleChanged)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Sprite *_t = static_cast<Sprite *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->opacity(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isEnabled(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isVisible(); break;
        case 3: *reinterpret_cast< QPointF*>(_v) = _t->pos(); break;
        case 4: *reinterpret_cast< qreal*>(_v) = _t->x(); break;
        case 5: *reinterpret_cast< qreal*>(_v) = _t->y(); break;
        case 6: *reinterpret_cast< qreal*>(_v) = _t->zValue(); break;
        case 7: *reinterpret_cast< qreal*>(_v) = _t->rotation(); break;
        case 8: *reinterpret_cast< qreal*>(_v) = _t->scale(); break;
        case 9: *reinterpret_cast< QPointF*>(_v) = _t->transformOriginPoint(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->currentAnimationFrame(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Sprite *_t = static_cast<Sprite *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setVisible(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setPos(*reinterpret_cast< QPointF*>(_v)); break;
        case 4: _t->setX(*reinterpret_cast< qreal*>(_v)); break;
        case 5: _t->setY(*reinterpret_cast< qreal*>(_v)); break;
        case 6: _t->setZValue(*reinterpret_cast< qreal*>(_v)); break;
        case 7: _t->setRotation(*reinterpret_cast< qreal*>(_v)); break;
        case 8: _t->setScale(*reinterpret_cast< qreal*>(_v)); break;
        case 9: _t->setTransformOriginPoint(*reinterpret_cast< QPointF*>(_v)); break;
        case 10: _t->setCurrentAnimationFrame(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Sprite::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Sprite.data,
      qt_meta_data_Sprite,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Sprite::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Sprite::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Sprite.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(this);
    return QObject::qt_metacast(_clname);
}

int Sprite::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Sprite::animationFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Sprite::opacityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Sprite::visibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Sprite::enabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Sprite::xChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Sprite::yChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Sprite::zChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Sprite::rotationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Sprite::scaleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
