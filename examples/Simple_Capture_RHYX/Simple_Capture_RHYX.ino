/* Simple Capture Example for AViShaESPCam RHYX Camera
  * This example demonstrates how to capture a frame from the camera and return it.
  * It initializes the camera, captures a frame, and then returns it.
  * The captured frame can be processed or displayed as needed.
  *
  * Created by Ajang Rahmat, 2024
  * This example is in the public domain.
  *
  * For more information, visit: github.com/AViSha/AViShaESPCam
*/ 


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
    espcam.returnFrame(frame);
  }
  delay(5000);
}
