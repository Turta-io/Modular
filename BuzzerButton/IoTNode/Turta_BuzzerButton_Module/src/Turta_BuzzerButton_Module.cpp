/*
  Turta_BuzzerButton_Module.cpp - IoT Node - ESP32 Library for Buzzer Button.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include "Turta_BuzzerButton_Module.h"

#define INT_PIN 37
#define PWM_PIN 2

Turta_BuzzerButton_Module::Turta_BuzzerButton_Module() {}

void Turta_BuzzerButton_Module::begin() {
  pinMode(INT_PIN, INPUT);

  ledcSetup(15, 1000, 8);
  ledcAttachPin(PWM_PIN, 15);
}

bool Turta_BuzzerButton_Module::buttonRead() {
  bool result;
  result = !digitalRead(INT_PIN);
  return result;
}

void Turta_BuzzerButton_Module::buzzerTone(int frequency, short dutyCycle) {
  ledcWriteTone(15, frequency);
  ledcWrite(15, dutyCycle);
}

void Turta_BuzzerButton_Module::buzzerStop() {
  ledcWrite(15, 1);
}

void Turta_BuzzerButton_Module::buzzerTonePeriod(int frequency, short dutyCycle, int durationMs) {
  ledcWriteTone(15, frequency);
  ledcWrite(15, dutyCycle);
  delay(durationMs);
  ledcWrite(15, 1);
}