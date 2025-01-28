#ifndef AViShaESPCam_H
#define AViShaESPCam_H

#include "Arduino.h"
#include "Base64.h"
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "driver/rtc_io.h"
#include "EEPROM.h"

#define EEPROM_SIZE 1

#define FrameBuffer camera_fb_t

// Daftar resolusi yang didukung
enum CameraResolution {
  R_96X96 = FRAMESIZE_96X96,
  QQVGA = FRAMESIZE_QQVGA,
  QCIF = FRAMESIZE_QCIF,
  HQVGA = FRAMESIZE_HQVGA,
  R_240X240 = FRAMESIZE_240X240,
  QVGA = FRAMESIZE_QVGA,
  CIF = FRAMESIZE_CIF,
  HVGA = FRAMESIZE_HVGA,
  VGA = FRAMESIZE_VGA,
  SVGA = FRAMESIZE_SVGA,
  XGA = FRAMESIZE_XGA,
  HD = FRAMESIZE_HD,
  SXGA = FRAMESIZE_SXGA,
  UXGA = FRAMESIZE_UXGA,
  FHD = FRAMESIZE_FHD,
  QHD = FRAMESIZE_QHD
};

// Struktur konfigurasi kamera
struct CameraConfig {
  int xclkFreq;
  pixformat_t pixelFormat;
  int pinPwdn;
  int pinReset;
  int pinXclk;
  int pinSioc;
  int pinSiod;
  int pinY2;
  int pinY3;
  int pinY4;
  int pinY5;
  int pinY6;
  int pinY7;
  int pinY8;
  int pinY9;
  int pinVsync;
  int pinHref;
  int pinPclk;
};

CameraConfig AI_THINKER();

class AViShaESPCam {
  public:
    AViShaESPCam();
    bool init(CameraConfig config, CameraResolution resolution);
    bool initSDCard();
    void setResolution(CameraResolution resolution);
    void enableLogging(bool enable);
    camera_fb_t* capture();
    void returnFrame(camera_fb_t* frame);
    bool saveToSD(camera_fb_t* frame, const char* filename);
    String frameToBase64(camera_fb_t* frame);
    String urlencode(String str);

  private:
    bool frameToBase64(camera_fb_t* frame, char* output, size_t outputSize);
    bool urlencode(const char* input, char* output, size_t outputSize);
    camera_config_t _espConfig;
    bool _enableLogging;
};

#endif