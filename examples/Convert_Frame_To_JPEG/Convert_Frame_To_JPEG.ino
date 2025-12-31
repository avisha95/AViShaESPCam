#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.setPixelFormat(RGB565);
  espcam.init(AI_THINKER(), R_240X240);
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    FrameBuffer* jpegFrame = espcam.convertFrameToJpeg(frame);
    if (jpegFrame) {
      espcam.returnFrame(jpegFrame);
    }
    espcam.returnFrame(frame);
  }
  delay(5000);
}
