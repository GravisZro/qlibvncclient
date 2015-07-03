#include "rfbclientwapper.h"

std::map<rfbClient*, RFBClientWapper*> RFBClientWapper::s_instances;
std::map<rfbClient*, RFBClientWapper*>::iterator RFBClientWapper::s_current = RFBClientWapper::s_instances.begin();

static void s_HandleTextChatProc            (rfbClient* c, int value, char *text          ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_HandleTextChat(value, text); }
static void s_HandleXvpMsgProc              (rfbClient* c, uint8_t version, uint8_t opcode) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_HandleXvpMsg(version, opcode); }
static void s_HandleKeyboardLedStateProc    (rfbClient* c, int value, int pad             ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_HandleKeyboardLedState(value, pad); }
static rfbBool s_HandleCursorPosProc        (rfbClient* c, int x, int y                   ) { RFBClientWapper::context(c); return RFBClientWapper::obj()->do_HandleCursorPos(x, y); }
static void s_SoftCursorLockAreaProc        (rfbClient* c, int x, int y, int w, int h     ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_SoftCursorLockArea(x, y, w, h); }
static void s_SoftCursorUnlockScreenProc    (rfbClient* c                                 ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_SoftCursorUnlockScreen(); }
static void s_GotFrameBufferUpdateProc      (rfbClient* c, int x, int y, int w, int h     ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_GotFrameBufferUpdate(x, y, w, h); }
static void s_FinishedFrameBufferUpdateProc (rfbClient* c                                 ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_FinishedFrameBufferUpdate(); }
static char* s_GetPasswordProc              (rfbClient* c                                 ) { RFBClientWapper::context(c); return RFBClientWapper::obj()->do_GetPassword(); }
static rfbCredential* s_GetCredentialProc   (rfbClient* c, int credentialType             ) { RFBClientWapper::context(c); return RFBClientWapper::obj()->do_GetCredential(credentialType); }
static rfbBool s_MallocFrameBufferProc      (rfbClient* c                                 ) { RFBClientWapper::context(c); return RFBClientWapper::obj()->do_MallocFrameBuffer(); }
static void s_GotXCutTextProc               (rfbClient* c, const char *text, int textlen  ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_GotXCutText(text, textlen); }
static void s_BellProc                      (rfbClient* c                                 ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_Bell(); }

static void s_GotCursorShapeProc            (rfbClient* c, int xhot, int yhot, int width, int height, int bytesPerPixel) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_GotCursorShape(xhot, yhot, width, height, bytesPerPixel); }
static void s_GotCopyRectProc               (rfbClient* c, int src_x, int src_y, int w, int h, int dest_x, int dest_y  ) { RFBClientWapper::context(c); RFBClientWapper::obj()->do_GotCopyRect(src_x, src_y, w, h, dest_x, dest_y); }


RFBClientWapper::RFBClientWapper(rfbClient* c)
  : m_connected(false)
{
  //auto rval = s_instances.emplace(c, this);
  auto rval = s_instances.insert({c, this});
  //Q_ASSERT(rval.second);
  s_current = rval.first;

  c->HandleTextChat = s_HandleTextChatProc;
  c->HandleKeyboardLedState = s_HandleKeyboardLedStateProc;
  c->HandleCursorPos = s_HandleCursorPosProc;
  c->SoftCursorLockArea = s_SoftCursorLockAreaProc;
  c->SoftCursorUnlockScreen = s_SoftCursorUnlockScreenProc;
  c->GotFrameBufferUpdate = s_GotFrameBufferUpdateProc;
  c->GetPassword = s_GetPasswordProc;
  c->MallocFrameBuffer = s_MallocFrameBufferProc;
  c->GotXCutText = s_GotXCutTextProc;
  c->Bell = s_BellProc;
  c->GotCursorShape = s_GotCursorShapeProc;
  c->GotCopyRect = s_GotCopyRectProc;
  c->GetCredential = s_GetCredentialProc;
  c->HandleXvpMsg = s_HandleXvpMsgProc;
  c->FinishedFrameBufferUpdate = s_FinishedFrameBufferUpdateProc;

  //c->programName = QVNCVIEWER_APP_TITLE_CSTR;
  c->frameBuffer = nullptr;
  c->canHandleNewFBSize = TRUE;
  c->canUseCoRRE = TRUE;
  c->canUseHextile = TRUE;
  c->appData.requestedDepth = 0;
  c->appData.forceTrueColour = TRUE;
  c->appData.useRemoteCursor = TRUE;
  c->appData.enableJPEG = TRUE;
  c->appData.compressLevel = 9;
  c->appData.qualityLevel = 5;
  c->appData.encodingsString = "ultra";
}

RFBClientWapper::~RFBClientWapper(void)
{
  rfbDisconnect();
  s_instances.erase(s_current);
}

void RFBClientWapper::rfbConnect(const QString& hostname, int port)
{
  try
  {
    rfbClient* c = client();

    if(!ConnectToRFBServer(c, hostname.toUtf8(), port))
      throw "unable to connect";

    if(!InitialiseRFBConnection(c))
      throw "initialization failed";

    //qDebug() << "desktop name:" << c->desktopName;

    c->appData.encodingsString = "ultra";
    c->appData.qualityLevel = 5;
    c->appData.compressLevel = 9;

    if(!SetFormatAndEncodings(c))
      throw "setting encoding failed";

    c->width = c->si.framebufferWidth;
    c->height = c->si.framebufferHeight;
    c->frameBuffer = (uint8_t *)malloc(c->width * c->height * QVNCVIEWER_BYTES_PER_PIXEL);

    m_connected = true;
  }
  catch(const char* err)
  {
    //qDebug() << "error connecting:" << err;
    rfbDisconnect();
  }
}


void RFBClientWapper::rfbDisconnect(void)
{
  m_connected = false;

  //qDebug() << "disconnecting";
  rfbClient* c = client();
  if(c->sock)
  {
    ::close(c->sock);
    c->sock = 0;
  }

  if(c->frameBuffer)
  {
    ::free(c->frameBuffer);
    c->frameBuffer = nullptr;
  }

  ::rfbClientCleanup(c);
}

bool RFBClientWapper::isConnected(void)
{
  return m_connected;
}

