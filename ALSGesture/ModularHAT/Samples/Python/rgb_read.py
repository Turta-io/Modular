#!/usr/bin/env python3

#This sample demonstrates reading RGB light.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_ALS_Gesture_Module

#Initialize
light = Turta_ALS_Gesture_Module.LightSensor(slot = 1)

try:
    while True:
        #Read clear & RGB light values
        crgb = light.read_crgb_light()

        #Print the readings
        print("Clear...........: " + str(crgb[0]))
        print("Red.............: " + str(crgb[1]))
        print("Green...........: " + str(crgb[2]))
        print("Blue............: " + str(crgb[3]))

        #Wait
        print("-----")
        sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
