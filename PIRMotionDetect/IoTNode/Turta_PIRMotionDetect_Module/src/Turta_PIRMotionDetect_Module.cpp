/*
  Turta_PIRMotionDetect_Module.cpp - IoT Node - ESP32 Library for PIR Motion Detect.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include "Turta_PIRMotionDetect_Module.h"

#define INT_PIN 37

Turta_PIRMotionDetect_Module::Turta_PIRMotionDetect_Module() {}

void Turta_PIRMotionDetect_Module::begin() {
  pinMode(INT_PIN, INPUT);
}

bool Turta_PIRMotionDetect_Module::readMotionState() {
  bool result;
  result = digitalRead(INT_PIN);
  return result;
}