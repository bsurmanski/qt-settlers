/****************************************************************************
** Meta object code from reading C++ file 'GLFrame.h'
**
** Created: Thu Jul 14 21:06:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GLFrame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GLFrame[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,    8,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GLFrame[] = {
    "GLFrame\0\0playerInfoUpdate()\0update()\0"
};

const QMetaObject GLFrame::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLFrame,
      qt_meta_data_GLFrame, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GLFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GLFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GLFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLFrame))
        return static_cast<void*>(const_cast< GLFrame*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: playerInfoUpdate(); break;
        case 1: update(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GLFrame::playerInfoUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
