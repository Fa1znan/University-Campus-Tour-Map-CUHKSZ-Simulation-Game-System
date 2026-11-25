QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    clickableellipseitem.cpp \
    clickablepathitem.cpp \
    customgraphicsview.cpp \
    main.cpp \
    mapscene.cpp \
    menubutton.cpp \
    popupmenu.cpp \
    widget.cpp

HEADERS += \
    clickableellipseitem.h \
    clickablepathitem.h \
    customgraphicsview.h \
    mapscene.h \
    menubutton.h \
    popupmenu.h \
    widget.h

RESOURCES += \
    mapData/resources.qrc
