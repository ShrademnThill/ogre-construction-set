#-------------------------------------------------
#
# Project created by QtCreator 2011-11-16T14:00:23
#
#-------------------------------------------------

QT += core gui xml

TARGET = OCS
TEMPLATE = app

MOC_DIR = ../bin/moc/
OBJECTS_DIR = ../bin/objects/
DESTDIR = ../bin/

SOURCES += main.cpp \
    MainWindow.cpp \
    OgreWidget.cpp \
    ConfWidget.cpp \
    ModelList.cpp \
    Model.cpp \
    InstModel.cpp \
    Entity.cpp \
    InstEntity.cpp \
    Camera.cpp \
    RessourcesPathList.cpp \
    EditPathDialog.cpp \
    EditEntityDialog.cpp \
    SelectionRenderListener.cpp \
    SelectionBuffer.cpp \
    MaterialSwitcher.cpp \
    CurrentEntityModel.cpp \
    InstItem.cpp \
    SelectionManager.cpp \
    ConstraintManager.cpp \
    EditInstItemDialog.cpp \
    AxisObject.cpp \
    Item.cpp \
    DataManager.cpp \
    EntityGroup.cpp \
    EntityModelItem.cpp \
    GridObject.cpp \
    SceneXML.cpp

HEADERS += MainWindow.hpp \
    OgreWidget.hpp \
    ConfWidget.hpp \
    ModelList.hpp \
    Model.hpp \
    InstModel.hpp \
    Entity.hpp \
    InstEntity.hpp \
    Camera.hpp \
    RessourcesPathList.hpp \
    EditPathDialog.hpp \
    EditEntityDialog.hpp \
    SelectionRenderListener.hpp \
    SelectionBuffer.hpp \
    MaterialSwitcher.hpp \
    CurrentEntityModel.hpp \
    InstItem.hpp \
    SelectionManager.hpp \
    ConstraintManager.hpp \
    EditInstItemDialog.hpp \
    AxisObject.hpp \
    Item.hpp \
    DataManager.hpp \
    EntityGroup.hpp \
    EntityModelItem.hpp \
    GridObject.hpp \
    SceneXML.hpp

FORMS += \
    ConfWidget.ui \
    MainWindow.ui \
    EditPathDialog.ui \
    EditEntityDialog.ui \
    EditInstItemDialog.ui

unix: INCLUDEPATH += /usr/include/OGRE
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += OGRE

win32: INCLUDEPATH += $$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/
win32: DEPENDPATH += $$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/

win32: LIBS += -L$$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/lib/ -llibboost_thread-vc90-mt-1_44
win32: LIBS += -L$$PWD/../OgreSDK_vc9_v1-7-2/boost_1_44/lib/ -llibboost_date_time-vc90-mt-1_44
win32: LIBS += -L$$PWD/../OgreSDK_vc9_v1-7-2/lib/release/ -lOgreMain

win32: INCLUDEPATH += $$PWD/../OgreSDK_vc9_v1-7-2/include/OGRE
win32: DEPENDPATH += $$PWD/../OgreSDK_vc9_v1-7-2/include/OGRE

#RESOURCES += \
#    data.qrc














