# Caveatron

The Caveatron is a unique compact, handheld, electronic device for mapping caves or other underground environments. It functions as a cave survey tool, recording conventional distance, azimuth and inclination measurements and can also go far beyond that with its integrated LIDAR to capture a full 3D scan of the cave interior. It greatly speeds up the cave survey process by recording station-to-station shots in just a few seconds and performs a station-to-station 3D scan traverse in less than a minute. The user can fully interact with the device and review data with the built-in touchscreen. A USB port provides access to the stored data from a PC or Mac and is also used to charge the integrated long-life battery pack. The station measurement file is compatible with Walls Cave Mapping Software and the LIDAR scan file can be quickly processed into a standard point cloud with custom-written software. The Caveatron is designed to be easily portable, lightweight, and low cost as well as water and dirt resistant and durable enough to survive regular use in a cave.

## Release Notes

Version: 1.00

Initial Public Release


## Instructions for setting up Arduino IDE to compile and install the Caveatron Software

### Install Arduino IDE

Download the IDE from the link below. It has been tested on the current version 1.8.5:

`https://www.arduino.cc/en/Main/Software`

If a newer version no longer functions properly, previous versions of the Arduino IDE can be found here:

`https://www.arduino.cc/en/Main/OldSoftwareReleases#previous`

Once installed and launched, in the IDE menu, go to `Tools->Boards->Board Manager` and install the "Arduino SAM Boards (32-bits ARM Cortex-M3)" for Arduino Due (not the SAMD Boards version). Install version 1.6.11.

### Increase Serial Buffer Size to 1280 kB

Edit RingBuffer.h file:

- Windows: `C:\Users\{user}\AppData\Local\Arduino15\packages\arduino\hardware\sam\1.6.11\cores\arduino\RingBuffer.h`
- Mac: `{user}/Library/Arduino15/packages/arduino/hardware/sam/1.6.11/cores/arduino/RingBuffer.h`

Change this line to read as follows:

```
#define SERIAL_BUFFER_SIZE 1280
```

### Modify `cstdio` to allow Eigen to compile

Edit `cstdio` file:

- Windows: `C:\Users\{user}\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\4.8.3-2014q1\arm-none-eabi\include\c++\4.8.3\cstdio`
- Mac: `{user}/Library/Arduino15/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/arm-none-eabi/include/c++/4.8.3/cstdio`

Comment out this line to read as follows:

```
//#undef printf
```

### Download and Install Caveatron Libraries

1. Clone the `https://github.com/Caveatron/caveatron` repository.
2. Locate your `Arduino Libraries` folder:
    - Windows: `C:\Users\{user}\Documents\Arduino\libraries\`
    - Mac: `{user}/Documents/Arduino/libraries/`
3. Copy the `Caveatron_GUI` and `Caveatron_Hardware` libraries into the `Arduino Libraries` folder

### Download and Install Third-Party Libraries

1. Clone the `https://github.com/Caveatron/caveatron_libraries` repository. (This contains additional, third_party libraries that Caveatron needs, but which are covered by difference software licenses.)
2. Locate your `Arduino Libraries` folder:
    - Windows: `C:\Users\{user}\Documents\Arduino\libraries\`
    - Mac: `{user}/Documents/Arduino/libraries/`
3. Copy all 10 folders into the `Arduino Libraries` folder


### Purchase a Coldtears Electronics 3.5" LCD Display to obtain the additional library you will need

The final library you will need is the UTFT_GHL library. At the request of the author, it cannot be distributed publically and requires the purchase of a Coldtears Electronics LCD display to obtain it (which you will need anyway in order to build a Caveatron). The displays can be purchased here:

`https://www.ebay.com/itm/3-5-inch-TFT-LCD-module-w-Font-IC-480x320-full-angle-arduino-DUE-MEGA-3-2-4-3-/121057702584?hash=item1c2f99f2b8`

To obtain the library after you have purchased the display:
1. Navigate to www.uftghl.com/downloads.php
2. Here you will be asked to fill in three fields, which are on a sticker applied to the ColdTears Electronics screen that you bought.
3. Download the file (warning: the link will be one-time usage, so be sure to keep the file somewhere safe)
4. The download will be a .7z file. On Windows you can use 7zip (`http://www.7-zip.org`). On Mac you can use Keka (`http://www.kekaosx.com`)
5. Copy the UFT_GHL library folder to the Arduino libraries folder


### Compiling the Code

In the Arduino IDE, open the Caveatron software code by double clicking on the Caveatron.ino file. This will open all the other files which you should see a series of 10 tabs across the top of the windows with the different segments of the code. Click the checkmark icon in the upper left of the Arduino window to compile. The message section at the bottom of the window may display a few warnings, but should compile successfully with a message of "Done Compiling" just above the black message section.

### Upload to Caveatron

Connect to the USB port on the Caveatron. In the Arduino IDE menu, under Tools->Boards be sure that the Arduino Due is selected. Also under Tools->Port, be sure the correct serial port on your computer is selected. Click the right arrow icon in the upper left corner of the Arduino IDE window (just to the right of the check icon) and the installation process will begin. This will take several minutes. A message of "Done Uploading" will display just above the black message section at the bottom of the window when this is complete.

### Initial Testing of the Code

You can do an initial test of the code with only the Arduino Due, the LCD Shield, and the LCD Display all plugged together without additional hardware by making a few small edits at the top of the main (Caveatron) tab of the code.
1. Edit `#define AUTO_CONFIG` to have a value of 0 instead of 1 (1 is automatic configuration loading from the EEPROM, 0 uses manually entered values in the Calibrate tab.
2. Edit `char hardwareCode[] =` to have an 11 digit value inside the quotation marks based on your hardware components. See the Caveatron Hardware Code Description document for these values.
Use the calibration values in the `LoadTestCalibrationParameters()` function at the bottom of the Calibrate tab or you can enter your own. Note that until you have calibrated the touchscreen, the screen will not accurately register a touch position.

