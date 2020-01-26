# Caveatron

The Caveatron is a unique compact, handheld, electronic device for mapping caves or other underground environments. It functions as a cave survey tool, recording conventional distance, azimuth and inclination measurements and can also go far beyond that with its integrated LIDAR to capture a full 3D scan of the cave interior. It greatly speeds up the cave survey process by recording station-to-station shots in just a few seconds and performs a station-to-station 3D scan traverse in less than a minute. The user can fully interact with the device and review data with the built-in touchscreen. A USB port provides access to the stored data from a PC or Mac and is also used to charge the integrated long-life battery pack. The station measurement file is compatible with Walls Cave Mapping Software and the LIDAR scan file can be quickly processed into a standard point cloud with custom-written software. The Caveatron is designed to be easily portable, lightweight, and low cost as well as water and dirt resistant and durable enough to survive regular use in a cave.

## NEW DESIGN!

The Caveatron has been redesigned with numerous new features:
- Much improved microprocessor (Teensy 3.6)
- Much greater scan resolution (4-8x more points per rotation)
- Better position estimation (now includes gyroscope)
- Easier file transfer (standard MTP interface over USB)
- Various improvements including auto power off and saved preferences
- Better water/dust resistance
- Fully 3D printed enclosure design that is slightly smaller and lighter
- Much easier assembly with custom printed circuit board

The previous version (Rev A) information is all still available and can now be found in the rev-A_version directory.

In the Rev A version, many people had difficulty with compiling the code in terms of getting all the libraries to work together and getting all the settings right. With the Teensy, it is relatively easy to load a pre-compiled HEX file, avoiding all the software headaches. For now I am just going to provide the HEX file instead of all the code to simplify things, since the code has gotten even more complex in the new version and I am no longer using the Arduino IDE. If you want to work on the code, please contact me directly.

## Summary of Caveatron Directory Contents

### Documentation
- Bill of Materials
Will include assembly notes once they are prepared.

### Electronics
- Schematic diagram
- PCB gerber files

### Mechanical
- STL files for all 3D printed parts
Will include printing notes and drawings for the LIDAR and LRF windows (which must be custom cut) once they are prepared.

### Software
- Pre-compiled HEX file of the Caveatron software
Will include instructions for loading the code once they are prepared.


## Software Release Notes
### Version 2.01
- Added LIDAR window distortion calibration parameters to IMU file.
- Added absolute scaling values for IMU sensors.
- Fixed shot/scan review to show most recent 96/88 entries (previously didn't work properly when entries maxed out).
- Changed shot & scan review pages to show 8 entries per page (previously showed 7).
- Improved statistics page.
- Bug fixes.

### Version 2.00
- Rewritten to work on the Teensy 3.6 platform. Arduino Due no longer supported.
- Numerous changes throughout to support new hardware and improve performance.
Major revisions:
- RPLIDAR can now scan at 8K pts/sec. Both rotation speed and scan rate are adjustable in settings.
- Gyroscope support added. IMU data now captured at 40msec intervals.
- LIDAR scanning code rewritten to grab LRF and IMU data more quickly and consistently.
- CVL LIDAR file format changed (v2) to store raw IMU data for enhanced position/orientation post processing.
- Can directly open microSD card on PC as MTP disk and drag and drop files. Caveatron Connect app no longer used.
- Auto power shutoff after user selectable interval.
- Support for saved preferences.
- Fonts and graphics now stored on separate SPI flash IC. No longer dependent on display module.
- Removed support for XV LIDAR and Sparkfun Battery Babysitter. No longer supports changing LIDAR modules.