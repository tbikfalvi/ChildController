
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui
CONFIG      += qt
RESOURCES   += childcontroller.qrc
RC_FILE      = childcontroller.rc
TEMPLATE     = app
DESTDIR      = ..
TRANSLATIONS = childcontroller_hu.ts
TARGET       = ChildController

HEADERS     += dlgmain.h

SOURCES     += main.cpp\
               dlgmain.cpp

FORMS       += dlgmain.ui
