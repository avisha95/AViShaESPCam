/* Capture Save To SD Card with Deep Sleep Example for AViSha ESPCam
 * This example captures a frame from the camera and saves it to an SD card.
 * It is designed to be used with the AI Thinker ESP32-CAM module.
 * 
 * Created by Ajang Rahmat, 2024
 * 
 * This example is in the public domain.
 *
 * For more information, visit: github.com/AViSha/AViShaESPCam

*/

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
