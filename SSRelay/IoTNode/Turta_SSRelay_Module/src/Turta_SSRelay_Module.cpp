/*
  Turta_SSRelay_Module.cpp - IoT Node - ESP32 Library for Solid State Relay.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include "Turta_SSRelay_Module.h"

#define RELAY_PIN 14

Turta_SSRelay_Module::Turta_SSRelay_Module() {}

void Turta_SSRelay_Module::begin() {
  pinMode(RELAY_PIN, OUTPUT);
}

void Turta_SSRelay_Module::relayWrite(bool state) {
  digitalWrite(RELAY_PIN, state);
}

bool Turta_SSRelay_Module::relayRead() {
  bool result;
  result = digitalRead(RELAY_PIN);
  return result;
}