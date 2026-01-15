# AViShaESPCam Library Documentation

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Installation](#installation)
* [Hardware Compatibility](#hardware-compatibility)
* [Quick Start](#quick-start)
* [Camera Configuration](#camera-configuration)
* [API Reference](#api-reference)
* [Examples](#examples)
* [Troubleshooting](#troubleshooting)
* [Performance Optimization](#performance-optimization)

## Overview

AViShaESPCam is a comprehensive library designed to simplify camera operations on ESP32-CAM modules. It provides a high-level API for camera initialization, image capture, storage, and conversion, supporting various ESP32-CAM boards including AI-Thinker.

## Features

* **Easy Camera Initialization**: Simplified setup for ESP32-CAM modules
* **Multiple Image Formats**: Support for JPEG, RGB565, YUV422, and Grayscale
* **SD Card Storage**: Direct image saving to SD card with automatic file naming
* **Base64 Conversion**: Convert images for web applications
* **URL Encoding**: Safe string encoding for web transmission
* **Format Conversion**: Convert between different pixel formats
* **PSRAM Optimization**: Automatic PSRAM detection and double buffering
* **Comprehensive Logging**: Detailed debug output for troubleshooting
* **Fresh Frame Capture**: Advanced buffer management for real-time applications

## Installation

### Method 1: Library Manager

1. Open Arduino IDE
2. Go to Sketch > Include Library > Manage Libraries
3. Search for "AViShaESPCam"
4. Click Install

### Method 2: Manual Installation

1. Download the library from the repository
2. Extract to: `Documents/Arduino/libraries/AViShaESPCam/`
3. Restart Arduino IDE

## Hardware Compatibility

* **Microcontroller**: ESP32
* **Board**: AI-Thinker ESP32-CAM
* **PSRAM**: Recommended for higher resolutions
* **Flash Size**: 4MB minimum
* **Power Supply**: 5V, 2A recommended

## Quick Start

```cpp
#include <AViShaESPCam.h>

AViShaESPCam camera;

void setup() {
  Serial.begin(115200);
  
  // Enable logging
  camera.enableLogging(true);
  
  // Initialize camera with AI-Thinker configuration
  if (!camera.init(AI_THINKER(), VGA)) {
    Serial.println("Camera initialization failed!");
    return;
  }
  
  Serial.println("Camera ready!");
}

void loop() {
  // Capture image
  camera_fb_t* frame = camera.capture();
  if (frame) {
    Serial.printf("Captured image: %d bytes\n", frame->len);
    camera.returnFrame(frame);
  }
  
  delay(5000);
}
```

## Camera Configuration

### Available Configurations

The library provides predefined configurations for common ESP32-CAM boards:

```cpp
// AI-Thinker ESP32-CAM configuration
CameraConfig config = AI_THINKER();
camera.init(config, VGA);
```

### Custom Configuration

```cpp
CameraConfig customConfig = {
  .xclkFreq = 20000000,
  .pixelFormat = PIXFORMAT_JPEG,
  .pinPwdn = 32,
  .pinReset = -1,
  .pinXclk = 0,
  .pinSioc = 27,
  .pinSiod = 26,
  // ... additional pin definitions
};
```

### Supported Resolutions

| Resolution | Enum | Dimensions | Use Case |
| ---------- | ---- | ---------- | -------- |
| QQVGA | `QQVGA` | 160x120 | Low bandwidth |
| QVGA | `QVGA` | 320x240 | Standard quality |
| VGA | `VGA` | 640x480 | Recommended |
| SVGA | `SVGA` | 800x600 | High quality |
| XGA | `XGA` | 1024x768 | Very high quality |
| HD | `HD` | 1280x720 | HD video |
| UXGA | `UXGA` | 1600x1200 | Maximum quality |

### Pixel Formats

```cpp
// JPEG format (default, recommended)
camera.setPixelFormat(JPEG);

// RGB565 format (16-bit color)
camera.setPixelFormat(RGB565);

// YUV422 format (video processing)
camera.setPixelFormat(YUV422);

// Grayscale format (8-bit)
camera.setPixelFormat(GRAYSCALE);
```

## API Reference

### Core Functions

#### `bool init(CameraConfig config, CameraResolution resolution)`

Initialize the camera with specified configuration.

* **Parameters:**
    * `config`: Camera pin configuration
    * `resolution`: Desired image resolution
* **Returns:** `true` if successful, `false` otherwise

#### `camera_fb_t* capture()`

Capture a fresh image from the camera.

* **Returns:** Pointer to frame buffer or `nullptr` if failed

#### `void returnFrame(camera_fb_t* frame)`

Return frame buffer to camera system.

* **Parameters:**
    * `frame`: Frame buffer to return

#### `void enableLogging(bool enable)`

Enable or disable debug logging.

* **Parameters:**
    * `enable`: `true` to enable, `false` to disable

### Storage Functions

#### `bool initSDCard()`

Initialize SD card for storage.

* **Returns:** `true` if successful, `false` otherwise

#### `bool saveToSD(camera_fb_t* frame, const char* filename)`

Save frame to SD card with automatic numbering.

* **Parameters:**
    * `frame`: Frame buffer to save
    * `filename`: Base filename (number will be appended)
* **Returns:** `true` if successful, `false` otherwise

### Conversion Functions

#### `String frameToBase64(camera_fb_t* frame)`

Convert frame to Base64 string for web transmission.

* **Parameters:**
    * `frame`: Frame buffer to convert
* **Returns:** Base64 encoded string

#### `FrameBuffer* convertFrameToJpeg(FrameBuffer* frame)`

Convert non-JPEG frame to JPEG format.

* **Parameters:**
    * `frame`: Source frame buffer
* **Returns:** New JPEG frame buffer or `nullptr` if failed

#### `String urlencode(String str)`

URL encode string for safe web transmission.

* **Parameters:**
    * `str`: String to encode
* **Returns:** URL encoded string

### Configuration Functions

#### `void setPixelFormat(pixformat_t format)`

Set camera pixel format.

* **Parameters:**
    * `format`: Desired pixel format (JPEG, RGB565, YUV422, GRAYSCALE)

#### `void setResolution(CameraResolution resolution)`

Change camera resolution (requires reinitialization).

* **Parameters:**
    * `resolution`: New resolution setting

## Examples

### Basic Image Capture with SD Storage

```cpp
#include <AViShaESPCam.h>

AViShaESPCam camera;

void setup() {
  Serial.begin(115200);
  camera.enableLogging(true);
  
  // Initialize camera
  if (!camera.init(AI_THINKER(), VGA)) {
    Serial.println("Camera init failed!");
    return;
  }
  
  // Initialize SD card
  if (!camera.initSDCard()) {
    Serial.println("SD card init failed!");
    return;
  }
  
  Serial.println("System ready");
}

void loop() {
  camera_fb_t* frame = camera.capture();
  if (frame) {
    if (camera.saveToSD(frame, "photo")) {
      Serial.println("Photo saved to SD card");
    }
    camera.returnFrame(frame);
  }
  
  delay(10000); // Capture every 10 seconds
}
```

### Web Server with Base64 Image Display

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <AViShaESPCam.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

WebServer server(80);
AViShaESPCam camera;

void setup() {
  Serial.begin(115200);
  
  // Initialize camera
  camera.init(AI_THINKER(), VGA);
  
  // Connect WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  
  // Setup web server
  server.on("/", []() {
    camera_fb_t* frame = camera.capture();
    if (frame) {
      String base64 = camera.frameToBase64(frame);
      String html = "<html><body>";
      html += "<img src='data:image/jpeg;base64," + base64 + "'>";
      html += "</body></html>";
      server.send(200, "text/html", html);
      camera.returnFrame(frame);
    }
  });
  
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
```

### Format Conversion Example

```cpp
#include <AViShaESPCam.h>

AViShaESPCam camera;

void setup() {
  Serial.begin(115200);
  camera.enableLogging(true);
  
  // Initialize with RGB565 format
  camera.setPixelFormat(RGB565);
  camera.init(AI_THINKER(), QVGA);
}

void loop() {
  // Capture RGB565 frame
  FrameBuffer* rgbFrame = camera.capture();
  if (rgbFrame) {
    // Convert to JPEG for storage/transmission
    FrameBuffer* jpegFrame = camera.convertFrameToJpeg(rgbFrame);
    if (jpegFrame) {
      Serial.printf("Converted RGB565 (%d bytes) to JPEG (%d bytes)\n", 
                   rgbFrame->len, jpegFrame->len);
      
      // Clean up converted frame
      free(jpegFrame->buf);
      delete jpegFrame;
    }
    camera.returnFrame(rgbFrame);
  }
  
  delay(5000);
}
```

## Troubleshooting

### Common Issues and Solutions

#### Camera Initialization Failed

```cpp
void setup() {
  Serial.begin(115200);
  
  // Check PSRAM availability
  Serial.printf("PSRAM: %s\n", psramFound() ? "Available" : "Not found");
  
  if (!camera.init(AI_THINKER(), VGA)) {
    Serial.println("Camera init failed! Check:");
    Serial.println("1. PSRAM enabled in Tools menu");
    Serial.println("2. Board: AI-Thinker ESP32-CAM selected");
    Serial.println("3. Power supply: 5V, 2A");
    ESP.restart();
  }
}
```

#### Memory Issues

* Enable PSRAM: Tools → PSRAM → "Enabled"
* Use lower resolution if PSRAM unavailable
* Return frames promptly after use

#### SD Card Problems

* Format SD card as FAT32
* Insert card before powering on
* Check connections

#### Poor Image Quality

```cpp
// Improve JPEG quality (lower number = better quality)
camera.setJPEGQuality(8);

// Use higher resolution
camera.init(AI_THINKER(), SVGA);
```

## Performance Optimization

### With PSRAM

```cpp
// Enable high resolutions
camera.init(AI_THINKER(), UXGA);  // 1600x1200
camera.setJPEGQuality(10);        // High quality
```

### Without PSRAM

```cpp
// Stick to lower resolutions
camera.init(AI_THINKER(), VGA);   // Maximum without PSRAM
camera.setJPEGQuality(15);        // Moderate quality
```

### Frame Rate Optimization

```cpp
// For faster processing
camera.setJPEGQuality(20);        // Lower quality
camera.init(AI_THINKER(), QVGA);  // Smaller resolution
delay(100);                       // Minimal delay between captures
```

### Memory Management Best Practices

```cpp
void loop() {
  camera_fb_t* frame = camera.capture();
  if (frame) {
    // Process frame immediately
    processFrame(frame);
    
    // Always return frame to prevent memory leaks
    camera.returnFrame(frame);
  }
}
```
