#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Qt VNC Client");

    // setup main window and run
    CMainWindow* mainWindow = new CMainWindow();
    mainWindow->show();
    mainWindow->setGeometry(app.desktop()->screenGeometry());
    return app.exec();
}
