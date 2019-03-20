/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         General Shot Acquisition Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Takes an azimuth, inclination and distance measurement - used in multiple modes
void TakeShot() {
  float dis[3];
  float azi = 0;
  float inc = 0;
  float rol = 0;
  String stationFromPrev, stationToPrev;
  uint8_t LRFstatus = 0, IMUstatus, numGoodShots;
  Timer4.stop();
  myGLCD.setColor(WHITE_STD);
  myGLCD.fillRect(0, 0, 319, 479);
  ctGUI.print("TAKING", 159, 186, caveatron.FONT_43, 3, CENTER_J, BLACK_STD, WHITE_STD);
  ctGUI.print("SHOT", 159, 260, caveatron.FONT_43, 3, CENTER_J, BLACK_STD, WHITE_STD);
  BuzzerBEEP(800, 200);
  Serial1Flush();
  delay(500);
  for (int i = 0; i < 3; i++) {
    if (LRFFlag == true) caveatron.LRF_Fire();
    ReadCompassData(300);
    if (LRFFlag == true) LRFstatus = caveatron.LRF_GetRange();
    dis[i] = caveatron.LRFdistance;
    azi = SumAzimuth (azi, (azimuth/57.29578));
    inc += inclination;
    rol += roll;
    //Check if roll angle is greater than 30 deg or less then 150 deg (when level)
    if ((fabs(roll) > 90-(cos(inclination/57.3)*60))&&(fabs(roll) < 90+(cos(inclination/57.3)*60))) LRFstatus = 7;
    //Break for error
    if ((LRFFlag==true) && (LRFstatus != 1)) break;
    numGoodShots = i+1;
  }
  if (laserFlag == true) caveatron.LRF_LaserOff();
  if ((LRFstatus==1) && ((fabs(dis[2] - dis[1]) > 0.05) || (fabs(dis[1] - dis[0]) > 0.05) || (fabs(dis[2] - dis[0]) > 0.05))) LRFstatus = 5;
  
  distance = (dis[0] + dis[1] + dis[2]) / 3;
  azimuth = AverageAzimuth(azi, 3);
  inclination = inc / 3;
  roll = rol / 3;
  if (LRFstatus==1) IMUstatus = IMUErrorCheck(0);
  myGLCD.clrScr();
  drawInfoBar("Shot Complete");
  
  switch (currentMode) {
    case modeShot:
      stationFromPrev = stationFrom;
      stationToPrev = stationTo;
      stationFrom = stationFromNew;
      stationTo = stationToNew;
      caveatron.LRF_LaserOff();
      caveatron.LRF_PowerOff();
      temperature = caveatron.RTC_GetTemperature();
      ComputeBoxCorrection();
      if (LRFstatus == 1) LRFstatus = SaveShotData();  //Save shot data to survey file if shot succeeded     
      logFile.println("$@\t"+String(caveatron.LRFdistance)+'\t'+String(rawIMU[0])+'\t'+String(rawIMU[1])+'\t'+String(rawIMU[2])+'\t'+String(rawIMU[3])+'\t'+String(rawIMU[4])+'\t'+String(rawIMU[5]));  //Write averaged raw accel and mag values to log file
      logFile.println("$!\t"+String(LRFstatus)+'\t'+String(IMUstatus)+'\t'+String(numGoodShots)+"\tC"); //Write LRF status code to log file   
      logFile.flush();   
      drawStatusBar("SHOT MODE");
      ctGUI.print("STATIONS", 15, 26, caveatron.FONT_28, 3, LEFT_J, YELLOW_STD, BLACK_STD);
      if (backSight==true) {
        ctGUI.print(stationFrom + " - " + stationTo + " (B)", 304, 52, caveatron.FONT_34, 3, RIGHT_J, TAN_STD, BLACK_STD);
        stationFrom = stationFromPrev;
        stationTo = stationToPrev;
      } else ctGUI.print(stationFrom + " - " + stationTo, 304, 52, caveatron.FONT_34, 3, RIGHT_J, WHITE_STD, BLACK_STD);
      break;
    case modeManual:
      drawStatusBar("MANUAL MODE");
      break;
  }
  ctGUI.redrawAllObjects();
  if ((LRFFlag==true) && (LRFstatus != 1)) {
    ShotFailed(LRFstatus);
    if (currentMode==modeShot) {
      stationFrom = stationFromPrev;
      stationTo = stationToPrev;
    }
  } else {
    DataDisplaySetup();
    RefreshDataDisplay(LRFstatus);
    BuzzerBEEP(800, 1000);
  }
  laserFlag = false;
  shotFlag = false;
  Timer4.start();
}

