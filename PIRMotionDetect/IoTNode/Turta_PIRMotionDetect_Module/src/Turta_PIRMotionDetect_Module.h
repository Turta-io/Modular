/*
  Turta_PIRMotionDetect_Module.h - IoT Node - ESP32 Library for PIR Motion Detect.
  Created by Turta, Sep 30, 2018.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_PIRMotionDetect_Module_h
#define Turta_PIRMotionDetect_Module_h

#include "Arduino.h"

class Turta_PIRMotionDetect_Module
{
  public:
    Turta_PIRMotionDetect_Module();
    void begin();
    bool pirRead();
};

#endif