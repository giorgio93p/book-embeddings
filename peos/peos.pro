#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T18:32:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = peos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphscene.cpp \
    mainwidget.cpp

HEADERS  += mainwindow.h \
    graphscene.h \
    mainwidget.h
        mainwindow

FORMS    += mainwindow.ui \
    ../Desktop/book.ui
