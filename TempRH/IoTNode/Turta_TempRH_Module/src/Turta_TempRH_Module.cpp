/*
  Turta_TempRH_Module.cpp - IoT Node - ESP32 Library for Temperature & RH Sensor.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include <Wire.h>
#include "Turta_TempRH_Module.h"

#define INT_PIN 37
#define SCL_PIN 22
#define SDA_PIN 23
#define BUS_SPEED 400000

// Global variables.
int T0_degC;
int T1_degC;
short T1_T0_MSB;
int T1_OUT;
int T0_OUT;
int H0_rH;
int H1_rH;
int H0_T0_OUT;
int H1_T0_OUT;

Turta_TempRH_Module::Turta_TempRH_Module() {}

void Turta_TempRH_Module::begin() {
  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);

  // Configure sensor
  if (Turta_TempRH_Module::i2CReadOneByte(HTS221_WHO_AM_I) == 0xBC)
    Turta_TempRH_Module::config();
}

void Turta_TempRH_Module::config() {
  // Configure sensor.
  Turta_TempRH_Module::i2CWriteOneByte(HTS221_CTRL_REG1, 0x81);

  // Read calibration coefficients for temperature.
  T0_degC = Turta_TempRH_Module::i2CReadOneByte(HTS221_T0_degC_x8);
  T1_degC = Turta_TempRH_Module::i2CReadOneByte(HTS221_T1_degC_x8);
  T1_T0_MSB = Turta_TempRH_Module::i2CReadOneByte(HTS221_T1_T0_MSB);
  T0_degC = ((T1_T0_MSB & 0x03) << 8 | T0_degC);
  T1_degC = ((T1_T0_MSB & 0x0C) << 6 | T1_degC);
  T0_OUT = (Turta_TempRH_Module::i2CReadOneByte(HTS221_T0_OUT_H) << 8) |
    Turta_TempRH_Module::i2CReadOneByte(HTS221_T0_OUT_L);
  T1_OUT = (Turta_TempRH_Module::i2CReadOneByte(HTS221_T1_OUT_H) << 8) |
    Turta_TempRH_Module::i2CReadOneByte(HTS221_T1_OUT_L);

  // Read calibration coefficients for humidity.
  H0_rH = Turta_TempRH_Module::i2CReadOneByte(HTS221_H0_rH_x2);
  H1_rH = Turta_TempRH_Module::i2CReadOneByte(HTS221_H1_rH_x2);
  H0_T0_OUT = (Turta_TempRH_Module::i2CReadOneByte(HTS221_H0_T0_OUT_H) << 8) |
    Turta_TempRH_Module::i2CReadOneByte(HTS221_H0_T0_OUT_L);
  H1_T0_OUT = (Turta_TempRH_Module::i2CReadOneByte(HTS221_H1_T0_OUT_H) << 8) |
    Turta_TempRH_Module::i2CReadOneByte(HTS221_H1_T0_OUT_L);
}

double Turta_TempRH_Module::readTemperature() {
  int T_OUT = (Turta_TempRH_Module::i2CReadOneByte(HTS221_TEMP_OUT_H) << 8) |
    Turta_TempRH_Module::i2CReadOneByte(HTS221_TEMP_OUT_L);

  double r1 = (double)(((int16_t)T_OUT - (int16_t)T0_OUT) * ((double)((int16_t)(T1_degC) - (int16_t)(T0_degC)) / 8.0)) / (double)((int16_t)T1_OUT - (int16_t)T0_OUT);
  double r2 = (double)((int16_t)T0_degC) / 8.0;

  return r1 + r2;
}

double Turta_TempRH_Module::readHumidity() {
  int H_OUT = (Turta_TempRH_Module::i2CReadOneByte(HTS221_HUMIDITY_OUT_H) << 8) |
    Turta_TempRH_Module::i2CReadOneByte(HTS221_HUMIDITY_OUT_L);

  double r1 = (double)(((int16_t)H_OUT - (int16_t)H0_T0_OUT) * ((double)((int16_t)(H1_rH) - (int16_t)(H0_rH)) / 2.0)) / (double)((int16_t)H1_T0_OUT - (int16_t)H0_T0_OUT);
  double r2 = (double)((int16_t)H0_rH) / 2.0;
  double result = r1 + r2;

  if (result > 100.0)
    result = 100.0;

  return result;
}

// I2C Communication
void Turta_TempRH_Module::i2CWriteOneByte(byte reg, byte data) {
  Wire.beginTransmission((uint8_t)HTS221_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte Turta_TempRH_Module::i2CReadOneByte(byte reg) {
  byte data;
  Wire.beginTransmission((uint8_t)HTS221_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)HTS221_I2C_ADDRESS, (uint8_t)1);
  data = Wire.read();
  Wire.endTransmission();
  return data;
}