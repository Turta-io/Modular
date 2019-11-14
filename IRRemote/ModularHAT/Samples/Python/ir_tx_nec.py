#!/usr/bin/env python3

#This sample demonstrates sending infrared remote controller messages using NEC protocol.
#Install Modular library with "pip3 install turta-modular"

from time import sleep
from turta_modular import Turta_IRRemote_Module

#Initialize
ir = Turta_IRRemote_Module.IRRemoteTx(slot = 1)

try:
    while True:
        #Prepare the payload
        payload = [0x01, 0xFE, 0x02, 0xFD]

        #Send remote controller message in NEC protocol
        ir.write(Turta_IRRemote_Module.PROTOCOLS.NEC, payload)

        #Print the payload
        print("IR Message Sent..: " + str(payload))

        #Wait
        sleep(2.0)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
