/****************************************************************************
** Meta object code from reading C++ file 'firewidget.h'
**
** Created: Fri May 22 16:09:33 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../firewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FireWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   11,   28,   11, 0x0a,
      42,   40,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FireWidget[] = {
    "FireWidget\0\0releaseSignal()\0bool\0"
    "next()\0x\0actionReceived(int)\0"
};

void FireWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FireWidget *_t = static_cast<FireWidget *>(_o);
        switch (_id) {
        case 0: _t->releaseSignal(); break;
        case 1: { bool _r = _t->next();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->actionReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FireWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FireWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FireWidget,
      qt_meta_data_FireWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FireWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FireWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FireWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FireWidget))
        return static_cast<void*>(const_cast< FireWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FireWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FireWidget::releaseSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
