# Turta Modular Helper for Raspbian.
# Distributed under the terms of the MIT license.

# Python Library for Broadcom APDS-9960 Ambient & RGB Light, Proximity and Gesture Detection Sensor.
# Version 1.0.0
# Released: November 14th, 2019

# Visit https://docs.turta.io for documentation.

from enum import IntEnum
import RPi.GPIO as GPIO
from smbus import SMBus
from time import sleep

#Enumerations

#MODES: Operating Modes
class MODES(IntEnum):
    OFF = 0
    ALS_RGB = 1
    ALS_RGB_PROX = 2
    ALS_RGB_PROX_GES = 3
    PROX = 4
    PROX_GES = 5

#GESTURES: Gesture Types
class GESTURES(IntEnum):
    NONE    = 0
    LEFT    = 1
    RIGHT   = 2
    UP      = 3
    DOWN    = 4

#ENABLE - GEN: Gesture Enable
class ENABLE_GEN(IntEnum):
    OFF = 0b00000000
    ON  = 0b01000000

#ENABLE - PIEN: Proximity Interrupt Enable
class ENABLE_PIEN(IntEnum):
    OFF = 0b00000000
    ON  = 0b00100000

#ENABLE - AIEN: ALS Interrupt Enable
class ENABLE_AIEN(IntEnum):
    OFF = 0b00000000
    ON  = 0b00010000

#ENABLE - WEN: Wait Enable
class ENABLE_WEN(IntEnum):
    OFF = 0b00000000
    ON  = 0b00001000

#ENABLE - PEN: Proximity Detect Enable
class ENABLE_PEN(IntEnum):
    OFF = 0b00000000
    ON  = 0b00000100

#ENABLE - AEN: ALS Enable
class ENABLE_AEN(IntEnum):
    OFF = 0b00000000
    ON = 0b00000010

#ENABLE - PON: Power On
class ENABLE_PON(IntEnum):
    OFF = 0b00000000
    ON = 0b00000001

#CONTROL - LDRIVE: LED Drive Strenght
class CONTROL_LDRIVE(IntEnum):
    MA_100   = 0b00000000
    MA_050   = 0b01000000
    MA_025   = 0b10000000
    MA_012_5 = 0b11000000

#CONTROL - PGAIN: Proximity Gain Control
class CONTROL_PGAIN(IntEnum):
    X1 = 0b00000000
    X2 = 0b00000100
    X4 = 0b00001000
    X8 = 0b00001100

#CONTROL - AGAIN: ALS and Color Gain Control
class CONTROL_AGAIN(IntEnum):
    X01 = 0b00000000
    X04 = 0b00000001
    X16 = 0b00000010
    X64 = 0b00000011

#GCONF2 - GGAIN: Gesture Gain Control
class GCONF2_GGAIN(IntEnum):
    X1 = 0b00000000
    X2 = 0b00100000
    X4 = 0b01000000
    X8 = 0b01100000

#GCONF2 - GLDRIVE: Gesture LED Drive Strenght
class GCONF2_GLDRIVE(IntEnum):
    MA_100   = 0b00000000
    MA_050   = 0b00001000
    MA_025   = 0b00010000
    MA_012_5 = 0b00011000

#GCONF2 - GWTIME: Gesture Wait Time
class GCONF2_GWTIME(IntEnum):
    MS_00_0 = 0b00000000
    MS_02_8 = 0b00000001
    MS_05_6 = 0b00000010
    MS_08_4 = 0b00000011
    MS_14_0 = 0b00000100
    MS_22_4 = 0b00000101
    MS_30_8 = 0b00000110
    MS_39_2 = 0b00000111

#GCONF4 - GFIFO_CLR: Gesture FIFO Clear
class GCONF4_GFIFO_CLR(IntEnum):
    OFF = 0b00000000
    ON  = 0b00000100

#GCONF4 - GIEN: Gesture Interrupt Enable
class GCONF4_GIEN(IntEnum):
    OFF = 0b00000000
    ON  = 0b00000010

#GCONF4 - GMODE Values
class GCONF4_GMODE(IntEnum):
    ALS_PROX_COLOR = 0b00000000
    GESTURE        = 0b00000001

