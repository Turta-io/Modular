/*
  Turta_Photocoupler_Module.cpp - IoT Node - ESP32 Library for Photocoupler.
  Created by Turta, Dec 21, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include "Turta_Photocoupler_Module.h"

#define INPUT1_PIN 37
#define INPUT2_PIN 14

Turta_Photocoupler_Module::Turta_Photocoupler_Module() {}

void Turta_Photocoupler_Module::begin() {
  pinMode(INPUT1_PIN, INPUT);
  pinMode(INPUT2_PIN, INPUT);
}

bool Turta_Photocoupler_Module::input1Read() {
  bool result;
  result = digitalRead(INPUT1_PIN);
  return result;
}

bool Turta_Photocoupler_Module::input2Read() {
  bool result;
  result = digitalRead(INPUT2_PIN);
  return result;
}