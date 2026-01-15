# AViShaESPCam Library Documentation

**AViShaESPCam** is a comprehensive library designed to simplify camera operations on ESP32-CAM modules. It provides a high-level API for camera initialization, image capture, storage, and conversion, supporting various ESP32-CAM boards including AI-Thinker and RHYX.

## ✨ Features

* **Easy Camera Initialization**: Simple functions to configure and initialize ESP32-CAM modules
* **Multiple Image Formats**: Support for JPEG, RGB565, YUV422, and Grayscale formats
* **SD Card Storage**: Save captured images directly to an SD card
* **Base64 Conversion**: Convert images to Base64 format for web applications
* **URL Encoding**: URL-safe string encoding for web transmission
* **Real-time Image Processing**: Convert between different pixel formats
* **Comprehensive Logging**: Detailed debug logging for troubleshooting
* **Multiple Resolutions**: Support from 96x96 to QHD resolutions
* **PSRAM Optimization**: Automatic PSRAM detection and optimization

## 📦 Installation

### Method 1: Library Manager (Recommended)
1. Open Arduino IDE (2.x recommended)
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for **AViShaESPCam**
4. Click **Install**

### Method 2: Manual Installation
1. Download the library from [GitHub Repository](https://github.com/avisha95/AViShaESPCam)
2. Extract to: `Documents/Arduino/libraries/`
3. Restart Arduino IDE

### Board Compatibility
- **Tested with**: ESP32 Arduino Core 2.0.14
- **Board**: ESP32 Dev Module or AI-Thinker ESP32-CAM
- **PSRAM**: Recommended (enables double buffering)
- **Flash Size**: 4MB or higher recommended

## 🚀 Quick Start

### Basic Capture Example
```cpp
#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.init(AI_THINKER(), VGA);
  Serial.println("Camera ready!");
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    Serial.printf("Image captured: %d bytes\n", frame->len);
    espcam.returnFrame(frame);
  }
  delay(5000);
}
```

## 📷 Camera Configurations

### Available Camera Configurations
```cpp
// For AI-Thinker ESP32-CAM
espcam.init(AI_THINKER(), VGA);

// For custom configurations, create your own:
CameraConfig customConfig = {
  .xclkFreq = 20000000,
  .pixelFormat = PIXFORMAT_JPEG,
  .pinPwdn = 32,
  .pinReset = -1,
  .pinXclk = 0,
  // ... pin definitions
};
```

### Supported Resolutions
```cpp
// Low Resolutions (fast processing)
espcam.init(AI_THINKER(), QQVGA);    // 160x120
espcam.init(AI_THINKER(), QVGA);     // 320x240
espcam.init(AI_THINKER(), VGA);      // 640x480 (Recommended)

// Medium Resolutions
espcam.init(AI_THINKER(), SVGA);     // 800x600
espcam.init(AI_THINKER(), XGA);      // 1024x768

// High Resolutions (requires PSRAM)
espcam.init(AI_THINKER(), HD);       // 1280x720
espcam.init(AI_THINKER(), SXGA);     // 1280x1024
espcam.init(AI_THINKER(), UXGA);     // 1600x1200

// Square Formats (cropped from center)
espcam.init(AI_THINKER(), R_240X240); // 240x240 (for RHYX)
espcam.init(AI_THINKER(), R_96X96);   // 96x96
```

### Pixel Formats
```cpp
espcam.setPixelFormat(JPEG);        // JPEG compression (default)
espcam.setPixelFormat(RGB565);      // 16-bit RGB
espcam.setPixelFormat(YUV422);      // YUV 4:2:2
espcam.setPixelFormat(GRAYSCALE);   // 8-bit Grayscale
```

## 📋 Complete Examples

### Example 1: Basic Camera with SD Card
```cpp
#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  
  // Initialize camera
  if (!espcam.init(AI_THINKER(), VGA)) {
    Serial.println("Camera init failed!");
    ESP.restart();
  }
  
  // Initialize SD card
  if (espcam.initSDCard()) {
    Serial.println("SD card ready");
  }
  
  Serial.println("System ready");
}

void loop() {
  // Capture and save to SD
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    if (espcam.saveToSD(frame, "image")) {
      Serial.println("Saved to SD card");
    }
    espcam.returnFrame(frame);
  }
  delay(10000); // Capture every 10 seconds
}
```

### Example 2: RHYX Camera with RGB565
```cpp
#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  
  // RHYX specific settings
  espcam.setPixelFormat(RGB565);          // RGB565 format
  espcam.init(AI_THINKER(), R_240X240);   // 240x240 resolution
  
  Serial.println("RHYX Camera ready");
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    // Convert RGB565 to JPEG for transmission
    FrameBuffer* jpegFrame = espcam.convertFrameToJpeg(frame);
    if (jpegFrame) {
      // Convert to Base64 for web display
      String base64Image = "data:image/jpeg;base64,";
      base64Image += espcam.frameToBase64(jpegFrame);
      
      // Send via Serial (for web interface)
      Serial.println(base64Image);
      
      // Clean up converted frame
      free(jpegFrame->buf);
      delete jpegFrame;
    }
    espcam.returnFrame(frame);
  }
  delay(5000);
}
```

### Example 3: Telegram Bot Integration
```cpp
#include <WiFi.h>
#include <AsyncTelegram2.h>
#include <AViShaESPCam.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* pass = "YOUR_WIFI_PASSWORD";
const char* token = "YOUR_BOT_TOKEN";
int64_t userid = YOUR_USER_ID;

WiFiClientSecure client;
AsyncTelegram2 bot(client);
AViShaESPCam cam;

void setup() {
  Serial.begin(115200);
  
  // Camera initialization
  cam.enableLogging(true);
  cam.init(AI_THINKER(), VGA);
  
  // WiFi connection
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  // Telegram setup
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
  client.setCACert(telegram_cert);
  bot.setUpdateTime(1000);
  bot.setTelegramToken(token);
  
  if (bot.begin()) {
    bot.sendTo(userid, "ESP32-CAM Bot is online!");
  }
}

void loop() {
  TBMessage msg;
  
  if (bot.getNewMessage(msg) && msg.sender.id == userid) {
    if (msg.text.equalsIgnoreCase("/photo")) {
      bot.sendMessage(msg, "Taking photo...");
      
      FrameBuffer* frame = cam.capture();
      if (frame) {
        bot.sendPhoto(msg, frame->buf, frame->len);
        cam.returnFrame(frame);
      }
    }
  }
  delay(100);
}
```

### Example 4: Web Server with Live Stream
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <AViShaESPCam.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);
AViShaESPCam cam;

