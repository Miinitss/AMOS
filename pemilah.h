#ifndef PEMILAH_H
#define PEMILAH_H

#include "config.h"
#include <Arduino.h>
#include <Servo.h>

// ==========================================
// DEKLARASI STATE SYSTEM (STATE MACHINE)
// ==========================================
enum SystemState {
  STATE_IDLE,
  STATE_WAIT_TRASH,
  STATE_DISCARD_METAL,
  STATE_DISCARD_NONMETAL,
  STATE_SERVO_RETURN,
  STATE_WAIT_LEAVE
};

// ==========================================
// KONTROL PEMILAH SAMPAH (AMOS)
// ==========================================
class AMOS {
private:
  Servo myservo;
  SystemState currentState;
  unsigned long stateStartTime;
  unsigned long lastPingTime;
  unsigned long lastCycleEndTime;
  bool obstacleCleared;
  long lastMeasuredDistance;
  bool isRobotMoving;
  bool wasMoving;
  unsigned long lastMoveStopTime;

  long getDistance() {
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGGER, LOW);

    long duration = pulseIn(PIN_ECHO, HIGH, 20000);
    if (duration == 0) {
      lastMeasuredDistance = 999;
      return 999;
    }
    lastMeasuredDistance = duration * 0.034 / 2;
    return lastMeasuredDistance;
  }

public:
  AMOS() {
    currentState = STATE_IDLE;
    lastPingTime = 0;
    lastCycleEndTime = 0;
    obstacleCleared = true;
    lastMeasuredDistance = 999;
    isRobotMoving = false;
    wasMoving = false;
    lastMoveStopTime = 0;
  }

  void begin() {
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIN_PROXIMITY, INPUT_PULLUP);

    myservo.attach(PIN_SERVO);
    myservo.write(settings.servoStandby);
  }

  void reset() {
    myservo.write(settings.servoStandby);
    currentState = STATE_IDLE;
    obstacleCleared = true;
    lastCycleEndTime = 0;
    isRobotMoving = false;
    wasMoving = false;
    lastMoveStopTime = 0;
  }

  long getLastDistance() {
    if (isRobotMoving)
      return 999;
    return lastMeasuredDistance;
  }

  String getProximityStatus() {
    if (isRobotMoving) {
      return "Nonaktif (Bergerak)";
    }
    if (currentState == STATE_WAIT_LEAVE) {
      return "PLATFORM TERTUTUP";
    }
    if (digitalRead(PIN_PROXIMITY) == PROXIMITY_ACTIVE_STATE) {
      return "LOGAM";
    }
    if (currentState == STATE_WAIT_TRASH) {
      return "MENGEVALUASI SAMPAH...";
    }
    return "TIDAK ADA SAMPAH";
  }

  String getStateString() {
    switch (currentState) {
    case STATE_IDLE:
      return "SIAGA (Menunggu Sampah)";
    case STATE_WAIT_TRASH:
      return "MENGEVALUASI SAMPAH";
    case STATE_DISCARD_METAL:
      return "MEMBUANG LOGAM";
    case STATE_DISCARD_NONMETAL:
      return "MEMBUANG NON-LOGAM";
    case STATE_SERVO_RETURN:
      return "RESET PENUTUP";
    case STATE_WAIT_LEAVE:
      return "MENUNGGU PLATFORM KOSONG";
    default:
      return "STANDBY";
    }
  }

  void update(bool incomingMovingState) {
    unsigned long currentMillis = millis();

    if (incomingMovingState) {
      wasMoving = true;
    } else if (wasMoving && !incomingMovingState) {
      wasMoving = false;
      lastMoveStopTime = currentMillis;
    }
    isRobotMoving = incomingMovingState;

    if (currentMillis - lastPingTime > 150) {
      lastPingTime = currentMillis;

      if (!isRobotMoving &&
          (currentState == STATE_IDLE || currentState == STATE_SERVO_RETURN ||
           currentState == STATE_WAIT_LEAVE)) {
        long distance = getDistance();
        if (distance > 0 && distance > settings.maxDistanceCm) {
          obstacleCleared = true;
        } else {
          obstacleCleared = false;
        }
      }
    }

    switch (currentState) {
    case STATE_IDLE:
      if (!isRobotMoving && (currentMillis - lastMoveStopTime > 1500)) {
        if (!obstacleCleared && lastMeasuredDistance > 0 &&
            lastMeasuredDistance <= settings.maxDistanceCm) {
          Serial.println(
              ">> Benda terdeteksi di platform! Membuka mode evaluasi sampah.");
          currentState = STATE_WAIT_TRASH;
          stateStartTime = currentMillis;
        }
      }
      break;

    case STATE_WAIT_TRASH:
      if (digitalRead(PIN_PROXIMITY) == PROXIMITY_ACTIVE_STATE) {
        Serial.println(">> LOGAM TERDETEKSI! Servo membuang ke KIRI.");
        myservo.write(settings.servoLogam);
        currentState = STATE_DISCARD_METAL;
        stateStartTime = currentMillis;
      } else if (currentMillis - stateStartTime > settings.waitTrashTimeoutMs) {
        Serial.println(">> Waktu habis (Timeout). ASUMSI NON-LOGAM! Servo "
                       "membuang ke KANAN.");
        myservo.write(settings.servoNonlogam);
        currentState = STATE_DISCARD_NONMETAL;
        stateStartTime = currentMillis;
      }
      break;

    case STATE_DISCARD_METAL:
    case STATE_DISCARD_NONMETAL:
      if (currentMillis - stateStartTime > settings.servoDelayMs) {
        Serial.println(">> Mengembalikan servo ke posisi Lurus (Standby).");
        myservo.write(settings.servoStandby);
        currentState = STATE_SERVO_RETURN;
        stateStartTime = currentMillis;
      }
      break;

    case STATE_SERVO_RETURN:
      if (currentMillis - stateStartTime > settings.servoDelayMs) {
        if (obstacleCleared) {
          Serial.println(">> Platform kosong. Kembali ke mode IDLE.");
          currentState = STATE_IDLE;
        } else {
          Serial.println(">> Platform masih terisi/tersumbat. Mengunci hingga "
                         "dibersihkan.");
          currentState = STATE_WAIT_LEAVE;
        }
      }
      break;

    case STATE_WAIT_LEAVE:
      if (obstacleCleared) {
        Serial.println(">> Platform sudah dibersihkan. Mereset ke mode IDLE.");
        currentState = STATE_IDLE;
      }
      break;
    }
  }
};

#endif
