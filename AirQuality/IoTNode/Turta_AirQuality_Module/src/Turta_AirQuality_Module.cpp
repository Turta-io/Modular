/*
  Turta_AirQuality_Module.cpp - IoT Node - ESP32 Library for Air Quality Sensor.
  Created by Turta, Oct 14, 2019.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include <Wire.h>
#include "Turta_AirQuality_Module.h"

#define SCL_PIN 22
#define SDA_PIN 23
#define N_WAKE 14
#define N_INT 37
#define BUS_SPEED 400000

#define DRIVE_MODE_0_IDLE 0x00
#define DRIVE_MODE_1_1SEC 0x10
#define DRIVE_MODE_2_10SEC 0x20
#define DRIVE_MODE_3_60SEC 0x30
#define DRIVE_MODE_4_250MS 0x40

#define INT_DATARDY_DIS 0x00
#define INT_DATARDY_EN 0x08

#define INT_THRESH_DIS 0x00
#define INT_THRESH_EN 0x04

#define HW_ID 0x81

Turta_AirQuality_Module::Turta_AirQuality_Module() {}

void Turta_AirQuality_Module::begin() {
  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);
  pinMode(N_WAKE, OUTPUT);
  pinMode(N_INT, INPUT);
  digitalWrite(N_WAKE, false);
  delay(10);

  // Configure sensor
  Turta_AirQuality_Module::swReset();
  if (Turta_AirQuality_Module::checkHwId()) {
    Turta_AirQuality_Module::appStart();
    Turta_AirQuality_Module::configureSensor(
      DRIVE_MODE_1_1SEC,
      INT_DATARDY_DIS,
      INT_THRESH_DIS);
  }
}

void Turta_AirQuality_Module::beginAdvanced(ushort mode, bool int_datardy, bool int_thresh) {
  ushort conf_mode;
  ushort conf_int_datardy;
  ushort conf_int_thresh;
  
  switch(mode) {
    case 0:
      conf_mode = DRIVE_MODE_0_IDLE;
      break;

    case 1:
      conf_mode = DRIVE_MODE_1_1SEC;
      break;

    case 2:
      conf_mode = DRIVE_MODE_2_10SEC;
      break;

    case 3:
      conf_mode = DRIVE_MODE_3_60SEC;
      break;

    case 4:
      conf_mode = DRIVE_MODE_4_250MS;
      break;

    default:
      conf_mode = DRIVE_MODE_0_IDLE;
      break;
  }

  conf_int_datardy = int_datardy ? INT_DATARDY_EN : INT_DATARDY_DIS;
  conf_int_thresh = int_thresh ? INT_THRESH_EN : INT_THRESH_DIS;

  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);
  pinMode(N_WAKE, OUTPUT);
  pinMode(N_INT, INPUT);
  digitalWrite(N_WAKE, false);
  delay(10);

  // Configure sensor
  Turta_AirQuality_Module::swReset();
  if (Turta_AirQuality_Module::checkHwId()) {
    Turta_AirQuality_Module::appStart();
    Turta_AirQuality_Module::configureSensor(conf_mode, conf_int_datardy, conf_int_thresh);
  }
}

// Sensor Configuration
void Turta_AirQuality_Module::configureSensor(ushort mode, ushort int_datardy, ushort int_thresh) {
  // Set sensor drive mode and interrupts
  byte configValue = 0x00;
  configValue |= (byte)mode;
  configValue |= (byte)int_datardy;
  configValue |= (byte)int_thresh;
  Turta_AirQuality_Module::i2CWriteOneByte(CCS811_MEAS_MODE, configValue);
  delay(100);
}

// Sensor Status
bool Turta_AirQuality_Module::checkHwId() {
  return Turta_AirQuality_Module::i2CReadOneByte(CCS811_HW_ID) == HW_ID ? true : false;
}

byte Turta_AirQuality_Module::getHwVersion() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_HW_VERSION);
  return readValue;
}

byte Turta_AirQuality_Module::getStatusRegister() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);
  return readValue;
}

bool Turta_AirQuality_Module::getFwModeState() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);
  return ((readValue & CCS811_FW_MODE_MASK) == CCS811_FW_MODE_MASK) ? true : false;
}

bool Turta_AirQuality_Module::getAppEraseState() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);
  return ((readValue & CCS811_APP_ERASE_MASK) == CCS811_APP_ERASE_MASK) ? true : false;
}

bool Turta_AirQuality_Module::getAppVerifyState() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);
  return ((readValue & CCS811_APP_VERIFY_MASK) == CCS811_APP_VERIFY_MASK) ? true : false;
}

bool Turta_AirQuality_Module::getAppValidState() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);

  return ((readValue & CCS811_APP_VALID_MASK) == CCS811_APP_VALID_MASK) ? true : false;
}

bool Turta_AirQuality_Module::getDataReadyState() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);
  return ((readValue & CCS811_DATA_READY_MASK) == CCS811_DATA_READY_MASK) ? true : false;
}

bool Turta_AirQuality_Module::getErrorState() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_STATUS);
  return ((readValue & CCS811_ERROR_MASK) == CCS811_ERROR_MASK) ? true : false;
}

bool Turta_AirQuality_Module::getIntPinState() {
  return !digitalRead(N_INT);
}

void Turta_AirQuality_Module::setWakeState(bool state) {
  digitalWrite(N_WAKE, !state);
}

// Sensor Readouts
uint16_t Turta_AirQuality_Module::readECO2() {
  byte data[2] = {0};
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_ALG_RESULT_DATA, 2, data);
  return (uint16_t)data[0] << 8 | (uint16_t)data[1];
}

uint16_t Turta_AirQuality_Module::readTVOC() {
  byte data[4] = {0};
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_ALG_RESULT_DATA, 4, data);
  return (uint16_t)data[2] << 8 | (uint16_t)data[3];
}

void Turta_AirQuality_Module::readECO2TVOC(uint16_t *result) {
  byte data[4] = {0};
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_ALG_RESULT_DATA, 4, data);
  result[0] = (uint16_t)data[0] << 8 | (uint16_t)data[1];
  result[1] = (uint16_t)data[2] << 8 | (uint16_t)data[3];
}

void Turta_AirQuality_Module::readAllResultData(byte *data) {
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_ALG_RESULT_DATA, 8, data);
}

void Turta_AirQuality_Module::readRawData(uint16_t *result) {
  byte data[2] = {0};
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_RAW_DATA, 2, data);
  result[0] = (uint16_t)data[0] >> 2;
  result[1] = (uint16_t)(data[0] & 0x03) << 8 | (uint16_t)data[1];
}

// Adjustments and Settings
void Turta_AirQuality_Module::setEnvData(double tempC, ushort rh) {
  uint16_t tHi, tLo, tCalc, hHi, hLo;
  double tFraction;

  // Calculate temperature registers
  tFraction = modf(tempC, &tempC);
  tHi = (uint16_t)(tempC + 25) << 9;
  tLo = (uint16_t)(tFraction / 0.001953125) & 0x01FF;
  tCalc = tHi | tLo;

  // Calculate relative humidity registers
  hHi = rh << 1;
  hLo = 0;

  // Prepare buffer
  byte data[] = {hHi, hLo, (tCalc >> 8) & 0xFF, tCalc & 0xFF};

  // Write data
  Turta_AirQuality_Module::i2CWriteMultipleBytes(CCS811_ENV_DATA, data, 4);
}

void Turta_AirQuality_Module::setThresolds(byte thresolds[]) {
  Turta_AirQuality_Module::i2CWriteMultipleBytes(CCS811_THRESOLDS, thresolds, 4);
}

void Turta_AirQuality_Module::getBaseline(byte *data) {
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_BASELINE, 2, data);
}

void Turta_AirQuality_Module::setBaseline(byte data[]) {
  Turta_AirQuality_Module::i2CWriteMultipleBytes(CCS811_BASELINE, data, 2);
}

void Turta_AirQuality_Module::getFwBootVersion(byte *data) {
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_FW_BOOT_VERSION, 2, data);
}

void Turta_AirQuality_Module::getFwAppVersion(byte *data) {
  Turta_AirQuality_Module::i2CReadMultipleBytes(CCS811_FW_APP_VERSION, 2, data);
}

byte Turta_AirQuality_Module::getErrorId() {
  byte readValue = Turta_AirQuality_Module::i2CReadOneByte(CCS811_ERROR_ID);
  return readValue;
}

void Turta_AirQuality_Module::swReset() {
  byte data[4] = {0x11, 0xE5, 0x72, 0x8A};
  Turta_AirQuality_Module::i2CWriteMultipleBytes(CCS811_SW_RESET, data, 4);
  delay(100);
}

bool Turta_AirQuality_Module::appStart() {
  bool state = Turta_AirQuality_Module::getAppValidState();
  if (state) {
    Turta_AirQuality_Module::i2CWriteWithNoData(CCS811_APP_START);
    delay(100);
  }
  return state;
}

// I2C Communication
void Turta_AirQuality_Module::i2CWriteWithNoData(byte reg) {
  Wire.beginTransmission((uint8_t)CCS811_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
}

void Turta_AirQuality_Module::i2CWriteOneByte(byte reg, byte data) {
  Wire.beginTransmission((uint8_t)CCS811_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write(data);
  Wire.endTransmission();
}

void Turta_AirQuality_Module::i2CWriteMultipleBytes(byte reg, byte data[], ushort length) {
  Wire.beginTransmission((uint8_t)CCS811_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write(data, length);
  Wire.endTransmission();
}

byte Turta_AirQuality_Module::i2CReadOneByte(byte reg) {
  byte data;
  Wire.beginTransmission((uint8_t)CCS811_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)CCS811_I2C_ADDRESS, (uint8_t)1);
  data = Wire.read();
  Wire.endTransmission();
  return data;
}

void Turta_AirQuality_Module::i2CReadMultipleBytes(byte reg, short len, byte *data) {
  short i = 0;
  Wire.beginTransmission((uint8_t)CCS811_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)CCS811_I2C_ADDRESS, (uint8_t)len);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
}