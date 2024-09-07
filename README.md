# Caveatron

The Caveatron is a unique compact, handheld, electronic device for mapping caves or other underground environments. It functions as a cave survey tool, recording conventional distance, azimuth and inclination measurements and can also go far beyond that with its integrated LIDAR to capture a full 3D scan of the cave interior. It greatly speeds up the cave survey process by recording station-to-station shots in just a few seconds and performs a station-to-station 3D scan traverse in less than a minute. The user can fully interact with the device and review data with the built-in touchscreen. A USB port provides access to the stored data from a PC or Mac and is also used to charge the integrated long-life battery pack. The station measurement file is compatible with Walls Cave Mapping Software and the LIDAR scan file can be quickly processed into a standard point cloud with custom-written software. The Caveatron is designed to be easily portable, lightweight, and low cost as well as water and dirt resistant and durable enough to survive regular use in a cave.

## Key Features

- Survey and 3D scan caves
- 12 m or 30 m LIDAR range at up to 32000 pts/sec
- Laser rangefinder up to 40 m
- Integrated intertial measurement unit including digital compass, inclinometer, and gyroscope
- Position estimation during scanning
- Fast Teensy processor
- File transfer to PC over USB
- Water/dust resistant 3D printed enclosure
- Custom printed circuit board

## What's New (2024-05-03)

Rev C is the current and preferred version and is the only one in active development. This version uses the faster Teensy 4.1 processor and has several enhancements including ability to use the 30m RPLIDAR S2 or 12 RPLIDAR A1M8, a new magnetometer module for much improved compass accuracy and reliability, a capacitive touchscreen, faster battery charging, and a smaller size.
New revisions of the Rev C board (C.3 and C.4) have been posted along with an updated bill of materials. The Rev C assembly documentation has not yet been updated for the new PCBs but is not significantly different.

Rev B is based on the older Teensy 3.6 processor and is now deprecated and some components may no longer be available.

The original, Arduino Due-based version (Rev A) information is all still available and can now be found in the legacy_material_rev-A directory.
In the Rev A version, many people had difficulty with compiling the code in terms of getting all the libraries to work together and getting all the settings right. With the Teensy, it is relatively easy to load a pre-compiled HEX file, avoiding all the software headaches. For now I am just going to provide the HEX file instead of all the code to simplify things, since the code has gotten even more complex in the new version and I am no longer using the Arduino IDE. If you want to work on the code, please contact me directly.

## Summary of Caveatron Directory Contents for each Revision

### Documentation
- Bill of Materials
- Assembly Instructions
- Wire Harness Assembly Instructions
- 3D Printed Parts Description

### PCB
- Schematic diagram
- KiCad files
- PCB gerber files

### Mechanical
- STL files for all 3D printed parts
- STEP files for all 3D printed parts

## These items can be found in other folders:

### Calibration
Info and calculators can be found at https://github.com/Caveatron/Calibration

### Software
Software can be found at https://github.com/Caveatron/Software
