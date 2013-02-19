# Introduction

This directory contains the content necessary for converting an
Excitron DB15/Atmega168 device ($100+ each new, many years ago) to
be repurposed for use with the Arduino interface ($10 device). ;)

This doc will explain how to set up the different portions of it
for use.

Forum link from when i got the Optiboot bootloader for this:

    http://arduino.cc/forum/index.php/topic,117501.0.html

NOTE: due to it running at 7.3MHz, it will not run some libraries 
like Software Serial.

Also note that even though this document is specifically for the D15,
many of the concepts are adaptable for kickstarting any Atmega chip,
or building a programmer for other devices.


--------------------------------------------------------------------------------
# Setup

First we need to install the definition and bootloader hex files to the Arduino package.
This only needs to be performed once.

1. prepend the "add_to_boards.txt" content into "boards.txt" 
  - Mac: Arduino.app/Contents/Resources/Java/hardware
  - Windows: hardware\arduino\ next to the Arduino.exe

    Here's the text block to be added:

##############################################################
atmega168f737.name=ATmega168 at 7372800Hz (D15)

atmega168f737.upload.protocol=arduino
atmega168f737.upload.maximum_size=14336
atmega168f737.upload.speed=57600

atmega168f737.bootloader.low_fuses=0xfd
atmega168f737.bootloader.high_fuses=0xdd
atmega168f737.bootloader.extended_fuses=0x04
atmega168f737.bootloader.path=optiboot
atmega168f737.bootloader.file=optiboot_atmega168_737.hex
atmega168f737.bootloader.unlock_bits=0x3F
atmega168f737.bootloader.lock_bits=0x0F

atmega168f737.build.mcu=atmega168
atmega168f737.build.f_cpu=7372800L
atmega168f737.build.core=arduino
atmega168f737.build.variant=standard

##############################################################


2. Drop the "optiboot_atmega168_737.hex" file into bootloaders/optiboot
  - Mac: Arduino.app/Contents/Resources/Java/hardware/arduino/bootloaders/optiboot
  - Windows: hardware\arduino\bootloaders\optiboot next to the Arduino.exe

ref: http://arduino.cc/forum/index.php/topic,117501.0.html
Thanks to Tom Carpenter on the Arduino Forums for providing the above!
NOTE: he does mention that since it's not 8Mhz, timing may be 5% off. (delay() and millis()) 
A 1 second delay can be accomplished via:  delay( 949 );


Okay. We now have the core bootloader installed.

Next we need to build a programmer.

First build the D15-to-programming headers adapter.

D15 pin 4: (GRN)   RESET
D15 pin 5: (ORN)   SCK
D15 pin 6: (YEL)   MISO
D15 pin 7: (PUR)   MOSI
D15 pin 10: (BLK)  GND
D15 pin 11: (BLK)  GND
D15 pin 12: (BLK)  GND
D15 pin 13: (RED)  +VCC (5v)
D15 pin 14: (RED)  +VCC (5v)
D15 pin 15: (RED)  +VCC (5v)

Optionally, this can be wired to a standard arduino 6 pin programming header:

 [1 MISO ] [2 +VCC]
 [3 SCK  ] [4 MOSI]
 [5 RESET] [6 GND ]


1. Plug in the red/black pair to the +5v/Gnd header on the arduino.
2. Plug in the green/purple/yellow/orange header into the arduino:
   - Digital 10 - Green
   - Digital 11 - Purple
   - Digital 12 - Yellow
   - Digital 13 - Green (reset)

3. Optionally plug in an LED header:
   - Digital 9 - (orange) Amber LED (heartbeat)
   - Digital 8 - (red) Red LED (error)
   - Digital 7 - (green) Green LED (programming communications)
   - +5v - (red)

LED circuit is:  

	(Digital Pin)----(LED)----(220 ohm resistor)----(+5v)


--------------------------------------------------------------------------------
# Setting up the Programmer on an Arduino

1. Plug in your host arduino
2. set the device type for this D15 arduino
3. Set the serial port
4. Open, build and run the "01.Basics - Blink" sketch to make sure it's hooked up correctly
5. Open, build and run the "ArduinoISP" sketch.
6. Plug in the headers as described above (if you haven't already)
7. Add a 120 ohm resistor from the reset pin to +5 on the host Arduino
   (This disables host reset)
8. Select the target device: Tools-Board-"ATmega 168 at 7372800Hz (D15)"
9. Select the programmer: Tools-Programmer-"Arduino as ISP"


--------------------------------------------------------------------------------
# Programming a device

1. Plug a D15 device into the programmer cable.
2. Immediately program the device:  Tools-"Burn Bootloader"
3. If it worked, disconnect the D15 device, and repeat at step 1

At this point, the D15 device is now arduino compatible.


--------------------------------------------------------------------------------
# FTDI adapter cable

This cable will let you use the D15 device without using a programmer,
as a "regular" Arduino, using the newly programmed Optiboot
serial-based bootloader.

The wiring is essentially:

FTDI pin 1: (GRN) (reset)  -> .1uf Cap  -> D15 Pin 4
FTDI pin 2: (YEL) (tx)                  -> D15 Pin 1
FTDI pin 3: (ORN) (rx)                  -> D15 Pin 2
FTDI pin 4: (RED) (+5v)                 -> D15 Pins 13, 14, 15
                  (+5v) -> 10k Resistor -> D15 Pin 4 (Reset Pullup)
FTDI pin 5: (BRN) (not connected)
FTDI pin 6: (GRN) (ground)              -> D15 pins 10, 11, 12


--------------------------------------------------------------------------------
# Your first D15 test...

1. Plug in the D15 device into the FTDI adapter cable.
2. Plug in your FTDI cable/adapter into this.
3. Plug the FTDI cable into your host computer
4. Select the proper device: Tools-Board-"ATmega 168 at 7372800Hz (D15)"
5. Make sure the serial port is selected to the FTDI cable
6. Open, Build, and run the "D15_Test" sketch.
7. the LED on the D15 should blink 
8. Tools-Serial Monitor to start the serial interface (9600 Baud)
9. typing 0,1,2,3,4,5,6,7,8,9 should change the blink rate of the LED

