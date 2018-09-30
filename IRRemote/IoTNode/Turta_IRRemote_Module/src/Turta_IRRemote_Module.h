/*
  Turta_IRRemote_Module.h - IoT Node - ESP32 Library for IR Remote Transceiver.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_IRRemote_Module_h
#define Turta_IRRemote_Module_h

#include "Arduino.h"

class Turta_IRRemote_Module
{
  public:
    Turta_IRRemote_Module();
    void begin();
    void sendNECCommand(byte d0, byte d1, byte d2, byte d3);
    bool isMessageReceived();
    void getData(byte &d1, byte &d2, byte &d3, byte &d4);
    void setReception(bool enabled);
  private:
    void irStartBitNEC();
    void irBitNEC();
    static void onRxTimer();
    static void irStateMachine();
};

#endif