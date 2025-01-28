#include "AViShaESPCam.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

CameraConfig AI_THINKER() {
  CameraConfig config = {
    .xclkFreq = 20000000,
    .pixelFormat = PIXFORMAT_JPEG,
    .pinPwdn = 32,
    .pinReset = -1,
    .pinXclk = 0,
    .pinSioc = 27,
    .pinSiod = 26,
    .pinY2 = 5,
    .pinY3 = 18,
    .pinY4 = 19,
    .pinY5 = 21,
    .pinY6 = 36,
    .pinY7 = 39,
    .pinY8 = 34,
    .pinY9 = 35,
    .pinVsync = 25,
    .pinHref = 23,
    .pinPclk = 22
  };
  return config;
}

AViShaESPCam::AViShaESPCam() {
  _enableLogging = true;
}

void AViShaESPCam::enableLogging(bool enable) {
  _enableLogging = enable;
}

bool AViShaESPCam::init(CameraConfig config, CameraResolution resolution) {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  _espConfig.ledc_channel = LEDC_CHANNEL_0;
  _espConfig.ledc_timer = LEDC_TIMER_0;
  _espConfig.pin_d0 = config.pinY2;
  _espConfig.pin_d1 = config.pinY3;
  _espConfig.pin_d2 = config.pinY4;
  _espConfig.pin_d3 = config.pinY5;
  _espConfig.pin_d4 = config.pinY6;
  _espConfig.pin_d5 = config.pinY7;
  _espConfig.pin_d6 = config.pinY8;
  _espConfig.pin_d7 = config.pinY9;
  _espConfig.pin_xclk = config.pinXclk;
  _espConfig.pin_pclk = config.pinPclk;
  _espConfig.pin_vsync = config.pinVsync;
  _espConfig.pin_href = config.pinHref;
  _espConfig.pin_sscb_sda = config.pinSiod;
  _espConfig.pin_sscb_scl = config.pinSioc;
  _espConfig.pin_pwdn = config.pinPwdn;
  _espConfig.pin_reset = config.pinReset;
  _espConfig.xclk_freq_hz = config.xclkFreq;
  _espConfig.pixel_format = config.pixelFormat;
  _espConfig.frame_size = (framesize_t)resolution;

  if (psramFound()) {
    _espConfig.fb_count = 2;
    _espConfig.jpeg_quality = 10;
  } else {
    _espConfig.fb_count = 1;
    _espConfig.jpeg_quality = 12;
  }

  esp_err_t err = esp_camera_init(&_espConfig);

  if (_enableLogging) {
    if (err == ESP_OK) {
      Serial.println("[CAM]: Initialized Successfully");
    } else {
      Serial.printf("[CAM]: Init Failed With Error 0x%x\n", err);
    }
  }

  return (err == ESP_OK);
}

bool AViShaESPCam::initSDCard() {
  if (!SD_MMC.begin()) {
    if (_enableLogging) {
      Serial.println("[SD]: Mount Failed");
    }
    return false;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    if (_enableLogging) {
      Serial.println("[SD]: No SD Card attached");
    }
    return false;
  }

  if (_enableLogging) {
    Serial.println("[SD]: Initialization done.");
  }
  return true;
}

void AViShaESPCam::setResolution(CameraResolution resolution) {
  esp_camera_deinit();
  _espConfig.frame_size = (framesize_t)resolution;
  esp_camera_init(&_espConfig);

  if (_enableLogging) {
    Serial.printf("[CAM]: Resolution changed to %d\n", resolution);
  }
}

camera_fb_t* AViShaESPCam::capture() {
  camera_fb_t* frame = esp_camera_fb_get();

  if (_enableLogging) {
    if (frame) {
      Serial.printf("[CAM]: Captured. Size: %d bytes\n", frame->len);
    } else {
      Serial.println("[CAM]: Capture Failed!");
    }
  }

  return frame;
}

void AViShaESPCam::returnFrame(camera_fb_t* frame) {
  if (frame) {
    esp_camera_fb_return(frame);
    if (_enableLogging) {
      Serial.println("[CAM]: Frame returned to buffer");
    }
  }
}

bool AViShaESPCam::saveToSD(camera_fb_t* frame, const char* filename) {
  if (!frame) return false;

  EEPROM.begin(EEPROM_SIZE);
  int pictureNumber = EEPROM.read(0) + 1;

  String path = "/" + String(filename) + String(pictureNumber) + ".jpg";

  if (_enableLogging) {
    Serial.printf("[SD]: Picture file name: %s\n", path.c_str());
  }

  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    if (_enableLogging) {
      Serial.println("[SD]: Failed to open file in writing mode");
    }
    return false;
  } else {
    file.write(frame->buf, frame->len);
    if (_enableLogging) {
      Serial.printf("[SD]: Saved file to path: %s\n", path.c_str());
    }
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();
  return true;
}

String AViShaESPCam::frameToBase64(camera_fb_t* frame) {
  if (!frame) return "";

  const size_t bufferSize = 200 * 1024;
  char* outputBuffer = (char*)malloc(bufferSize);
  if (!outputBuffer) {
    if (_enableLogging) Serial.println("[BASE64]: Failed to allocate memory for Base64.");
    return "";
  }

  if (frameToBase64(frame, outputBuffer, bufferSize)) {
    String result = String(outputBuffer);
    free(outputBuffer);
    return result;
  }

  free(outputBuffer);
  if (_enableLogging) Serial.println("[BASE64]: Failed to convert frame to Base64.");
  return "";
}

// Detailed implementation for Base64 conversion
bool AViShaESPCam::frameToBase64(camera_fb_t* frame, char* output, size_t outputSize) {
  if (!frame || !output) return false;

  // Allocate temporary buffer for base64_encode
  char* tempBuffer = (char*)malloc(frame->len);
  if (!tempBuffer) {
    if (_enableLogging) Serial.println("[BASE64]: Failed to allocate memory for Base64.");
    return false;
  }

  // Copy data from frame->buf to tempBuffer
  memcpy(tempBuffer, frame->buf, frame->len);

  // Call base64_encode
  size_t encodedSize = base64_encode(output, tempBuffer, frame->len);

  // Free temporary buffer
  free(tempBuffer);

  return encodedSize < outputSize;
}

// URL Encode string
String AViShaESPCam::urlencode(String str) {
  const size_t bufferSize = 512; // Static buffer for encoding
  char buffer[bufferSize];
  if (urlencode(str.c_str(), buffer, bufferSize)) {
    return String(buffer); // Return the encoded result as a String
  } else {
    return ""; // Return empty string if encoding fails
  }
}

// Detailed implementation for URL Encode
bool AViShaESPCam::urlencode(const char* input, char* output, size_t outputSize) {
  if (!input || !output || outputSize < 1) return false;

  const char* hex = "0123456789ABCDEF";
  size_t outputIndex = 0;

  while (*input != '\0' && outputIndex < outputSize - 1) {
    char c = *input;
    if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
      if (outputIndex + 1 >= outputSize) break; // Ensure space for the character
      output[outputIndex++] = c;
    } else {
      if (outputIndex + 3 >= outputSize) break; // Ensure space for '%XX'
      output[outputIndex++] = '%';
      output[outputIndex++] = hex[(unsigned char)c >> 4];
      output[outputIndex++] = hex[c & 0x0F];
    }
    input++;
  }

  output[outputIndex] = '\0'; // Ensure null-termination
  return true;
}
