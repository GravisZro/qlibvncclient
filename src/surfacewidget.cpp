#include "surfacewidget.h"

#include "qt2keysum.h"


std::map<rfbClient*, rfbExtraClientData> QRFBClient::s_data;

QRFBClient::QRFBClient(QWidget* parent)
  : QWidget(parent),
    RFBClientWapper(rfbGetClient(QVNCVIEWER_BITS_PER_SAMPLE, QVNCVIEWER_SAMPLES_PER_PIXEL, QVNCVIEWER_BYTES_PER_PIXEL)),
    m_client(client()),
    m_connected(false)
{
  data();

  m_timer = new QTimer(this);
  m_timer->setTimerType(Qt::CoarseTimer);
  m_timer->setInterval(10);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(getMessage()));
  connect(this, SIGNAL(connected()), m_timer, SLOT(start()));
  connect(this, SIGNAL(disconnected()), m_timer, SLOT(stop()));
}

QRFBClient::~QRFBClient(void)
{
  if(client()->frameBuffer)
    free(client()->frameBuffer);
  s_data.erase(client());
}

void QRFBClient::getMessage(void)
{
  if(WaitForMessage(client(), 0))
  {
    if(HandleRFBServerMessage(client()))
    {
      QTimer::singleShot(QVNCVIEWER_CONNPEND_TIMEOUT, Qt::CoarseTimer, this, SLOT(requestUpdate()));
    }
    else
    {
    }
  }
}

void QRFBClient::requestUpdate(void)
{
  client()->updateRect.x = client()->updateRect.y = 0;
  client()->updateRect.w = client()->width;
  client()->updateRect.h = client()->height;
  ::SendIncrementalFramebufferUpdateRequest(client());
}

void QRFBClient::rfbConnect(const QString& hostname, int port)
{
  m_connected = RFBClientWapper::rfbConnect(hostname.toUtf8(), port);
  if(m_connected)
    emit connected();
}

void QRFBClient::rfbDisconnect(void)
{
  RFBClientWapper::rfbDisconnect();
  m_connected = false;
  emit disconnected();
}

rfbBool QRFBClient::do_MallocFrameBuffer(void)
{
  if(client()->frameBuffer)
    free(client()->frameBuffer);

  client()->frameBuffer = (uint8_t *)malloc(client()->width * client()->height * QVNCVIEWER_BYTES_PER_PIXEL);

  emit framebufferResize(QSize(client()->width, client()->height));
  return true;
}

void QRFBClient::do_GotFrameBufferUpdate(int x, int y, int w, int h)
{
  QImage image = QImage(w, h, QImage::Format_ARGB32);
  for (int xx = x; xx < x + w; xx++) {
    for (int yy = y; yy < y + h; yy++) {
      qint32 pos = (yy * client()->width * QVNCVIEWER_BYTES_PER_PIXEL) + (xx * QVNCVIEWER_BYTES_PER_PIXEL);
      image.setPixel(xx - x, yy - y, qRgb((quint8)client()->frameBuffer[pos + 0], (quint8)client()->frameBuffer[pos + 1], (quint8)client()->frameBuffer[pos + 2]));
    }
  }

  data().append({ QRect(x, y, w, h), QPixmap::fromImage(image)});
  emit framebufferUpdate();
}

SurfaceWidget::SurfaceWidget(QWidget *parent)
  : QRFBClient(parent),
    m_surfacePixmap(-1, -1),
    m_scale(1.0),
    m_sx(1.0),
    m_sy(1.0),
    m_bilinearFilter(true),
    m_scaled(true),
    m_keepAspect(true),
    m_showFps(false),
    m_buttonMask(0),
    m_frameCounter(0),
    m_currentFps(0)
{
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  setBackgroundBrush(Qt::black);
  setForegroundBrush(Qt::white);
  setTextBackgroundBrush(QBrush(QColor(0, 0, 0, 128), Qt::SolidPattern));
  setDefaultMessage(tr("Disconnected"));
  setScaled(true);
  setBilinearFilter(true);
  setKeepAspect(true);

  QFont f = font();
  f.setBold(true);
  f.setPointSize(f.pointSize() * 2);
  setFont(f);

  connect(this, SIGNAL(connected()), this, SLOT(initialConnection()));
  connect(this, SIGNAL(framebufferResize(QSize)), this, SLOT(setSurfaceSize(QSize)));
  connect(this, SIGNAL(framebufferUpdate()), this, SLOT(updateSurface()));

  QTimer::singleShot(QVNCVIEWER_ONE_SECOND, this, SLOT(frameTimerTimeout()));
}

