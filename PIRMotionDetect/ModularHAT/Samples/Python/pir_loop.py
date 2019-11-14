#!/usr/bin/env python3

#This sample demonstrates reading motion state in a loop.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_PIR_Motion_Module

#Initialize
pir = Turta_PIR_Motion_Module.PIRMotion(slot = 1)

try:
    while True:
        #Read PIR motion state
        motion = pir.read()

        #Print the reading
        print("Motion state....: " + ("Active." if motion else "Passive."))

        #Wait
        sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
