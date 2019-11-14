# Python Samples
This directory includes Python samples for ALS Gesture Module.

## Index
* __gesture_event.py:__ Demonstrates detecting hand gestures using GPIO interrupt.
* __gesture_loop.py:__ Demonstrates detecting hand gestures in a loop.
* __light_read.py:__ Demonstrates reading the ambient light data.
* __proximity_read.py:__ Demonstrates measuring proximity.
* __rgb_read.py:__ Demonstrates reading RGB light.

## Running the Python Samples
* Copy the sample code to your Raspberry Pi.
* Install the libraries with 'pip3 install turta-modular' command.
* Run the sample with 'python3 <Sample_Name>.py' command.
* Exit from the sample using CTRL+C or ^C key.

## Raspberry Pi Configuration
* You should enable SPI and I2C from the Raspberry Pi's configuration. To do so, type 'sudo raspi-config' to the terminal, then go to 'Interfacing Options' and enable both SPI and I2C.

## Documentation
Visit [docs.turta.io](https://docs.turta.io) for documentation.
