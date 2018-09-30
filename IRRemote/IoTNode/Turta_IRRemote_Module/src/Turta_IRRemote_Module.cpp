/*
  Turta_IRRemote_Module.cpp - IoT Node - ESP32 Library for IR Remote Transceiver.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include "Turta_IRRemote_Module.h"

#define INT_PIN GPIO_NUM_37
#define PWM_PIN GPIO_NUM_2

// Global constants.
const int startSpaceLenghtUsNEC = 4500;
const int burstLenghtUsNEC = 572;
const short bitHiDelayNEC = 9;
const short bitLoDelayNEC = 16;
const int burst1SpaceUs = 1700;
const int burst0SpaceUs = 570;

// Global varibles.
short irReceiveBuffer[4] = {0};
short irReceivedByte[4] = {0};
char irReceivedFlag;
short irCommandQueue = 0;
char irState = 0;
int  irTimerCount;
char irBitCount;
char irByteCount;
bool irReceivedInt;

// Timer variables.
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

Turta_IRRemote_Module::Turta_IRRemote_Module() {}

void Turta_IRRemote_Module::begin() {
  // Configure pins.
  pinMode(INT_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);

  // Configure timer.
  timer = timerBegin(3, 80, true);
  timerAttachInterrupt(timer, &Turta_IRRemote_Module::onRxTimer, true);
  timerAlarmWrite(timer, 100, true);
}

// IR Remote Transmitter

void Turta_IRRemote_Module::irStartBitNEC() {
  int i;
  for(i = 0; i < 352; i++)
  {
    gpio_set_level(PWM_PIN, 1);
    ets_delay_us(bitHiDelayNEC);
    gpio_set_level(PWM_PIN, 0);
    ets_delay_us(bitLoDelayNEC);
  }
}

void Turta_IRRemote_Module::irBitNEC() {
  int i;
  for(i = 0; i < 22; i++)
  {
    gpio_set_level(PWM_PIN, 1);
    ets_delay_us(bitHiDelayNEC);
    gpio_set_level(PWM_PIN, 0);
    ets_delay_us(bitLoDelayNEC);
  }
}

void Turta_IRRemote_Module::sendNECCommand(byte d0, byte d1, byte d2, byte d3) {
  byte ir_byte[4] = {0};
  short bitnum;
  short bytenum;
  ir_byte[0] = d0;
  ir_byte[1] = d1;
  ir_byte[2] = d2;
  ir_byte[3] = d3;

  // Disable interrupts.
  portDISABLE_INTERRUPTS();
  timerAlarmDisable(timer);

  // Leading pulse.
  Turta_IRRemote_Module::irStartBitNEC();
  ets_delay_us(startSpaceLenghtUsNEC);

  // Data bits.
  for(bytenum = 0; bytenum < 4; bytenum++) // 4 bytes to send.
  {
    for(bitnum = 0; bitnum < 8; bitnum++) // 8 bits LSB first.
    {
      Turta_IRRemote_Module::irBitNEC();

      if(ir_byte[bytenum] & (1 << bitnum))
        ets_delay_us(burst1SpaceUs);
      else
        ets_delay_us(burst0SpaceUs);
    }
  }

  // Stop bit.
  Turta_IRRemote_Module::irBitNEC();

  // Enable interrupts.
  portENABLE_INTERRUPTS();
  timerAlarmEnable(timer);
}

// IR Remote Receiver

bool Turta_IRRemote_Module::isMessageReceived() {
  return irReceivedInt;
}

void Turta_IRRemote_Module::getData(byte & d1, byte & d2, byte & d3, byte & d4) {
  d1 = irReceiveBuffer[0];
  d2 = irReceiveBuffer[1];
  d3 = irReceiveBuffer[2];
  d4 = irReceiveBuffer[3];
  irReceiveBuffer[0] = 0;
  irReceiveBuffer[1] = 0;
  irReceiveBuffer[2] = 0;
  irReceiveBuffer[3] = 0;
  irReceivedInt = false;
}

void Turta_IRRemote_Module::irStateMachine() {
  switch(irState) {
    case 0:
    irTimerCount = 0;
    if (gpio_get_level(INT_PIN) == 0)
    irState = 1;
         break;

    case 1:
    if (gpio_get_level(INT_PIN) == 0)
      irTimerCount++;
    if (gpio_get_level(INT_PIN) == 1)
      if ((irTimerCount > 80) && (irTimerCount < 110)) {
        irState = 2;
        irTimerCount = 0;
      }
      else {
        irState = 0;
        irTimerCount = 0;
      }
         break;

    case 2:
    if (gpio_get_level(INT_PIN) == 1)
      irTimerCount ++;
    if (gpio_get_level(INT_PIN) == 0)
      if ((irTimerCount > 37) && (irTimerCount < 53)) {
        irState = 3;
        irTimerCount = 0;
        irBitCount = 0;
        irByteCount = 0;
      }
      else {
        irState = 0;
        irTimerCount = 0;
      }
         break;

    case 3:
    if (gpio_get_level(INT_PIN) == 0)
      irTimerCount ++;
    if (gpio_get_level(INT_PIN) == 1)
      if ((irTimerCount > 3) && (irTimerCount < 7)) {
        irState = 4;
        irTimerCount = 0;
        if (irReceivedFlag == 0xFF) {
          irReceivedFlag = 1;
          irState = 0;
          irReceiveBuffer[0] = irReceivedByte[0];
          irReceiveBuffer[1] = irReceivedByte[1];
          irReceiveBuffer[2] = irReceivedByte[2];
          irReceiveBuffer[3] = irReceivedByte[3];
          irReceivedInt = true;
        }
      }
      else {
        irState = 0;
        irTimerCount = 0;
      }
         break;

      case 4:
      if (gpio_get_level(INT_PIN) == 1)
        irTimerCount ++;
      if (gpio_get_level(INT_PIN) == 0)
        if ((irTimerCount > 3) && (irTimerCount < 19)) {
          irReceivedByte[irByteCount] = (irReceivedByte[irByteCount] >> 1);
          irBitCount ++;
          if (irTimerCount > 8)
            irReceivedByte[irByteCount] += 0x80;
          if (irBitCount == 8) {
            irBitCount = 0;
            irByteCount ++;
          }
          if (irByteCount == 4)
            irReceivedFlag = 0xFF;
          irState = 3;
          irTimerCount = 0;
        }
        else {
          irState = 0;
          irTimerCount = 0;
        }
           break;
   }
}

// Timer.
void IRAM_ATTR Turta_IRRemote_Module::onRxTimer() {
  Turta_IRRemote_Module::irStateMachine();
}

void Turta_IRRemote_Module::setReception(bool enabled) {
  if (enabled)
    timerAlarmEnable(timer);
  else
    timerAlarmDisable(timer);
}