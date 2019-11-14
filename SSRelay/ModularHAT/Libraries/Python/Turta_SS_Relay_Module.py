# Turta Modular Helper for Raspbian.
# Distributed under the terms of the MIT license.

# Python Library for Solid State Relay Module.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

import RPi.GPIO as GPIO

class Relay:
    """Solid State Relay."""

    #Variables
    is_initialized = False

    #Relay Pins
    relay = None
    relay_1 = 27
    relay_2 = 12

    #Initialize

    def __init__(self, slot):
        """Initiates the Solid State Relay Module.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')

        if slot == 1:
            self.relay = self.relay_1
        elif slot == 2:
            self.relay = self.relay_2

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.relay, GPIO.OUT, initial = GPIO.LOW)
        self.is_initialized = True
        return

    #Relay Write Methods

    def write(self, st):
        """Controls the relay.

        Parameters:
        st (bool): Relay state (True or False)"""

        GPIO.output(self.relay, GPIO.HIGH if st else GPIO.LOW)
        return

    def toggle(self):
        """Inverts the relay's state."""

        self.write(not self.read())
        return

    #Relay Read Methods

    def read(self):
        """Reads the relay state.

        Returns:
        bool: Relay state (True or False)"""

        return GPIO.input(self.relay)

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                GPIO.output(self.relay, GPIO.LOW)
                GPIO.cleanup()
                del self.is_initialized
        except:
            pass