//Handles a failed shot and displays error
void ShotFailed(uint8_t LRFstatus) {
    ctGUI.print("SHOT", 160, 150, caveatron.FONT_43, 3, CENTER_J, RED_STD, BLACK_STD);
    ctGUI.print("FAILED", 160, 200, caveatron.FONT_43, 3, CENTER_J, RED_STD, BLACK_STD);
    switch (LRFstatus) {
      case 2: 
        ctGUI.print("LRF Error", 160, 250, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
        break;
      case 3: 
        ctGUI.print("LRF Timed Out", 160, 250, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
        break;
      case 4: 
        ctGUI.print("LRF Out-of-Range", 160, 250, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
        break;
      case 5: 
        ctGUI.print("Mismatched readings", 160, 250, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
        break;
      case 6: 
        ctGUI.print("Data Save Error", 160, 250, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
        break;
      case 7: 
        ctGUI.print("Excessive Roll Angle", 160, 250, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
        break;
    }
    BuzzerBEEP(350, 1500);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        General Functions to Setup User Entry of Station Names and Scan Numbers
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Station Names User Entry Setup for Shot, Passage, and Room Modes
void StationEntrySetup() {
  String stationEntry = stationTo;
  ctGUI.print("Enter Station", 159, 25, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  CreateLetterPad(117);
  CreateNumberPad(315);
  btn1 = ctGUI.addButton(234, 24, 84, 84, BLUE_STD, "ENTER", caveatron.FONT_22, optVisible, 1);
  if (stationTo.length() <= 1) {
    btn2 = ctGUI.addButton(1, 24, 84, 84, BLUE_STD, "DELETE", caveatron.FONT_22, optInvisible, 2);
    btn3 = ctGUI.addButton(1, 24, 84, 84, RED_STD, "CANCEL", caveatron.FONT_22, optVisible, 3);
  } else {
    btn2 = ctGUI.addButton(1, 24, 84, 84, BLUE_STD, "DELETE", caveatron.FONT_22, optVisible, 2);
    btn3 = ctGUI.addButton(1, 24, 84, 84, RED_STD, "CANCEL", caveatron.FONT_22, optInvisible, 3);
  }
  if (currentMode == modeShot) {
    stationEntry = "";
    drawStatusBar("SHOT MODE");
    if (guiStep==1) {
      drawInfoBar("Last Shot: " + stationFrom + " - " + stationTo);
      ctGUI.print("FROM", 159, 50, caveatron.FONT_22, 3, CENTER_J, YELLOW_STD, BLACK_STD);
    } else if (guiStep==2) {
      drawInfoBar("From Station:  "+stationFromNew);
      ctGUI.print("   TO   ",159, 50, caveatron.FONT_22, 3, CENTER_J, YELLOW_STD, BLACK_STD);
    }
  } 
  else if (currentMode == modePassage) {
    drawStatusBar("PASSAGE MODE");
    drawInfoBar("Last Shot: " + stationFrom + " - " + stationTo);
    ctGUI.print("TO", 159, 50, caveatron.FONT_22, 3, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  else if (currentMode == modeRoom) {
    drawStatusBar("ROOM MODE");
    drawInfoBar("Last Shot: " + stationFrom + " - " + stationTo);
    ctGUI.print("SPLAY", 159, 50, caveatron.FONT_22, 3, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  txtbox1 = ctGUI.addTextInputBox(90, 75, 140, 33, stationEntry, caveatron.FONT_22, 2, CENTER_J, BLACK_STD, 7, optVisible, 0);
  if (ctGUI.GUIobject_data1[txtbox1] != 0) ctGUI.toggleButton(btn2, btn3);
}

//Preselection screen for Shot station type and name
void ShotStationSetup() {
  if (numStations==0) CreateScreen(screenStationEntry); //For the first station, skip directly to station entry screen
  else {
    stationNext = PredictNextStation(stationTo); 
    if (stationNext!="0") ctGUI.addButton(170, 64, 135, 68, GREEN_DRK, stationTo+" - "+stationNext, caveatron.FONT_22, optVisible, 1);
    ctGUI.addButton(15, 142, 135, 68, GREEN_DRK, stationTo+" - ___", caveatron.FONT_22, optVisible, 2);
    ctGUI.addButton(170, 142, 135, 68, GREEN_DRK, "___ - ___", caveatron.FONT_22, optVisible, 3);
    ctGUI.addButton(170, 226, 135, 68, TAN_DRK, stationTo+" - "+stationFrom, caveatron.FONT_22, optVisible, 4);
    ctGUI.addButton(15, 304, 135, 68, TAN_DRK, stationFrom+" - ___", caveatron.FONT_22, optVisible, 5);
    ctGUI.addButton(170, 304, 135, 68, TAN_DRK, "___ - ___", caveatron.FONT_22, optVisible, 6);
    ctGUI.addButton(15, 388, 290, 68, RED_DRK, "CANCEL", caveatron.FONT_28, optVisible, 7);
  
    myGLCD.setColor(GRAY_025);
    myGLCD.drawLine(15,56,304,56);
    myGLCD.drawLine(15,218,304,218);
    myGLCD.drawLine(15,380,304,380);
  
    ctGUI.print("CHOOSE SHOT TYPE", 160, 25, caveatron.FONT_28, 3, CENTER_J, YELLOW_STD, BLACK_STD);
    ctGUI.print("Front sight", 15, 64, caveatron.FONT_28, 2, LEFT_J, YELLOW_STD, BLACK_STD);
    ctGUI.print("Back sight", 15, 226, caveatron.FONT_28, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  
    drawStatusBar("SHOT MODE");
    drawInfoBar("Last Shot: " + stationFrom + " - " + stationTo);
  }
}

//Increment previous station code for next potential station
//If final character is A-Y, increase to next letter
//If final characters are numbers, extract digits from any preceeding letters and increment by 1
//If final character is something else return "0"
String PredictNextStation(String s_prev) {
  int first_dig, cascii, n;
  int slen = s_prev.length();
  cascii = int(s_prev.charAt(slen-1));
  if (isdigit(s_prev.charAt(slen-1))) { //Final character is a number
    int i = slen-1;
    while ((isdigit(s_prev.charAt(i))) && (i>=0)) {
      first_dig = i;
      i--;
    } 
    n = (s_prev.substring(first_dig, slen)).toInt();
    n++;
    s_prev.remove(first_dig);
    return s_prev+String(n);
  } else if ((cascii>=65) && (cascii<90)) { //Final character is a letter A-Y
    cascii++;
    s_prev.setCharAt(slen-1, char(cascii));
    return s_prev;
  } else return "0"; //Final character is something else
}

//Handle button presses
void ShotStationSelectHandler(int URN) {
  switch (URN) {
    case 1:
      stationFromNew = stationTo;
      stationToNew = stationNext;
      backSight = false;
      guiStep = 3;
      CreateScreen(screenShotMode);
      break;
    case 2:
      stationFromNew = stationTo;
      backSight = false;
      guiStep = 2;
      CreateScreen(screenStationEntry);
      break;
    case 3:
      backSight = false;
      CreateScreen(screenStationEntry);
      break;
    case 4:
      stationFromNew = stationTo;
      stationToNew = stationFrom;
      backSight = true;
      guiStep = 3;
      CreateScreen(screenShotMode);
      break;
    case 5:
      stationFromNew = stationFrom;
      backSight = true;
      guiStep = 2;
      CreateScreen(screenStationEntry);
      break;
    case 6:
      backSight = true;
      CreateScreen(screenStationEntry);
      break;   
    case 7:
      CloseSurveyFiles();
      currentMode = modeNull; 
      CreateScreen(screenMainMenu);
      break;           
  }
}

//Traverse Number User Entry Setup for Passage Mode
void TraverseEntrySetup() {
  GetLastScanNum();
  traverseNum = String(sta.lastTraverseNum + 1);
  sta.code = traverseTo;
  ctGUI.print("Enter Traverse", 159, 25, caveatron.FONT_22, 1, CENTER_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("Number", 159, 50, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  CreateNumberPad(150);
  btn1 = ctGUI.addButton(234, 24, 84, 84, BLUE_STD, "ENTER", caveatron.FONT_22, optVisible, 1);
  if (stationTo.length() <= 1) {
    btn2 = ctGUI.addButton(1, 24, 84, 84, BLUE_STD, "DELETE", caveatron.FONT_22, optInvisible, 2);
    btn3 = ctGUI.addButton(1, 24, 84, 84, RED_STD, "CANCEL", caveatron.FONT_22, optVisible, 3);
  } else {
    btn2 = ctGUI.addButton(1, 24, 84, 84, BLUE_STD, "DELETE", caveatron.FONT_22, optVisible, 2);
    btn3 = ctGUI.addButton(1, 24, 84, 84, RED_STD, "CANCEL", caveatron.FONT_22, optInvisible, 3);
  }
  txtbox1 = ctGUI.addTextInputBox(100, 75, 120, 33, traverseNum, caveatron.FONT_22, 2, CENTER_J, BLACK_STD, 3, optVisible, 0);
  if (ctGUI.GUIobject_data1[txtbox1] != 0) ctGUI.toggleButton(btn2, btn3);
  drawStatusBar("PASSAGE MODE");
  if (sta.lastTraverseNum < 1) drawInfoBar("Last Traverse: " + traverseTo + " / Trav: " + "NONE");
  else drawInfoBar("Last Traverse: " + traverseTo + " / #" + sta.lastTraverseNum);
}

//Splay Number User Entry Setup for Room Mode
void SplayEntrySetup() {
  GetLastScanNum(); 
  splayNum = String(sta.lastSplayNum + 1);
  sta.code = splayStation;
  ctGUI.print("Enter Splay", 159, 25, caveatron.FONT_22, 1, CENTER_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("Number", 159, 50, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  CreateNumberPad(150);
  btn1 = ctGUI.addButton(234, 24, 84, 84, BLUE_STD, "ENTER", caveatron.FONT_22, optVisible, 1);
  if (stationTo.length() <= 1) {
    btn2 = ctGUI.addButton(1, 24, 84, 84, BLUE_STD, "DELETE", caveatron.FONT_22, optInvisible, 2);
    btn3 = ctGUI.addButton(1, 24, 84, 84, RED_STD, "CANCEL", caveatron.FONT_22, optVisible, 3);
  } else {
    btn2 = ctGUI.addButton(1, 24, 84, 84, BLUE_STD, "DELETE", caveatron.FONT_22, optVisible, 2);
    btn3 = ctGUI.addButton(1, 24, 84, 84, RED_STD, "CANCEL", caveatron.FONT_22, optInvisible, 3);
  }
  txtbox1 = ctGUI.addTextInputBox(100, 75, 120, 33, splayNum, caveatron.FONT_22, 2, CENTER_J, BLACK_STD, 3, optVisible, 0);
  if (ctGUI.GUIobject_data1[txtbox1] != 0) ctGUI.toggleButton(btn2, btn3);
  drawStatusBar("ROOM MODE");
  if (sta.lastSplayNum < 1) drawInfoBar("Last Splay: " + splayStation + " / Splay: " + "NONE");
  else drawInfoBar("Last Splay: " + splayStation + " / #" + sta.lastSplayNum);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shot Mode Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Setup screen to take shot
void ShotModeSetup() {
  caveatron.LRF_PowerOn();
  drawStatusBar("SHOT MODE");
  drawInfoBar("Place bottom corner on station");
  btn1 = ctGUI.addButton(15, 70, 290, 272, GREEN_DRK, "TAKE", "SHOT", caveatron.FONT_34, optVisible, 1);
  btn2 = ctGUI.addButton(109, 367, 102, 84, RED_DRK, "CANCEL", caveatron.FONT_28, optVisible, 2);
  btn3 = ctGUI.addButton(15, 357, 84, 94, RED_DRK, "REDO", caveatron.FONT_28, optInvisible, 3);
  btn4 = ctGUI.addButton(124, 357, 180, 94, GREEN_DRK, "DONE", caveatron.FONT_28, optInvisible, 4);
  btn5 = ctGUI.addButton(221, 367, 84, 84, BUTTON_STD, "Use", "Right", caveatron.FONT_22, optVisible, 5);
  btn6 = ctGUI.addButton(15, 367, 84, 84, BUTTON_STD, "Use", "Left", caveatron.FONT_22, optInvisible, 6);
  if (rightFlag==true) {
    ctGUI.GUIobject_visible[btn5] = optInvisible;
    ctGUI.clearObjectArea(btn5);
    ctGUI.GUIobject_visible[btn6] = optVisible; 
    ctGUI.redrawObject(btn6);
    DrawRightArrow();
  } else DrawLeftArrow();
  ctGUI.print("Stations:", 15, 35, caveatron.FONT_28, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  if (backSight==true) ctGUI.print(stationFromNew + " - " + stationToNew + " (B)", 304, 41, caveatron.FONT_22, 2, RIGHT_J, TAN_STD, BLACK_STD);
  else ctGUI.print(stationFromNew + " - " + stationToNew, 304, 41, caveatron.FONT_22, 2, RIGHT_J, WHITE_STD, BLACK_STD);
  delay(250);
  caveatron.LRF_LaserOn();
  laserFlag = true;
}

//Handles button presses
void ShotModeHandler(int URN) {
  String outstr;
  switch (URN) {
    case 1:                            //Take Shot
      ctGUI.GUIobject_visible[btn1] = optInvisible;
      ctGUI.GUIobject_visible[btn2] = optInvisible;
      ctGUI.GUIobject_visible[btn3] = optVisible;
      ctGUI.GUIobject_visible[btn4] = optVisible;
      ctGUI.GUIobject_visible[btn5] = optInvisible;
      ctGUI.GUIobject_visible[btn6] = optInvisible;
      IMUErrorCheck(1); //Get initial IMU reading for comparison in case of stuck values
      GetCurrentTime();
      logFile.println("$$\t"+stationFromNew+'\t'+stationToNew+'\t'+String(backSight)+'\t'+String(rightFlag));  //Write stations to log file
      logFile.println("$&\t" + String(caveatron.BATT_GetLevel()) + '\t' + timeHour + ":" + timeMinute + ":" + timeSecond);  //Write battery level and time stamp to log file
      logFile.flush();
      LRFFlag = true;
      shotFlag = true;
      break;
    case 2:                            //Cancel
      caveatron.LRF_LaserOff();
      caveatron.LRF_PowerOff();
      laserFlag = false;
      LRFFlag = false;
      rightFlag = false;
      CloseSurveyFiles();
      currentMode = modeNull;
      CreateScreen(screenMainMenu);
      break;
    case 3:                            //Redo
      lengthTrav -= distance;
      lengthHoriz -= fabs(distance * cos(inclination / 57.29578));
      lengthVert -= distance * sin(inclination / 57.29578);
      numStations--;
      logFile.seekCur(-3);
      logFile.println("R");
      theFile.rewind();
      theFile.seekSet(filePosition);
      CreateScreen(screenShotMode);
      break;
    case 4:                            //Done
      logFile.seekCur(-3);
      logFile.println("O");
      CloseSurveyFiles();
      LRFFlag = false;
      rightFlag = false;
      currentMode = modeNull;
      CreateScreen(screenMainMenu);
      break;
    case 5:                            //Right Rear Corner Select
      ctGUI.GUIobject_visible[btn5] = optInvisible;
      ctGUI.clearObjectArea(btn5);
      ctGUI.GUIobject_visible[btn6] = optVisible; 
      ctGUI.redrawObject(btn6);
      DrawRightArrow();
      rightFlag = true;
      break;
    case 6:                            //Left Rear Corner Select
      ctGUI.GUIobject_visible[btn6] = optInvisible;
      //myGLCD.setColor(BLACK_STD);
      //myGLCD.fillRoundRect(15, 367, 99, 451);
      ctGUI.clearObjectArea(btn6);
      ctGUI.GUIobject_visible[btn5] = optVisible;
      ctGUI.redrawObject(btn5); 
      DrawLeftArrow();
      rightFlag = false;
      break;
  }
}

//Save shot measurement to SD card
uint8_t SaveShotData() {
  char bufin[91];
  String outStr, chkStr;
  char sc1[8], sc2[8];
  float fval, fval1;
  uint8_t saveStatus;

  if (backSight==false) {
    outStr = stationFrom + "\t" + stationTo + "\t" + String(distance) + "\t" + String(azimuth) + "\t" + String(inclination) + "\t;Time: " + String(timeHour) + ":" + String(timeMinute) + ":" + String(timeSecond) + "\t;Roll: " + String(roll);
  } else {
    outStr = stationFrom + "\t" + stationTo + "\t" + String(distance) + "\t" + String(azimuth) + "\t" + String(inclination) + "\t#S Back" + "\t;Time: " + String(timeHour) + ":" + String(timeMinute) + ":" + String(timeSecond) + "\t;Roll: " + String(roll);
  }
  outStr.trim();
  theFile.println(outStr);
  theFile.flush();
  theFile.truncate(theFile.curPosition());

  //Read in last line to validate data
  theFile.rewind();
  theFile.seekSet(filePosition);
  theFile.fgets(bufin, 90);
  chkStr = String(bufin);  
  chkStr.trim();
  if (chkStr!=outStr) {
    saveStatus=6;     //Save was not successful
    theFile.seekEnd(0);
    theFile.println(";");
    theFile.println(";ERROR");
    ctGUI.GUIobject_visible[btn3] = optInvisible;
  }
  else {
    saveStatus=1;   //Save was successful
    if (backSight==false) {
      lengthTrav += distance;
      lengthHoriz += fabs(distance * cos(inclination / 57.29578));
      lengthVert += distance * sin(inclination / 57.29578);
      numStations++;
      sta.lastTraverseNum = 0; traverseNum = "0";
      sta.lastSplayNum = 0; splayNum = "0";
      UpdateSettingsFile();
    }
  }
  return saveStatus;
}

//Function for drawing left corner selection arrow
void DrawLeftArrow() {
  myGLCD.setColor(YELLOW_STD);
  myGLCD.drawLine(17, 449, 97, 369);
  myGLCD.drawLine(17, 448, 97, 368);
  myGLCD.drawLine(17, 447, 97, 367);
  myGLCD.drawLine(18, 449, 98, 369);
  myGLCD.drawLine(19, 449, 99, 369);
  myGLCD.drawLine(17, 449, 17, 411);
  myGLCD.drawLine(18, 449, 18, 411);
  myGLCD.drawLine(19, 449, 19, 411);
  myGLCD.drawLine(17, 447, 55, 447);
  myGLCD.drawLine(17, 448, 55, 448);
  myGLCD.drawLine(17, 449, 55, 449);
}

//Function for drawing right corner selection arrow
void DrawRightArrow() {
  myGLCD.setColor(YELLOW_STD);
  myGLCD.drawLine(303, 449, 223, 369);
  myGLCD.drawLine(303, 448, 223, 368);
  myGLCD.drawLine(303, 447, 223, 367);
  myGLCD.drawLine(302, 449, 222, 369);
  myGLCD.drawLine(301, 449, 221, 369);
  myGLCD.drawLine(303, 449, 303, 411);
  myGLCD.drawLine(302, 449, 302, 411);
  myGLCD.drawLine(301, 449, 301, 411);
  myGLCD.drawLine(303, 447, 265, 447);
  myGLCD.drawLine(303, 448, 265, 448);
  myGLCD.drawLine(303, 449, 265, 449);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Passage Mode Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Setup screen for initiating Passage mode scan
void PassageModeSetup() {
  caveatron.LRF_PowerOn();
  drawStatusBar("PASSAGE MODE");
  drawInfoBar("Align laser to station");
  btn1 = ctGUI.addButton(15, 70, 290, 272, GREEN_DRK, "START", "TRAVERSE", caveatron.FONT_34, optVisible, 1);
  btn2 = ctGUI.addButton(15, 367, 290, 84, RED_DRK, "CANCEL", caveatron.FONT_28, optVisible, 2);
  btn3 = ctGUI.addButton(15, 70, 290, 272, BLUE_STD, "END", "TRAVERSE", caveatron.FONT_34, optInvisible, 3);
  btn4 = ctGUI.addButton(15, 268, 84, 84, RED_DRK, "Redo", caveatron.FONT_28, optInvisible, 4);
  btn5 = ctGUI.addButton(124, 268, 180, 84, BLUE_STD, "Another", "Traverse", caveatron.FONT_28, optInvisible, 5);
  btn6 = ctGUI.addButton(15, 367, 290, 84, GREEN_DRK, "DONE", caveatron.FONT_28, optInvisible, 6);

  ctGUI.print("Traverse:", 15, 35, caveatron.FONT_28, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print(traverseTo + " / #" + traverseNum, 304, 35, caveatron.FONT_28, 2, RIGHT_J, WHITE_STD, BLACK_STD);
 
  InitializeLIDAR();
  caveatron.LRF_LaserOn();
  laserFlag = true;
}

//Handle button presses
void PassageModeHandler(int URN) {
  switch (URN) {
    case 1:                            //Start Traverse
      ctGUI.makeObjectInvisible(btn1);
      ctGUI.makeObjectInvisible(btn2);
      ctGUI.makeObjectVisible(btn3);
      ctGUI.print("Wait for 2nd beep", 160, 372, caveatron.FONT_28, 2, CENTER_J, YELLOW_STD, BLACK_STD);
      ctGUI.print("to begin traverse", 160, 400, caveatron.FONT_28, 2, CENTER_J, YELLOW_STD, BLACK_STD);
      LRFFlag = true;
      InitializeTraverse();
      break;
    case 2:                            //Cancel
      caveatron.LRF_LaserOff();
      caveatron.LRF_PowerOff();
      CloseLIDAR();
      laserFlag = false;
      LRFFlag = false;
      CloseSurveyFiles();
      currentMode = modeNull;
      CreateScreen(screenMainMenu);
      break;
    /*case 3:                            //End Traverse (Handled in LIDAR tab)
      break;*/
    case 4:                            //Redo
      lidarFile << "#REDO" << endl;
      logFile.seekCur(-3);
      logFile.println("R");
      sta.lastTraverseNum--;
      numTraverses--;
      CreateScreen(screenPassageMode);
      break;
    case 5:                            //Another Traverse  
      guiStep = 2;
      CreateScreen(screenTraverseEntry);
      break;
    case 6:                            //Done
      logFile.seekCur(-3);
      logFile.println("O");
      CloseSurveyFiles();
      LRFFlag = false;
      currentMode = modeNull;
      CreateScreen(screenMainMenu);
      break;
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Room Mode Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


//Setup screen for initiating Room mode scan
void RoomModeSetup() {
  caveatron.LRF_PowerOn();  
  drawStatusBar("ROOM MODE");
  drawInfoBar("Align laser to station");
  
  btn1 = ctGUI.addButton(15, 105, 290, 237, GREEN_DRK, "START", "SCAN", caveatron.FONT_34, optInvisible, 1);
  btn2 = ctGUI.addButton(172, 367, 133, 84, RED_DRK, "CANCEL", caveatron.FONT_28, optVisible, 2);
  btn3 = ctGUI.addButton(15, 105, 290, 237, BLUE_STD, "END", "SCAN", caveatron.FONT_34, optInvisible, 3);
  btn4 = ctGUI.addButton(15, 268, 84, 84, RED_DRK, "Redo", caveatron.FONT_28, optInvisible, 4);
  btn5 = ctGUI.addButton(124, 268, 180, 84, BLUE_STD, "Another", "Splay", caveatron.FONT_28, optInvisible, 5);
  btn6 = ctGUI.addButton(15, 367, 290, 84, GREEN_DRK, "DONE", caveatron.FONT_28, optInvisible, 6);
  btn7 = ctGUI.addButton(15, 70, 290, 272, GREEN_DRK, "TAKE", "SPLAY SHOT", caveatron.FONT_28, optVisible, 7);
  btn8 = ctGUI.addButton(15, 367, 133, 84, BLUE_STD, "Use", "Station", caveatron.FONT_28, optVisible, 8);
  
  ctGUI.print("Splay:", 15, 35, caveatron.FONT_28, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print(splayStation + " / #" + splayNum, 304, 35, caveatron.FONT_28, 2, RIGHT_J, WHITE_STD, BLACK_STD);

  delay(250);

  caveatron.LRF_LaserOn();
  laserFlag = true;
}

//Handle button presses
void RoomModeHandler(int URN) {
  switch (URN) {
    case 1:                            //Start Scan
      ctGUI.makeObjectInvisible(btn1);
      ctGUI.makeObjectInvisible(btn2);
      ctGUI.makeObjectVisible(btn3);
      ctGUI.print("Wait for beep", 160, 372, caveatron.FONT_28, 2, CENTER_J, YELLOW_STD, BLACK_STD);
      ctGUI.print("to begin scan", 160, 400, caveatron.FONT_28, 2, CENTER_J, YELLOW_STD, BLACK_STD);
      InitializeSplay();
      break;
    case 2:                            //Cancel
      caveatron.LRF_LaserOff();
      caveatron.LRF_PowerOff();
      CloseLIDAR();
      laserFlag = false;
      LRFFlag = false;
      CloseSurveyFiles();
      currentMode = modeNull;
      CreateScreen(screenMainMenu);
      break;
    case 4:                            //Redo
      sta.lastSplayNum--;
      numSplays--;
      lidarFile << "#REDO" << endl;
      logFile.seekCur(-3);
      logFile.println("R");
      CreateScreen(screenRoomMode);
      break;
    case 5:                            //Another Splay
      guiStep = 2;
      CreateScreen(screenSplayEntry);
      break;
    case 6:                            //Done
      logFile.seekCur(-3);
      logFile.println("O");
      CloseSurveyFiles();
      LRFFlag = false;
      currentMode = modeNull;
      CreateScreen(screenMainMenu);
      break;
    case 7:                            //Take Reference Splay Shot
      {
        ctGUI.makeObjectInvisible(btn7);
        ctGUI.makeObjectInvisible(btn2);
        ctGUI.makeObjectInvisible(btn8);
        ctGUI.print("Taking reference", 160, 372, caveatron.FONT_28, 2, CENTER_J, YELLOW_STD, BLACK_STD);
        ctGUI.print("splay shot", 160, 400, caveatron.FONT_28, 2, CENTER_J, YELLOW_STD, BLACK_STD);
        boolean splayResult = TakeSplayShot();
        if (splayResult==true) {
          myGLCD.setColor(BLACK_STD);
          myGLCD.fillRect(15, 367, 305, 451);
          if (!InitializeLIDAR()) return;
          drawInfoBar("Rotate box over splay point");
          ctGUI.print("Scans to left side", 305, 73, caveatron.FONT_22, 2, RIGHT_J, YELLOW_STD, BLACK_STD);
          myGLCD.drawLine(15,82,90,82);
          myGLCD.drawLine(15,83,90,83);
          myGLCD.drawLine(15,82,30,73);   
          myGLCD.drawLine(15,83,30,74);
          myGLCD.drawLine(15,82,30,91);
          myGLCD.drawLine(15,83,30,92);
          ctGUI.makeObjectVisible(btn2);
          ctGUI.makeObjectVisible(btn1);
        }
      }
      break;
    case 8:                            //Set reference position to current station
      {
        ctGUI.makeObjectInvisible(btn7);
        ctGUI.makeObjectInvisible(btn2);
        ctGUI.makeObjectInvisible(btn8);
        distance=0; azimuth=0; inclination=0; roll=0;
        for (int j=0;j<6;j++) rawIMU[j]=0;
        caveatron.LRF_LaserOff();
        caveatron.LRF_PowerOff();
        myGLCD.setColor(BLACK_STD);
        myGLCD.fillRect(15, 367, 305, 451);
        if (!InitializeLIDAR()) return;
        drawInfoBar("Rotate box over splay point");
        ctGUI.print("Scans to left side", 305, 73, caveatron.FONT_22, 2, RIGHT_J, YELLOW_STD, BLACK_STD);
        myGLCD.drawLine(15,82,90,82);
        myGLCD.drawLine(15,83,90,83);
        myGLCD.drawLine(15,82,30,73);   
        myGLCD.drawLine(15,83,30,74);
        myGLCD.drawLine(15,82,30,91);
        myGLCD.drawLine(15,83,30,92);
        ctGUI.makeObjectVisible(btn2);
        ctGUI.makeObjectVisible(btn1);
      }
      break;
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         Manual Mode Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Setup Manual mode screen
void ManualModeSetup() {
  caveatron.LRF_PowerOn();
  drawStatusBar("MANUAL MODE");
  drawInfoBar("");
  ctGUI.addButton(200, 351, 110, 100, BLUE_STD, "DONE", caveatron.FONT_28, optVisible, 1);
  btn1 = ctGUI.addToggleSwitch(10, 351, 85, 100, 28, BUTTON_STD, RED_DRK, "RANGE", "On", "Off", caveatron.FONT_22, Switch_Down, optVisible, 2);
  btn3 = ctGUI.addButton(10, 351, 110, 100, BUTTON_STD, "Resume", caveatron.FONT_28, optInvisible, 4);
  ctGUI.addHiddenButton(15, 20, 304, 330, optVisible, 5);
  DataDisplaySetup();
  ctGUI.print("Tap screen to take shot", 160, 30, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  LRFFlag = false;
  delay(250);
  caveatron.LRF_LaserOn();
  laserFlag = true;
}

//Handle button presses
void ManualModeHandler(int URN) {
  delay(50);
  switch (URN) {
    case 1:                            //Done
      holdFlag = false;
      currentMode = modeNull;
      laserFlag = false;
      LRFFlag = false;
      caveatron.LRF_LaserOff();
      caveatron.LRF_PowerOff();
      CreateScreen(screenMainMenu);
      break;
    case 2:                            //LRF On or Off
      //laserFirstRun = true;
      if (ctGUI.GUIobject_currentposition[btn1] == false) LRFFlag = false;
      else LRFFlag = true;
      break;
    case 4:                            //Resume
      myGLCD.setColor(BLACK_STD);
      myGLCD.fillRect(50, 150, 269, 243);
      ctGUI.makeObjectInvisible(btn3);
      ctGUI.makeObjectVisible(btn1);
      ctGUI.makeObjectVisible(btn2);
      drawInfoBar("");
      DataDisplaySetup();
      ctGUI.print("Tap screen to take shot", 160, 30, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
      caveatron.LRF_LaserOn();
      laserFlag = true;
      holdFlag = false;
      break;
    case 5:                            //Take Shot
      if (holdFlag == false) {
          ctGUI.GUIobject_visible[btn1] = optInvisible;
          ctGUI.GUIobject_visible[btn2] = optInvisible;
          ctGUI.GUIobject_visible[btn3] = optVisible;
          shotFlag = true;
          myGLCD.setColor(WHITE_STD);
          myGLCD.fillRect(0, 0, 319, 479);
      }
      break;
  }
}

