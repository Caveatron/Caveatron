/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Survey Setup Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Setup and handler for the Survey Menu screen
void SurveyMenuSetup() {
  drawStatusBar("SURVEY PROPERTIES");
  drawInfoBar("SURVEY: "+caveName);
  ctGUI.addButton(20,33,280,66,GREEN_DRK,"NEW SURVEY SETUP",caveatron.FONT_28,optVisible,1);
  ctGUI.addButton(20,114,280,66,BUTTON_STD,"LINE PLOT",caveatron.FONT_28,optVisible,2);
  ctGUI.addButton(20,195,280,66,BUTTON_STD,"REVIEW SHOTS/SCANS",caveatron.FONT_28,optVisible,3);
  ctGUI.addButton(20,276,280,66,BUTTON_STD,"STATISTICS",caveatron.FONT_28,optVisible,4);
  ctGUI.addButton(20,364,280,84,BLUE_STD,"RETURN TO MAIN MENU",caveatron.FONT_28,optVisible,5);
}

void SurveyMenuHandler(int URN) {  
  switch (URN) {
      case 1:                            //New Survey Setup
        guiStep++;
        CreateScreen(screenCaveNameEntry);
        break;
      case 2:                            //View Line Plot
        CreateScreen(screenLinePlot);
        break;
      case 3:                            //Shot & Scan Review Menu
        guiStep++;
        CreateScreen(screenShotScanReview);
        break;
      case 4:                            //Statistics
        CreateScreen(screenSurveyStats);
        break;
      case 5:                            //Exit
        currentMode = modeNull;
        CreateScreen(screenMainMenu);
        break;
  }
}

// Setup for the Cave Name Entry screen
void CaveNameEntrySetup() {
  String s;
  int i=0;
  int j=0;
  drawStatusBar("NEW SURVEY SETUP");
  drawInfoBar("");
  btn1 = ctGUI.addButton(223,75,96,68,BLUE_STD,"ENTER",caveatron.FONT_22,optVisible,1);
  btn2 = ctGUI.addButton(1,75,96,68,BLUE_STD,"DELETE",caveatron.FONT_22,optVisible,2); 
  btn3 = ctGUI.addButton(1,75,96,68,RED_STD,"CANCEL",caveatron.FONT_22,optInvisible,3);
  btn4 = ctGUI.addButton(196,402,123,56,BLUE_STD,"MORE",caveatron.FONT_22,optVisible,4);

  if (guiStep==2) {
    CreateTextPad(150, false);
    ctGUI.print("ENTER CAVE NAME", 5, 23, caveatron.FONT_19, 2, LEFT_J, YELLOW_STD, BLACK_STD);
    btn5 = ctGUI.addButton(104,87,112,56,BUTTON_STD,"Space",caveatron.FONT_22,optVisible,5);
    btn6 = ctGUI.addButton(1,402,90,56,BUTTON_STD,"CAVE",caveatron.FONT_22,optVisible,6); 
    btn7 = ctGUI.addButton(99,402,90,56,BUTTON_STD,"PIT",caveatron.FONT_22,optVisible,7);
    txtbox1 = ctGUI.addTextInputBox(5,45,310,23,"",caveatron.FONT_22,2,CENTER_J,BLACK_STD,23,optVisible,0);
  } 
  else if (guiStep==3) {
    CreateTextPad(150, true);
    ctGUI.makeObjectInvisible(btn5);
    ctGUI.makeObjectInvisible(btn6);
    ctGUI.makeObjectInvisible(btn7);
    s = caveName.substring(0,4);
    for (int i=0; i<4; i++) {
      if (!isAlphaNumeric(s.charAt(i))) s.setCharAt(i,'_');
    }
    ctGUI.print("FILENAME: Edit or press Enter to accept", 5, 23, caveatron.FONT_19, 2, LEFT_J, YELLOW_STD, BLACK_STD);
    drawInfoBar("Only Letters & Numbers Allowed");
    ctGUI.print(dateMonth+dateDay, 170, 46, caveatron.FONT_22, 2, LEFT_J, WHITE_STD, BLACK_STD);
    txtbox1 = ctGUI.addTextInputBox(84,45,66,23,s,caveatron.FONT_22,2,CENTER_J,BLACK_STD,4,optVisible,0);
  }
  if (ctGUI.GUIobject_data1[txtbox1]==0) ctGUI.toggleButton(btn3, btn2);
}

