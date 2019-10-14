/*
  Turta_AirQuality_Module.h - IoT Node - ESP32 Library for Air Quality Sensor.
  Created by Turta, Oct 14, 2019.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_AirQuality_Module_h
#define Turta_AirQuality_Module_h

#include <Arduino.h>
#include <Wire.h>

// Device Registers
const byte CCS811_I2C_ADDRESS = 0x5A;

// Registers
const byte CCS811_STATUS = 0x00;
const byte CCS811_MEAS_MODE = 0x01;
const byte CCS811_ALG_RESULT_DATA = 0x02;
const byte CCS811_RAW_DATA = 0x03;
const byte CCS811_ENV_DATA = 0x05;
const byte CCS811_THRESOLDS = 0x10;
const byte CCS811_BASELINE = 0x11;
const byte CCS811_HW_ID = 0x20;
const byte CCS811_HW_VERSION = 0x21;
const byte CCS811_FW_BOOT_VERSION = 0x23;
const byte CCS811_FW_APP_VERSION = 0x24;
const byte CCS811_ERROR_ID = 0xE0;
const byte CCS811_APP_START = 0xF4;
const byte CCS811_SW_RESET = 0xFF;

// Masks
const byte CCS811_FW_MODE_MASK = 0x80;
const byte CCS811_APP_ERASE_MASK = 0x40;
const byte CCS811_APP_VERIFY_MASK = 0x20;
const byte CCS811_APP_VALID_MASK = 0x10;
const byte CCS811_DATA_READY_MASK = 0x08;
const byte CCS811_ERROR_MASK = 0x01;

// Globals
const byte CCS811_REF_RES = 100000;

class Turta_AirQuality_Module
{
  public:
    Turta_AirQuality_Module();
    void begin();
    void beginAdvanced(ushort mode, bool int_datardy, bool int_thresh);
    bool checkHwId();
    byte getHwVersion();
    byte getStatusRegister();
    bool getFwModeState();
    bool getAppEraseState();
    bool getAppVerifyState();
    bool getAppValidState();
    bool getDataReadyState();
    bool getErrorState();
    bool getIntPinState();
    void setWakeState(bool state);
    uint16_t readECO2();
    uint16_t readTVOC();
    void readECO2TVOC(uint16_t *result);
    void readAllResultData(byte *data);
    void readRawData(uint16_t *result);
    void setEnvData(double tempC, ushort rh);
    void setThresolds(byte thresolds[]);
    void getBaseline(byte *data);
    void setBaseline(byte data[]);
    void getFwBootVersion(byte *data);
    void getFwAppVersion(byte *data);
    byte getErrorId();
    void swReset();
    bool appStart();
  private:
    void configureSensor(ushort mode, ushort int_datardy, ushort int_thresh);
    void i2CWriteWithNoData(byte reg);
    void i2CWriteOneByte(byte reg, byte data);
    void i2CWriteMultipleBytes(byte reg, byte data[], ushort length);
    byte i2CReadOneByte(byte reg);
    void i2CReadMultipleBytes(byte reg, short len, byte *data);
};

#endif