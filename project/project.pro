#-------------------------------------------------
#
# Project created by QtCreator 2015-06-18T18:12:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        graphscene.cpp \
    graphs.cpp \
    pagescene.cpp \
    agscene.cpp \
    colourcloset.cpp \
    biconnectedcomponent.cpp\
    auxiliarygraph.cpp \
    agnode.cpp \
    commands.cpp \
    pageview.cpp \
    edge_graphics.cpp \
    node_graphics.cpp \
    graphview.cpp


HEADERS  += mainwindow.h \
            graphscene.h \
    graphs.h \
    pagescene.h \
    agscene.h \
    colourcloset.h \
    biconnectedcomponent.h\
    auxiliarygraph.h \
    agnode.h \
    commands.h \
    pageview.h \
    edge_graphics.h \
    node_graphics.h \
    graphview.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

macx {
    QMAKE_CXXFLAGS += -stdlib=libstdc++
}

win32 {
    INCLUDEPATH +=
    LIBS +=
    QMAKE_CXXFLAGS +=
}

!win32{
    INCLUDEPATH += $$_PRO_FILE_PWD_/../OGDF/include
    LIBS += -L$$_PRO_FILE_PWD_/../OGDF/_release -lOGDF -lCOIN
    QMAKE_CXXFLAGS +=-pthread
}

RESOURCES += \
    icons.qrc
