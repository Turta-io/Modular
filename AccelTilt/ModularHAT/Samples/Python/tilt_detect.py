#!/usr/bin/env python3

#This sample demonstrates detecting tilt without using I2C bus.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_Accel_Tilt_Module

#Initialize
accel = Turta_Accel_Tilt_Module.AccelTiltSensor(slot = 1)

try:
    while True:
        #Read tilt states in one shot
        tilt_xy = accel.read_tilt_xy()

        #Print the readings
        print("X-Tilt..........: " + ("Tilt detected." if tilt_xy[0] else "No tilt."))
        print("Y-Tilt..........: " + ("Tilt detected." if tilt_xy[1] else "No tilt."))

        #Wait
        print("-----")
        sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
