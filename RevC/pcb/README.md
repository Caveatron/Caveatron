# Caveatron Printed Circuit Boards (PCBs)

This folder provides the files needed to create PCBs for the Caveatron. I use PCBWay for fabrication and the Gerber files are setup for their requirements. For each board version, the following are providedL
- Gerber files: Send these to the PCB fabricator to get your board produced. Nothing else should be needed.
- Kicad files: The files for the schematic and board layout if you need to make an edit.
- PDF of the circuit schematic

## What is the difference between the PCB revisions?

### Rev C.4
This is the preferred board version. Use this one for most builds.

### Rev C.3
Use this version if want to use the Pololu MinIMU or AltIMU modules to provide all of the IMU data and not use the RM3100. This is lower cost but will result in lower quality azimuth data. You can also use this version if you want to use the MinIMU or AltIMU modules to provide just the accelerometer and gyroscope data alongside using the RM3100 for the magnetometer.

### Rev C.2
Use this version if you want to use a coin cell battery to maintain the RTC, although the battery will have to be replaced every 6 weeks. Can be upgraded to replace coin cell with modification. It is otherwise the same as C.3. Not recommended.