// Screen for displaying current survey statistics
void SurveyStatsSetup() {
  drawStatusBar("SURVEY STATISTICS");
  drawInfoBar("");
  myGLCD.setBackColor(BLACK_STD);
  myGLCD.setColor(YELLOW_STD);
  ctGUI.print("Cave Name:", 15, 26, caveatron.FONT_22, 2);
  ctGUI.print("Base File Name:", 15, 86, caveatron.FONT_22, 2);
  ctGUI.print("Distance Surveyed:", 15, 132, caveatron.FONT_22, 2);
  ctGUI.print("Horizontal Length:", 15, 166, caveatron.FONT_22, 2);
  ctGUI.print("Depth of Survey:", 15, 200, caveatron.FONT_22, 2);
  ctGUI.print("Number of Stations:", 15, 234, caveatron.FONT_22, 2);
  ctGUI.print("# Traverses/Splays:", 15, 268, caveatron.FONT_22, 2);
  ctGUI.print("Start Date / Time:", 15, 314, caveatron.FONT_22, 2);
  myGLCD.setColor(GRAY_025);
  myGLCD.fillRect(15,120,304,121);
  myGLCD.fillRect(15,302,304,303);
  myGLCD.setColor(WHITE_STD);
  ctGUI.print(caveName, 304, 52, caveatron.FONT_22, 2, RIGHT_J);
  ctGUI.print(fileNameBase, 304, 86, caveatron.FONT_22, 2, RIGHT_J);
  ctGUI.printNumF(lengthTrav, 2, 304, 132, caveatron.FONT_22, 2, RIGHT_J);
  ctGUI.printNumF(lengthHoriz, 2, 304, 166, caveatron.FONT_22, 2, RIGHT_J);
  ctGUI.printNumF(lengthVert, 2, 304, 200, caveatron.FONT_22, 2, RIGHT_J);
  ctGUI.printNumI(numStations, 304, 234, caveatron.FONT_22, 2, RIGHT_J);
  String tsNum = String(numTraverses)+" / "+String(numSplays);
  ctGUI.print(tsNum, 304, 268, caveatron.FONT_22, 2, RIGHT_J);
  String surveyStartTime = "20"+String(surveyYear)+"-"+String(surveyMonth)+"-"+String(surveyDay)+" / "+String(surveyHour)+":"+String(surveyMinute);
  ctGUI.print(surveyStartTime, 304, 340, caveatron.FONT_22, 2, RIGHT_J);
  ctGUI.addButton(20,374,280,76,BLUE_STD,"RETURN TO MAIN MENU",caveatron.FONT_28,optVisible,5);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Date and Time Setting Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Setup and handler for the Time settings screen
void TimeUnitsSetup() {
  drawStatusBar("TIME SETTINGS");
  drawInfoBar("");
  ctGUI.addButton(20,33,280,66,BUTTON_STD,"SET TIME",caveatron.FONT_28,optVisible,1);
  ctGUI.addButton(20,364,280,84,BLUE_STD,"RETURN TO SETTINGS",caveatron.FONT_28,optVisible,5);
}

void TimeUnitsMenuHandler(int URN) {  
  switch (URN) {
      case 1:                            //Set Time
        guiStep++;
        currentMode = modeSetTime;
        CreateScreen(screenSetTimeDate);
        break;
      case 5:                            //Exit
        guiStep--;
        currentMode = modeNull;
        CreateScreen(screenSettingsMenu);
        break;
  }
  
}

// Setup and handler for changing date and time
void SetTimeDateSetup() {
  drawStatusBar("SET TIME");
  drawInfoBar("");
  ctGUI.print("SET:", 9, 146, caveatron.FONT_34, 4, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.addButton(116,150,88,86,BUTTON_STD,"HOUR",caveatron.FONT_28,optVisible,1);
  ctGUI.addButton(223,150,88,86,BUTTON_STD,"MINUTE",caveatron.FONT_28,optVisible,2);
  ctGUI.addButton(9,257,88,86,BUTTON_STD,"YEAR",caveatron.FONT_28,optVisible,3);
  ctGUI.addButton(116,257,88,86,BUTTON_STD,"MONTH",caveatron.FONT_28,optVisible,4);
  ctGUI.addButton(223,257,88,86,BUTTON_STD,"DAY",caveatron.FONT_28,optVisible,5);
  myGLCD.setColor(GRAY_025);
  myGLCD.drawLine(10,130,309,130);
  myGLCD.drawLine(10,131,309,131);
  myGLCD.drawLine(10,361,309,361);
  myGLCD.drawLine(10,362,309,362);
  if(newTimeDate==true) {
    ctGUI.addButton(10,382,140,70,BLUE_STD,"CANCEL",caveatron.FONT_28,optVisible,7);
    ctGUI.addButton(170,382,140,70,GREEN_DRK,"SET",caveatron.FONT_28,optVisible,8);
  } else {
    ctGUI.addButton(10,382,300,70,BLUE_STD,"DONE",caveatron.FONT_28,optVisible,6);    
    ctGUI.print("Current Time & Date", 10, 20, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
    newSecond = 0;
    newHour = caveatron.RTChour;
    newMinute = caveatron.RTCminute;
    newYear = caveatron.RTCyear;
    newMonth = caveatron.RTCmonth;
    newDay = caveatron.RTCday;
  }
  DisplayTimeDate();
}


void SetTimeDateHandler(int URN) {  
  switch (URN) {
      case 1:                            //Set Hour
        timeSetType = 1;
        CreateScreen(screenTimeDateEntry);
        break;
      case 2:                            //Set Minute
        timeSetType = 2;
        CreateScreen(screenTimeDateEntry);
        break;
      case 3:                            //Set Year
        timeSetType = 3;
        CreateScreen(screenTimeDateEntry);
        break;
      case 4:                            //Set Month
        timeSetType = 4;
        CreateScreen(screenTimeDateEntry);
        break;
      case 5:                            //Set Day
        timeSetType = 5;
        CreateScreen(screenTimeDateEntry);
        break;
      case 8:                            //Set
        caveatron.RTC_SetDateTime(newYear, newMonth, newDay, newHour, newMinute);
      case 6:                            //Done
      case 7:                            //Cancel
        newTimeDate = false;
        guiStep--;
        currentMode = modeNull;
        CreateScreen(screenTimeUnits);
        break;
  }
}

// Setup and handler for screen for entering new date and time value
void TimeDateEntrySetup () {
  char nullString[] = "";
  drawStatusBar("SET TIME");
  drawInfoBar("");
  CreateNumberPad(315);
  btn1 = ctGUI.addButton(223,224,96,84,BLUE_STD,"ENTER",caveatron.FONT_22,optVisible,1);
  btn2 = ctGUI.addButton(1,224,96,84,BLUE_STD,"DELETE",caveatron.FONT_22,optInvisible,2); 
  btn3 = ctGUI.addButton(1,224,96,84,RED_STD,"CANCEL",caveatron.FONT_22,optVisible,3);
  myGLCD.setColor(GRAY_025);
  myGLCD.drawLine(10,130,309,130);
  myGLCD.drawLine(10,131,309,131);
  if (timeSetType==1) {
    ctGUI.print("SET HOUR", 160, 180, caveatron.FONT_34, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  if (timeSetType==2) {
    ctGUI.print("SET MINUTE", 160, 180, caveatron.FONT_34, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  if (timeSetType==3) {
    ctGUI.print("SET YEAR", 160, 180, caveatron.FONT_34, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  if (timeSetType==4) {
    ctGUI.print("SET MONTH", 160, 180, caveatron.FONT_34, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  if (timeSetType==5) {
    ctGUI.print("SET DAY", 160, 180, caveatron.FONT_34, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  }
  txtbox1 = ctGUI.addTextInputBox(120,246,80,40,nullString,caveatron.FONT_34,2,CENTER_J,BLACK_STD,2,optVisible,0);
  ctGUI.print("Current Time & Date", 10, 20, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  liveTimeDate = true;
}


boolean TimeEntryHandler() {
  boolean validresult = false;
  String textEntry = ctGUI.GUIobject_text[txtbox1];
  switch (timeSetType) {
    case 1:
      if (textEntry.toInt()<24) {newHour = textEntry.toInt(); validresult=true;}
      break;
    case 2:
      if (textEntry.toInt()<60) {newMinute = textEntry.toInt(); validresult=true;}
      break;
    case 3:
      newYear = textEntry.toInt(); 
      validresult=true;
      break;
    case 4:
      if ((textEntry.toInt()>0) && (textEntry.toInt()<13)) {newMonth = textEntry.toInt(); validresult=true;}
      break;
    case 5:
      if ((textEntry.toInt()>0) && (textEntry.toInt()<32)) {newDay = textEntry.toInt();  validresult=true;}
      break;
  }
  timeSetType=0;
  return validresult;
}

// Displays current date and time
void DisplayTimeDate() {
  if ((newTimeDate==false) || (liveTimeDate==true)) {
    GetCurrentTime();
    String timeString = timeHour + " : " + timeMinute+ " : " + timeSecond;
    ctGUI.print(timeString, 160, 50, caveatron.FONT_34, 4, CENTER_J, YELLOW_STD, BLACK_STD);
    String dateString = dateMonth+ " / " +dateDay+ " / " +"20"+dateYear;
    ctGUI.print(dateString, 160, 86, caveatron.FONT_34, 4, CENTER_J, YELLOW_STD, BLACK_STD);
  } else {
    ctGUI.print("New Time & Date", 10, 20, caveatron.FONT_22, 2, LEFT_J, WHITE_STD, BLACK_STD);
    FormatTimeDate(newHour, newMinute, newSecond, newYear, newMonth, newDay);
    String timeString = timeHour + " : " + timeMinute+ " : " + timeSecond;
    ctGUI.print(timeString, 160, 50, caveatron.FONT_34, 4, CENTER_J, WHITE_STD, BLACK_STD);
    String dateString = dateMonth+ " / " +dateDay+ " / " +"20"+dateYear;
    ctGUI.print(dateString, 160, 86, caveatron.FONT_34, 4, CENTER_J, WHITE_STD, BLACK_STD);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           SD Card Setting Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void SDCardMenuSetup() {
  drawStatusBar("SD Card");
  drawInfoBar("");
  btn1 = ctGUI.addButton(20,33,280,66,BUTTON_STD,"SD CARD INFO",caveatron.FONT_28,optVisible,1);
  ctGUI.addButton(20,364,280,84,BLUE_STD,"RETURN TO SETTINGS",caveatron.FONT_28,optVisible,5);
}

void SDCardMenuHandler(int URN) {
    switch (URN) {
      case 1:                            //SD Card Info
        myGLCD.setColor(BLACK_STD);
        myGLCD.fillRect(0,21,319,363);
        ctGUI.makeObjectInvisible(btn1);
        SDCardInfo();
        break;
      case 5:                            //Exit
        guiStep--;
        currentMode = modeNull;
        CreateScreen(screenSettingsMenu);
        break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              About Screen Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void AboutScreenSetup() {
  drawStatusBar("ABOUT CAVEATRON");
  drawInfoBar("");
  ctGUI.addButton(20,364,280,84,BLUE_STD,"RETURN TO SETTINGS",caveatron.FONT_28,optVisible,5);
  myGLCD.setColor(255,0,255);
  myGLCD.setBackColor(BLACK_STD);
  ctGUI.showImage(20,30,caveatron.gLOGO); 
  ctGUI.print("Serial #", 10, 158, caveatron.FONT_22, 3, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print(caveatron.serialNumber, 300, 158, caveatron.FONT_22, 3, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print("Software version:", 10, 186, caveatron.FONT_22, 3, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print(softwareVersion, 300, 186, caveatron.FONT_22, 3, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print("Developed by:", 10, 228, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("Joe Mitchell", 310, 228, caveatron.FONT_22, 2, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print("Steve Gutting", 310, 256, caveatron.FONT_22, 2, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print("Machining:", 10, 284, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("Gregg Williams", 310, 284, caveatron.FONT_22, 2, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print("Icon Design:", 10, 312, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("Jill Orr", 310, 312, caveatron.FONT_22, 2, RIGHT_J, WHITE_STD, BLACK_STD);
}

