#!/usr/bin/env python3

#This sample demonstrates reading the 3D acceleration data.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_Accel_Tilt_Module

#Initialize
accel = Turta_Accel_Tilt_Module.AccelTiltSensor(slot = 1)

try:
    while True:
        #Read X, Y and Z-Axis G values in one shot
        accel_xyz = accel.read_accel_xyz()

        #Read the readings
        print("X-Axis..........: " + str(round(accel_xyz[0], 2)) + "G")
        print("Y-Axis..........: " + str(round(accel_xyz[1], 2)) + "G")
        print("Z-Axis..........: " + str(round(accel_xyz[2], 2)) + "G")

        #Wait
        print("-----")
        sleep(0.2)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
