#ifndef SURFACEWIDGET_H
#define SURFACEWIDGET_H

#include <QtWidgets>

#include "rfbclientwapper.h"

struct rfbExtraClientDataSegment
{
  QRect   rect;
  QPixmap pixmap;
};

typedef QList<rfbExtraClientDataSegment> rfbExtraClientData;

class QRFBClient : public QWidget,
                   public RFBClientWapper
{
  Q_OBJECT
public:
  QRFBClient(QWidget* parent);
  virtual ~QRFBClient(void);

  void rfbConnect(const QString& hostname, int port = QVNCVIEWER_VNC_BASE_PORT);
  void rfbDisconnect(void);

protected slots:
  void getMessage(void);
  void requestUpdate(void);

signals:
  void connected(void);
  void disconnected(void);

  void framebufferResize(QSize sz);
  void framebufferUpdate(void);

protected:
  rfbBool do_MallocFrameBuffer(void);
  void do_GotFrameBufferUpdate(int x, int y, int w, int h);

  rfbExtraClientData& data(void) { context(m_client); return s_data[client()]; }

private:
  rfbClient* m_client;
  static std::map<rfbClient*, rfbExtraClientData> s_data;

  QTimer* m_timer;
  QElapsedTimer m_lastMessageReceivedTimer;
  QMutex m_mutex;
  bool m_connected;
};

class SurfaceWidget : public QRFBClient
{
  Q_OBJECT
public:
  SurfaceWidget(QWidget *parent = nullptr);
  ~SurfaceWidget();

  QSize surfaceSize() { return m_surfacePixmap.size(); }

  void setBackgroundBrush(QBrush brush) { m_backgroundBrush = brush; }
  QBrush backgroundBrush() { return m_backgroundBrush; }

  void setForegroundBrush(QBrush brush) { m_foregroundBrush = brush; }
  QBrush foregroundBrush() { return m_foregroundBrush; }

  void setTextBackgroundBrush(QBrush brush) { m_textBackgroundBrush = brush; }
  QBrush textBackgroundBrush() { return m_textBackgroundBrush; }

  bool bilinearFilter() { return scaled() && m_bilinearFilter; }
  void setBilinearFilter(bool bilinearFilter) { m_bilinearFilter = bilinearFilter; }

  bool scaled() { return m_scaled; }
  void setScaled(bool scaled) { m_scaled = scaled; }

  bool keepAspect() { return m_keepAspect; }
  void setKeepAspect(bool keep) { m_keepAspect = keep; }

  bool showFps() { return m_showFps; }
  void setShowFps(bool enable) { m_showFps = enable; }

  void setDefaultMessage(QString message) { m_defaultMessage = message; }
  QString &defaultMessage() { return m_defaultMessage; }

  int currentFps() { return m_currentFps; }
  void setCurrentFps(int fps) { m_currentFps = fps; }

  int frameCounter() { return m_frameCounter; }
  void incFrameCounter() { m_frameCounter++; }
  void setFrameCounter(int counter) { m_frameCounter = counter; }

public slots:
  void setSurfaceSize(QSize surfaceSize);
  void updateSurface();

  void clearSurface();
  void frameTimerTimeout();

protected:
  bool event(QEvent* e);
  void resizeEvent(QResizeEvent *e);
  void paintEvent(QPaintEvent *);

protected slots:
  void initialConnection(void);
  void connectionLost(void);

private:
  static int translateMouseButton(Qt::MouseButton button);

private:
  QPixmap m_surfacePixmap;
  QTimer* m_frameTimer;
  QBrush m_backgroundBrush;
  QBrush m_foregroundBrush;
  QBrush m_textBackgroundBrush;
  qreal m_scale;
  qreal m_sx;
  qreal m_sy;
  QPainter m_painter;
  QRect m_surfaceRect;
  QTransform m_transform;
  bool m_bilinearFilter;
  bool m_scaled;
  bool m_keepAspect;
  bool m_showFps;
  int m_buttonMask;
  QString m_defaultMessage;
  uint m_frameCounter;
  uint m_currentFps;
  QRect m_textBoundingRect;
  QRect m_textBackgroundRect;
  QSize m_textBackgroundSize;
  QString m_fpsMessage;
};

#endif // SURFACEWIDGET_H
