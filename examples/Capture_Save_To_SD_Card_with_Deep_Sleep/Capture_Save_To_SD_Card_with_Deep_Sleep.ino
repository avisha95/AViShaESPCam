#include "AViShaESPCam.h"
AViShaESPCam espcam;

void setup() {

  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.init(AI_THINKER(), VGA);
  espcam.initSDCard();

  FrameBuffer* frame = espcam.capture();
  if (frame) {
    espcam.saveToSD(frame, "foto");
    espcam.returnFrame(frame);
    Serial.println();
  }
  delay(500);

  esp_sleep_enable_timer_wakeup(100000000); //100 detik
  esp_deep_sleep_start();
}

void loop() {}
