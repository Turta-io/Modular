#!/usr/bin/env python3

#This sample demonstrates reading motion detect state using interrupt.
#Install Modular library with "pip3 install turta-modular"

#Set 'pir' 17 for slot 1, 25 for slot 2

from time import sleep
import RPi.GPIO as GPIO

#Motion Detect Event
def motion_event(ch):
    print ("Motion state is " + ("active. " if GPIO.input(pir) else "passive."))

#PIR Module Pin
#Slot 1 uses pin 17, slot 2 uses pin 25
pir = 17

#Initialize
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(pir, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.add_event_detect(pir, GPIO.BOTH, callback=motion_event, bouncetime=10)
#GPIO Edge detection options: GPIO.BOTH, GPIO.RISING or GPIO.FALLING

#Add your configuration code here

try:
    while True:
        #Add your code here

        #Wait
        sleep(1.0)

#Exit on CTRL+C
except KeyboardInterrupt:
    print('Bye.')
