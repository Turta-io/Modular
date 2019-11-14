# Turta Modular Helper for Raspbian.
# Distributed under the terms of the MIT license.

# Python Library for NXP MMA8491Q 3-Axis Accelerometer & Tilt Sensor.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

import RPi.GPIO as GPIO
from smbus import SMBus
import time

class AccelTiltSensor(object):
    "MMA8491Q Accel & Tilt Sensor."

    #Variables
    is_initialized = False

    #Pins
    mma8491q_en, mma8491q_x_int, mma8491q_y_int = None, None, None
    mma8491q_en_1, mma8491q_x_int_1, mma8491q_y_int_1 = 18, 17, 27
    mma8491q_en_2, mma8491q_x_int_2, mma8491q_y_int_2 = 13, 25, 12

    #I2C Slave Address
    I2C_ADDRESS = 0x55

    #Registers
    MMA8491Q_STATUS = 0x00
    MMA8491Q_OUT_X_MSB = 0x01
    MMA8491Q_OUT_Y_MSB = 0x03
    MMA8491Q_OUT_Z_MSB = 0x05

    #I2C Config
    bus = SMBus(1)

    #I2C Communication

    def _read_register_1ubyte(self, reg_addr):
        """Reads data from the I2C device.

        Parameters:
        reg_addr (byte): Read register address

        Returns:
        byte: Response from the device"""

        buffer = self.bus.read_i2c_block_data(self.I2C_ADDRESS, reg_addr, 1)
        return buffer[0]

    def _read_2bytes_rs2(self, reg_addr):
        """Reads 2 bytes from the I2C device, and then shifts two bits right.

        Parameters:
        reg_addr (byte): Read register address

        Returns:
        int: Modified response from the device"""

        buffer = self.bus.read_i2c_block_data(self.I2C_ADDRESS, reg_addr, 2)
        return (buffer[0] << 6) + (buffer[1] >> 2)

    def _read_array(self, reg_addr, cnt):
        """Reads data from the I2C device.

        Parameters:
        reg_addr (byte): Read register start address
        cnt (byte): Read register address

        Returns:
        byte array: Response from the device"""

        return self.bus.read_i2c_block_data(self.I2C_ADDRESS, reg_addr, cnt)

    #Initialization

    def __init__(self, slot):
        """Initiates the MMA8491Q sensor.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        if slot == 1:
            self.mma8491q_en = self.mma8491q_en_1
            self.mma8491q_x_int = self.mma8491q_x_int_1
            self.mma8491q_y_int = self.mma8491q_y_int_1
        elif slot == 2:
            self.mma8491q_en = self.mma8491q_en_2
            self.mma8491q_x_int = self.mma8491q_x_int_2
            self.mma8491q_y_int = self.mma8491q_y_int_2
        GPIO.setup(self.mma8491q_en, GPIO.OUT)
        GPIO.setup(self.mma8491q_x_int, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
        GPIO.setup(self.mma8491q_y_int, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
        self.is_initialized = True

    #Sensor Read Methods

    def _convert_to_g(self, analog_data):
        """Converts raw sensor data to G value.

        Parameters:
        analog_data (int): Raw sensor output data

        Returns:
        int: G Force of the given axis"""

        if ((analog_data & 0x2000) == 0x2000): #If zero or negative G
            return (0x3FFF - analog_data) / -1024.0
        else:                                  #If positive G
            return analog_data / 1024.0

    def read_accel_x(self):
        """Reads the G force of X-axis.

        Returns:
        int: G force of X-axis"""

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)

        while ((self._read_register_1ubyte(self.MMA8491Q_STATUS) & 0x01) != 0x01):
            time.sleep(0.001)
        tempData = self._read_2bytes_rs2(self.MMA8491Q_OUT_X_MSB)
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        return self._convert_to_g(tempData)

    def read_accel_y(self):
        """Reads the G force of Y-axis.

        Returns:
        int: G force of Y-axis"""

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)

        while ((self._read_register_1ubyte(self.MMA8491Q_STATUS) & 0x02) != 0x02):
            time.sleep(0.001)
        tempData = self._read_2bytes_rs2(self.MMA8491Q_OUT_Y_MSB)
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        return self._convert_to_g(tempData)

    def read_accel_z(self):
        """Reads the G force of Z-axis.

        Returns:
        int: G force of Z-axis"""

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)

        while ((self._read_register_1ubyte(self.MMA8491Q_STATUS) & 0x04) != 0x04):
            time.sleep(0.001)
        tempData = self._read_2bytes_rs2(self.MMA8491Q_OUT_Z_MSB)
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        return self._convert_to_g(tempData)

    def read_accel_xyz(self):
        """Reads the G forces of X, Y and Z-axes.

        Returns:
        int array: G forces of X, Y and Z-axes respectively"""

        xyz = [ 0, 0, 0 ]

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)

        while ((self._read_register_1ubyte(self.MMA8491Q_STATUS) & 0x08) != 0x08):
            time.sleep(0.001)
        xyzArray = self._read_array(self.MMA8491Q_OUT_X_MSB, 6)
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        xyz[0] = self._convert_to_g((xyzArray[0] << 6) + (xyzArray[1] >> 2)) #X-Axis
        xyz[1] = self._convert_to_g((xyzArray[2] << 6) + (xyzArray[3] >> 2)) #Y-Axis
        xyz[2] = self._convert_to_g((xyzArray[4] << 6) + (xyzArray[5] >> 2)) #Z-Axis

        return xyz

    def read_tilt_x(self):
        """Reads the X-Axis tilt state.

        Returns:
        bool: True if acceleration is > 0.688g or X-axis > 45, false if not"""

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)
        state = True if GPIO.input(self.mma8491q_x_int) else False
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        return state

    def read_tilt_y(self):
        """Reads the Y-Axis tilt state.

        Returns:
        bool: True if acceleration is > 0.688g or Y-axis > 45, false if not"""

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)
        state = True if GPIO.input(self.mma8491q_y_int) else False
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        return state

    def read_tilt_xy(self):
        """Reads the X and Y-Axis tilt states respectively.

        Returns:
        bool array: True if acceleration is > 0.688g or axis > 45, false if not"""

        xy = [False, False]

        GPIO.output(self.mma8491q_en, GPIO.HIGH)
        time.sleep(0.001)
        xy[0] = True if GPIO.input(self.mma8491q_x_int) else False
        xy[1] = True if GPIO.input(self.mma8491q_y_int) else False
        GPIO.output(self.mma8491q_en, GPIO.LOW)

        return xy

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                GPIO.output(self.mma8491q_en, GPIO.LOW)
                GPIO.cleanup()
                del self.is_initialized
        except:
            pass
