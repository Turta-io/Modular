#!/usr/bin/env python3

#This sample demonstrates toggling the solid state relay.
#Do not toggle DC motors or solenoids frequently.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_SS_Relay_Module

#Initialize
relay = Turta_SS_Relay_Module.Relay(slot = 1)

try:
    while True:
        #Option one: Toggle the relay with 'write' method
        
        #Turn relay on
        relay.write(True)

        #Print relay state
        print("Relay State.....: " + ("On." if relay.read() else "Off."))

        #Wait
        sleep(2.0)

        #Turn relay off
        relay.write(False)

        #Print relay state
        print("Relay State.....: " + ("On." if relay.read() else "Off."))

        #Wait
        sleep(2.0)

        #Option two: Toggle the relay with 'toggle' method
        #Each method call inverts the relay's state

        #Toggle relay
        relay.toggle()

        #Print relay state
        print("Relay State.....: " + ("On." if relay.read() else "Off."))

        #Wait
        sleep(2.0)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
