#-------------------------------------------------
#
# Project created by QtCreator 2019-03-19T14:53:51
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 2.0.0.1
QMAKE_TARGET_COPYRIGHT = BrookLite

TARGET = Rib
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++1z
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dbconnect.cpp \
    printwindow.cpp \
    subjects.cpp \
    addwindow.cpp \
    orderwindow.cpp \
    updatewindow.cpp \
    modificationwindow.cpp \
    xmlwindow.cpp

HEADERS += \
        mainwindow.h \
    dbconnect.h \
    printwindow.h \
    subjects.h \
    addwindow.h \
    orderwindow.h \
    updatewindow.h \
    modificationwindow.h \
    xmlwindow.h

FORMS += \
        mainwindow.ui \
    addwindow.ui \
    orderwindow.ui \
    printwindow.ui \
    updatewindow.ui \
    modificationwindow.ui \
    xmlwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=





