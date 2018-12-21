/*
  Turta_Photocoupler_Module.h - IoT Node - ESP32 Library for Photocoupler.
  Created by Turta, Dec 21, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_Photocoupler_Module_h
#define Turta_Photocoupler_Module_h

#include "Arduino.h"

class Turta_Photocoupler_Module
{
  public:
    Turta_Photocoupler_Module();
    void begin();
    bool input1Read();
    bool input2Read();
};

#endif