SurfaceWidget::~SurfaceWidget()
{
}

void SurfaceWidget::initialConnection(void)
{
  QPixmap pixmap(1, 1);
  pixmap.fill(Qt::transparent);
  setCursor(QCursor(pixmap));

  setSurfaceSize({client()->width, client()->height});
  client()->updateRect.x = client()->updateRect.y = 0;
  client()->updateRect.w = client()->width;
  client()->updateRect.h = client()->height;
  SendIncrementalFramebufferUpdateRequest(client());
}

void SurfaceWidget::setSurfaceSize(QSize surfaceSize)
{
  m_surfacePixmap = QPixmap(surfaceSize);
  m_surfacePixmap.fill(backgroundBrush().color());
  m_surfaceRect = m_surfacePixmap.rect();
  m_surfaceRect.setWidth(m_surfaceRect.width() * m_scale);
  m_surfaceRect.setHeight(m_surfaceRect.height() * m_scale);
  m_transform = QTransform::fromScale(1.0 / m_scale, 1.0 / m_scale);
  QTimer::singleShot(0, this, SLOT(updateSurface()));
}

int SurfaceWidget::translateMouseButton(Qt::MouseButton button)
{
  switch(button)
  {
    case Qt::LeftButton:   return rfbButton1Mask;
    case Qt::MiddleButton: return rfbButton2Mask;
    case Qt::RightButton:  return rfbButton3Mask;
    default:               return 0;
  }
}

void SurfaceWidget::updateSurface()
{
  resizeEvent(0);
  update();
}

void SurfaceWidget::clearSurface()
{
  if(m_surfacePixmap.isNull())
    setSurfaceSize({client()->width, client()->height});
  else
    setSurfaceSize(m_surfacePixmap.size());

  setCurrentFps(0);
  setFrameCounter(0);
}

void SurfaceWidget::frameTimerTimeout()
{
  setCurrentFps(frameCounter());
  setFrameCounter(0);
  QTimer::singleShot(QVNCVIEWER_ONE_SECOND, this, SLOT(frameTimerTimeout()));
}

bool SurfaceWidget::event(QEvent* e)
{
  if(isConnected())
  {
    switch ( e->type() )
    {
      case QEvent::MouseButtonPress:
      case QEvent::MouseButtonRelease:
      case QEvent::MouseButtonDblClick:
        m_surfaceRect.moveCenter(rect().center()); // fall through!
      case QEvent::MouseMove:
      {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        QPoint mappedPos = m_transform.map(mouseEvent->pos());
        int button = translateMouseButton(mouseEvent->button());

        switch ( e->type() )
        {
          case QEvent::MouseButtonPress:
            m_buttonMask |= button;
            break;

          case QEvent::MouseButtonRelease:
            m_buttonMask &= ~button;
            break;

          case QEvent::MouseButtonDblClick:
            SendPointerEvent(client(), mappedPos.x(), mappedPos.y(), m_buttonMask | button);
            SendPointerEvent(client(), mappedPos.x(), mappedPos.y(), m_buttonMask);
            SendPointerEvent(client(), mappedPos.x(), mappedPos.y(), m_buttonMask | button);
            break;

          default:
            break;
        }

        SendPointerEvent(client(), mappedPos.x(), mappedPos.y(), m_buttonMask);
        break;
      }

      case QEvent::KeyPress:
      case QEvent::KeyRelease:
      {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        SendKeyEvent(client(), qt2keysym(keyEvent->key()), false);
        if ( keyEvent->key() == Qt::Key_Alt )
          setFocus();
        return true; // prevent futher processing of event
      }

      default:
        break;
    }
  }

  return QWidget::event(e);
}

