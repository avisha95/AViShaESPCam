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
