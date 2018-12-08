/*
  Turta_Environmental_Module.cpp - IoT Node - ESP32 Library for Environmental Sensor.
  Created by Turta, Dec 8, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include <Wire.h>
#include "Turta_Environmental_Module.h"
#include <math.h>

#define SCL_PIN 22
#define SDA_PIN 23
#define BUS_SPEED 400000

#define OperationModes_Sleep 0b00000000
#define OperationModes_ForcedMode 0b00000001

#define TemperatureOversamplings_Skipped 0b00000000
#define TemperatureOversamplings_x01 0b00100000
#define TemperatureOversamplings_x02 0b01000000
#define TemperatureOversamplings_x04 0b01100000
#define TemperatureOversamplings_x08 0b10000000
#define TemperatureOversamplings_x16 0b10100000

#define HumidityOversamplings_Skipped 0b00000000
#define HumidityOversamplings_x01 0b00000001
#define HumidityOversamplings_x02 0b00000010
#define HumidityOversamplings_x04 0b00000011
#define HumidityOversamplings_x08 0b00000100
#define HumidityOversamplings_x16 0b00000101

#define PressureOversamplings_Skipped 0b00000000
#define PressureOversamplings_x01 0b00000100
#define PressureOversamplings_x02 0b00001000
#define PressureOversamplings_x04 0b00001100
#define PressureOversamplings_x08 0b00010000
#define PressureOversamplings_x16 0b00010100

#define IIRFilterCoefficients_FC_000 0b00000000
#define IIRFilterCoefficients_FC_001 0b00000100
#define IIRFilterCoefficients_FC_003 0b00001000
#define IIRFilterCoefficients_FC_007 0b00001100
#define IIRFilterCoefficients_FC_015 0b00010000
#define IIRFilterCoefficients_FC_031 0b00010100
#define IIRFilterCoefficients_FC_063 0b00011000
#define IIRFilterCoefficients_FC_127 0b00011100

#define HeaterProfileSetPoints_SP_0 0b00000000
#define HeaterProfileSetPoints_SP_1 0b00000001
#define HeaterProfileSetPoints_SP_2 0b00000010
#define HeaterProfileSetPoints_SP_3 0b00000011
#define HeaterProfileSetPoints_SP_4 0b00000100
#define HeaterProfileSetPoints_SP_5 0b00000101
#define HeaterProfileSetPoints_SP_6 0b00000110
#define HeaterProfileSetPoints_SP_7 0b00000111
#define HeaterProfileSetPoints_SP_8 0b00001000
#define HeaterProfileSetPoints_SP_9 0b00001001

// Global variables
uint16_t calT1;
int16_t calT2;
short calT3;
uint16_t calP1;
int16_t calP2;
short calP3;
int16_t calP4;
int16_t calP5;
short calP6;
short calP7;
int16_t calP8;
int16_t calP9;
ushort calP10;
uint16_t calH1;
uint16_t calH2;
short calH3;
short calH4;
short calH5;
ushort calH6;
short calH7;
short calGH1;
int16_t calGH2;
short calGH3;
double fineTemperature = 0;
short calAmbTemp = 25;
ushort calResHeatRange;
short calResHeatVal;
ushort calRangeSwErr;
uint heatDur;

// Gas range constants for resistance calculation 
const double const_array1[] = { 1, 1, 1, 1, 1, 0.99, 1, 0.992, 1, 1, 0.998, 0.995, 1, 0.99, 1, 1 };
const double const_array2[] = { 8000000, 4000000, 2000000, 1000000, 499500.4995, 248262.1648, 125000, 63004.03226, 31281.28128, 15625, 7812.5, 3906.25, 1953.125, 976.5625, 488.28125, 244.140625 };

Turta_Environmental_Module::Turta_Environmental_Module() {}

void Turta_Environmental_Module::begin() {
  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);

  // Configure sensor
  if (Turta_Environmental_Module::checkSensor()) {
    Turta_Environmental_Module::resetSensor();
    delay(10);
    Turta_Environmental_Module::readCalibrationData();
    Turta_Environmental_Module::configureSensor(
      TemperatureOversamplings_x08,
      PressureOversamplings_x16,
      HumidityOversamplings_x08,
      IIRFilterCoefficients_FC_003,
      250,
      250);
    heatDur = 250 + 100;
  }
}

void Turta_Environmental_Module::beginAdvanced(ushort heatDuration, uint heatTemperature) {
  Wire.begin(SDA_PIN, SCL_PIN, BUS_SPEED);

  // Configure sensor
  if (Turta_Environmental_Module::checkSensor()) {
    Turta_Environmental_Module::resetSensor();
    delay(10);
    Turta_Environmental_Module::readCalibrationData();
    Turta_Environmental_Module::configureSensor(
      TemperatureOversamplings_x08,
      PressureOversamplings_x16,
      HumidityOversamplings_x08,
      IIRFilterCoefficients_FC_003,
      heatDuration,
      heatTemperature);
    heatDur = heatDuration + 100;
  }
}

bool Turta_Environmental_Module::checkSensor() {
  return Turta_Environmental_Module::i2CReadOneByte(BME680_ID) == 0x61 ? true : false;
}

void Turta_Environmental_Module::resetSensor() {
  Turta_Environmental_Module::i2CWriteOneByte(BME680_RESET, 0xB6);
}

void Turta_Environmental_Module::configureSensor(byte temperatureOversampling, byte pressureOversampling, byte humidityOversampling, byte iirFilter, ushort heatDuration, uint heatTemperature) {
  // Select humidity oversampling
  Turta_Environmental_Module::i2CWriteOneByte(BME680_CTRL_HUM, humidityOversampling);
  delay(1);

  // Select temperature and pressure oversamplings
  byte configValue = 0x00;
  configValue |= temperatureOversampling;
  configValue |= (byte)pressureOversampling;
  Turta_Environmental_Module::i2CWriteOneByte(BME680_CTRL_MEAS, configValue);
  delay(1);

  // Select IIR Filter for temperature sensor
  Turta_Environmental_Module::i2CWriteOneByte(BME680_CONFIG, iirFilter);
  delay(1);

  // Enable gas measurements
  Turta_Environmental_Module::setGasMeasurement(true);
  delay(1);

  // Select index of heater set-point
  Turta_Environmental_Module::selectHeaterProfileSetPoint(HeaterProfileSetPoints_SP_0);
  delay(1);

  // Define heater-on time
  Turta_Environmental_Module::i2CWriteOneByte(BME680_GAS_WAIT_0, Turta_Environmental_Module::calculateHeatDuration(heatDuration));
  delay(1);

  // Set heater temperature
  Turta_Environmental_Module::i2CWriteOneByte(BME680_RES_HEAT_0, Turta_Environmental_Module::calculateHeaterResistance(heatTemperature));
  delay(1);

  // Set mode to forced mode
  configValue = Turta_Environmental_Module::i2CReadOneByte(BME680_CTRL_MEAS);
  configValue |= OperationModes_ForcedMode;
  Turta_Environmental_Module::i2CWriteOneByte(BME680_CTRL_MEAS, configValue);
  delay(1);
}

void Turta_Environmental_Module::readCalibrationData() {
  // Temperature calibration
  calT1 = Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_T1_LSB_REG);
  calT2 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_T2_LSB_REG);
  calT3 = Turta_Environmental_Module::i2CReadOneByte(BME680_T3_REG);

  // Pressure calibration
  calP1 = Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_P1_LSB_REG);
  calP2 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_P2_LSB_REG);
  calP3 = Turta_Environmental_Module::i2CReadOneByte(BME680_P3_REG);
  calP4 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_P4_LSB_REG);
  calP5 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_P5_LSB_REG);
  calP6 = Turta_Environmental_Module::i2CReadOneByte(BME680_P6_REG);
  calP7 = Turta_Environmental_Module::i2CReadOneByte(BME680_P7_REG);
  calP8 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_P8_LSB_REG);
  calP9 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_P9_LSB_REG);
  calP10 = Turta_Environmental_Module::i2CReadOneByte(BME680_P10_REG);

  // Humidity calibration
  calH1 = (uint16_t)(Turta_Environmental_Module::i2CReadOneByte(BME680_H1_MSB_REG) << 4 | (Turta_Environmental_Module::i2CReadOneByte(BME680_H1_LSB_REG) & 0x0F));
  calH2 = (uint16_t)(Turta_Environmental_Module::i2CReadOneByte(BME680_H2_MSB_REG) << 4 | ((Turta_Environmental_Module::i2CReadOneByte(BME680_H2_LSB_REG)) >> 4));
  calH3 = Turta_Environmental_Module::i2CReadOneByte(BME680_H3_REG);
  calH4 = Turta_Environmental_Module::i2CReadOneByte(BME680_H4_REG);
  calH5 = Turta_Environmental_Module::i2CReadOneByte(BME680_H5_REG);
  calH6 = Turta_Environmental_Module::i2CReadOneByte(BME680_H6_REG);
  calH7 = Turta_Environmental_Module::i2CReadOneByte(BME680_H7_REG);

  // Gas calibration
  calGH1 = Turta_Environmental_Module::i2CReadOneByte(BME680_GH1_REG);
  calGH2 = (int16_t)Turta_Environmental_Module::i2CReadTwoBytesAsInt(BME680_GH2_LSB_REG);
  calGH3 = Turta_Environmental_Module::i2CReadOneByte(BME680_GH3_REG);

  // Heat calibration
  calResHeatRange = (ushort)((Turta_Environmental_Module::i2CReadOneByte(BME680_RES_HEAT_RANGE) & 0x30) / 16);
  calResHeatVal = Turta_Environmental_Module::i2CReadOneByte(BME680_RES_HEAT_VAL);
  calRangeSwErr = (ushort)((Turta_Environmental_Module::i2CReadOneByte(BME680_RANGE_SW_ERR) & 0xF0) / 16);
}

double Turta_Environmental_Module::compensateTemperature(int tempADC) {
  double val, var1, var2;

  var1 = (((tempADC / 16384.0) - (calT1 / 1024.0)) * calT2);
  var2 = ((((tempADC / 131072.0) - (calT1 / 8192.0)) * ((tempADC / 131072.0) - (calT1 / 8192.0))) * (calT3 * 16.0));
  fineTemperature = (var1 + var2);
  val = fineTemperature / 5120.0;

  return val;
}

double Turta_Environmental_Module::compensatePressure(int presADC) {
  double val, var1, var2, var3;

  var1 = (fineTemperature / 2.0) - 64000.0;
  var2 = var1 * var1 * (calP6 / 131072.0);
  var2 = var2 + (var1 * calP5 * 2.0);
  var2 = (var2 / 4.0) + (calP4 * 65536.0);
  var1 = (((calP3 * var1 * var1) / 16384.0) + (calP2 * var1)) / 524288.0;
  var1 = (1.0 + (var1 / 32768.0)) * calP1;
  val = 1048576.0f - presADC;

  if (var1 != 0)   {
    val = ((val - (var2 / 4096.0)) * 6250.0) / var1;
    var1 = (calP9 * val * val) / 2147483648.0;
    var2 = val * (calP8 / 32768.0);
    var3 = (val / 256.0) * (val / 256.0) * (val / 256.0) * (calP10 / 131072.0);
    val = val + (var1 + var2 + var3 + (calP7 * 128.0)) / 16.0;
  }
  else
    val = 0;

  return val;
}        

double Turta_Environmental_Module::compensateHumidity(int humADC) {
  double val, var1, var2, var3, var4, temp_comp;

  temp_comp = fineTemperature / 5120.0;
  var1 = humADC - ((calH1 * 16.0) + ((calH3 / 2.0) * temp_comp));
  var2 = var1 * (((calH2 / 262144.0) * (1.0 + ((calH4 / 16384.0) * temp_comp) + ((calH5 / 1048576.0) * temp_comp * temp_comp))));
  var3 = calH6 / 16384.0;
  var4 = calH7 / 2097152.0;
  val = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);

  if (val > 100.0)
    val = 100.0;
  else if (val < 0.0)
    val = 0.0;

  return val;
}

void Turta_Environmental_Module::forceRead(bool gasMeasurementEnabled) {
  byte temp;

  Turta_Environmental_Module::setGasMeasurement(gasMeasurementEnabled);

  temp = Turta_Environmental_Module::i2CReadOneByte(BME680_CTRL_MEAS);
  temp |= OperationModes_ForcedMode;
  Turta_Environmental_Module::i2CWriteOneByte(BME680_CTRL_MEAS, temp);
            
  while(Turta_Environmental_Module::getMeasuringStatus())
    delay(1);

  if (gasMeasurementEnabled) {
    for (int d = 0; d < heatDur; d++)
      delay(1);

    while (Turta_Environmental_Module::getGasMeasuringStatus())
      delay(1);
  }
}

double Turta_Environmental_Module::readTemperature() {
  int tempADC;

  Turta_Environmental_Module::forceRead(false);

  tempADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_MSB) << 12) | (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_LSB) << 4) | (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_XLSB) >> 4);

  return Turta_Environmental_Module::compensateTemperature(tempADC);
}

double Turta_Environmental_Module::readHumidity() {
  int humADC;

  Turta_Environmental_Module::forceRead(false);

  humADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_HUM_MSB) << 8) | (Turta_Environmental_Module::i2CReadOneByte(BME680_HUM_LSB));

  return Turta_Environmental_Module::compensateHumidity(humADC);
}

double Turta_Environmental_Module::readPressure() {
  int presADC;

  Turta_Environmental_Module::forceRead(false);

  presADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_PRESS_MSB) << 12) | (Turta_Environmental_Module::i2CReadOneByte(BME680_PRESS_LSB) << 4) | (Turta_Environmental_Module::i2CReadOneByte(BME680_PRESS_XLSB) >> 4);

  return Turta_Environmental_Module::compensatePressure(presADC);
}

double Turta_Environmental_Module::calcAltitude(double meanSeaLevelPressureInBar) {
  double phPa = Turta_Environmental_Module::readPressure() / 100;

  return 44330.0 * (1.0 - pow((phPa / meanSeaLevelPressureInBar), 0.1903));
}

double Turta_Environmental_Module::readGasResistance() {
  ushort gasRange;
  int gasResADC, tempADC;
  double val;
            
  Turta_Environmental_Module::forceRead(true);

  tempADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_MSB) << 12) | (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_LSB) << 4) | (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_XLSB) >> 4);
  gasResADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_GAS_R_MSB) << 2) | (Turta_Environmental_Module::i2CReadOneByte(BME680_GAS_R_LSB) >> 6);
  gasRange = (Turta_Environmental_Module::i2CReadOneByte(BME680_GAS_R_LSB) & 0x0F);

  calAmbTemp = (int16_t)(Turta_Environmental_Module::compensateTemperature(tempADC));
  val = Turta_Environmental_Module::calculateGasResistance(gasResADC, gasRange);

  return val;
}

void Turta_Environmental_Module::readTPH(double & t, double & p, double & h) {
  int tempADC, humADC, presADC;

  Turta_Environmental_Module::forceRead(false);

  tempADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_MSB) << 12) | (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_LSB) << 4) | (Turta_Environmental_Module::i2CReadOneByte(BME680_TEMP_XLSB) >> 4);

  presADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_PRESS_MSB) << 12) | (Turta_Environmental_Module::i2CReadOneByte(BME680_PRESS_LSB) << 4) | (Turta_Environmental_Module::i2CReadOneByte(BME680_PRESS_XLSB) >> 4);

  humADC = (Turta_Environmental_Module::i2CReadOneByte(BME680_HUM_MSB) << 8) | (Turta_Environmental_Module::i2CReadOneByte(BME680_HUM_LSB));

  t = Turta_Environmental_Module::compensateTemperature(tempADC);
  p = Turta_Environmental_Module::compensatePressure(presADC);
  h = Turta_Environmental_Module::compensateHumidity(humADC);
}

void Turta_Environmental_Module::setGasMeasurement(bool state) {
  byte configValue;
  configValue = Turta_Environmental_Module::i2CReadOneByte(BME680_CTRL_GAS_1);

  if (state)
    configValue |= 0b00010000;
  else
    configValue &= 0b11101111;

  Turta_Environmental_Module::i2CWriteOneByte(BME680_CTRL_GAS_1, configValue);
}

void Turta_Environmental_Module::selectHeaterProfileSetPoint(byte heaterProfileSetPoint) {
  Turta_Environmental_Module::i2CWriteOneByte(BME680_CTRL_GAS_1, (byte)heaterProfileSetPoint);
}

bool Turta_Environmental_Module::getMeasuringStatus() {
  byte readValue = Turta_Environmental_Module::i2CReadOneByte(BME680_EAS_STATUS_0);

  return ((readValue & 0b00100000) == 0b00100000) ? true : false;
}

bool Turta_Environmental_Module::getGasMeasuringStatus() {
  byte readValue = Turta_Environmental_Module::i2CReadOneByte(BME680_EAS_STATUS_0);

  return ((readValue & 0b01000000) == 0b01000000) ? true : false;
}

bool Turta_Environmental_Module::getNewDataStatus() {
  byte readValue = Turta_Environmental_Module::i2CReadOneByte(BME680_EAS_STATUS_0);

  return ((readValue & 0b10000000) == 0b10000000) ? true : false;
}

double Turta_Environmental_Module::calculateGasResistance(int gasResADC, ushort gasRange) {
  double var1 = (1340.0 + 5.0 * calRangeSwErr) * const_array1[gasRange];
  double gasres = var1 * const_array2[gasRange] / (gasResADC - 512.0 + var1);

  return gasres;
}
        
byte Turta_Environmental_Module::calculateHeaterResistance(uint targetTemp) {
  double var1 = 0, var2 = 0, var3 = 0, var4 = 0, var5 = 0;
  byte res_heat = 0;

  if (targetTemp > 400) // Maximum temperature
    targetTemp = 400;

  var1 = (calGH1 / 16.0) + 49.0;
  var2 = ((calGH2 / 32768.0) * 0.0005) + 0.00235;
  var3 = calGH3 / 1024.0;
  var4 = var1 * (1.0 + (var2 * targetTemp));
  var5 = var4 + (var3 * calAmbTemp);
  res_heat = (byte)(3.4 * ((var5 * (4 / (4 + calResHeatRange)) * (1 / (1 + (calResHeatVal * 0.002)))) - 25));

  return res_heat;
}

byte Turta_Environmental_Module::calculateHeatDuration(int dur) {
  ushort factor = 0, durval;

  if (dur >= 0xFC) {
    durval = 0xFF; // Max duration
  }
  else {
    while (dur > 0x3F)
    {
      dur = dur / 4;
      factor += 1;
    }
    durval = (ushort)(dur + (factor * 64));
  }

  return (byte)durval;
}

// I2C Communication
void Turta_Environmental_Module::i2CWriteOneByte(byte reg, byte data) {
  Wire.beginTransmission((uint8_t)BME680_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte Turta_Environmental_Module::i2CReadOneByte(byte reg) {
  byte data;
  Wire.beginTransmission((uint8_t)BME680_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)BME680_I2C_ADDRESS, (uint8_t)1);
  data = Wire.read();
  Wire.endTransmission();
  return data;
}

int Turta_Environmental_Module::i2CReadTwoBytesAsInt(byte reg) {
  int i = 0;
  byte data[2] = {0};
  Wire.beginTransmission((uint8_t)BME680_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)BME680_I2C_ADDRESS, (uint8_t)2);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
  return data[0] | (data[1] << 8);
}