/*
  Turta_AccelTilt_Module.h - IoT Node - ESP32 Library for Accel & Tilt Sensor.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_AccelTilt_Module_h
#define Turta_AccelTilt_Module_h

#include <Arduino.h>
#include <Wire.h>

// Device Registers
const byte MMA8491Q_I2C_ADDRESS = 0x55;
const byte MMA8491Q_STATUS = 0x00;
const byte MMA8491Q_OUT_X_MSB = 0x01;
const byte MMA8491Q_OUT_Y_MSB = 0x03;
const byte MMA8491Q_OUT_Z_MSB = 0x05;

class Turta_AccelTilt_Module
{
  public:
    Turta_AccelTilt_Module();
    void begin();
    double readXAxis();
    double readYAxis();
    double readZAxis();
    void readXYZAxis(double & x, double & y, double & z);
    void readTiltState(bool & x, bool & y, bool & z);
  private:
    double convertToG(int analog_data);
    void i2CWriteOneByte(byte reg, byte data);
    byte i2CReadOneByte(byte reg);
    int i2CReadTwoBytesAsIntRS2B(byte reg);
    void i2CReadMultipleBytes(byte reg, short len, byte *data);
};

#endif