void setup() {
  Serial.begin(115200);
  
  // Initialize camera
  cam.enableLogging(true);
  cam.init(AI_THINKER(), VGA);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  Serial.print("Camera Stream IP: ");
  Serial.println(WiFi.localIP());
  
  // Web server routes
  server.on("/", HTTP_GET, []() {
    String html = "<html><body>";
    html += "<h1>ESP32-CAM Stream</h1>";
    html += "<img src='/stream' style='width:100%;'>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });
  
  server.on("/stream", HTTP_GET, []() {
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
      delay(100);
    }
  });
  
  server.begin();
}

void loop() {
  server.handleClient();
}
```

### Example 5: Motion Detection with PIR Sensor
```cpp
#include <AViShaESPCam.h>
AViShaESPCam cam;

#define PIR_PIN 12
#define LED_PIN 4

unsigned long lastCapture = 0;
unsigned long captureDelay = 10000;

void setup() {
  Serial.begin(115200);
  
  // Initialize PIR sensor
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize camera
  cam.enableLogging(true);
  cam.init(AI_THINKER(), VGA);
  
  Serial.println("Motion detection system ready");
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    if (millis() - lastCapture > captureDelay) {
      Serial.println("Motion detected! Taking photo...");
      
      // Flash LED
      digitalWrite(LED_PIN, HIGH);
      FrameBuffer* frame = cam.capture();
      digitalWrite(LED_PIN, LOW);
      
      if (frame) {
        // Save to SD card
        cam.saveToSD(frame, "motion");
        cam.returnFrame(frame);
        
        Serial.println("Photo saved");
        lastCapture = millis();
      }
    }
  }
  delay(100);
}
```

## 🔧 API Reference

### Core Functions
```cpp
// Initialize camera with specified configuration
bool init(CameraConfig config, CameraResolution resolution);

