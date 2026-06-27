#include "config.h"
#include "motor.h"
#include "pemilah.h"
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include "web_ui.h"

// ==========================================
// INSTANSIASI & VARIABEL GLOBAL
// ==========================================
ESP8266WebServer server(WEBSERVER_PORT);
MotorControl motor;
AMOS sorter;
AppSettings settings;

bool isMoving = false;
bool isSystemActive = false;
unsigned long lastCmdTime = 0;


// ==========================================
// MONITORING BATERAI
// ==========================================
float getBatteryVoltage() {
  int adc = analogRead(A0);
  float voltageAtPin = (adc / 1023.0) * 3.3;
  return voltageAtPin * (BATTERY_R1 + BATTERY_R2) / BATTERY_R2;
}

int getBatteryPercentage() {
  int adc = analogRead(A0);
  if (adc < 100) {
    return -1; 
  }
  float voltageAtPin = (adc / 1023.0) * 3.3;
  float volts = voltageAtPin * (BATTERY_R1 + BATTERY_R2) / BATTERY_R2;
  int pct = (int)((volts - BATTERY_VOLTS_MIN) / (BATTERY_VOLTS_MAX - BATTERY_VOLTS_MIN) * 100.0);
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return pct;
}

// ==========================================
// HANDLER REQUEST WEB SERVER
// ==========================================
void handleRoot() { sendHtmlChunked(server); }

void handleStatus() {
  JsonDocument doc;
  doc["power"] = isSystemActive ? "ON" : "OFF";
  doc["battery"] = getBatteryPercentage();
  doc["distance"] = sorter.getLastDistance();
  doc["max_dist"] = settings.maxDistanceCm;
  doc["proximity"] = sorter.getProximityStatus();
  doc["state"] = sorter.getStateString();

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleGetConfig() {
  JsonDocument doc;
  doc["dist"] = settings.maxDistanceCm;
  doc["time"] = settings.waitTrashTimeoutMs;
  doc["s_stb"] = settings.servoStandby;
  doc["s_log"] = settings.servoLogam;
  doc["s_non"] = settings.servoNonlogam;
  doc["s_del"] = settings.servoDelayMs;
  doc["spd_l"] = settings.calibrateLeft;
  doc["spd_r"] = settings.calibrateRight;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSaveConfig() {
  if (server.hasArg("dist")) settings.maxDistanceCm = server.arg("dist").toInt();
  if (server.hasArg("time")) settings.waitTrashTimeoutMs = server.arg("time").toInt();
  if (server.hasArg("s_stb")) settings.servoStandby = server.arg("s_stb").toInt();
  if (server.hasArg("s_log")) settings.servoLogam = server.arg("s_log").toInt();
  if (server.hasArg("s_non")) settings.servoNonlogam = server.arg("s_non").toInt();
  if (server.hasArg("s_del")) settings.servoDelayMs = server.arg("s_del").toInt();
  if (server.hasArg("spd_l")) settings.calibrateLeft = server.arg("spd_l").toInt();
  if (server.hasArg("spd_r")) settings.calibrateRight = server.arg("spd_r").toInt();

  EEPROM.put(0, settings);
  EEPROM.commit();

  server.send(200, "text/plain", "OK");

  delay(500);
  ESP.restart();
}

void handleCmd() {
  if (server.hasArg("a")) {
    String action = server.arg("a");
    lastCmdTime = millis();
    
    if (action == "ON") {
      isSystemActive = true;
      Serial.println("System: ON (Aktif)");
    } else if (action == "OFF") {
      isSystemActive = false;
      motor.berhenti();
      sorter.reset();
      isMoving = false;
      Serial.println("System: OFF (Standby/Sleep)");
    } else if (isSystemActive) {
      isMoving = true;
      if (action == "F") {
        motor.maju();
        Serial.println("Motor: Maju");
      } else if (action == "B") {
        motor.mundur();
        Serial.println("Motor: Mundur");
      } else if (action == "L") {
        motor.kiri();
        Serial.println("Motor: Kiri");
      } else if (action == "R") {
        motor.kanan();
        Serial.println("Motor: Kanan");
      } else {
        motor.berhenti();
        isMoving = false;
        Serial.println("Motor: Berhenti");
      }
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// ==========================================
// FUNGSI INISIALISASI UTAMA (SETUP)
// ==========================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n\n--- Memulai Sistem SmartBin ESP ---");

  EEPROM.begin(sizeof(AppSettings));
  EEPROM.get(0, settings);
  
  if (settings.magic != 1235) {
    Serial.println("EEPROM Kosong/Corrupt! Memuat konfigurasi bawaan (default)...");
    settings.magic = 1235;
    settings.servoStandby = DEFAULT_SERVO_STANDBY;
    settings.servoLogam = DEFAULT_SERVO_LOGAM;
    settings.servoNonlogam = DEFAULT_SERVO_NONLOGAM;
    settings.maxDistanceCm = DEFAULT_MAX_DISTANCE_CM;
    settings.waitTrashTimeoutMs = DEFAULT_WAIT_TRASH_TIMEOUT;
    settings.calibrateLeft = DEFAULT_CALIBRATE_LEFT;
    settings.calibrateRight = DEFAULT_CALIBRATE_RIGHT;
    settings.servoDelayMs = DEFAULT_SERVO_RETURN_DELAY;
    EEPROM.put(0, settings);
    EEPROM.commit();
  } else {
    Serial.println("Konfigurasi dari EEPROM berhasil dimuat.");
  }

  motor.begin();
  motor.setSpeed(255);

  sorter.begin();

  Serial.print("Menyiapkan Hotspot Wi-Fi: ");
  Serial.println(WIFI_AP_SSID);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);

  Serial.print("IP Address untuk dikontrol: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/status", handleStatus); 
  server.on("/get_config", handleGetConfig);
  server.on("/save_config", handleSaveConfig);
  server.begin();
  if (MDNS.begin(MDNS_HOSTNAME)) {
    Serial.print("mDNS Responder aktif: http://");
    Serial.print(MDNS_HOSTNAME);
    Serial.println(".local");
    MDNS.addService("http", "tcp", WEBSERVER_PORT);
  }
  Serial.println("Web Server berjalan. Buka browser dan masukkan IP di atas.");
}

// ==========================================
// LOOP UTAMA JALUR EKSEKUSI (LOOP)
// ==========================================
void loop() {
  server.handleClient();
  MDNS.update();

  if (isSystemActive && isMoving && (millis() - lastCmdTime > 1000)) {
    motor.berhenti();
    isMoving = false;
    Serial.println(">> FAIL-SAFE ACTIVE: Koneksi Wi-Fi terputus! Robot berhenti otomatis.");
  }

  if (isSystemActive) {
    sorter.update(isMoving);
  }
}
