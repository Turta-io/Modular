#!/usr/bin/env python3

#This sample demonstrates measuring proximity.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_ALS_Gesture_Module

#Initialize
light = Turta_ALS_Gesture_Module.LightSensor(slot = 1, mode = Turta_ALS_Gesture_Module.MODES.PROX)

try:
    while True:
        #Read proximity
        proximity = light.read_proximity()

        #Print the reading
        print("Proximity.......: " + str(proximity))

        #Wait
        sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
