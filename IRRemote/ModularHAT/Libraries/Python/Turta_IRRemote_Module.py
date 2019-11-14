# Turta Modular Helper for Raspbian
# Distributed under the terms of the MIT license.

# Python Library for IR Remote Controller Transmitter.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

from enum import Enum
from time import sleep
import RPi.GPIO as GPIO

#Enumerations

#PROTOCOLS: IR Remote Controller Protocols
class PROTOCOLS(Enum):
    NEC = 0

class IRRemoteTx(object):
    """IR Remote Controller Transmitter."""

    #Variables
    is_initialized = False

    #IR LED Pin
    ir_tx = None
    ir_tx_1 = 18
    ir_tx_2 = 13

    #Initialize

    def __init__(self, slot):
        """Initiates the IR Transmitter.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')

        if slot == 1:
            self.ir_tx = self.ir_tx_1
        elif slot == 2:
            self.ir_tx = self.ir_tx_2

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.ir_tx, GPIO.OUT, initial = GPIO.LOW)
        self.is_initialized = True
        return

    #Data Transmit

    def write(self, protocol, command):
        """Sends the IR remote controller signal using the selected protocol.

        Parameters:
        protocol (PROTOCOLS): Remote controller protocol
        command (byte array): Command to be sent"""

        if protocol not in PROTOCOLS:
            raise ValueError('protocol is not a member of PROTOCOLS.')

        if (protocol == PROTOCOLS.NEC):
            self._transmit_nec(command)

        return

    #IR Remote Controller Protocols

    #NEC Protocol
    def _transmit_nec(self, cmd):
        """Encodes and transmits the data in NEC protocol.

        Parameters:
        cmd (byte array): Command to be sent (2 Address + 2 Data = 4 Bytes)"""

        start_space_lenght_us_NEC = 0.0045 #4.5us
        burst_1_space_us = 0.001600 #0.001682s
        burst_0_space_us = 0.000534 #0.000534s

        #Leading Pulse
        self._nec_bit(320) #346

        #Delay
        sleep(start_space_lenght_us_NEC)

        #Data Bits
        for c in cmd: #4 Bytes
            for b in range(8): #8 Bits LSB First
                self._nec_bit(22)
                if (c & (1 << b)): #if 1
                    sleep(burst_1_space_us) #1.687mS space
                else: #if 0
                    sleep(burst_0_space_us) #562uS space

        #Stop Bit
        self._nec_bit(22)
        return

    def _nec_bit(self, cnt):
        """NEC Protocol pulse.

        Parameters:
        cnt (int): Number of pulses"""

        for x in range(cnt):
            GPIO.output(self.ir_tx, GPIO.HIGH)
            GPIO.output(self.ir_tx, GPIO.HIGH)
            GPIO.output(self.ir_tx, GPIO.LOW)
            GPIO.output(self.ir_tx, GPIO.LOW)
            GPIO.output(self.ir_tx, GPIO.LOW)
            GPIO.output(self.ir_tx, GPIO.LOW)
        return

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                GPIO.output(self.ir_tx, GPIO.LOW)
                GPIO.cleanup()
                del self.is_initialized
        except:
            pass
