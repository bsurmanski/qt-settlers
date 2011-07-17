/****************************************************************************
** Meta object code from reading C++ file 'SettingsWidget.h'
**
** Created: Sat Jul 16 22:15:58 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SettingsWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SettingsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingsWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,   15,   15,   15, 0x09,
      49,   43,   15,   15, 0x09,
      71,   43,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SettingsWidget[] = {
    "SettingsWidget\0\0startGame()\0startPressed()\0"
    "state\0toggleNetworking(int)\0"
    "toggleServer(int)\0"
};

const QMetaObject SettingsWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SettingsWidget,
      qt_meta_data_SettingsWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingsWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsWidget))
        return static_cast<void*>(const_cast< SettingsWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SettingsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startGame(); break;
        case 1: startPressed(); break;
        case 2: toggleNetworking((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: toggleServer((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SettingsWidget::startGame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
