#!/usr/bin/env python3

#This sample demonstrates reading onboard button press state in a loop.
#Install Modular library with "pip3 install turta-modular"

import time
from turta_modular import Turta_Buzzer_Button_Module

#Initialize
bb = Turta_Buzzer_Button_Module.BuzzerButton(slot = 1)

try:
    while True:
        #Read button state
        button_state = bb.read()

        #Print the reading
        print("Button State....: " + ("Pressed." if button_state else "Released."))

        #Wait
        time.sleep(0.5)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
