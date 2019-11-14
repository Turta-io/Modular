# Turta Modular Helper for Raspbian.
# Distributed under the terms of the MIT license.

# Python Library for Buzzer Button Module.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

import RPi.GPIO as GPIO
from time import sleep

class BuzzerButton(object):
    """Buzzer Button Module."""

    #Variables
    is_initialized = False
    pwm = None

    #Pins
    button, sounder = None, None
    button_1, sounder_1 = 17, 18
    button_2, sounder_2 = 25, 13

    #Initialize

    def __init__(self, slot):
        """Initiates the Buzzer Button Module.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')

        if slot == 1:
            self.button = self.button_1
            self.sounder = self.sounder_1
        elif slot == 2:
            self.button = self.button_2
            self.sounder = self.sounder_2

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.button, GPIO.IN, pull_up_down = GPIO.PUD_UP)
        GPIO.setup(self.sounder, GPIO.OUT, initial = GPIO.LOW)
        self.pwm = GPIO.PWM(self.sounder, 5000)
        self.pwm.start(0)
        self.is_initialized = True
        return

    #Button Read Methods

    def read(self):
        """Reads the button press state.

        Returns:
        bool: Button press state (True if pressed, False if not)"""

        return not GPIO.input(self.button)

    #Buzzer Control Mehtods

    def start(self, freq = 5000):
        """Generates sound.

        Parameters:
        freq (int): Frequency (1000 to 10000, default is 5000)"""

        self.pwm.ChangeFrequency(freq)
        self.pwm.start(50)
        return

    def stop(self):
        """Stops generating sound."""

        self.pwm.stop()
        return

    def beep(self):
        """Plays a 'be beep' tone."""
        self.start(1000)
        sleep(0.15)
        self.start(10000)
        sleep(0.07)
        self.stop()

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                self.pwm.stop()
                GPIO.cleanup()
                del self.is_initialized
        except:
            pass
