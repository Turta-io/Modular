/*
  Turta_ALSGesture_Module.h - IoT Node - ESP32 Library for ALS Gesture Sensor.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_ALSGesture_Module_h
#define Turta_ALSGesture_Module_h

#include <Arduino.h>
#include <Wire.h>

// Device Registers
const byte APDS9960_I2C_ADDRESS = 0x39;
const byte APDS9960_ENABLE = 0x80;
const byte APDS9960_ATIME = 0x81;
const byte APDS9960_WTIME = 0x83;
const byte APDS9960_AILTL = 0x84;
const byte APDS9960_AILTH = 0x85;
const byte APDS9960_AIHTL = 0x86;
const byte APDS9960_AIHTH = 0x87;
const byte APDS9960_PILT = 0x89;
const byte APDS9960_PIHT = 0x8B;
const byte APDS9960_PERS = 0x8C;
const byte APDS9960_CONFIG1 = 0x8D;
const byte APDS9960_PPULSE = 0x8E;
const byte APDS9960_CONTROL = 0x8F;
const byte APDS9960_CONFIG2 = 0x90;
const byte APDS9960_ID = 0x92;
const byte APDS9960_STATUS = 0x93;
const byte APDS9960_CDATAL = 0x94;
const byte APDS9960_CDATAH = 0x95;
const byte APDS9960_RDATAL = 0x96;
const byte APDS9960_RDATAH = 0x97;
const byte APDS9960_GDATAL = 0x98;
const byte APDS9960_GDATAH = 0x99;
const byte APDS9960_BDATAL = 0x9A;
const byte APDS9960_BDATAH = 0x9B;
const byte APDS9960_PDATA = 0x9C;
const byte APDS9960_POFFSET_UR = 0x9D;
const byte APDS9960_POFFSET_DL = 0x9E;
const byte APDS9960_CONFIG3 = 0x9F;
const byte APDS9960_GPENTH = 0xA0;
const byte APDS9960_GEXTH = 0xA1;
const byte APDS9960_GCONF1 = 0xA2;
const byte APDS9960_GCONF2 = 0xA3;
const byte APDS9960_GOFFSET_U = 0xA4;
const byte APDS9960_GOFFSET_D = 0xA5;
const byte APDS9960_GOFFSET_L = 0xA7;
const byte APDS9960_GOFFSET_R = 0xA9;
const byte APDS9960_GPULSE = 0xA6;
const byte APDS9960_GCONF3 = 0xAA;
const byte APDS9960_GCONF4 = 0xAB;
const byte APDS9960_GFLVL = 0xAE;
const byte APDS9960_GSTATUS = 0xAF;
const byte APDS9960_IFORCE = 0xE4;
const byte APDS9960_PICLEAR = 0xE5;
const byte APDS9960_CICLEAR = 0xE6;
const byte APDS9960_AICLEAR = 0xE7;
const byte APDS9960_GFIFO_U = 0xFC;
const byte APDS9960_GFIFO_D = 0xFD;
const byte APDS9960_GFIFO_L = 0xFE;
const byte APDS9960_GFIFO_R = 0xFF;

class Turta_ALSGesture_Module
{
  public:
    Turta_ALSGesture_Module();
    void begin();
    int readAmbientLight();
    void readARGBLight(int & a, int & r, int & g, int & b);
    short readProximity();
  private:
    void config();
    void i2CWriteOneByte(byte addr, byte reg, byte data);
    byte i2CReadOneByte(byte addr, byte reg);
    int i2CReadTwoBytesAsInt(byte addr, byte reg);
    void i2CReadMultipleBytes(byte addr, byte reg, short len, byte *data);
};

#endif