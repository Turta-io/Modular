/*
  Turta_AccelTilt_Module.cpp - IoT Node - ESP32 Library for Accel & Tilt Sensor.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include <Wire.h>
#include "Turta_AccelTilt_Module.h"

#define EN_PIN 2
#define INTX_PIN 37
#define INTY_PIN 14
#define INTZ_PIN 38
#define SCL_PIN 22
#define SDA_PIN 23
#define BUS_SPEED 400000

Turta_AccelTilt_Module::Turta_AccelTilt_Module() {}

void Turta_AccelTilt_Module::begin() {
  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);
  pinMode(INTX_PIN, INPUT);
  pinMode(INTY_PIN, INPUT);
  pinMode(INTZ_PIN, INPUT);
  pinMode(EN_PIN, OUTPUT);
}

double Turta_AccelTilt_Module::convertToG(int analog_data) {
  if ((analog_data & 0x2000) == 0x2000) // Zero or negative G
    return (0x3FFF - analog_data) / -1024.0;
  else // Positive G
    return analog_data / 1024.0;
}

double Turta_AccelTilt_Module::readXAxis() {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Module::i2CReadOneByte(MMA8491Q_I2C_ADDRESS, MMA8491Q_STATUS) & 0x01) != 0x01)
    delay(1);
  int tempData = Turta_AccelTilt_Module::i2CReadTwoBytesAsIntRS2B(MMA8491Q_I2C_ADDRESS, MMA8491Q_OUT_X_MSB);
  digitalWrite(EN_PIN, LOW);

  return Turta_AccelTilt_Module::convertToG(tempData);
}

double Turta_AccelTilt_Module::readYAxis() {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Module::i2CReadOneByte(MMA8491Q_I2C_ADDRESS, MMA8491Q_STATUS) & 0x02) != 0x02)
    delay(1);
  int tempData = Turta_AccelTilt_Module::i2CReadTwoBytesAsIntRS2B(MMA8491Q_I2C_ADDRESS, MMA8491Q_OUT_Y_MSB);
  digitalWrite(EN_PIN, LOW);

  return Turta_AccelTilt_Module::convertToG(tempData);
}

double Turta_AccelTilt_Module::readZAxis() {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Module::i2CReadOneByte(MMA8491Q_I2C_ADDRESS, MMA8491Q_STATUS) & 0x04) != 0x04)
    delay(1);
  int tempData = Turta_AccelTilt_Module::i2CReadTwoBytesAsIntRS2B(MMA8491Q_I2C_ADDRESS, MMA8491Q_OUT_Z_MSB);
  digitalWrite(EN_PIN, LOW);

  return Turta_AccelTilt_Module::convertToG(tempData);
}

void Turta_AccelTilt_Module::readXYZAxis(double & x, double & y, double & z) {
  byte bfr[6] = {0};

  digitalWrite(EN_PIN, HIGH);
  delay(1);
  while ((Turta_AccelTilt_Module::i2CReadOneByte(MMA8491Q_I2C_ADDRESS, MMA8491Q_STATUS) & 0x08) != 0x08)
    delay(1);

  Turta_AccelTilt_Module::i2CReadMultipleBytes(MMA8491Q_I2C_ADDRESS, MMA8491Q_OUT_X_MSB, 6, bfr);
  digitalWrite(EN_PIN, LOW);

  x = Turta_AccelTilt_Module::convertToG((bfr[0] << 6) + (bfr[1] >> 2));
  y = Turta_AccelTilt_Module::convertToG((bfr[2] << 6) + (bfr[3] >> 2));
  z = Turta_AccelTilt_Module::convertToG((bfr[4] << 6) + (bfr[5] >> 2));
}

void Turta_AccelTilt_Module::readTiltState(bool & x, bool & y, bool & z) {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Module::i2CReadOneByte(MMA8491Q_I2C_ADDRESS, MMA8491Q_STATUS) & 0x08) != 0x08)
    delay(1);
  x = (digitalRead(INTX_PIN) == HIGH) ? false : true;
  y = (digitalRead(INTY_PIN) == HIGH) ? false : true;
  z = (digitalRead(INTZ_PIN) == HIGH) ? false : true;
  digitalWrite(EN_PIN, LOW);
}

// I2C Communication
void Turta_AccelTilt_Module::i2CWriteOneByte(byte addr, byte reg, byte data) {
  Wire.beginTransmission((byte)addr);
  Wire.write((byte)reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte Turta_AccelTilt_Module::i2CReadOneByte(byte addr, byte reg) {
  byte data;
  Wire.beginTransmission((byte)addr);
  Wire.write((byte)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((byte)addr, 1);
  data = Wire.read();
  Wire.endTransmission();
  return data;
}

int Turta_AccelTilt_Module::i2CReadTwoBytesAsIntRS2B(byte addr, byte reg) {
  int i = 0;
  byte data[2] = {0};
  Wire.beginTransmission((byte)addr);
  Wire.write((byte)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((byte)addr, 2);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
  return (data[0] << 6) + (data[1] >> 2);
}

void Turta_AccelTilt_Module::i2CReadMultipleBytes(byte addr, byte reg, short len, byte *data) {
  short i = 0;
  Wire.beginTransmission((byte)addr);
  Wire.write((byte)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((byte)addr, 8);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
}