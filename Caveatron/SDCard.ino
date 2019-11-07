/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       SD Card and General File Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Startup SD card and read in values from Settings file
void InitSD() {
  char lineBuffer[25], c1;
  #if defined(_SAM3XA_)
    SDFlag = sd.begin(caveatron.SD_CSpin, SPI_FULL_SPEED);
  #endif
  if (SDFlag==false) {
    infoBarMsg = "No SD Card";
    settingsFlag=false;
    //sd.initErrorHalt();
  }
  else {
    inFile.open("settings.txt", ios::in);
    settingsFlag = inFile.is_open();
    if (settingsFlag==false) {
      infoBarMsg = "No Settings File";
      //sd.errorHalt("");
    }
    else {
      inFile.getline(lineBuffer, 9, '\n');
      fileNameBase = String(lineBuffer);
      inFile.getline(lineBuffer, 25, '\n');
      caveName = String(lineBuffer);
      inFile >> surveyYear >> surveyMonth >> surveyDay >> surveyHour >> surveyMinute;
      inFile.getline(lineBuffer, 1, '\n');
      streamPositionData = inFile.tellg();
      inFile >> lengthTrav >> lengthHoriz >> lengthVert;
      inFile.getline(lineBuffer, 1, '\n');
      inFile >> numStations >> numTraverses >> numSplays;
      inFile.getline(lineBuffer, 1, '\n');  
      inFile.getline(lineBuffer, 8, '\t');
      stationFrom = String(lineBuffer);
      inFile.getline(lineBuffer, 8, '\t');
      stationTo = String(lineBuffer);
      streamPositionScans = inFile.tellg();
      inFile.close();
      infoBarMsg = "Survey: "+caveName;
    }
  }
}

