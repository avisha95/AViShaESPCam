#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.init(AI_THINKER(), UXGA);
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    espcam.returnFrame(frame);
  }
  delay(5000);
}