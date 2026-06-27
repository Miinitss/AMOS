#ifndef CONFIG_H
#define CONFIG_H

// ==========================================
// KONFIGURASI PIN ESP8266 (NodeMCU)
// ==========================================

// Pin Motor DC (L298N)
#define PIN_MOTOR_IN1 D1
#define PIN_MOTOR_IN2 D2
#define PIN_MOTOR_IN3 D5
#define PIN_MOTOR_IN4 D6

// Pin Sensor Proximity
#define PIN_PROXIMITY D3

// Pin Servo
#define PIN_SERVO D4

// Pin Sensor Ultrasonik
#define PIN_TRIGGER D8
#define PIN_ECHO D7

// ==========================================
// STRUKTUR DATA KONFIGURASI DINAMIS (EEPROM)
// ==========================================
struct AppSettings {
  int magic;
  int servoStandby;
  int servoLogam;
  int servoNonlogam;
  int maxDistanceCm;
  int waitTrashTimeoutMs;
  int calibrateLeft;
  int calibrateRight;
  int servoDelayMs;
};

extern AppSettings settings;

// ==========================================
// NILAI DEFAULT KONFIGURASI
// ==========================================
#define DEFAULT_SERVO_STANDBY 90
#define DEFAULT_SERVO_LOGAM 45
#define DEFAULT_SERVO_NONLOGAM 135
#define DEFAULT_MAX_DISTANCE_CM 10
#define DEFAULT_WAIT_TRASH_TIMEOUT 5000
#define DEFAULT_CALIBRATE_LEFT 255
#define DEFAULT_CALIBRATE_RIGHT 255
#define DEFAULT_SERVO_RETURN_DELAY 2000

// ==========================================
// KONFIGURASI LOGIKA SENSOR STATIS
// ==========================================
#define PROXIMITY_ACTIVE_STATE LOW

// ==========================================
// KALIBRASI BATERAI (JALUR ANALOG A0)
// ==========================================
#define BATTERY_R1 100.0
#define BATTERY_R2 47.0
#define BATTERY_VOLTS_MIN 6.0
#define BATTERY_VOLTS_MAX 8.4

// ==========================================
// KONFIGURASI WI-FI & WEB SERVER (RC MODE)
// ==========================================
#define WIFI_AP_SSID "AMOS RC"
#define WIFI_AP_PASS "Kelompok 3"
#define WEBSERVER_PORT 80
#define MDNS_HOSTNAME "amos"

#endif