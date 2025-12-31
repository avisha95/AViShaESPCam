#include "AViShaESPCam.h"
AViShaESPCam espcam;

void setup() {

  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.setPixelFormat(RGB565);
  espcam.init(AI_THINKER(), R_240X240);
  espcam.initSDCard();

  FrameBuffer* frame = espcam.capture();
  if (frame) {
    FrameBuffer* jpegFrame = espcam.convertFrameToJpeg(frame);
    if (jpegFrame) {
      espcam.saveToSD(jpegFrame, "foto");
      espcam.returnFrame(jpegFrame);
      Serial.println();
      espcam.returnFrame(jpegFrame);
    }
    espcam.returnFrame(frame);
  }

  delay(500);
  esp_sleep_enable_timer_wakeup(100000000); //100 detik
  esp_deep_sleep_start();
}

void loop() {}