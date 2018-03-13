/****************************************************************************
** Meta object code from reading C++ file 'Kiax2DotTelDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Kiax2DotTelDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Kiax2DotTelDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Kiax2DotTelDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      39,   18,   18,   18, 0x0a,
      80,   61,   18,   18, 0x0a,
     127,  110,   18,   18, 0x0a,
     189,   18,  181,   18, 0x0a,
     207,  201,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Kiax2DotTelDialog[] = {
    "Kiax2DotTelDialog\0\0dialButtonClicked()\0"
    "cancelButtonClicked()\0number,description\0"
    "addTelNumber(QString,QString)\0"
    "current,previous\0"
    "currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "QString\0getNumber()\0descr\0"
    "addTelDescription(QString)\0"
};

void Kiax2DotTelDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Kiax2DotTelDialog *_t = static_cast<Kiax2DotTelDialog *>(_o);
        switch (_id) {
        case 0: _t->dialButtonClicked(); break;
        case 1: _t->cancelButtonClicked(); break;
        case 2: _t->addTelNumber((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 4: { QString _r = _t->getNumber();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: _t->addTelDescription((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Kiax2DotTelDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Kiax2DotTelDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Kiax2DotTelDialog,
      qt_meta_data_Kiax2DotTelDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Kiax2DotTelDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Kiax2DotTelDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Kiax2DotTelDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Kiax2DotTelDialog))
        return static_cast<void*>(const_cast< Kiax2DotTelDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Kiax2DotTelDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
