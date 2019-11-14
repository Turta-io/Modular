# Turta Modular Helper for Raspbian.
# Distributed under the terms of the MIT license.

# Python Library for PIR Motion Module.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

import RPi.GPIO as GPIO

class PIRMotion:
    """PIR Motion Module"""

    #Variables
    is_initialized = False

    #PIR Interrupt Pin
    pir = None
    pir_1 = 17
    pir_2 = 25

    #Initialize

    def __init__(self, slot):
        """Initiates the PIR Motion Detect Module.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')

        if slot == 1:
            self.pir = self.pir_1
        elif slot == 2:
            self.pir = self.pir_2

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pir, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
        self.is_initialized = True
        return

    #PIR Read Methods

    def read(self):
        """Reads the PIR input state.

        Returns:
        bool: PIR input state (True for motion detect, False for no motion)"""

        return GPIO.input(self.pir)

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                GPIO.cleanup()
                del self.is_initialized
        except:
            pass
