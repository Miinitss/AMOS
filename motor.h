#ifndef MOTOR_H
#define MOTOR_H

#include "config.h"
#include <Arduino.h>

// ==========================================
// KONTROL MOTOR DC (L298N)
// ==========================================
class MotorControl {
private:
  int currentSpeed;

public:
  MotorControl() {
    currentSpeed = 255;
  }

  void begin() {
    pinMode(PIN_MOTOR_IN1, OUTPUT);
    pinMode(PIN_MOTOR_IN2, OUTPUT);
    pinMode(PIN_MOTOR_IN3, OUTPUT);
    pinMode(PIN_MOTOR_IN4, OUTPUT);
    berhenti();
  }

  void setSpeed(int speed) {
    if (speed < 0) speed = 0;
    if (speed > 255) speed = 255;
    currentSpeed = speed;
  }

  void maju() {
    int speedLeft = (currentSpeed * settings.calibrateLeft) / 255;
    int speedRight = (currentSpeed * settings.calibrateRight) / 255;
    
    analogWrite(PIN_MOTOR_IN1, speedLeft);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    analogWrite(PIN_MOTOR_IN3, speedRight);
    digitalWrite(PIN_MOTOR_IN4, LOW);
  }

  void mundur() {
    int speedLeft = (currentSpeed * settings.calibrateLeft) / 255;
    int speedRight = (currentSpeed * settings.calibrateRight) / 255;

    digitalWrite(PIN_MOTOR_IN1, LOW);
    analogWrite(PIN_MOTOR_IN2, speedLeft);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    analogWrite(PIN_MOTOR_IN4, speedRight);
  }

  void kiri() {
    int speedLeft = (currentSpeed * settings.calibrateLeft) / 255;
    int speedRight = (currentSpeed * settings.calibrateRight) / 255;

    digitalWrite(PIN_MOTOR_IN1, LOW);
    analogWrite(PIN_MOTOR_IN2, speedLeft);
    analogWrite(PIN_MOTOR_IN3, speedRight);
    digitalWrite(PIN_MOTOR_IN4, LOW);
  }

  void kanan() {
    int speedLeft = (currentSpeed * settings.calibrateLeft) / 255;
    int speedRight = (currentSpeed * settings.calibrateRight) / 255;

    analogWrite(PIN_MOTOR_IN1, speedLeft);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    analogWrite(PIN_MOTOR_IN4, speedRight);
  }

  void berhenti() {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    digitalWrite(PIN_MOTOR_IN4, LOW);
  }
};

#endif