void SurfaceWidget::resizeEvent(QResizeEvent *e)
{
  if (!m_surfacePixmap.isNull())
  {
    m_sx = m_sy = m_scale = 1.0;

    if ( scaled())
    {
      if ( keepAspect() )
      {
        if ( m_surfacePixmap.width() > m_surfacePixmap.height() ) {
          m_scale = (qreal)width() / (qreal)m_surfacePixmap.width();
          if ( m_surfacePixmap.height() * m_scale > height() )
            m_scale = (qreal)height()  / (qreal)m_surfacePixmap.height();
        } else {
          m_scale = (qreal)height() / (qreal)m_surfacePixmap.height();
          if ( m_surfacePixmap.width() * m_scale > width() )
            m_scale = (qreal)width() / (qreal)m_surfacePixmap.width();
        }
        m_sx = m_sy = m_scale;
      }
      else
      {
        m_sx = (qreal)width() / (qreal)m_surfacePixmap.width();
        m_sy = (qreal)height() / (qreal)m_surfacePixmap.height();
      }
    }

    m_surfaceRect = m_surfacePixmap.rect();
    m_surfaceRect.setWidth(m_surfaceRect.width() * m_sx);
    m_surfaceRect.setHeight(m_surfaceRect.height() * m_sy);
    m_surfaceRect.moveCenter(rect().center());
    m_transform = QTransform::fromScale(1.0 / m_sx, 1.0 / m_sy);
    m_transform.translate(-m_surfaceRect.x(), -m_surfaceRect.y());
  }
  if ( e )
    QWidget::resizeEvent(e);
}

void SurfaceWidget::paintEvent(QPaintEvent *)
{
  if(isConnected())
  {
    m_painter.begin(&m_surfacePixmap);
    for(auto& client_data : QRFBClient::data())
      m_painter.drawPixmap(client_data.rect.topLeft(), client_data.pixmap);
    QRFBClient::data().clear();
    m_painter.end();

    m_painter.begin(this);
    if ( bilinearFilter() )
      m_painter.setRenderHints(QPainter::SmoothPixmapTransform);
    m_painter.fillRect(rect(), m_backgroundBrush);
    if ( scaled() ) {
      if ( keepAspect() ) {
        m_surfaceRect.moveCenter(rect().center());
        m_painter.scale(m_scale, m_scale);
        m_painter.drawPixmap(m_surfaceRect.x() / m_scale, m_surfaceRect.y() / m_scale, m_surfacePixmap);
      } else {
        m_painter.scale(m_sx, m_sy);
        m_painter.drawPixmap(0, 0, m_surfacePixmap);
      }
    } else {
      m_painter.scale(1.0, 1.0);
      m_painter.drawPixmap((width() - m_surfacePixmap.width()) / 2, (height() - m_surfacePixmap.height()) / 2, m_surfacePixmap);
    }
    m_painter.end();
    if ( showFps() ) {
      QPainterPath pp;
      m_painter.begin(this);
      m_painter.setPen(m_foregroundBrush.color());
      m_painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
      m_painter.setFont(font());
      m_fpsMessage = tr("FPS: %1").arg(currentFps());
      m_textBoundingRect = m_painter.boundingRect(rect(), Qt::AlignHCenter | Qt::AlignVCenter, m_fpsMessage);
      m_textBackgroundSize = m_textBoundingRect.size();
      m_textBackgroundSize.scale(m_textBoundingRect.width() + 5, m_textBoundingRect.height() + 5, Qt::KeepAspectRatioByExpanding);
      m_textBackgroundRect = m_textBoundingRect;
      m_textBackgroundRect.setSize(m_textBackgroundSize);
      m_textBackgroundRect.moveBottom(rect().bottom());
      pp.addRoundedRect(m_textBackgroundRect, 5, 5);
      m_textBoundingRect.moveCenter(m_textBackgroundRect.center());
      m_painter.fillPath(pp, textBackgroundBrush());
      m_painter.drawText(m_textBoundingRect, m_fpsMessage);
      m_painter.end();
    }
  }
  else
  {
    m_painter.begin(this);
    m_painter.fillRect(rect(), backgroundBrush());
    m_painter.setPen(foregroundBrush().color());
    m_painter.setFont(font());
    m_textBoundingRect = m_painter.boundingRect(rect(), Qt::AlignHCenter | Qt::AlignVCenter, defaultMessage());
    m_textBoundingRect.moveCenter(rect().center());
    m_painter.drawText(m_textBoundingRect, defaultMessage());
    m_painter.end();
  }
  incFrameCounter();
}
