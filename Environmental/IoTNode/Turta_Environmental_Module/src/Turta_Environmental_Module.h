/*
  Turta_Environmental_Module.h - IoT Node - ESP32 Library for Environmental Sensor.
  Created by Turta, Dec 8, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_Environmental_Module_h
#define Turta_Environmental_Module_h

#include <Arduino.h>
#include <Wire.h>

// Device Registers
const byte BME680_I2C_ADDRESS = 0x76;

// Registers
const byte BME680_STATUS = 0x73;
const byte BME680_RESET = 0xE0;
const byte BME680_ID = 0xD0;
const byte BME680_CONFIG = 0x75;
const byte BME680_CTRL_MEAS = 0x74;
const byte BME680_CTRL_HUM = 0x72;
const byte BME680_CTRL_GAS_1 = 0x71;
const byte BME680_CTRL_GAS_0 = 0x70;

const byte BME680_GAS_WAIT_0 = 0x64;
const byte BME680_GAS_WAIT_1 = 0x65;
const byte BME680_GAS_WAIT_2 = 0x66;
const byte BME680_GAS_WAIT_3 = 0x67;
const byte BME680_GAS_WAIT_4 = 0x68;
const byte BME680_GAS_WAIT_5 = 0x69;
const byte BME680_GAS_WAIT_6 = 0x6A;
const byte BME680_GAS_WAIT_7 = 0x6B;
const byte BME680_GAS_WAIT_8 = 0x6C;
const byte BME680_GAS_WAIT_9 = 0x6D;

const byte BME680_RES_HEAT_0 = 0x5A;
const byte BME680_RES_HEAT_1 = 0x5B;
const byte BME680_RES_HEAT_2 = 0x5C;
const byte BME680_RES_HEAT_3 = 0x5D;
const byte BME680_RES_HEAT_4 = 0x5E;
const byte BME680_RES_HEAT_5 = 0x5F;
const byte BME680_RES_HEAT_6 = 0x60;
const byte BME680_RES_HEAT_7 = 0x61;
const byte BME680_RES_HEAT_8 = 0x62;
const byte BME680_RES_HEAT_9 = 0x63;

const byte BME680_IDAC_HEAT_0 = 0x50;
const byte BME680_IDAC_HEAT_1 = 0x51;
const byte BME680_IDAC_HEAT_2 = 0x52;
const byte BME680_IDAC_HEAT_3 = 0x53;
const byte BME680_IDAC_HEAT_4 = 0x54;
const byte BME680_IDAC_HEAT_5 = 0x55;
const byte BME680_IDAC_HEAT_6 = 0x56;
const byte BME680_IDAC_HEAT_7 = 0x57;
const byte BME680_IDAC_HEAT_8 = 0x58;
const byte BME680_IDAC_HEAT_9 = 0x59;

// Registers: Readout
const byte BME680_GAS_R_MSB = 0x2A;
const byte BME680_GAS_R_LSB = 0x2B;
const byte BME680_HUM_MSB = 0x25;
const byte BME680_HUM_LSB = 0x26;
const byte BME680_TEMP_MSB = 0x22;
const byte BME680_TEMP_LSB = 0x23;
const byte BME680_TEMP_XLSB = 0x24;
const byte BME680_PRESS_MSB = 0x1F;
const byte BME680_PRESS_LSB = 0x20;
const byte BME680_PRESS_XLSB = 0x21;
const byte BME680_EAS_STATUS_0 = 0x1D;

// Registers: Calibration
const byte BME680_T2_LSB_REG = 0x8A;
const byte BME680_T2_MSB_REG = 0x8B;
const byte BME680_T3_REG = 0x8C;
const byte BME680_P1_LSB_REG = 0x8E;
const byte BME680_P1_MSB_REG = 0x8F;
const byte BME680_P2_LSB_REG = 0x90;
const byte BME680_P2_MSB_REG = 0x91;
const byte BME680_P3_REG = 0x92;
const byte BME680_P4_LSB_REG = 0x94;
const byte BME680_P4_MSB_REG = 0x95;
const byte BME680_P5_LSB_REG = 0x96;
const byte BME680_P5_MSB_REG = 0x97;
const byte BME680_P7_REG = 0x98;
const byte BME680_P6_REG = 0x99;
const byte BME680_P8_LSB_REG = 0x9C;
const byte BME680_P8_MSB_REG = 0x9D;
const byte BME680_P9_LSB_REG = 0x9E;
const byte BME680_P9_MSB_REG = 0x9F;
const byte BME680_P10_REG = 0xA0;
const byte BME680_H2_MSB_REG = 0xE1;
const byte BME680_H2_LSB_REG = 0xE2;
const byte BME680_H1_LSB_REG = 0xE2;
const byte BME680_H1_MSB_REG = 0xE3;
const byte BME680_H3_REG = 0xE4;
const byte BME680_H4_REG = 0xE5;
const byte BME680_H5_REG = 0xE6;
const byte BME680_H6_REG = 0xE7;
const byte BME680_H7_REG = 0xE8;
const byte BME680_T1_LSB_REG = 0xE9;
const byte BME680_T1_MSB_REG = 0xEA;
const byte BME680_GH2_LSB_REG = 0xEB;
const byte BME680_GH2_MSB_REG = 0xEC;
const byte BME680_GH1_REG = 0xED;
const byte BME680_GH3_REG = 0xEE;
const byte BME680_RES_HEAT_VAL = 0x00;
const byte BME680_RES_HEAT_RANGE = 0x02;
const byte BME680_RANGE_SW_ERR = 0x04;

class Turta_Environmental_Module
{
  public:
    Turta_Environmental_Module();
    void begin();
    void beginAdvanced(ushort heatDuration, uint heatTemperature);
    double readTemperature();
    double readHumidity();
    double readPressure();
    double calcAltitude(double meanSeaLevelPressureInBar);
    double readGasResistance();
    void readTPH(double & t, double & p, double & h);
  private:
    void configureSensor(byte temperatureOversampling, byte pressureOversampling, byte humidityOversampling, byte iirFilter, ushort heatDuration, uint heatTemperature);
    bool checkSensor();
    void resetSensor();
    void readCalibrationData();
    double compensateTemperature(int tempADC);
    double compensatePressure(int presADC);
    double compensateHumidity(int humADC);
    void forceRead(bool gasMeasurementEnabled);
    void setGasMeasurement(bool state);
    void selectHeaterProfileSetPoint(byte heaterProfileSetPoint);
    bool getMeasuringStatus();
    bool getGasMeasuringStatus();
    bool getNewDataStatus();
    double calculateGasResistance(int gasResADC, ushort gasRange);
    byte calculateHeaterResistance(uint targetTemp);
    byte calculateHeatDuration(int dur);
    void i2CWriteOneByte(byte reg, byte data);
    byte i2CReadOneByte(byte reg);
    int i2CReadTwoBytesAsInt(byte reg);
};

#endif