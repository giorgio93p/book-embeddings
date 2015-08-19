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
    embedding_edge.cpp \
    pagescene.cpp


HEADERS  += mainwindow.h \
            graphscene.h \
    graphs.h \
    embedding_edge.h \
    pagescene.h \
    colors.h

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
