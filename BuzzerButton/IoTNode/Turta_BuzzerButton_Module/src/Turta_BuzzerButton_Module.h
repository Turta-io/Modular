/*
  Turta_BuzzerButton_Module.h - IoT Node - ESP32 Library for Buzzer Button.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_BuzzerButton_Module_h
#define Turta_BuzzerButton_Module_h

#include "Arduino.h"

class Turta_BuzzerButton_Module
{
  public:
    Turta_BuzzerButton_Module();
    void begin();
    bool readButton();
    void buzzerTone(int frequency, short dutyCycle);
    void buzzerStop();
    void buzzerTonePeriod(int frequency, short dutyCycle, int durationMs);
};

#endif