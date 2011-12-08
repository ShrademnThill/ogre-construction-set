#-------------------------------------------------
#
# Project created by QtCreator 2011-11-16T14:00:23
#
#-------------------------------------------------

QT += core gui

TARGET = OCS
TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    OgreWidget.cpp \
    ConfWidget.cpp \
    ModelList.cpp \
    Model.cpp \
    InstModel.cpp \
    Entity.cpp

HEADERS += MainWindow.hpp \
    OgreWidget.hpp \
    ConfWidget.hpp \
    ModelList.hpp \
    Model.hpp \
    InstModel.hpp \
    Entity.hpp

FORMS += \
    confwidget.ui \
    mainwindow.ui

win32: INCLUDEPATH += $$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/
win32: DEPENDPATH += $$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/

win32: LIBS += -L$$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/lib/ -llibboost_thread-vc90-mt-1_44
win32: LIBS += -L$$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/lib/ -llibboost_date_time-vc90-mt-1_44

win32: LIBS += -L$$PWD/../OgreSDK_vc9_v1-7-2/lib/release/ -lOgreMain

win32: INCLUDEPATH += $$PWD/../OgreSDK_vc9_v1-7-2/include/OGRE
win32: DEPENDPATH += $$PWD/../OgreSDK_vc9_v1-7-2/include/OGRE










