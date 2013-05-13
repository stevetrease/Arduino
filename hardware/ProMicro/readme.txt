*************************************************************************************
*																					*
* 						SparkFun Pro Micro Arduino Addons							*
*																					*
* 								by: Jim Lindblom									*
* 								SparkFun Electronics								*
* 								date: January 24, 2012								*
*																					*
*************************************************************************************

Unzip the ProMicro folder into the "hardware" directory of your Arduino sketchbook.
If there is not a hardware directory in your sketchbook, make one.

If you don't know where your sketchbook is located, you can find it's location by
going to File > Preferences within Arduino.

For Windows XP users, the sketchbook is usually located in your "My Documents/Arduino"
directory. So once unzipped, this readme should reside in
"My Documents/Arduino/hardware/proMicro"

Once unzipped, close any Arduino windows and reopen. The following boards should
be listed in the Tools > Board menu now:
SparkFun Pro Micro 5V/16MHz
SparkFun Pro Micro 3.3V/8MHz

IMPORTANT: Make sure you select the option that matches your board. If you program a 5V Pro Micro
with 3.3V code (or vice-versa), the USB will not work (PINDV bit of PLLCSR register
will be incorrectly set) on the board, and you'll have to re-flash the bootloader on 
using an ISP.

*************************************************************************************
*																					*
*	 						Explanation of Files Included							*
*																					*
*************************************************************************************

This addon should work with no need to modify files. If you're curious what we've done
to the default Arduino files though, read on...

* bootloaders/diskloader: This is a slightly modified version of the Diskloader
bootloader. Which is used in the Arduino Leonardo. Defines were modified so the VID
and PID were set to SparkFun-specific values (VID: 0x1B4F, PIDs: 0x0002 and 0x0003).
There is also a modification to the setting of the PLLCSR register. This register must
be set to different values depending on whether the clock is running at 8MHz or 16MHz.

To compile the bootloader for a Pro Micro, you must first set AVR_FREQ and 
ARDUINO_MODEL_PID near the top of the makefile.

This directory also includes DiskLoader-ProMicro8.hex and DiskLoader-ProMicro16.hex,
which are the compiled images for both 8MHz and 16MHz Pro Micro boards.

* cores/arduino: This is a mostly unchanged listing of all the core files required by
the ProMicro to compile under Arduino. The files with changes are:
	USBDesc.h: The USB_PID_PROMICRO8 and USB_PID_PROMICRO16 were added, USB_PID_LEONARDO
	was removed. The SparkFun VID replaces the Arduino LLC VID.
	USBCore.cpp: Changed STRING_IPRODUCT[] and STRING_IMANUFACTURER[] variables.
	Also added conditional statement for seting PLLCSR.
	
* variants/proMicro8 and variants/proMicro16: Both of these directories contain a 
pins_arduino.h file, which is nearly identical to the Leonardo's pins_arduino.h. The
only change being a modification of the ARDUINO_MODEL_USB_PID near the top of the file.

* boards.txt: This file is what Arduino looks at when populating its Tools > Board menu.
There are two board entires in this file:
SparkFun Pro Micro 5V/16MHz -and-
SparkFun Pro Micro 3.3V/8MHz

* programmers.txt: Needs testing, I don't think this file actually needs to be included.


*************************************************************************************
*																					*
*	 						Questions/Comments/Concerns?							*
*																					*
*************************************************************************************

Please let us know if this gives you any problems. You can contact our tech support team
(techsupport@sparkfun.com), or post a comment on the product page and we'll try to get
back to you as quickly as possible.

All of this stuff is still in a somewhat beta-stage, so don't be surprised if things get
funky.

Have fun!
-Jim (jim at sparkfun.com)