// Capture image
camera_fb_t* capture();

// Return frame buffer to camera
void returnFrame(camera_fb_t* frame);

// Enable/disable debug logging
void enableLogging(bool enable);
```

### Storage Functions
```cpp
// Initialize SD card
bool initSDCard();

// Save frame to SD card
bool saveToSD(camera_fb_t* frame, const char* filename);
```

### Conversion Functions
```cpp
// Convert frame to Base64 string
String frameToBase64(camera_fb_t* frame);

// Convert between pixel formats
FrameBuffer* convertFrameToJpeg(FrameBuffer* frame);

// URL encode string
String urlencode(String str);
```

### Configuration Functions
```cpp
// Set pixel format
void setPixelFormat(pixformat_t format);

// Set JPEG quality (0-63, lower = better)
bool setJPEGQuality(int quality);

// Change resolution dynamically
bool setFrameSize(CameraResolution resolution);

// Check camera status
bool isCameraInitialized();
```

## 🛠️ Troubleshooting

### Common Issues

1. **Camera initialization failed**
   - Check PSRAM settings (Tools → PSRAM: Enabled)
   - Ensure correct board selection (AI-Thinker ESP32-CAM)
   - Verify power supply (5V, 2A recommended)

2. **Images not saving to SD card**
   - Format SD card as FAT32
   - Insert SD card before powering on
   - Check SD card wiring

3. **Low quality images**
   - Enable PSRAM for better quality
   - Adjust JPEG quality: `cam.setJPEGQuality(10)` (lower = better)
   - Increase resolution: VGA or higher

4. **Memory errors**
   - Reduce resolution if no PSRAM
   - Enable PSRAM in Arduino IDE
   - Close other applications to free memory

### Debugging Tips
```cpp
void setup() {
  Serial.begin(115200);
  
  // Enable detailed logging
  cam.enableLogging(true);
  
  // Check PSRAM
  Serial.printf("PSRAM: %s\n", psramFound() ? "Available" : "Not found");
  
  // Initialize with error checking
  if (!cam.init(AI_THINKER(), VGA)) {
    Serial.println("Camera init failed! Check:");
    Serial.println("1. PSRAM enabled");
    Serial.println("2. Correct board selected");
    Serial.println("3. Adequate power supply");
    ESP.restart();
  }
}
```

## 📊 Performance Tips

### With PSRAM
```cpp
// Enable PSRAM in Arduino IDE first
// Tools → PSRAM: "Enabled"

// Then use high-resolution modes
cam.init(AI_THINKER(), UXGA);  // 1600x1200
cam.init(AI_THINKER(), FHD);   // 1920x1080 (cropped)
```

### Without PSRAM
```cpp
// Stick to lower resolutions
cam.init(AI_THINKER(), VGA);   // 640x480 (maximum)
cam.init(AI_THINKER(), QVGA);  // 320x240 (recommended)
```

### Optimizing Frame Rate
```cpp
// Lower quality for faster processing
cam.setJPEGQuality(20);

// Use smaller resolutions
cam.init(AI_THINKER(), QVGA);

// Reduce delay between captures
delay(100);  // Instead of 5000
```

## 🔗 Additional Resources

- **GitHub Repository**: [https://github.com/avisha95/AViShaESPCam](https://github.com/avisha95/AViShaESPCam)
- **ESP32-CAM Pinout**: [AI-Thinker ESP32-CAM Pinout Guide](https://randomnerdtutorials.com/esp32-cam-pinout-gpios/)
- **Arduino ESP32 Core**: [ESP32 Arduino Core GitHub](https://github.com/espressif/arduino-esp32)
- **Telegram Bot API**: [Telegram Bot API Documentation](https://core.telegram.org/bots/api)


## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

---

**Note**: This library is tested with ESP32 Arduino Core 2.0.14. For other versions, check compatibility in the GitHub repository.