// Create shot and LIDAR files for New Survey setup
void CreateSurveyFiles() {
  boolean newsettings=false;
  fileNameBase = caveCode+dateMonth+dateDay;
  surveyYear = caveatron.RTCyear;
  surveyMonth = caveatron.RTCmonth;
  surveyDay = caveatron.RTCday;
  surveyHour = caveatron.RTChour;
  surveyMinute = caveatron.RTCminute;
  surveySecond = caveatron.RTCsecond;
  lengthTrav = 0;
  lengthHoriz = 0;
  lengthVert = 0;
  numStations = 0;
  stationFrom="";
  stationTo=""; 
  sta.lastTraverseNum = 0;
  sta.lastSplayNum = 0;
  numTraverses = 0;
  numSplays = 0;
 
  // Create Settings File 
  if (!sd.exists("settings.txt")) newsettings=true;
  outFile.open("settings.txt", ios::out);
  if (!outFile.is_open()) {
    ErrorBox("Could Not Create", "Settings File", "", 99);
  } else {
    char _fileNameBase[sizeof(fileNameBase)+1];
    fileNameBase.toCharArray(_fileNameBase, sizeof(_fileNameBase)+1);
    char _caveName[24];
    caveName.toCharArray(_caveName, sizeof(_caveName)+1);
    outFile << _fileNameBase << endl;
    outFile << _caveName << endl;
    outFile << surveyYear << '\t' << surveyMonth << '\t' << surveyDay << '\t' << surveyHour << '\t' << surveyMinute << endl;
    streamPositionData = outFile.tellp();
    outFile << "0000.00" << '\t' << "0000.00" << '\t' << "0000.00" << endl;
    outFile << "0000" << '\t' << "0000" << '\t' << "0000" << endl;
    streamPositionScans = inFile.tellg();
    outFile << '\t' << '\t' << "              " << endl;
    outFile.close();
    settingsFlag=true;
    infoBarMsg = "Survey: "+caveName;
    if (newsettings) {
      theFile.open("settings.txt", O_READ);
      theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
      theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
      theFile.close();
    }
  } 
  
  // Create Shot File
  fileName = caveCode+dateMonth+dateDay+".srv";
  char _fileNameS[sizeof(fileName)+1];
  fileName.toCharArray(_fileNameS, sizeof(_fileNameS));
  if (!theFile.open(_fileNameS, O_RDWR | O_CREAT)) {
    ErrorBox("Could Not Create", "Shot File", "", 99);    
  }
  else {
    theFile.println(";CAVEATRON SHOT FILE");
    theFile.println(";Serial Number: "+caveatron.serialNumber+" - Firmware version: "+softwareVersion);
    theFile.println(";"+caveName);
    theFile.println(";Survey Date: "+dateMonth+'/'+dateDay+"/20"+dateYear);
    theFile.println();
    //if (dUnits==false) theFile.println("#UNITS Feet Order=DAV LRUD=F:LRUD");
    theFile.println("#UNITS Meters Order=DAV");
    theFile.println("#Date 20"+dateYear+'-'+dateMonth+'-'+dateDay);
    theFile.println(); theFile.println();
    theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.close();
  }
  
  // Create LIDAR File
  fileName = caveCode+dateMonth+dateDay+".cvl";
  char _fileNameL[sizeof(fileName)+1];
  fileName.toCharArray(_fileNameL, sizeof(_fileNameL));
  if (!theFile.open(_fileNameL, O_RDWR | O_CREAT)) {
    ErrorBox("Could Not Create", "LIDAR File", "", 99);    
  }
  else {
    theFile.println(";CAVEATRON LIDAR FILE");
    theFile.println(";Serial Number: "+caveatron.serialNumber+" - Firmware version: "+softwareVersion);
    theFile.println(";"+caveName);
    theFile.println(";Survey Date: "+dateMonth+'/'+dateDay+"/20"+dateYear);
    theFile.println();
    theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.close();
  }

  // Create IMU Calibration File
  fileName = caveCode+dateMonth+dateDay+".imu";
  char _fileNameI[sizeof(fileName)+1];
  fileName.toCharArray(_fileNameI, sizeof(_fileNameI));
  if (!theFile.open(_fileNameI, O_RDWR | O_CREAT)) {
    ErrorBox("Could Not Create", "IMU File", "", 99);    
  }
  else {
    theFile.println(";CAVEATRON IMU CALIBRATION PARAMETERS FILE");
    theFile.println(";"+caveName); theFile.println();
    SDWriteCalibrationValues(1);
    theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.close();
  }
  
  // Create Log File
  fileName = caveCode+dateMonth+dateDay+".log";
  char _fileNameZ[sizeof(fileName)+1];
  fileName.toCharArray(_fileNameZ, sizeof(_fileNameZ));
  if (!theFile.open(_fileNameZ, O_RDWR | O_CREAT)) {
    ErrorBox("Could Not Create", "Log File", "", 99);    
  }
  else {
    theFile.println(";CAVEATRON LOG FILE");
    theFile.println(";Serial Number: "+caveatron.serialNumber+" - Firmware version: "+softwareVersion);
    theFile.println(";"+caveName);
    theFile.println(";Survey Date: "+dateMonth+'/'+dateDay+"/20"+dateYear);
    theFile.println();
    theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
    theFile.close();
  }
}

