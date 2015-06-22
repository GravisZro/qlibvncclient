#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtCore>

//#include "surfacewidget.h"
class SurfaceWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::Widget);

private:
  SurfaceWidget* m_surface;
};

#endif // MAINWINDOW_H
