#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T00:46:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3lab
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    converter.cpp \
    number.cpp \
    name.cpp

HEADERS  += mainwindow.h \
    converter.h \
    number.h \
    name.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES +=
