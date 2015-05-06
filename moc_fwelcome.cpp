/****************************************************************************
** Meta object code from reading C++ file 'fwelcome.h'
**
** Created: Wed May 6 14:29:15 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fwelcome.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fwelcome.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Fwelcome[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x0a,
      27,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Fwelcome[] = {
    "Fwelcome\0\0x\0set_proba(int)\0set_coef(int)\0"
};

void Fwelcome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Fwelcome *_t = static_cast<Fwelcome *>(_o);
        switch (_id) {
        case 0: _t->set_proba((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->set_coef((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Fwelcome::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Fwelcome::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Fwelcome,
      qt_meta_data_Fwelcome, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Fwelcome::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Fwelcome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Fwelcome::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Fwelcome))
        return static_cast<void*>(const_cast< Fwelcome*>(this));
    return QDialog::qt_metacast(_clname);
}

int Fwelcome::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
