#include "mainwindow.h"
#include "surfacewidget.h"

CMainWindow::CMainWindow(QWidget* parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
{
  m_surface = new CSurfaceWidget();
  m_surface->rfbConnect("localhost");

  //m_surface->setShowFps(true);

  setCentralWidget(m_surface);
}
