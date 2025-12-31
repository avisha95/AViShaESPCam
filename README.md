# AViShaESPCam Library

**AViShaESPCam** is a library designed to simplify the use of the ESP32-CAM module. With this library, users can easily access the ESP32-CAM camera without writing complex low-level code. It supports image capture, saving images to an SD card, and converting images to Base64 format.

## Features

* **Camera Initialization**: Simple functions to configure and initialize the ESP32-CAM.
* **Image Capture**: Capture images directly from the camera.
* **SD Card Storage**: Save captured images directly to an SD card.
* **Base64 Conversion**: Convert images to Base64 format for web applications or network communication.

## Installation

1. Open Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries**.
3. Search for **AViShaESPCam** and click **Install**.

## Usage

### Example 1: Simple Image Capture

```cpp
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
  delay(5000); // Wait 5 seconds
}
```

### Example 2: Capture Image and Save to SD Card

```cpp
#include "AViShaESPCam.h"
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.init(AI_THINKER(), VGA);
  espcam.initSDCard();

  FrameBuffer* frame = espcam.capture();
  if (frame) {
    espcam.saveToSD(frame, "photo");
    espcam.returnFrame(frame);
    Serial.println("Photo saved successfully.");
  }
  delay(500);

  esp_sleep_enable_timer_wakeup(100000000); // Deep sleep for 100 seconds
  esp_deep_sleep_start();
}

void loop() {}
```

---

## Additional Examples (RHYX Camera)

### Example 3: Save Image to SD Card with Deep Sleep (RHYX)

This example captures an image, converts it to JPEG, saves it to the SD card, then puts the ESP32 into deep sleep.

```cpp
#include "AViShaESPCam.h"
AViShaESPCam espcam;

void setup() {

  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.setPixelFormat(RGB565);
  espcam.init(AI_THINKER(), R_240X240);
  espcam.initSDCard();

  FrameBuffer* frame = espcam.capture();
  if (frame) {
    FrameBuffer* jpegFrame = espcam.convertFrameToJpeg(frame);
    if (jpegFrame) {
      espcam.saveToSD(jpegFrame, "photo");
      espcam.returnFrame(jpegFrame);
      Serial.println();
      espcam.returnFrame(jpegFrame);
    }
    espcam.returnFrame(frame);
  }

  delay(500);
  esp_sleep_enable_timer_wakeup(100000000); // 100 seconds
  esp_deep_sleep_start();
}

void loop() {}
```

### Example 4: Convert Image to Base64 (RHYX)

This example captures an image, converts it to JPEG, then outputs a Base64-encoded image string via Serial (useful for web or API usage).

```cpp
#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.setPixelFormat(RGB565);
  espcam.init(AI_THINKER(), R_240X240);
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    FrameBuffer* jpegFrame = espcam.convertFrameToJpeg(frame);
    if (jpegFrame) {

      String base64Image = "data:image/jpeg;base64,";
      base64Image = base64Image + espcam.frameToBase64(jpegFrame);
      Serial.println(base64Image);

      espcam.returnFrame(jpegFrame);
    }
    espcam.returnFrame(frame);
  }
  delay(5000);
}
```

### Example 5: Simple Capture Only (RHYX)

A minimal example that only captures and releases the frame buffer.

```cpp
#include <AViShaESPCam.h>
AViShaESPCam espcam;

void setup() {
  Serial.begin(115200);
  espcam.enableLogging(true);
  espcam.setPixelFormat(RGB565);
  espcam.init(AI_THINKER(), R_240X240);
}

void loop() {
  FrameBuffer* frame = espcam.capture();
  if (frame) {
    espcam.returnFrame(frame);
  }
  delay(5000);
}
```

---

## Main Functions

### `enableLogging(bool enable)`

Enable or disable debug logging.

### `init(CameraConfig config, CameraResolution resolution)`

Initialize the camera with the selected configuration and resolution.

### `initSDCard()`

Initialize the SD card for image storage.

### `capture()`

Capture an image and return a frame buffer.

### `saveToSD(FrameBuffer* frame, const char* filename)`

Save the image to the SD card using the specified filename.

### `returnFrame(FrameBuffer* frame)`

Release the frame buffer back to the camera driver.

### `frameToBase64(FrameBuffer* frame)`

Convert an image frame to a Base64 string.

### `urlencode(String str)`

Encode a string into a URL-safe format.

---

## Compatibility

* ESP32
* ESP32-CAM modules (e.g. AI-THINKER, RHYX)

## Link

GitHub Repository:
[https://github.com/avisha95/AViShaESPCam](https://github.com/avisha95/AViShaESPCam)