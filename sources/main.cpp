#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <Ogre.h>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(a.applicationDirPath());

    a.setOrganizationName("Shrademn'Company");
    a.setApplicationName("OCS");

    MainWindow w;
    w.show();

    return (a.exec());
}
