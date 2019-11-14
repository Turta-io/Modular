# Python Libraries
This directory includes Python libraries for ALS Gesture Module.

## Index
* __Turta_ALS_Gesture_Module.py:__ Python Library for Broadcom APDS-9960 Ambient & RGB Light, Proximity and Gesture Detection Sensor.

## Installation of Python Libraries
* Use 'pip3 install turta-modular' to download and install libraries automatically.
* Use 'pip3 install --upgrade --user turta-modular' to update your libraries.
* Use 'pip3 uninstall turta-modular' to uninstall the libraries.
* If you wish to install libraries manually, copy the ingredients of Python folder to the project folder.

## Dependencies for Python Libraries
The package installer installs other libraries required for the module's operation.
* We're using 'SMBus' for I2C communication. To install it manually, type 'sudo pip3 install smbus' to the terminal.
* We're using 'spidev' for SPI communication. To install it manually, type 'sudo pip3 install spidev' to the terminal.
* We're using 'RPi.GPIO' for GPIO access. To install it manually, type 'pip3 install RPi.GPIO' to the terminal.
* We're using Python 3 for the libraries and samples.

## Raspberry Pi Configuration
* You should enable SPI and I2C from the Raspberry Pi's configuration. To do so, type 'sudo raspi-config' to the terminal, then go to 'Interfacing Options' and enable both SPI and I2C.

## Documentation
Visit [docs.turta.io](https://docs.turta.io) for documentation.
