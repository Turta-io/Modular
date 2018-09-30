/*
  Turta_SSRelay_Module.h - IoT Node - ESP32 Library for Solid State Relay.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_SSRelay_Module_h
#define Turta_SSRelay_Module_h

#include "Arduino.h"

class Turta_SSRelay_Module
{
  public:
    Turta_SSRelay_Module();
    void begin();
    void relayWrite(bool state);
    bool relayRead();
};

#endif