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
    mypath.cpp \
    myfirstdialog.cpp


HEADERS  += mainwindow.h \
            graphscene.h \
    mypath.h \
    myfirstdialog.h

FORMS    += mainwindow.ui

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




