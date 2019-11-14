#!/usr/bin/env python3

#This sample demonstrates detecting hand gestures in a loop.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_ALS_Gesture_Module

#Initialize
light = Turta_ALS_Gesture_Module.LightSensor(slot = 1, mode = Turta_ALS_Gesture_Module.MODES.PROX_GES)

try:
    while True:
        #Check gestures
        g = light.read_gesture()
        if g is not None:
            print("Gesture Detected: " + str(g))

        #Wait
        sleep(0.05)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
