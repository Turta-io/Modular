/*
  Turta_TempRH_Module.h - IoT Node - ESP32 Library for Temperature & RH Sensor.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_TempRH_Module_h
#define Turta_TempRH_Module_h

#include <Arduino.h>
#include <Wire.h>

// Device Registers
const byte HTS221_I2C_ADDRESS = 0x5F;
const byte HTS221_WHO_AM_I = 0x0F;
const byte HTS221_AV_CONF = 0x10;

const byte HTS221_CTRL_REG1 = 0x20;
const byte HTS221_CTRL_REG2 = 0x21;
const byte HTS221_CTRL_REG3 = 0x22;

const byte HTS221_STATUS_REG = 0x27;
const byte HTS221_HUMIDITY_OUT_L = 0x28;
const byte HTS221_HUMIDITY_OUT_H = 0x29;
const byte HTS221_TEMP_OUT_L = 0x2A;
const byte HTS221_TEMP_OUT_H = 0x2B;
    
const byte HTS221_H0_rH_x2 = 0x30;
const byte HTS221_H1_rH_x2 = 0x31;
const byte HTS221_T0_degC_x8 = 0x32;
const byte HTS221_T1_degC_x8 = 0x33;
const byte HTS221_T1_T0_MSB = 0x35;
const byte HTS221_H0_T0_OUT_L = 0x36;
const byte HTS221_H0_T0_OUT_H = 0x37;
const byte HTS221_H1_T0_OUT_L = 0x3A;
const byte HTS221_H1_T0_OUT_H = 0x3B;
const byte HTS221_T0_OUT_L = 0x3C;
const byte HTS221_T0_OUT_H = 0x3D;
const byte HTS221_T1_OUT_L = 0x3E;
const byte HTS221_T1_OUT_H = 0x3F;

class Turta_TempRH_Module
{
  public:
    Turta_TempRH_Module();
    void begin();
    double readTemperature();
    double readHumidity();
  private:
    void config();
    void i2CWriteOneByte(byte reg, byte data);
    byte i2CReadOneByte(byte reg);
    void i2CReadMultipleBytes(byte reg, short len, byte *data);
};

#endif