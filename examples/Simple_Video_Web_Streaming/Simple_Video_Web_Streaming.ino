#include <WiFi.h>
#include <WebServer.h>
#include <AViShaESPCam.h>

const char* ssid = "wifi name";
const char* password = "wifi password";

WebServer server(80);
AViShaESPCam cam;

void setup() {
  Serial.begin(115200);

  // Initialize camera
  cam.enableLogging(true);
  cam.init(AI_THINKER(), QVGA);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.print("Camera Stream IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    WiFiClient client = server.client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    client.print(response);
    while (client.connected()) {
      FrameBuffer* frame = cam.capture();
      if (frame) {
        client.print("--frame\r\n");
        client.print("Content-Type: image/jpeg\r\n\r\n");
        client.write(frame->buf, frame->len);
        client.print("\r\n");
        cam.returnFrame(frame);
      }
      delay(1);
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
