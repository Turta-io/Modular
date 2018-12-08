/*
  Turta_ALSGesture_Module.cpp - IoT Node - ESP32 Library for ALS Gesture Sensor.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include <Wire.h>
#include "Turta_ALSGesture_Module.h"

#define INT_PIN 37
#define SCL_PIN 22
#define SDA_PIN 23
#define BUS_SPEED 400000

Turta_ALSGesture_Module::Turta_ALSGesture_Module() {}

void Turta_ALSGesture_Module::begin() {
  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);

  // Configure sensor
  Turta_ALSGesture_Module::config();
}

void Turta_ALSGesture_Module::config() {
  // Set enable register to turn off all functionality but power.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_ENABLE, 0x01);

  // Set ATIME to 72 cycles, 200ms, 65535 max count.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_ATIME, 0xB6);

  // Set wait time to 20ms.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_WTIME, 0xF9);

  // Set ALS interrupt low thresold.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_AILTL, 0xFF); // Low
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_AILTH, 0xFF); // High

  // Set ALS interrupt high thresold.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_AIHTL, 0x00); // Low
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_AIHTH, 0x00); // High

  // Set proximity interrupt low thresold.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_PILT, 0);

  // Set proximity interrupt high thresold.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_PIHT, 255);

  // Set interrupt persistence filters.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_PERS, 0x11);

  // Set configuration register one: No 12x wait (WLONG = 0).
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_CONFIG1, 0x60);

  // Set proximity pulse count to 8 and lenght to 8us.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_PPULSE, 0x48);

  // Set ALS and proximity functions on.
  Turta_ALSGesture_Module::i2CWriteOneByte(APDS9960_ENABLE, 0x0F);
}

int Turta_ALSGesture_Module::readAmbientLight() {
  return Turta_ALSGesture_Module::i2CReadTwoBytesAsInt(APDS9960_CDATAL);
}

void Turta_ALSGesture_Module::readARGBLight(int & a, int & r, int & g, int & b) {
  byte bfr[8] = {0};

  Turta_ALSGesture_Module::i2CReadMultipleBytes(APDS9960_CDATAL, 8, bfr);

  a = int(bfr[0] | (bfr[1] << 8)); // Ambient light channel
  r = int(bfr[2] | (bfr[3] << 8)); // Red channel
  g = int(bfr[4] | (bfr[5] << 8)); // Green channel
  b = int(bfr[6] | (bfr[7] << 8)); // Blue channel
}

short Turta_ALSGesture_Module::readProximity() {
  return (short)Turta_ALSGesture_Module::i2CReadOneByte(APDS9960_PDATA);
}

// I2C Communication
void Turta_ALSGesture_Module::i2CWriteOneByte(byte reg, byte data) {
  Wire.beginTransmission((uint8_t)APDS9960_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte Turta_ALSGesture_Module::i2CReadOneByte(byte reg) {
  byte data;
  Wire.beginTransmission((uint8_t)APDS9960_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)APDS9960_I2C_ADDRESS, (uint8_t)1);
  data = Wire.read();
  Wire.endTransmission();
  return data;
}

int Turta_ALSGesture_Module::i2CReadTwoBytesAsInt(byte reg) {
  int i = 0;
  byte data[2] = {0};
  Wire.beginTransmission((uint8_t)APDS9960_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)APDS9960_I2C_ADDRESS, (uint8_t)2);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
  return data[0] | (data[1] << 8);
}

void Turta_ALSGesture_Module::i2CReadMultipleBytes(byte reg, short len, byte *data) {
  short i = 0;
  Wire.beginTransmission((uint8_t)APDS9960_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)APDS9960_I2C_ADDRESS, (uint8_t)8);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
}