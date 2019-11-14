# Turta Modular Helper for Raspbian.
# Distributed under the terms of the MIT license.

# Python Library for Photocoupler Module.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

import RPi.GPIO as GPIO

class PhotocouplerInputs:
    """Photocoupler Module"""

    #Variables
    is_initialized = False

    #Photocoupler Pins
    pc1, pc2 = None, None
    pc1_1, pc2_1 = 17, 27
    pc1_2, pc2_2 = 25, 12

    #Initialize

    def __init__(self, slot):
        """Initiates the Photocoupler Module.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')

        if slot == 1:
            self.pc1 = self.pc1_1
            self.pc2 = self.pc2_1
        elif slot == 2:
            self.pc1 = self.pc1_2
            self.pc2 = self.pc2_2

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pc1, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
        GPIO.setup(self.pc2, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
        self.is_initialized = True
        return

    #Photocoupler Read Methods

    def read(self, ch):
        """Reads the photocoupler input state.

        Parameters:
        ch (byte): Photocoupler channel (1 or 2)

        Returns:
        bool: Photocoupler input state (True for high, False for low)"""

        if (ch == 1):
            return GPIO.input(self.pc1)
        elif (ch == 2):
            return GPIO.input(self.pc2)

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                GPIO.cleanup()
                del self.is_initialized
        except:
            pass