// Open survey and settings files when entering a mode
boolean OpenSurveyFiles() {
  boolean fileResult = false;
  char _fileName[sizeof(fileNameBase)+5];
  switch (currentMode) {
    case modeShot:
      fileName = fileNameBase+".srv";
      fileName.toCharArray(_fileName, sizeof(_fileName));
      if (!theFile.open(_fileName, O_RDWR | O_AT_END)) {
        ErrorBox("Could Not Open", "Shot File", "", 99);
      } 
      else {
        filePosition = theFile.curPosition();
        if (OpenSettingsFileWrite() && OpenLogFileWrite()) fileResult = true;
      }
      break;
    case modePassage:
    case modeRoom:
      fileName = fileNameBase+".cvl";
      fileName.toCharArray(_fileName, sizeof(_fileName));
      lidarFile.open(_fileName, ios::out | ios::app);
      if (!lidarFile.is_open()) {
        ErrorBox("Could Not Open", "LIDAR File", "", 99); 
      }
      else {
        if (OpenSettingsFileWrite() && OpenLogFileWrite()) fileResult = true;
      }
      break;
    case modeSurvey:
      fileName = fileNameBase+".srv";
      fileName.toCharArray(_fileName, sizeof(_fileName));
      inFile.open(_fileName, ios::in);
      if(inFile.is_open()==false) {
        ErrorBox("Could Not Open", "Shot File", "", 99);
      } 
      else fileResult = true;
      break;
    case modeEditSurvey:
      fileName = fileNameBase+".srv";
      fileName.toCharArray(_fileName, sizeof(_fileName));
      if (!theFile.open(_fileName, O_RDWR)) {
        ErrorBox("Could Not Open", "Shot File", "", 99);
      }
      else fileResult = true;
      break;
    case modeViewScans:
      fileName = fileNameBase+".log";
      fileName.toCharArray(_fileName, sizeof(_fileName));
      if (!theFile.open(_fileName, O_READ)) {
        ErrorBox("Could Not Open", "Log File", "", 99);
      }
      else fileResult = true;
      break;
  }
  if (fileResult==false) CloseSurveyFiles();
  return fileResult;
}

//Open Settings File to update values
boolean OpenSettingsFileWrite() {
  if (!settingsFile.open("settings.txt", O_RDWR)) {
    settingsFlag = false;
    ErrorBox("No Settings", "File Found-", "Start New Survey", 99);
    return false;
  } else return true;
}

// Open Log File as stream for writing
boolean OpenLogFileWrite() {
  fileName = fileNameBase+".log";
  char _fileName[sizeof(fileName)+1];
  fileName.toCharArray(_fileName, sizeof(_fileName));
  if (!logFile.open(_fileName, O_RDWR | O_AT_END)) {
    ErrorBox("Could Not Open", "Log File", "", 99);
    return false;
   } else return true;
}

// Close survey and settings files
void CloseSurveyFiles() {
  settingsFile.close();  
  theFile.close();
  lidarFile.close();
  logFile.close();
}


// Update values in Settings file when ending an operation
void UpdateSettingsFile() {
  char lenSt[23], numSt[14], scanSt[8];
  settingsFile.rewind();
  if (settingsFile.seekSet(streamPositionData)) {
    sprintf(lenSt, "%07.2f\t%07.2f\t%07.2f", lengthTrav, lengthHoriz, lengthVert);
    settingsFile.println(lenSt);
    sprintf(numSt, "%04d\t%04d\t%04d", numStations, numTraverses, numSplays);
    settingsFile.println(numSt);
    settingsFile.print(stationFrom); settingsFile.print("\t"); settingsFile.print(stationTo); settingsFile.print("\t");
    if ((currentMode==modePassage)||(currentMode==modeRoom)) {
      settingsFile.rewind();
      if (streamPositionScanLine==0) {
        if (!settingsFile.seekEnd()) errorFlag = true;
      } else {
        if (!settingsFile.seekSet(streamPositionScanLine)) errorFlag = true;
      }
      if (errorFlag==false) {
        sprintf(scanSt, "\t%03d\t%03d", sta.lastTraverseNum, sta.lastSplayNum);
        settingsFile.print(sta.code); settingsFile.println(scanSt);
      }
    } 
  } else errorFlag = true;
  if (errorFlag==true) ErrorBox("Settings File", "Write Failure", "", 99);
}



