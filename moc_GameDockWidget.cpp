/****************************************************************************
** Meta object code from reading C++ file 'GameDockWidget.h'
**
** Created: Wed Jul 20 16:25:53 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GameDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GameDockWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x09,
      32,   15,   15,   15, 0x09,
      50,   15,   15,   15, 0x09,
      68,   15,   15,   15, 0x09,
      87,   15,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GameDockWidget[] = {
    "GameDockWidget\0\0buyNewDevCard()\0"
    "activateDevCard()\0openTradeWindow()\0"
    "acceptTradeOffer()\0addChatMessage()\0"
};

const QMetaObject GameDockWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_GameDockWidget,
      qt_meta_data_GameDockWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GameDockWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GameDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GameDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GameDockWidget))
        return static_cast<void*>(const_cast< GameDockWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int GameDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buyNewDevCard(); break;
        case 1: activateDevCard(); break;
        case 2: openTradeWindow(); break;
        case 3: acceptTradeOffer(); break;
        case 4: addChatMessage(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
