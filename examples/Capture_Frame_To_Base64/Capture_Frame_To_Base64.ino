/* Capture Frame to Base64 Example for AViSha ESPCam
 * This example captures a frame from the camera and converts it to a Base64 string.
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
  espcam.init(AI_THINKER(), QVGA);
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {

    String base64Image = "data:image/jpeg;base64,";
    base64Image = base64Image + espcam.frameToBase64(frame);
    Serial.println(base64Image);

    espcam.returnFrame(frame);
  }
  delay(5000);
}
