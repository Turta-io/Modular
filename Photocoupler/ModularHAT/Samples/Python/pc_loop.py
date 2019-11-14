#!/usr/bin/env python3

#This sample demonstrates reading input states of photocoupler in a loop.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_Photocoupler_Module

#Initialize
photocoupler = Turta_Photocoupler_Module.PhotocouplerInputs(slot = 1)

try:
    while True:
        #Read photocoupler input states
        pc1 = photocoupler.read(1)
        pc2 = photocoupler.read(2)

        #Print the readings
        print("Photocoupler 1..: " + ("High." if pc1 else "Low."))
        print("Photocoupler 2..: " + ("High." if pc2 else "Low."))

        #Wait
        print("-----")
        sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