class LightSensor:
    """APDS-9960 Sensor"""

    #Gesture Configuration
    IN_OUT_THRESOLD = 80
    DIR_THRESOLD = 30

    #I2C Slave Address
    I2C_ADDRESS = 0x39

    #Registers
    APDS9960_ENABLE     = 0x80
    APDS9960_ATIME      = 0x81
    APDS9960_WTIME      = 0x83
    APDS9960_AILTL      = 0x84
    APDS9960_AILTH      = 0x85
    APDS9960_AIHTL      = 0x86
    APDS9960_AIHTH      = 0x87
    APDS9960_PILT       = 0x89
    APDS9960_PIHT       = 0x8B
    APDS9960_PERS       = 0x8C
    APDS9960_CONFIG1    = 0x8D
    APDS9960_PPULSE     = 0x8E
    APDS9960_CONTROL    = 0x8F
    APDS9960_CONFIG2    = 0x90
    APDS9960_ID         = 0x92
    APDS9960_STATUS     = 0x93
    APDS9960_CDATAL     = 0x94
    APDS9960_CDATAH     = 0x95
    APDS9960_RDATAL     = 0x96
    APDS9960_RDATAH     = 0x97
    APDS9960_GDATAL     = 0x98
    APDS9960_GDATAH     = 0x99
    APDS9960_BDATAL     = 0x9A
    APDS9960_BDATAH     = 0x9B
    APDS9960_PDATA      = 0x9C
    APDS9960_POFFSET_UR = 0x9D
    APDS9960_POFFSET_DL = 0x9E
    APDS9960_CONFIG3    = 0x9F
    APDS9960_GPENTH     = 0xA0
    APDS9960_GEXTH      = 0xA1
    APDS9960_GCONF1     = 0xA2
    APDS9960_GCONF2     = 0xA3
    APDS9960_GOFFSET_U  = 0xA4
    APDS9960_GOFFSET_D  = 0xA5
    APDS9960_GOFFSET_L  = 0xA7
    APDS9960_GOFFSET_R  = 0xA9
    APDS9960_GPULSE     = 0xA6
    APDS9960_GCONF3     = 0xAA
    APDS9960_GCONF4     = 0xAB
    APDS9960_GFLVL      = 0xAE
    APDS9960_GSTATUS    = 0xAF
    APDS9960_IFORCE     = 0xE4
    APDS9960_PICLEAR    = 0xE5
    APDS9960_CICLEAR    = 0xE6
    APDS9960_AICLEAR    = 0xE7
    APDS9960_GFIFO_U    = 0xFC
    APDS9960_GFIFO_D    = 0xFD
    APDS9960_GFIFO_L    = 0xFE
    APDS9960_GFIFO_R    = 0xFF

    #I2C Config
    bus = SMBus(1)

    #Interrupt Pin
    apds9960_int = None
    apds9960_int_1, apds9960_int_2 = 17, 25

    #Variables
    int_bypassed = False
    ud_diff_total = 0x00
    lr_diff_total = 0x00

    #I2C Communication

    def _write_register(self, reg_addr, data):
        """Writes data to the I2C device.

        Parameters:
        reg_addr (byte): Write register address
        data (byte): Data to be written to the device"""

        self.bus.write_i2c_block_data(self.I2C_ADDRESS, reg_addr, [ data & 0xFF ])

    def _read_register_1ubyte(self, reg_addr):
        """Reads data from the I2C device.

        Parameters:
        reg_addr (byte): Read register address

        Returns:
        byte: Response from the device"""

        buffer = self.bus.read_i2c_block_data(self.I2C_ADDRESS, reg_addr, 1)
        return buffer[0]

    def _read_2bytes_lsbf(self, reg_addr):
        """Reads data from the I2C device.

        Parameters:
        reg_addr (byte): Read register address

        Returns:
        int: Response from the device, LSB first"""

        buffer = self.bus.read_i2c_block_data(self.I2C_ADDRESS, reg_addr, 2)
        return buffer[0] + (buffer[1] << 8)

    def _read_array(self, reg_addr, cnt):
        """Reads data from the I2C device.

        Parameters:
        reg_addr (byte): Read register start address
        cnt (byte): Read register address

        Returns:
        byte array: Response from the device"""

        return self.bus.read_i2c_block_data(self.I2C_ADDRESS, reg_addr, cnt)

    #Initialization

    def __init__(self, slot, mode = MODES.ALS_RGB, bypass_int = False):
        """Initiates the APDS-9960 sensor to get ambient light, RGB light, proximity and gesture data.
        
        Parameters:
        slot (int): Modular sensor slot number (1 or 2)
        mode (MODES): Sensor operating mode (MODES.ALS_RGB is default)
        pypass_int (bool): Enables checking gesture interrupt without using the interrupt pin (False is default)"""

        if slot < 1 or slot > 2:
            raise ValueError('slot is outside of 1 and 2.')
        if mode not in MODES:
            raise ValueError('mode is not a member of MODES.')

        if slot == 1:
            self.apds9960_int = self.apds9960_int_1
        elif slot == 2:
            self.apds9960_int = self.apds9960_int_2

        self.int_bypassed = bypass_int

        if not bypass_int:
            GPIO.setwarnings(False)
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(self.apds9960_int, GPIO.IN, pull_up_down = GPIO.PUD_UP)
        self._set_initial_settings()
        self.set_mode(mode)
        self.is_initialized = True
        sleep(0.5)

    #Sensor Configuration

    def _set_initial_settings(self):
        """Writes the initial settings to the sensor."""

        #Set enable register to turn off all functionality but power
        self._write_register(self.APDS9960_ENABLE, 0x01)

        #Set ATIME to 72 cycles, 200ms, 65535 max count
        self._write_register(self.APDS9960_ATIME, 0xB6)

        #Set wait time to 20ms
        self._write_register(self.APDS9960_WTIME, 0xF9)

        #Set ALS interrupt low thresold
        self._write_register(self.APDS9960_AILTL, 0x00) #Low
        self._write_register(self.APDS9960_AILTH, 0x00) #High

        #Set ALS interrupt high thresold
        self._write_register(self.APDS9960_AIHTL, 0xFF) #Low
        self._write_register(self.APDS9960_AIHTH, 0xFF) #High

        #Set proximity interrupt low thresold
        self._write_register(self.APDS9960_PILT, 0)

        #Set proximity interrupt high thresold.
        self._write_register(self.APDS9960_PIHT, 0xFF)

        #Set interrupt persistence filters
        self._write_register(self.APDS9960_PERS, 0x11)

        #Set configuration register one: No 12x wait (WLONG = 0)
        self._write_register(self.APDS9960_CONFIG1, 0x60)

        #Set proximity pulse count to 8 and lenght to 8us
        self._write_register(self.APDS9960_PPULSE, 0x48)

        #Set control register one:
        #LDRIVE: LED Drive Strenght to 100mA
        #PGAIN: Proximity Gain to 4x
        #AGAIN: ALS and Color Gain to 4x
        self._write_register(self.APDS9960_CONTROL, CONTROL_LDRIVE.MA_100 | CONTROL_PGAIN.X4 | CONTROL_AGAIN.X04)

        #Set configuration register two:
        #PSIEN: Disabled
        #CPSIEN: Disabled
        #LED_BOOST: 100%
        #Field 0: 1
        self._write_register(self.APDS9960_CONFIG2, 0x01)

        #Set proximity offset for up and right photodiodes to 0
        self._write_register(self.APDS9960_POFFSET_UR, 0x00)

        #Set proximity offset for down and left photodiodes to 0
        self._write_register(self.APDS9960_POFFSET_DL, 0x00)

        #Set configuration register three: Enable all photodiodes, no PCMP, no SAI
        self._write_register(self.APDS9960_CONFIG3, 0x00)

        #Set gesture proximity enter thresold
        self._write_register(self.APDS9960_GPENTH, 30)

        #Set gesture exit thresold
        self._write_register(self.APDS9960_GEXTH, 60)

        #Set gesture configuration one: Interrupt after 4 dataset, end at 2nd, all directions are active
        self._write_register(self.APDS9960_GCONF1, 0x41)

        #Set gesture configuration register two:
        #GGAIN: Gesture Gain to x1
        #GLDRIVE: Gesture LED Drive Strenght to 100mA
        #GWTIME: Gesture Wait Time to 0ms
        self._write_register(self.APDS9960_GCONF2, GCONF2_GGAIN.X1 | GCONF2_GLDRIVE.MA_100 | GCONF2_GWTIME.MS_00_0)

        #Set gesture offsets to 0
        self._write_register(self.APDS9960_GOFFSET_U, 0x00)
        self._write_register(self.APDS9960_GOFFSET_D, 0x00)
        self._write_register(self.APDS9960_GOFFSET_L, 0x00)
        self._write_register(self.APDS9960_GOFFSET_R, 0x00)

        #Set gesture pulse count to 32 and lenght to 32us
        self._write_register(self.APDS9960_GPULSE, 0xDF)

        #Set gesture configuration three: All photodiodes are enabled to gather results during gesture
        self._write_register(self.APDS9960_GCONF3, 0x00)

        #Set gesture configuration four
        self._write_register(self.APDS9960_GCONF4, GCONF4_GFIFO_CLR.ON | GCONF4_GIEN.ON | GCONF4_GMODE.ALS_PROX_COLOR)

        return

    def set_mode(self, mode):
        """Sets the sensor mode.

        Parameters:
        mode (MODES): Sensor operating mode"""

        if mode not in MODES:
            raise ValueError('mode is not a member of MODES.')

        enableCommand = 0x00

        #Ambient and RGB Light
        if (mode == MODES.ALS_RGB or mode == MODES.ALS_RGB_PROX or mode == MODES.ALS_RGB_PROX_GES):
            enableCommand |= ENABLE_PON.ON
            enableCommand |= ENABLE_AEN.ON

        #Proximity Detection
        if (mode == MODES.PROX or mode == MODES.PROX_GES or mode == MODES.ALS_RGB_PROX or mode == MODES.ALS_RGB_PROX_GES):
            enableCommand |= ENABLE_PON.ON
            enableCommand |= ENABLE_PEN.ON

        #Gesture Recognition
        if (mode == MODES.PROX_GES or mode == MODES.ALS_RGB_PROX_GES):
            enableCommand |= ENABLE_PON.ON
            enableCommand |= ENABLE_GEN.ON

        #Enable Wait
        enableCommand |= ENABLE_WEN.ON

        self._write_register(self.APDS9960_ENABLE, enableCommand)

        return

    #Sensor Read Methods

    def read_ambient_light(self):
        """Reads the ambient light value.

        Returns:
        int: Ambient light value"""

        return int(self._read_2bytes_lsbf(self.APDS9960_CDATAL))

    def read_crgb_light(self):
        """Reads the CRGB light values.

        Returns:
        int array: Clear, red, green and blue light values respectively"""

        crgb = [ 0, 0, 0, 0 ]

        crgb = self._read_array(self.APDS9960_CDATAL, 8)

        crgb[0] = int(crgb[0] | (crgb[1] << 8)) #Clear channel
        crgb[1] = int(crgb[2] | (crgb[3] << 8)) #Red channel
        crgb[2] = int(crgb[4] | (crgb[5] << 8)) #Green channel
        crgb[3] = int(crgb[6] | (crgb[7] << 8)) #Blue channel

        return crgb

    def read_proximity(self):
        """Reads the proximity value.

        Returns:
        byte: Proximity value"""

        return int(self._read_register_1ubyte(self.APDS9960_PDATA))

    #Gesture Detection Methods

    def read_gesture(self):
        """Detects hand gestures.
        
        Returns:
        GESTURES: Gesture direction if gesture is detected, None if not"""

        if not self.int_bypassed:                   #If interrupt pin is enabled
            if not GPIO.input(self.apds9960_int):       #If interrupt pin is (active) low
                return self._process_gesture()              #Proceed checking the interrupt
            else:                                       #If interrupt pin is not (active) low
                return None                                 #Return None
        else :                                      #If interrupt pin is disabled
            if self._read_gesture_int_state():          #If gesture interrupt flag is set
                return self._process_gesture()              #Proceed checking the interrupt
            else:                                       #If gesture interrupt flag is not set
                return None                                 #Return None

    def _read_gesture_int_state(self):
        """Checks gesture interrupt bit to read interrupt state.

        Returns
        bool: True if interrupt flag is set, False if not"""

        if (self._read_register_1ubyte(self.APDS9960_GSTATUS) & 0x01 == 0x01):
            return True
        else:
            return False

    def _process_gesture(self):
        """Processes device buffer to find hand gestures.
        
        Returns:
        GESTURES: Gesture direction if gesture is detected, None if not"""

        u_in = 1
        u_out = 1
        d_in = 1
        d_out = 1
        l_in = 1
        l_out = 1
        r_in = 1
        r_out = 1
        ud_count = 0
        lr_count = 0
        gdir = None
        buffer = None
        gd_u = None
        gd_d = None
        gd_l = None
        gd_r = None

        #If gesture is not available, return None
        if not self._read_gesture_int_state():
            return None

        #Wait for buffer
        sleep(0.030)

        #Read buffer length and prepare empty buffers
        bl = self._read_register_1ubyte(self.APDS9960_GFLVL)
        buffer = [0x00] * (bl * 4)
        gd_u = [0x00] * bl
        gd_d = [0x00] * bl
        gd_l = [0x00] * bl
        gd_r = [0x00] * bl
        
        #If device buffer is empty, return None
        if (bl == 0):
            return None

        #Read buffer
        #As maximum package length per SMBus read operation is limited to 32 Bytes,
        #we're dividing the read operation
        for n in range(0, bl, 8):
            buffer[(32*n):(32*n)+31] = self._read_array(self.APDS9960_GFIFO_U, 32)

        #Distribute the buffer content to direction buffers
        for n in range(bl):
            gd_u[n] = buffer[(n*4)]
            gd_d[n] = buffer[(n*4)+1]
            gd_l[n] = buffer[(n*4)+2]
            gd_r[n] = buffer[(n*4)+3]

        #If buffer size is not enough for gesture recognition
        if (bl < 4):
            return None

        #Set in times for all directions
        for n in range(bl):
            if (gd_u[n] > self.IN_OUT_THRESOLD) and \
               (gd_d[n] > self.IN_OUT_THRESOLD) and \
               (gd_l[n] > self.IN_OUT_THRESOLD) and \
               (gd_r[n] > self.IN_OUT_THRESOLD):
                u_in = gd_u[n]
                d_in = gd_d[n]
                l_in = gd_l[n]
                r_in = gd_r[n]
                break

        #Set out times for all directions
        for n in range(bl - 1, 0, -1):
            if (gd_u[n] > self.IN_OUT_THRESOLD) and \
               (gd_d[n] > self.IN_OUT_THRESOLD) and \
               (gd_l[n] > self.IN_OUT_THRESOLD) and \
               (gd_r[n] > self.IN_OUT_THRESOLD):
                u_out = gd_u[n]
                d_out = gd_d[n]
                l_out = gd_l[n]
                r_out = gd_r[n]
                break

        #Calculate in/out ratios for all directions
        ud_ratio_in = ((u_in - d_in) * 100) / (u_in + d_in)
        lr_ratio_in = ((l_in - r_in) * 100) / (l_in + r_in)
        ud_ratio_out = ((u_out - d_out) * 100) / (u_out + d_out)
        lr_ratio_out = ((l_out - r_out) * 100) / (l_out + r_out)

        #Calculate in/out differences
        self.ud_diff_total += ud_ratio_out - ud_ratio_in
        self.lr_diff_total += lr_ratio_out - lr_ratio_in

        #Calculate up/down direction
        if self.ud_diff_total >= self.DIR_THRESOLD:
            ud_count = 1
        elif self.ud_diff_total <= -self.DIR_THRESOLD:
            ud_count = -1
        else:
            ud_count = 0
        
        #Calculate left/right direction
        if self.lr_diff_total >= self.DIR_THRESOLD:
            lr_count = 1
        elif self.lr_diff_total <= -self.DIR_THRESOLD:
            lr_count = -1
        else:
            lr_count = 0
        
        #Set final gesture direction
        if ((ud_count == -1) and (lr_count == 0)):
            gdir = GESTURES.UP
        elif ((ud_count == 1) and (lr_count == 0)):
            gdir = GESTURES.DOWN
        elif ((ud_count == 0) and (lr_count == 1)):
            gdir = GESTURES.RIGHT
        elif ((ud_count == 0) and (lr_count == -1)):
            gdir = GESTURES.LEFT

        elif ((ud_count == -1) and (lr_count == 1)):
            if (abs(self.ud_diff_total) > abs(self.lr_diff_total)):
                gdir = GESTURES.UP
            else:
                gdir = GESTURES.RIGHT
        elif ((ud_count == 1) and (lr_count == -1)):
            if (abs(self.ud_diff_total) > abs(self.lr_diff_total)):
                gdir = GESTURES.DOWN
            else:
                gdir = GESTURES.LEFT
        elif ((ud_count == -1) and (lr_count == -1)):
            if (abs(self.ud_diff_total) > abs(self.lr_diff_total)):
                gdir = GESTURES.UP
            else:
                gdir = GESTURES.LEFT
        elif ((ud_count == 1) and (lr_count == 1)):
            if (abs(self.ud_diff_total) > abs(self.lr_diff_total)):
                gdir = GESTURES.DOWN
            else:
                gdir = GESTURES.RIGHT

        #Return None if no gesture is found
        else:
            return None

        #Reset direction difference counters
        self.ud_diff_total = 0
        self.lr_diff_total = 0

        return gdir

    #Disposal

    def __del__(self):
        """Releases the resources."""

        try:
            if self.is_initialized:
                if not self.int_bypassed:
                    GPIO.cleanup()
                self.set_mode(MODES.OFF)
                del self.is_initialized
        except:
            pass
