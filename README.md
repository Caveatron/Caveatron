# Caveatron

The Caveatron is a unique compact, handheld, electronic device for mapping caves or other underground environments. It functions as a cave survey tool, recording conventional distance, azimuth and inclination measurements and can also go far beyond that with its integrated LIDAR to capture a full 3D scan of the cave interior. It greatly speeds up the cave survey process by recording station-to-station shots in just a few seconds and performs a station-to-station 3D scan traverse in less than a minute. The user can fully interact with the device and review data with the built-in touchscreen. A USB port provides access to the stored data from a PC or Mac and is also used to charge the integrated long-life battery pack. The station measurement file is compatible with Walls Cave Mapping Software and the LIDAR scan file can be quickly processed into a standard point cloud with custom-written software. The Caveatron is designed to be easily portable, lightweight, and low cost as well as water and dirt resistant and durable enough to survive regular use in a cave.

## Release Notes

Version: 0.99

Final beta version - Informal release for testing purposes only
Remaining work: Complete testing of 25 m LIDAR module

Known bugs:
- 25 m LIDAR occasionally fails to initialize.
- In Room mode, if 25 m LIDAR fails to initialize, corrupted GUI screen appears
- In Room mode, a successful scan will sometimes be marked as failed in the data file if a subsequent Room mode scan fails

## Instructions for setting up Arduino IDE to compile and install the Caveatron Software

### Install Arduino IDE

Download the IDE from the link below. It has been tested on the current version 1.8.5:

`https://www.arduino.cc/en/Main/Software`

If a newer version no longer functions properly, previous versions of the Arduino IDE can be found here:

`https://www.arduino.cc/en/Main/OldSoftwareReleases#previous`

Once installed and launched, in the IDE menu, go to `Tools->Boards->Board Manager` and install the "Arduino SAM Boards (32-bits ARM Cortex-M3)" for Arduino Due (not the SAMD Boards version). Install version 1.6.11.

### Increase Serial Buffer Size to 1280 kB

Edit RingBuffer.h file:

- Windows: `C:\Users\{user}\AppData\Local\Arduino15\packages\arduino\hardware\sam\1.6.x\cores\arduino\`
- Mac: `{user}/Library/Arduino15/packages/arduino/hardware/sam/1.6.x/cores/arduino/`

Change this line to read as follows:

```
#define SERIAL_BUFFER_SIZE 1280
```

### Modify `cstdio` to allow Eigen to compile

Edit `cstdio` file:

- Windows: `C:\Users\{user}\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\4.8.3-2014q1\arm-none-eabi\include\c++\4.8.3\`
- Mac: `{user}/Library/Arduino15/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/arm-none-eabi/include/c++/4.8.3/`

Comment out this line to read as follows:

```
//#undef printf
```

### Install Caveatron Libraries

1. Locate your `Arduino Libraries` folder:
    - Windows: `C:\Users\{user}\Documents\Arduino\libraries\`
    - Mac: `{user}/Documents/Arduino/libraries/`
1. Copy the `Caveatron_GUI` and `Caveatron_Hardware` libraries into the `Arduino Libraries` folder

### Install Third-Party Libraries

1. Clone the https://github.com/Caveatron/caveatron_libraries repository. (This contains additional, third_party libraries that Caveatron needs, but which are covered by difference software licenses.)
1. Locate your `Arduino Libraries` folder:
    - Windows: `C:\Users\{user}\Documents\Arduino\libraries\`
    - Mac: `{user}/Documents/Arduino/libraries/`
1. Copy all 10 folders into the `Arduino Libraries` folder


### Purchase a Coldtears Electronics 3.5" LCD Display to obtain the additional library you will need

The final library you will need is the UTFT_GHL library. At the request of the author, it cannot be distributed publically and requires the purchase of a Coldtears Electronics LCD display to obtain it (which you will need anyway in order to build a Caveatron). The displays can be purchased here:

`https://www.ebay.com/itm/3-5-inch-TFT-LCD-module-w-Font-IC-480x320-full-angle-arduino-DUE-MEGA-3-2-4-3-/121057702584?hash=item1c2f99f2b8`

Once you have the library, install it alongside the rest of the Arduino libraries as described above.

### Compiling the Code

In the Arduino IDE, open the Caveatron software code. You should see a series of 10 tabs across the top of the windows with the different segments of the code. Click the checkmark icon in the upper right of the Arduino window to compile. The message section at the bottom of the window may display a few warnings, but should compile successfully with a message of "Done Compiling" just above the black message section.

### Upload to Caveatron

Connect to the USB port on the Caveatron. In the Arduino IDE menu, under Tools->Boards be sure that the Arduino Due is selected. Also under Tools->Port, be sure the correct serial port on your computer is selected. Click the right arrow icon in the upper left corner of the Arduino IDE window (just to the right of the check icon) and the installation process will begin. This will take several minutes. A message of "Done Uploading" will display just above the black message section at the bottom of the window when this is complete.
