#!/usr/bin/env python3

#This sample demonstrates detecting hand gestures using GPIO interrupt.
#Install Modular library with "pip3 install turta-modular"

#Set 'gesture_int' 17 for slot 1, 25 for slot 2

from time import sleep
import RPi.GPIO as GPIO
from turta_modular import Turta_ALS_Gesture_Module

#Gesture Event
def gesture_event(ch):
    g = light.read_gesture()
    if g is not None:
        print("Gesture Detected: " + str(g))

#Interrupt Pin
#Slot 1 uses int pin 17, slot 2 uses 25
gesture_int = 17

#Initialize
light = Turta_ALS_Gesture_Module.LightSensor(slot = 1, mode = Turta_ALS_Gesture_Module.MODES.PROX_GES, bypass_int = True)
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(gesture_int, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.add_event_detect(gesture_int, GPIO.FALLING, callback=gesture_event, bouncetime=1)

#Add your configuration code here

try:
    while True:
        #Add your code here

        #Wait
        sleep(1.0)

#Exit on CTRL+C
except KeyboardInterrupt:
    GPIO.cleanup()
    print('Bye.')
