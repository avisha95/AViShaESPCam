#include <WiFi.h>
#include <AsyncTelegram2.h>
#include <AViShaESPCam.h>

const char* ssid = "wifi name";
const char* pass = "wifi password";
const char* token = "00000000:AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"; // your bot token here
int64_t userid = 123456789; // your telegram user id here

WiFiClientSecure client;
AsyncTelegram2 bot(client);
AViShaESPCam cam;

void setup() {
  Serial.begin(115200);

  cam.enableLogging(true);
  cam.init(AI_THINKER(), VGA);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
  client.setCACert(telegram_cert);
  bot.setUpdateTime(1000);
  bot.setTelegramToken(token);

  if (bot.begin()) {
    String msg = "ESP32-CAM Bot online.\n\n";
    msg += "Commands:\n/takePhoto - Take photo";
    bot.sendTo(userid, msg);
  }
}

void loop() {
  TBMessage msg;

  if (bot.getNewMessage(msg) && msg.sender.id == userid) {
    if (msg.text.equalsIgnoreCase("/takePhoto")) {
      bot.sendMessage(msg, "Taking photo...");
      FrameBuffer* frame = cam.capture();
      if (frame) {
        bot.sendPhoto(msg, frame->buf, frame->len);
        cam.returnFrame(frame);
      }
    }
    else {
      bot.sendMessage(msg, "Send /takePhoto to get a photo.");
    }
  }

  delay(100);
}
