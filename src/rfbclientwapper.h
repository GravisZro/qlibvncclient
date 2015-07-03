#ifndef RFBCLIENTWAPPER_H
#define RFBCLIENTWAPPER_H

#include <QString>
#include <rfb/rfbclient.h>
#include <map>

#define QVNCVIEWER_BITS_PER_SAMPLE          8
#define QVNCVIEWER_SAMPLES_PER_PIXEL        1
#define QVNCVIEWER_BYTES_PER_PIXEL          4
#define QVNCVIEWER_VNC_BASE_PORT            5900

enum class rfbEncoding : uint32_t
{
  Raw                 = 0x00000000,
  CopyRect            = 0x00000001,
  RRE                 = 0x00000002,
  CoRRE               = 0x00000004,
  Hextile             = 0x00000005,
  Zlib                = 0x00000006,
  Tight               = 0x00000007,
  TightPng            = 0xFFFFFEFC,
  ZlibHex             = 0x00000008,
  Ultra               = 0x00000009,
  ZRLE                = 0x00000016,
  ZYWRLE              = 0x00000017,

  H264                = 0x48323634,

/* Cache & XOR-Zlib - rdv@2002 */
  Cache               = 0xFFFF0000,
  CacheEnable         = 0xFFFF0001,
  XOR_Zlib            = 0xFFFF0002,
  XORMonoColor_Zlib   = 0xFFFF0003,
  XORMultiColor_Zlib  = 0xFFFF0004,
  SolidColor          = 0xFFFF0005,
  XOREnable           = 0xFFFF0006,
  CacheZip            = 0xFFFF0007,
  SolMonoZip          = 0xFFFF0008,
  UltraZip            = 0xFFFF0009,

/* Xvp pseudo-encoding */
  Xvp                 = 0xFFFFFECB,

/*
 * Special encoding numbers:
 *   0xFFFFFD00 .. 0xFFFFFD05 -- subsampling level
 *   0xFFFFFE00 .. 0xFFFFFE64 -- fine-grained quality level (0-100 scale)
 *   0xFFFFFF00 .. 0xFFFFFF0F -- encoding-specific compression levels;
 *   0xFFFFFF10 .. 0xFFFFFF1F -- mouse cursor shape data;
 *   0xFFFFFF20 .. 0xFFFFFF2F -- various protocol extensions;
 *   0xFFFFFF30 .. 0xFFFFFFDF -- not allocated yet;
 *   0xFFFFFFE0 .. 0xFFFFFFEF -- quality level for JPEG compressor;
 *   0xFFFFFFF0 .. 0xFFFFFFFF -- cross-encoding compression levels.
 */

  FineQualityLevel0   = 0xFFFFFE00,
  FineQualityLevel100 = 0xFFFFFE64,
  Subsamp1X           = 0xFFFFFD00,
  Subsamp4X           = 0xFFFFFD01,
  Subsamp2X           = 0xFFFFFD02,
  SubsampGray         = 0xFFFFFD03,
  Subsamp8X           = 0xFFFFFD04,
  Subsamp16X          = 0xFFFFFD05,

  CompressLevel0      = 0xFFFFFF00,
  CompressLevel1      = 0xFFFFFF01,
  CompressLevel2      = 0xFFFFFF02,
  CompressLevel3      = 0xFFFFFF03,
  CompressLevel4      = 0xFFFFFF04,
  CompressLevel5      = 0xFFFFFF05,
  CompressLevel6      = 0xFFFFFF06,
  CompressLevel7      = 0xFFFFFF07,
  CompressLevel8      = 0xFFFFFF08,
  CompressLevel9      = 0xFFFFFF09,

  XCursor             = 0xFFFFFF10,
  RichCursor          = 0xFFFFFF11,
  PointerPos          = 0xFFFFFF18,

  LastRect            = 0xFFFFFF20,
  NewFBSize           = 0xFFFFFF21,

  QualityLevel0       = 0xFFFFFFE0,
  QualityLevel1       = 0xFFFFFFE1,
  QualityLevel2       = 0xFFFFFFE2,
  QualityLevel3       = 0xFFFFFFE3,
  QualityLevel4       = 0xFFFFFFE4,
  QualityLevel5       = 0xFFFFFFE5,
  QualityLevel6       = 0xFFFFFFE6,
  QualityLevel7       = 0xFFFFFFE7,
  QualityLevel8       = 0xFFFFFFE8,
  QualityLevel9       = 0xFFFFFFE9,

/* LibVNCServer additions.   We claim 0xFFFE0000 - 0xFFFE00FF */
  KeyboardLedState    = 0xFFFE0000,
  SupportedMessages   = 0xFFFE0001,
  SupportedEncodings  = 0xFFFE0002,
  ServerIdentity      = 0xFFFE0003,
};

class RFBClientWapper
{
public:
  RFBClientWapper(rfbClient* c);
  virtual ~RFBClientWapper(void);

  bool isConnected(void);
  virtual void rfbConnect(const QString& hostname, int port = QVNCVIEWER_VNC_BASE_PORT);
  virtual void rfbDisconnect(void);

  virtual void do_HandleTextChat            (int value, char *text)  { }
  virtual void do_HandleXvpMsg              (uint8_t version, uint8_t opcode)  { }
  virtual void do_HandleKeyboardLedState    (int value, int pad)  { }
  virtual rfbBool do_HandleCursorPos        (int x, int y)  { return false; }
  virtual void do_SoftCursorLockArea        (int x, int y, int w, int h)  { }
  virtual void do_SoftCursorUnlockScreen    (void)  { }
  virtual void do_GotFrameBufferUpdate      (int x, int y, int w, int h)  { }
  virtual void do_FinishedFrameBufferUpdate (void)  { }
  virtual char* do_GetPassword              (void)  { return nullptr; }
  virtual rfbCredential* do_GetCredential   (int credentialType)  { return nullptr; }
  virtual rfbBool do_MallocFrameBuffer      (void)  { return false; }
  virtual void do_GotXCutText               (const char *text, int textlen)  { }
  virtual void do_Bell                      (void)  { }
  virtual void do_GotCursorShape            (int xhot, int yhot, int width, int height, int bytesPerPixel)  { }
  virtual void do_GotCopyRect               (int src_x, int src_y, int w, int h, int dest_x, int dest_y)  { }


  static rfbClient* client(void) { return s_current->first; }
  static RFBClientWapper* obj(void) { return s_current->second; }
  static void context(rfbClient* c) { s_current = s_instances.find(c); }

private:
  bool m_connected;
  static std::map<rfbClient*, RFBClientWapper*> s_instances;
  static std::map<rfbClient*, RFBClientWapper*>::iterator s_current;
};


#endif // RFBCLIENTWAPPER_H
