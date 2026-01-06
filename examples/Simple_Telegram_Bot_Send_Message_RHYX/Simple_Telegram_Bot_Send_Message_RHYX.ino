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
  cam.setPixelFormat(RGB565);
  cam.init(AI_THINKER(), R_240X240);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
  client.setCACert(telegram_cert);
  bot.setUpdateTime(1000);
  bot.setTelegramToken(token);

  if (bot.begin()) {
    bot.sendTo(userid, "RHYX Bot ready. Send /takePhoto");
  }
}

void loop() {
  TBMessage msg;

  if (bot.getNewMessage(msg) && msg.sender.id == userid) {
    if (msg.text.equalsIgnoreCase("/takePhoto")) {
      bot.sendMessage(msg, "Capturing...");

      FrameBuffer* frame = cam.capture();
      if (frame) {
        FrameBuffer* jpegFrame = cam.convertFrameToJpeg(frame);
        if (jpegFrame) {
          bot.sendPhoto(msg, jpegFrame->buf, jpegFrame->len);
          free(jpegFrame->buf);
          delete jpegFrame;
        }
        cam.returnFrame(frame);
      }
    }
  }

  delay(100);
}