// Detemine last traverse of splay number from settings file to prefill textbox
void GetLastScanNum() {
  char lineBuffer[25], staBuffer[20];
  settingsFile.rewind();
  settingsFile.seekSet(streamPositionScans);
  settingsFile.fgets(lineBuffer, 18);
  streamPositionScanLine = settingsFile.curPosition();
  while(settingsFile.fgets(lineBuffer, 18) > 0) {
    sta.code = String(strtok(lineBuffer, "\t"));
    sta.lastTraverseNum = atoi(strtok(NULL, "\t"));
    sta.lastSplayNum = atoi(strtok(NULL, "\t"));
    if (((currentMode==modePassage)&&(sta.code==traverseTo))||((currentMode==modeRoom)&&(sta.code==splayStation))) return;
    else streamPositionScanLine = settingsFile.curPosition();
  }
  streamPositionScanLine = 0;
  sta.lastTraverseNum = 0;
  sta.lastSplayNum = 0;
}


// Display SD card information
void SDCardInfo() {

  uint32_t cardSize;
  long filespace=0;
  
  myGLCD.setColor(255,255,255);
  myGLCD.setBackColor(0,0,0);
  myGLCD.setFont(SmallFont);

  cardSize = sd.card()->cardSize();
  if (cardSize == 0) {
    ctGUI.print("Card Size Failed", 160, 50, caveatron.FONT_19, 2, CENTER_J);
    return;
  }
  
  // print the type of card
  ctGUI.print("Card type: ", 20, 50, caveatron.FONT_19, 2, LEFT_J);
  switch (sd.card()->type()) {
    case SD_CARD_TYPE_SD1:
     ctGUI.print("SD1", 300, 50, caveatron.FONT_19, 2, RIGHT_J);
      break;
    case SD_CARD_TYPE_SD2:
      ctGUI.print("SD2", 300, 50, caveatron.FONT_19, 2, RIGHT_J);
      break;
    case SD_CARD_TYPE_SDHC:
      if (cardSize < 70000000) {
        ctGUI.print("SDHC", 300, 50, caveatron.FONT_19, 2, RIGHT_J);
      } else {
        ctGUI.print("SDXC", 300, 50, caveatron.FONT_19, 2, RIGHT_J);
      }
      break;
    default:
      ctGUI.print("Unknown", 300, 50, caveatron.FONT_19, 2, RIGHT_J);
  }
 
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  ctGUI.print("Volume type is FAT ", 20, 75, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumI(int(sd.vol()->fatType()), 300, 75, caveatron.FONT_19, 2, RIGHT_J);
  
  ctGUI.print("Card size (MB): ", 20, 115, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumI(0.000512*cardSize, 300, 115, caveatron.FONT_19, 2, RIGHT_J);
  
  uint32_t volFree = sd.vol()->freeClusterCount();
  float fs = 0.000512*volFree*sd.vol()->blocksPerCluster();

  ctGUI.print("Free Space (MB): ", 20, 140, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumI((int) fs, 300, 140, caveatron.FONT_19, 2, RIGHT_J);
  
  int numFiles=0;
  // Number of files on the card
  sd.vwd()->rewind();
  while (theFile.openNext(sd.vwd(), O_READ)) {
    if (theFile.isDir()) {
    } else {
      numFiles++; 
    }
    theFile.close();
  }
  
  ctGUI.print("Number of Files: ", 20, 180, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumI(numFiles, 300, 180, caveatron.FONT_19, 2, RIGHT_J);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                         Functions for reading/writing calibration files to/from SD Card
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Write Calibration values to SD card (full user save or partial survey file save)
void SDWriteCalibrationValues(int writeMode) {
  char checkArr[10];
  if (writeMode==2) {
    theFile.println(";CAVEATRON IMU CALIBRATION PARAMETERS FILE");
    theFile.println(";Full Parameter User Save"); theFile.println();
  }
  theFile.println(";Date & Time"); theFile.println("#D");
  theFile.println("20"+dateYear+"-"+dateMonth+"-"+dateDay+" "+timeHour+":"+timeMinute+":"+timeSecond); theFile.println();
  theFile.println(";Serial Number"); theFile.println("#N");
  theFile.println(caveatron.serialNumber); theFile.println();
  theFile.println(";Accelerometer Calibration"); theFile.println("#A");
  SDWriteCalibrationValueSet(ADDR_ACC_CAL,3,4,5);
  
  theFile.println(";Magnetometer Alignment Calibration - No LIDAR"); theFile.println("#L0");
  SDWriteCalibrationValueSet(ADDR_MAG_ALIGNCAL_NOLID,4,2,1);
  theFile.println(";Magnetometer Alignment Calibration - XV LIDAR"); theFile.println("#L1");
  SDWriteCalibrationValueSet(ADDR_MAG_ALIGNCAL_LIDXV,4,2,1);
  theFile.println(";Magnetometer Alignment Calibration - SWEEP LIDAR"); theFile.println("#L2");
  SDWriteCalibrationValueSet(ADDR_MAG_ALIGNCAL_LIDSW,4,2,1);
  theFile.println(";Magnetometer Alignment Calibration - RP LIDAR"); theFile.println("#L3");
  SDWriteCalibrationValueSet(ADDR_MAG_ALIGNCAL_LIDRP,4,2,1);
  
  theFile.println(";Magnetometer Hard & Soft Iron Calibration - No LIDAR"); theFile.println("#M0");
  SDWriteCalibrationValueSet(ADDR_MAG_HSCAL_NOLID,3,4,5);
  theFile.println(";Magnetometer Hard & Soft Iron Calibration - XV LIDAR"); theFile.println("#M1");
  SDWriteCalibrationValueSet(ADDR_MAG_HSCAL_LIDXV,3,4,5);
  theFile.println(";Magnetometer Hard & Soft Iron Calibration - SWEEP LIDAR"); theFile.println("#M2");
  SDWriteCalibrationValueSet(ADDR_MAG_HSCAL_LIDSW,3,4,5);
  theFile.println(";Magnetometer Hard & Soft Iron Calibration - RP LIDAR"); theFile.println("#M3");
  SDWriteCalibrationValueSet(ADDR_MAG_HSCAL_LIDRP,3,4,5);

  if (writeMode==2) {
    theFile.println(";LIDAR Orientation Angle - XV LIDAR"); theFile.println("#O1");
    SDWriteCalibrationValueSet(ADDR_LIDXV_ORIENTCAL,1,1,1);
    theFile.println(";LIDAR Orientation Angle - SWEEP LIDAR"); theFile.println("#O2");
    SDWriteCalibrationValueSet(ADDR_LIDSW_ORIENTCAL,1,1,1);
    theFile.println(";LIDAR Orientation Angle - RP LIDAR"); theFile.println("#O3");
    SDWriteCalibrationValueSet(ADDR_LIDRP_ORIENTCAL,1,1,1);
    theFile.println(";LRF Range Offset"); theFile.println("#R");
    SDWriteCalibrationValueSet(ADDR_LRF_RANGECAL,1,1,1);
    theFile.println(";Screen Calibration"); theFile.println("#S");
    for (int i=0; i<3; i++) {
      String arr = caveatron.EEPROM_readCharArray(ADDR_SCREEN_CAL+(i*0x20), 10);
      arr.toCharArray(checkArr, 10);
      theFile.print("0x"+String(checkArr));
      if (i<2) theFile.print(",");
    }
    theFile.println(); theFile.println();
    theFile.println(";Hardware Code"); theFile.println("#H");
    theFile.println(hardwareCode);
  }
}

//Reads an array from the EEPROM and writes to the SD card
void SDWriteCalibrationValueSet(uint32_t addr, int xNum, int yNum, int prec) {
  float f;
  for(int j=0;j<yNum;j++) {
    for(int i=0;i<xNum;i++) {
      if ((i!=0)) theFile.print(",");
      f = caveatron.EEPROM_readFloat((addr+(4*i))+(0x20*j));
      if (f==0) theFile.print("0.0");
      else if (abs(f) < 1) theFile.print(double2s(f, prec));
      else theFile.print(f, prec);
    }
    theFile.println();
  }  
  theFile.println();  
}

//Reads calibration values from SD card (from a new IMU file or a saved IMU file)
int SDLoadCalibrationValues(int readMode) {
  char linebuffer[61];
  char cp, cc, tp[4], d[11], n[6];
  float v[4], f;
  boolean dataValid=true;
  for (int i=0; i<3; i++) loadConfirmStr[i]="";
  if ((readMode==1)||(readMode==2)) inFile.open("Cal_new.imu", ios::in);
  else if ((readMode==3)||(readMode==4)) inFile.open("Cal_save.imu", ios::in);
  if(inFile.is_open()==false) {
    ErrorBox("Could Not Open", "Calibration File", "", 99);
    return(-1);
  }
  while((inFile.eof() == false)&&(inFile.peek()>=0)) { 
    cp = inFile.peek();
    if (cp=='#') {
      inFile >> tp >> ws;
      switch (tp[1]) {
        case 'D':
          inFile.get(d, 11, ' ');
          break;
        case 'N':
          inFile.get(n, 6, '\n');
          break;
        case 'A':
          loadConfirmStr[0] = "Accelerometer";
          for (int i=0; i<4; i++) {
            inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> ws;
            if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_ACC_CAL+(i*0x20), v, 3);
          }
          break;
        case 'L':
          if (loadConfirmStr[1].length()==0) loadConfirmStr[1] += "Mag Align:  ";
          switch (tp[2]) {
            case '0':
              loadConfirmStr[1] += "NL  ";
              for (int i=0; i<2; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> cc >> v[3] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_NOLID+(i*0x20), v, 4);
              }
              break;
            case '1':
              loadConfirmStr[1] += "XV  ";
              for (int i=0; i<2; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> cc >> v[3] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDXV+(i*0x20), v, 4);
              }
              break;
            case '2':
              loadConfirmStr[1] += "SW  ";
              for (int i=0; i<2; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> cc >> v[3] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDSW+(i*0x20), v, 4);
              }
              break;
            case '3':
              loadConfirmStr[1] += "RP  ";
              for (int i=0; i<2; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> cc >> v[3] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDRP+(i*0x20), v, 4);
              }
              break;
          }
          break;
        case 'M':
          if (loadConfirmStr[2].length()==0) loadConfirmStr[2] = "Mag H&S:  ";
          switch (tp[2]) {
            case '0':
              loadConfirmStr[2] += "NL  ";
              for (int i=0; i<4; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_HSCAL_NOLID+(i*0x20), v, 3);
              }
              break;
            case '1':
              loadConfirmStr[2] += "XV  ";
              for (int i=0; i<4; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_HSCAL_LIDXV+(i*0x20), v, 3);
              }
              break;
            case '2':
              loadConfirmStr[2] += "SW  ";
              for (int i=0; i<4; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_HSCAL_LIDSW+(i*0x20), v, 3);
              }
              break;
            case '3':
              loadConfirmStr[2] += "RP  ";
              for (int i=0; i<4; i++) {
                inFile >> v[0] >> cc >> v[1] >> cc >> v[2] >> ws;
                if ((readMode==2)||(readMode==4)) dataValid &= caveatron.EEPROM_writeFloatArray(ADDR_MAG_HSCAL_LIDRP+(i*0x20), v, 3);
              }
              break;
          }
          break;
        default:
          inFile.getline(linebuffer, 60, '\n');
          break;
      }
    }
    inFile.getline(linebuffer, 60, '\n');
  }
  inFile.close();
  snprintf(infoStr, sizeof(infoStr)+1, "%s  %s", d, n);
  return dataValid;
}

