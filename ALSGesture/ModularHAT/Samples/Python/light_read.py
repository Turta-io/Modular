#!/usr/bin/env python3

#This sample demonstrates reading the ambient light data.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_ALS_Gesture_Module

#Initialize
light = Turta_ALS_Gesture_Module.LightSensor(slot = 1)

try:
    while True:
        #Read ambient light
        al = light.read_ambient_light()

        #Print the reading
        print("Ambient Light...: " + str(al))

        #Wait
        sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
