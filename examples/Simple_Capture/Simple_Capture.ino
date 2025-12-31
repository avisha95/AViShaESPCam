/* Simple Capture Example for AViSha ESPCam
 * This example captures a frame from the camera and returns it to the buffer.
 * It is designed to be used with the AI Thinker ESP32-CAM module.
 * 
 * Created by Ajang Rahmat, 2024
 * 
 * This example is in the public domain.
 *
 * For more information, visit: github.com/AViSha/AViShaESPCam
*/

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