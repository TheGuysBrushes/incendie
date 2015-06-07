/****************************************************************************
** Meta object code from reading C++ file 'fwelcome.h'
**
** Created: Sun Jun 7 16:07:39 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fwelcome.h"
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
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x0a,
      26,   10,    9,    9, 0x0a,
      39,    9,    9,    9, 0x0a,
      56,    9,    9,    9, 0x0a,
      72,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Fwelcome[] = {
    "Fwelcome\0\0x\0setProba(int)\0setCoef(int)\0"
    "popImageDIalog()\0popSaveDialog()\0"
    "popSeedDialog()\0"
};

void Fwelcome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Fwelcome *_t = static_cast<Fwelcome *>(_o);
        switch (_id) {
        case 0: _t->setProba((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setCoef((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->popImageDIalog(); break;
        case 3: _t->popSaveDialog(); break;
        case 4: _t->popSeedDialog(); break;
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
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
