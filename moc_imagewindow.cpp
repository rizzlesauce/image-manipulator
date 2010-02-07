/****************************************************************************
** Meta object code from reading C++ file 'imagewindow.h'
**
** Created: Sat Feb 6 01:26:40 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "imagewindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      29,   12,   12,   12, 0x08,
      47,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImageWindow[] = {
    "ImageWindow\0\0equalizeImage()\0"
    "createHistogram()\0reloadPixmap()\0"
};

const QMetaObject ImageWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ImageWindow,
      qt_meta_data_ImageWindow, 0 }
};

const QMetaObject *ImageWindow::metaObject() const
{
    return &staticMetaObject;
}

void *ImageWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageWindow))
        return static_cast<void*>(const_cast< ImageWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ImageWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: equalizeImage(); break;
        case 1: createHistogram(); break;
        case 2: reloadPixmap(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
