/****************************************************************************
** Meta object code from reading C++ file 'fwelcome.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fwelcome.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fwelcome.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fwelcome_t {
    QByteArrayData data[12];
    char stringdata0[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fwelcome_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fwelcome_t qt_meta_stringdata_Fwelcome = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Fwelcome"
QT_MOC_LITERAL(1, 9, 8), // "setProba"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 1), // "x"
QT_MOC_LITERAL(4, 21, 7), // "setCoef"
QT_MOC_LITERAL(5, 29, 12), // "giveMinWidth"
QT_MOC_LITERAL(6, 42, 12), // "giveMaxWidth"
QT_MOC_LITERAL(7, 55, 13), // "giveMinHeight"
QT_MOC_LITERAL(8, 69, 13), // "giveMaxHeight"
QT_MOC_LITERAL(9, 83, 14), // "popImageDIalog"
QT_MOC_LITERAL(10, 98, 13), // "popSaveDialog"
QT_MOC_LITERAL(11, 112, 13) // "popSeedDialog"

    },
    "Fwelcome\0setProba\0\0x\0setCoef\0giveMinWidth\0"
    "giveMaxWidth\0giveMinHeight\0giveMaxHeight\0"
    "popImageDIalog\0popSaveDialog\0popSeedDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fwelcome[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       4,    1,   62,    2, 0x0a /* Public */,
       5,    0,   65,    2, 0x08 /* Private */,
       6,    0,   66,    2, 0x08 /* Private */,
       7,    0,   67,    2, 0x08 /* Private */,
       8,    0,   68,    2, 0x08 /* Private */,
       9,    0,   69,    2, 0x0a /* Public */,
      10,    0,   70,    2, 0x0a /* Public */,
      11,    0,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Fwelcome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Fwelcome *_t = static_cast<Fwelcome *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setProba((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setCoef((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->giveMinWidth(); break;
        case 3: _t->giveMaxWidth(); break;
        case 4: _t->giveMinHeight(); break;
        case 5: _t->giveMaxHeight(); break;
        case 6: _t->popImageDIalog(); break;
        case 7: _t->popSaveDialog(); break;
        case 8: _t->popSeedDialog(); break;
        default: ;
        }
    }
}

const QMetaObject Fwelcome::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Fwelcome.data,
      qt_meta_data_Fwelcome,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fwelcome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fwelcome::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fwelcome.stringdata0))
        return static_cast<void*>(const_cast< Fwelcome*>(this));
    return QDialog::qt_metacast(_clname);
}

int Fwelcome::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
