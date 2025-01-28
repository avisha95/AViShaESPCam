# AViShaESPCam Library

**AViShaESPCam** adalah library untuk mempermudah penggunaan ESP32-CAM. Dengan menggunakan library ini, pengguna dapat mengakses kamera ESP32-CAM dengan mudah tanpa perlu menulis kode yang rumit. Library ini mendukung pengambilan gambar, penyimpanan ke SD card, serta konversi gambar ke format Base64.

## Fitur

- **Inisialisasi Kamera**: Menyediakan fungsi untuk mengonfigurasi dan menginisialisasi kamera ESP32-CAM dengan mudah.
- **Pengambilan Gambar**: Mendukung pengambilan gambar dari kamera.
- **Penyimpanan ke SD Card**: Gambar yang diambil dapat disimpan langsung ke kartu SD.
- **Konversi ke Base64**: Mendukung konversi gambar menjadi format Base64 untuk digunakan dalam aplikasi web atau komunikasi jaringan.

## Instalasi

1. Buka Arduino IDE.
2. Pilih **Sketch > Include Library > Manage Libraries**.
3. Cari **AViShaESPCam** dan klik **Install**.

## Penggunaan

### Contoh 1: Pengambilan Gambar

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
  delay(5000); // Tunggu 5 detik
}
```

### Contoh 2: Mengambil Gambar dan Menyimpannya ke SD Card

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
    espcam.saveToSD(frame, "foto");
    espcam.returnFrame(frame);
    Serial.println("Foto berhasil disimpan.");
  }
  delay(500);

  esp_sleep_enable_timer_wakeup(100000000); // Tidur dalam mode deep sleep selama 100 detik
  esp_deep_sleep_start();
}

void loop() {}
```

## Fungsi-fungsi Utama

### `enableLogging(bool enable)`
Mengaktifkan atau menonaktifkan log untuk debugging.

### `init(CameraConfig config, CameraResolution resolution)`
Menginisialisasi kamera dengan konfigurasi yang diberikan.

### `initSDCard()`
Menginisialisasi SD card untuk menyimpan gambar.

### `capture()`
Mengambil gambar dari kamera dan mengembalikan buffer gambar.

### `saveToSD(FrameBuffer* frame, const char* filename)`
Menyimpan gambar ke SD card dengan nama file yang ditentukan.

### `returnFrame(FrameBuffer* frame)`
Mengembalikan frame yang telah diambil ke buffer kamera.

### `frameToBase64(FrameBuffer* frame)`
Mengonversi gambar menjadi string Base64.

### `urlencode(String str)`
Mengenkripsi string menjadi format URL yang aman.

## Kompatibilitas

- ESP32
- Kamera ESP32-CAM (seperti AI-THINKER)

## Link

[GitHub Repository](https://github.com/avisha95/AViShaESPCam)
