# Caveatron

The Caveatron is a unique compact, handheld, electronic device for mapping caves or other underground environments. It functions as a cave survey tool, recording conventional distance, azimuth and inclination measurements and can also go far beyond that with its integrated LIDAR to capture a full 3D scan of the cave interior. It greatly speeds up the cave survey process by recording station-to-station shots in just a few seconds and performs a station-to-station 3D scan traverse in less than a minute. The user can fully interact with the device and review data with the built-in touchscreen. A USB port provides access to the stored data from a PC or Mac and is also used to charge the integrated long-life battery pack. The station measurement file is compatible with Walls Cave Mapping Software and the LIDAR scan file can be quickly processed into a standard point cloud with custom-written software. The Caveatron is designed to be easily portable, lightweight, and low cost as well as water and dirt resistant and durable enough to survive regular use in a cave.

## Release Notes
### Version 1.40
CRITICAL NOTE! You must update the Caveatron_Hardware Library to v1.4

Changes since version 1.31:
- When creating a new survey, calibration parameters are now stored in a .IMU file of the same name as the survey files.
- Added ability to load and save calibration parameters from/to a file on the SD card. Use of Caveatron_Downloader/Uploader sketch is no longer required.
- Added compatibility with new version of Caveatron Connect (v1.1) to provide for transfer of .IMU calibration files from a PC to the Caveatron SD card.
- Added magnetometer alignment to the View IMU Calibration screen.
- Fixes in the Caveatron_Hardware library to improve LRF turn-on reliability.
- Reorganized Calibration menu screens.
- Added a dialog box function to the GUI tab.
- Various GUI bug fixes.

### Version 1.31
CRITICAL NOTE! If you updated the Caveatron_Hardware library to v1.3, replace it with v1.31. If not, no change is required.

Changes since version 1.30:
- Improvements to ensure data is written to SD card in the event of a system freeze.
- Fixed bug that can potentially cause a system freeze when ending a scan with the SWEEP LIDAR (requires updating SWEEP library to 2019-03-02 version).
- Changes to sequence of events when LIDAR scan ended to ensure log file is properly updated in the event of a system freeze.

### Version 1.30

CRITICAL NOTES!
This version requires updating the following libraries: Caveatron_GUI
It is highly recommended that you also upgrade the following libraries: Caveatron_Hardware, URTouch, UTUF, and UTFT_CTE. The new versions add future support for the Teensy 3.5/3.6.

Changes since version 1.20:
- Added support for dedicated back-sight shots.
- Added screen when entering shot mode to select front/back shot type and preselect recent station codes.
- Added next station code prediction.
- Added tolerance to ignore a single superious magnetometer or accelerometer reading and continue scan.
- Fixed bug that would result in erroneous azimuth readings when pointed due south and Passage Mode scan failure.
- Fixed bug that prevented display of the last shot list screen under certain conditions.
- Fixed bug with RPLIDAR viewing and recording that produced incorrect scan orientation.
- Fixed bug that prevented RPLIDAR scan data from being viewed with the Scan Review function.
- Fixed bug that resulted in some Room Scan data being incorrectly displayed in the Scan Review function.

### Version 1.20

CRITICAL NOTES!
This version requires updating the following libraries: Caveatron_Hardware and LSM303
This version requires adding the following new library: RPLIDAR'
Use new Caveatron Parameter Uploader 1.2 and Downloader 1.2 to update calibration parameters in EEPROM before using an RP LIDAR module. If you are not using an RP LIDAR module, you do not need to do this.

Changes since version 1.10:
- Added RP LIDAR integration for A1M8, A2M8, A2M6, and A3M1. This only functions at a 2 kHz sampling rate, and was only tested on A1M8.     A2M8 and A2M6 should almost certainly work. A3M1 functionality is less certain.
- RP LIDAR is auto detected with both SENSE1 and SENSE2 pins tied to ground. This requires an additional voltage regulator in LIDAR module to power LIDAR motor at 3.65V for 5 Hz rotation rate.
- Added new calibration parameter locations on EEPROM for RP LIDAR.
- Fixed bug that would choose the wrong shot when deleting a shot.
- Changed method for averaging azimuth readings as previous method produced incorrect value when pointed due north.
- Redo of a shot now starts with the same box corner selected as the previous shot attempt.
- Added function to Calibration menu to check the quality of the azimuth calibration.
- Added support for LIDAR subtypes and settings to .cvl file.
- Includes update of LSM303 library to increase magnetometer data rate to 12.5 Hz
- Misc small bug fixes.

### Version 1.10

Updated to add new function to view completed LIDAR scans on screen, added a log file of system and selected raw survey data to accompany each scan, and other minor changes.


## Instructions for setting up Arduino IDE to compile and install the Caveatron Software

### Install Arduino IDE

Download the IDE from the link below. It has been tested on the current version 1.8.5:

https://www.arduino.cc/en/Main/Software

If a newer version no longer functions properly, previous versions of the Arduino IDE can be found here:

https://www.arduino.cc/en/Main/OldSoftwareReleases#previous

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

1. Clone the https://github.com/Caveatron/caveatron repository.
2. Locate your `Arduino Libraries` folder:
    - Windows: `C:\Users\{user}\Documents\Arduino\libraries\`
    - Mac: `{user}/Documents/Arduino/libraries/`
3. Copy the `Caveatron_GUI` and `Caveatron_Hardware` libraries into the `Arduino Libraries` folder

### Download and Install Third-Party Libraries

1. Clone the https://github.com/Caveatron/caveatron_libraries repository. (This contains additional, third_party libraries that Caveatron needs, but which are covered by difference software licenses.)
2. Locate your `Arduino Libraries` folder:
    - Windows: `C:\Users\{user}\Documents\Arduino\libraries\`
    - Mac: `{user}/Documents/Arduino/libraries/`
3. Copy all 10 folders into the `Arduino Libraries` folder


### Purchase a Coldtears Electronics 3.5" LCD Display to obtain the additional library you will need

The final library you will need is the UTFT_GHL library. At the request of the author, it cannot be distributed publically and requires the purchase of a Coldtears Electronics LCD display to obtain it (which you will need anyway in order to build a Caveatron). The displays can be purchased here:

https://www.ebay.com/itm/3-5-inch-TFT-LCD-module-w-Font-IC-480x320-full-angle-arduino-DUE-MEGA-3-2-4-3-/121057702584?hash=item1c2f99f2b8

To obtain the library after you have purchased the display:
1. Navigate to http://www.utftghl.com/downloads.html
2. Here you will be asked to fill in three fields, which are on a sticker applied to the ColdTears Electronics screen that you bought.
3. Download the file (warning: the link will be one-time usage, so be sure to keep the file somewhere safe)
4. The download will be a .7z file. To decompress on Windows you can use 7zip (http://www.7-zip.org) or on a Mac you can use Keka (http://www.kekaosx.com)
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

