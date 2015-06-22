#include <QApplication>
#include "mainwindow.h"
#include "macros.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QVNCVIEWER_APP_NAME);

    // setup main window and run
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
    return app.exec();
}
