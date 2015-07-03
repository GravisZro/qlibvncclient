#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtCore>

//#include "surfacewidget.h"
class CSurfaceWidget;

class CMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  CMainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::Widget);

private:
  CSurfaceWidget* m_surface;
};

#endif // MAINWINDOW_H
