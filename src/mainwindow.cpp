#include "mainwindow.h"
#include "surfacewidget.h"

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
{
  m_surface = new SurfaceWidget();
  m_surface->rfbConnect("localhost");
  //m_surface->setShowFps(true);

  setCentralWidget(m_surface);
}
