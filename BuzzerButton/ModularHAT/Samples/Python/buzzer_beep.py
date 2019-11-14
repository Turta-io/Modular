#!/usr/bin/env python3

#This sample demonstrates using the piezo sounder.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_Buzzer_Button_Module

#Initialize
bb = Turta_Buzzer_Button_Module.BuzzerButton(slot = 1)

try:
    while True:
        #Generate sound
        #You can change the frequency between 1000 to 10000 Hz
        bb.start(1000)
        sleep(0.2)
        bb.start(2000)
        sleep(0.1)
        bb.stop()

        #Wait
        sleep(2.0)

        #Alternatively, beep method generates a 'be-beep' tone
        bb.beep()

        #Wait
        sleep(2.0)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
