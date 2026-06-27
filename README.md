# AMOS — Automated Metal Object Sorter

AMOS adalah sistem pintar pemilah sampah logam dan non-logam otomatis berbasis IoT yang ditenagai oleh mikrokontroler **ESP8266 (NodeMCU)**. Alat ini dilengkapi dengan antarmuka web kustom (Web UI) responsif untuk mengontrol pergerakan robot secara nirkabel (RC Mode), memantau sensor secara *real-time*, dan mengalibrasi parameter hardware secara dinamis tanpa perlu melakukan pemrograman ulang (*flashing*).

---

## ✨ Fitur Utama

1. **🧠 Smart Hybrid Mode**
   - **Deteksi Objek Presisi**
      Sensor Ultrasonik dipasang di dalam platform pemilahan untuk mendeteksi sampah (default jarak deteksi pemantauan <= 10 cm).
   - **Evaluasi Polling Pintar**
      Ketika sampah dideteksi oleh Ultrasonik, sistem akan memulai waktu tunggu (Timer Evaluasi). Jika sensor Proximity mendeteksi logam sebelum waktu habis, servo akan langsung membuang sampah ke jalur **Logam**. Jika waktu habis tanpa deteksi logam, sampah dianggap **Non-Logam** dan dibuang ke jalur sebaliknya.
   - **Proteksi Platform Tersumbat**
      Setelah proses pembuangan selesai, sistem mendeteksi platform sekali lagi. Jika sampah masih terdeteksi (karena tersumbat/nyangkut), robot masuk ke status aman `PLATFORM TERTUTUP` untuk melindungi servo dari eror pergerakan berulang.
   - **Abaikan US saat Membuang**
      Sensor Ultrasonik diabaikan secara otomatis ketika pintu servo sedang terbuka guna menghindari deteksi palsu akibat gerakan daun pintu penutup.
   - **Nonaktif saat Berjalan**
      Seluruh aktivitas sensor pembacaan sampah dinonaktifkan otomatis ketika robot dalam mode bergerak (RC Mode) agar hemat daya.

2. **⚙️ Kalibrasi Dinamis (EEPROM)**
   - Mengubah parameter sensitivitas secara instan dari menu Settings pada Web UI:
     - Jarak Pemantauan objek (cm).
     - Timer Evaluasi klasifikasi sampah (ms).
     - Kalibrasi sudut servo (Standby, Logam, Non-logam dalam derajat).
     - Delay servo membuka penutup sebelum kembali ke posisi awal (ms).
     - Kalibrasi kecepatan PWM motor roda kiri & kanan.
   - Seluruh nilai konfigurasi disimpan secara permanen di memori fisik EEPROM dan modul otomatis melakukan restart mandiri untuk menerapkan perubahan.

3. **🚀 Chunked Web Streaming & Optimalisasi RAM**
   - Halaman web dipecah secara modular menjadi file `web_ui.h`, `ui_css.h`, dan `ui_js.h` untuk mempermudah pemeliharaan kode.
   - Pengiriman data menggunakan metode Chunked Streaming (`server.sendContent_P`) langsung dari memori kilat (Flash Memory/PROGMEM). Hal ini membuat beban memori RAM aktif tetap minim (~0 byte terpakai) dan menghindari risiko crash/restart akibat fragmentasi memori (heap fragmentation).
   - Menggunakan pustaka ArduinoJson (v7.x.x) untuk pertukaran status JSON yang kokoh.

4. **🌐 Akses Web Tanpa IP (mDNS)**
   - Mendukung pencarian nama perangkat lokal. Cukup hubungkan ke hotspot robot dan ketik `http://amos.local` pada browser HP/Laptop untuk masuk ke antarmuka kontrol.

---

## 🔌 Skema Pin Out (NodeMCU ESP8266)

| Komponen | Pin Hardware | Keterangan |
| :--- | :--- | :--- |
| **Motor L298N IN1** | `D1` / GPIO5 | Kontrol Arah Motor Kiri (A) |
| **Motor L298N IN2** | `D2` / GPIO4 | Kontrol Arah Motor Kiri (B) |
| **Motor L298N IN3** | `D5` / GPIO14 | Kontrol Arah Motor Kanan (A) |
| **Motor L298N IN4** | `D6` / GPIO12 | Kontrol Arah Motor Kanan (B) |
| **Sensor Proximity** | `D3` / GPIO0 | Input Sensor Logam (NPN NO) |
| **Servo SG90** | `D4` / GPIO2 | Sinyal PWM Kontrol Servo |
| **Ultrasonic Trig** | `D8` / GPIO15 | Output Sinyal Trigger Sensor Jarak |
| **Ultrasonic Echo** | `D7` / GPIO13 | Input Sinyal Pantulan Gema |

---

## 📸 Dokumentasi Proyek
### 🤖 Fisik Robot AMOS
| Tampak Depan | Tampak Belakang |
| :---: | :---: |
| ![Tampak Depan](docs/images/robot_depan.jpg) | ![Tampak Belakang](docs/images/robot_belakang.jpg) |

### 🌐 Antarmuka Kontrol (Web UI)
| Control Hub (Power OFF) | Control Hub (Power ON) | Menu Konfigurasi Robot |
| :---: | :---: | :---: |
| ![UI Power Off](docs/images/ui_power_off.jpg) | ![UI Power On](docs/images/ui_power_on.jpg) | ![UI Settings](docs/images/ui_settings.jpg) |

---

## 🛠️ Prasyarat Instalasi

1. **Board Manager ESP8266**
   Pastikan paket board ESP8266 telah terinstal di Arduino IDE
2. **Library external**
   - **ArduinoJson** (Versi 7.x.x)

---

## 📂 Struktur Repositori

```text
AMOS/
├── AMOS.ino         # Berkas program utama (.ino)
├── config.h         # Definisi pin hardware dan konfigurasi statis
├── motor.h          # Kelas pengendali pergerakan DC motor roda
├── pemilah.h        # Kelas mesin logika pemilahan (Finite State Machine)
├── web_ui.h         # Handler server & fungsi streaming chunked HTML
├── ui_css.h         # Desain visual halaman web (CSS PROGMEM)
└── ui_js.h          # Logika interaksi & kontrol tombol web (JS PROGMEM)
```

---

## 📝 Lisensi
Proyek ini didistribusikan di bawah lisensi **MIT**. Silakan digunakan dan dikembangkan kembali secara bebas untuk keperluan edukasi dan penelitian.