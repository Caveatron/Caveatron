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
}

// Open survey and settings files when entering a mode
boolean OpenSurveyFiles() {
  boolean fileResult;
  if (currentMode==modeShot) { 
    fileName = fileNameBase+".srv";
    char _fileName[sizeof(fileName)+1];
    fileName.toCharArray(_fileName, sizeof(_fileName));
    if (!settingsFile.open("settings.txt", O_RDWR)) {
      settingsFlag = false;
      infoBarMsg = "No Settings File";
      ErrorBox("No Settings", "File Found-", "Start New Survey", 99);
      fileResult = false;  
    }
    else if (!theFile.open(_fileName, O_RDWR | O_AT_END)) {
      ErrorBox("Could Not Open", "Shot File", "", 99);
      settingsFile.close();  
      fileResult = false;  
    }
    else {
      fileResult = true;
      filePosition = theFile.curPosition();
    }
  }
  else if ((currentMode==modePassage)||(currentMode==modeRoom)) { 
    fileName = fileNameBase+".cvl";
    char _fileName[sizeof(fileName)+1];
    fileName.toCharArray(_fileName, sizeof(_fileName));
    if (!settingsFile.open("settings.txt", O_RDWR)) {
      settingsFlag = false;
      infoBarMsg = "No Settings File";
      
      ErrorBox("No Settings", "File Found-", "Start New Survey", 99);
      fileResult = false;  
    }
    else {
      lidarFile.open(_fileName, ios::out | ios::app);
      if (!lidarFile.is_open()) {
        ErrorBox("Could Not Open", "LIDAR File", "", 99);
        settingsFile.close();  
        fileResult = false;  
      }
      else fileResult = true;
    }
  }
  else if (currentMode==modeSurvey) {
    fileName = fileNameBase+".srv";
    char _fileName[sizeof(fileName)+1];
    fileName.toCharArray(_fileName, sizeof(_fileName));
    inFile.open(_fileName, ios::in);
    if(inFile.is_open()==false) {
      ErrorBox("Could Not Open", "Shot File", "", 99);
      fileResult = false;
    }
  }
  else if (currentMode==modeEditSurvey) {
    fileName = fileNameBase+".srv";
    char _fileName[sizeof(fileName)+1];
    fileName.toCharArray(_fileName, sizeof(_fileName));
    if (!theFile.open(_fileName, O_RDWR)) {
      ErrorBox("Could Not Open", "Shot File", "", 99);
      fileResult = false;  
    }
  }
  return fileResult;
}

// Close survey and settings files
void CloseSurveyFiles() {
  settingsFile.close();  
  theFile.close();
  lidarFile.close();
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


