######################################################################
# Automatically generated by qmake (3.1) Mon Apr 29 09:36:13 2024
######################################################################
QT       +=  widgets  network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# TEMPLATE = app
# TARGET = FramelessWindow-Borderless-Window
# INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += framelesswindow.h \
           http.h \
           pcs.h \
           resource.h \
           stdafx.h \
           stylewindow.h \
           titlebar.h \
           windoweffect.h
SOURCES += framelesswindow.cpp \
           http.cpp \
           main.cpp \
           pcs.cpp \
           stdafx.cpp \
           stylewindow.cpp \
           titlebar.cpp \
           windoweffect.cpp
RESOURCES += stylewindow.qrc

FORMS += \
    pcs.ui
