/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           General GUI Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Draws status bar at top of each screen
void drawStatusBar(String textStatusBar) {
  //Draw top bar
  myGLCD.setColor(BAR_TOP);
  if (textStatusBar.length() == 0) myGLCD.fillRect(0,0,75,18);  //If called with text length 0, just fill clock portion of bar
  else myGLCD.fillRect(0,0,319,18); 
  //Call hardware to get time and battery level
  GetCurrentTime();
  unsigned int battLevel = caveatron.BATT_GetLevel();
  //Draw battery indicator
  if (battLevel <= 3) myGLCD.setColor(RED_STD); //if 2% or less draw battery outline in red
  else myGLCD.setColor(WHITE_STD);
  myGLCD.drawRect(279, 2, 305, 16); //draw battery body
  myGLCD.fillRect(306, 5, 309, 13); // draw battery tip
  myGLCD.setColor(BLACK_STD);
  myGLCD.fillRect(280, 3, 304, 15); // clear the center of the battery
  //Fill battery indictor
  if (battLevel < 15) myGLCD.setColor(RED_STD);
  else myGLCD.setColor(GREEN_STD);
  if (battLevel>=100) myGLCD.fillRect(280, 3, 304, 15); //Account for 100% battery not overfilling rect
  else myGLCD.fillRect(280, 3, 280+(0.5+float(battLevel)/4), 15);
  //ctGUI.printNumI(battLevel, 280, 0, caveatron.FONT_19, 1, LEFT_J, WHITE_STD, BLACK_STD);
  //Draw time
  String timeString = timeHour + ":" + timeMinute;
  //ctGUI.printNumI(battLevel, 260, 2, caveatron.FONT_15, 1, RIGHT_J, WHITE_STD, BAR_TOP);
  //ctGUI.print("%", 262, 2, caveatron.FONT_15, 1, LEFT_J, WHITE_STD, BAR_TOP);
  //Draw time and center text
  ctGUI.print(timeString, 10, 0, caveatron.FONT_19, 3, LEFT_J, WHITE_STD, BAR_TOP);
  ctGUI.print(textStatusBar, 160, 0, caveatron.FONT_19, 2, CENTER_J, WHITE_STD, BAR_TOP);
}

// Draws info bar at bottom of each screen
void drawInfoBar(String textInfoBar) {
  myGLCD.setColor(GRAY_075);
  myGLCD.fillRect(0,463,319,479);
  ctGUI.print(textInfoBar, 160, 464, caveatron.FONT_15, 3, CENTER_J, WHITE_STD, GRAY_075);
}

// Redraws main portion of screen (except Status Bar and Info Bar)
void RedrawScreen() {
  myGLCD.setColor(BLACK_STD);
  myGLCD.fillRect(0,19,319,462);
  ctGUI.redrawAllObjects();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Keypad Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Create a keypad for letters setup for two pages
void CreateLetterPad(int yLtrPad) {
  int x,y, ASCIInum;
  int xKeySize = 58;
  int yKeySize = 58;
  int xKeyPitch = 65;
  int yKeyPitch = 65;
  numOfKeys = 14;
  char keyLtr[2][14] = {
    {'A','B','C','D','E','F','G','H','I','J','K','L','M','\''},
    {'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-'}
  };

  int k = 0;
  for (int j=0; j<3; j++) {
    for (int i=0; i<5; i++) {
      x = (i*xKeyPitch)+1;
      y = (j*yKeyPitch)+yLtrPad;
      if ((j==2) && (i==4)) {
        ctGUI.addButton(x,y,xKeySize,yKeySize,BLUE_STD,"MORE",caveatron.FONT_19,optVisible,4);
      } else {
        ASCIInum = (int) keyLtr[0][k];
        btn[k] = ctGUI.addButton(x,y,xKeySize,yKeySize,BUTTON_STD,String(keyLtr[0][k]),caveatron.FONT_28,optVisible,ASCIInum);
        ASCIInum = (int) keyLtr[1][k];
        btn[k+numOfKeys] = ctGUI.addButton(x,y,xKeySize,yKeySize,BUTTON_STD,String(keyLtr[1][k]),caveatron.FONT_28,optInvisible,ASCIInum);
        k++;
      }
    }
  }
  pageLtrKey = false;
}

// Create a keypad for numbers
void CreateNumberPad(int yNumPad) {
  int x,y;
  int xKeySize = 58;
  int yKeySize = 68;
  int xKeyPitch = 65;
  int yKeyPitch = 75;
  char keyNum[10] = {'1','2','3','4','5','6','7','8','9','0'};
  
  int k = 0;
  for (int j=0; j<2; j++) {
    for (int i=0; i<5; i++) {
      x = (i*xKeyPitch)+1;
      y = (j*yKeyPitch)+yNumPad;
      int ASCIInum = (int) keyNum[k];
      ctGUI.addButton(x,y,xKeySize,yKeySize,BUTTON_STD,String(keyNum[k]),caveatron.FONT_28,optVisible,ASCIInum);
      k++;
    }
  }
}

//Create compact alphanumeric keypad
void CreateTextPad(int yTxtPad, boolean alnumOnly) {
  int x,y, ASCIInum;
  int xKeySize = 58;
  int yKeySize = 56;
  int xKeyPitch = 65;
  int yKeyPitch = 63;
  numOfKeys = 20;
  char keyLtr[2][20] = {
    {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T'},
    {'U','V','W','X','Y','Z','-','\'','.','#','1','2','3','4','5','6','7','8','9','0'}
  };
  if (alnumOnly) {
    for (int i=0; i<4; i++) keyLtr[1][6+i] = '_';
  }
  int k = 0;
  for (int j=0; j<4; j++) {
    for (int i=0; i<5; i++) {
      x = (i*xKeyPitch)+1;
      y = (j*yKeyPitch)+yTxtPad;
      ASCIInum = (int) keyLtr[0][k];
      btn[k] = ctGUI.addButton(x,y,xKeySize,yKeySize,BUTTON_STD,String(keyLtr[0][k]),caveatron.FONT_28,optVisible,ASCIInum);
      ASCIInum = (int) keyLtr[1][k];
      btn[k+numOfKeys] = ctGUI.addButton(x,y,xKeySize,yKeySize,BUTTON_STD,String(keyLtr[1][k]),caveatron.FONT_28,optInvisible,ASCIInum);
      k++;
    }
  }
  pageLtrKey = false;
}

// Handler for keypads
void KeyPadHandler(int URN) {  
  int i;
  char entryChar;
  String textBoxText;
  switch (URN) {
    case 1:                    //ENTER KEY PRESSED
      guiStep++;
      switch(currentMode) {
        case modeShot:
          if (guiStep==2) {
            stationFromNew = ctGUI.GUIobject_text[txtbox1];   
            ctGUI.clearTextInputBox(txtbox1);
            drawInfoBar("From Station:  "+stationFromNew);
            ctGUI.print("   TO   ",159, 50, caveatron.FONT_22, 3, CENTER_J, YELLOW_STD, BLACK_STD);
          }
          else if (guiStep==3) {
            stationToNew = ctGUI.GUIobject_text[txtbox1];
            CreateScreen(screenShotMode);
          }
          break;
        case modePassage:
          if (guiStep==2) {
            traverseTo = ctGUI.GUIobject_text[txtbox1];
            CreateScreen(screenTraverseEntry);
          }
          else if (guiStep==3) {
            traverseNum = ctGUI.GUIobject_text[txtbox1];
            CreateScreen(screenPassageMode);
          }
          break;
        case modeRoom:
          if (guiStep==2) {
            splayStation = ctGUI.GUIobject_text[txtbox1];
            CreateScreen(screenSplayEntry);
          }
          else if (guiStep==3) {
            traverseNum = ctGUI.GUIobject_text[txtbox1];
            CreateScreen(screenRoomMode);
          }
          break;
        case modeSurvey:
          if (guiStep==3) {
            caveName = ctGUI.GUIobject_text[txtbox1];   
            CreateScreen(screenCaveNameEntry);
          }
          else if (guiStep==4) {
            caveCode = ctGUI.GUIobject_text[txtbox1];
            CreateSurveyFiles();
            CreateScreen(screenMainMenu);
          }
          break;
        case modeSetTime:
          boolean validtimedate = TimeEntryHandler();
          if (validtimedate==true) {
            liveTimeDate = false;
            newTimeDate = true;
            CreateScreen(screenSetTimeDate);
          }
          break;
      }
      break;
    case 2:                  //DELETE KEY PRESSED
      ctGUI.delcharTextInputBox(txtbox1);
      if (ctGUI.GUIobject_data1[txtbox1]==0) ctGUI.toggleButton(btn3, btn2);
      break;
    case 3:                  //CANCEL KEY PRESSED
      switch(currentMode) {
        case modeSetTime:
          if (newTimeDate==true) liveTimeDate = false;
          CreateScreen(screenSetTimeDate);
          break;
        default:
          CloseSurveyFiles();
          currentMode = modeNull; 
          CreateScreen(screenMainMenu);
          break;
      }
      break;  
    case 4:                    //MORE KEY PRESSED
      pageLtrKey = !pageLtrKey;
      if (pageLtrKey==true) {
        for (i=0; i<numOfKeys; i++) ctGUI.toggleButton(btn[i+numOfKeys], btn[i]);
      } else {
        for (i=0; i<numOfKeys; i++) ctGUI.toggleButton(btn[i], btn[i+numOfKeys]);
      }
      break;
    case 5:                  //SPACE KEY PRESSED
      ctGUI.addcharTextInputBox(txtbox1, String(' '));
      if (ctGUI.GUIobject_data1[txtbox1]==0) ctGUI.toggleButton(btn2, btn3);
      break;   
    case 6:                  //CAVE KEY PRESSED
      ctGUI.addcharTextInputBox(txtbox1, "CAVE"); 
      if (ctGUI.GUIobject_data1[txtbox1]==0) ctGUI.toggleButton(btn2, btn3);
      break;  
    case 7:                  //PIT KEY PRESSED
      ctGUI.addcharTextInputBox(txtbox1, "PIT");
      if (ctGUI.GUIobject_data1[txtbox1]==0) ctGUI.toggleButton(btn2, btn3);
      break;  
    default:
      entryChar = (char) URN;
      if (ctGUI.GUIobject_data1[txtbox1]==0) ctGUI.toggleButton(btn2, btn3);
      ctGUI.addcharTextInputBox(txtbox1, String(entryChar)); 
      break;
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Data Display Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Live data display for Manual mode
void UpdateLiveData() {
  uint8_t LRFstatus;
  /*if ((LRFFlag==true) && (laserFlag==true)) { //For continuous LRF readings. Not used at this time
    LRFstatus = 0;
    if (laserFirstRun==true) {
      caveatron.LRF_Fire();
      laserFirstRun = false;
    }
  }*/
  ReadCompassData(25);
  /*if ((LRFFlag==true) && (laserFlag==true)) LRFstatus = caveatron.LRF_GetRange(); //For continuous LRF readings. Not used at this time
  */
  temperature = caveatron.RTC_GetTemperature();
  if (shotFlag==false) RefreshDataDisplay(LRFstatus);
  /*if ((LRFFlag==true) && (laserFlag==true)) { //For continuous LRF readings. Not used at this time
    if (LRFstatus==3) LRFstatus = ResetLRF();
    else if (LRFstatus==1) caveatron.LRF_Fire();
  }*/
}

// Display live data
void RefreshDataDisplay(uint8_t LRFstatus) {
  int h=0;
  if (currentMode==modeManual) h=-25;
  myGLCD.setColor(BLACK_STD);
  if ((LRFFlag==true) && (LRFstatus != 0)) myGLCD.fillRect(140,114+h,270,169+h);
  myGLCD.fillRect(140,194+h,270,248+h);
  myGLCD.fillRect(160,274+h,270,328+h);
  if (currentMode==modeManual) {
    myGLCD.fillRect(71,336+h,131,358+h);
    myGLCD.fillRect(228,336+h,278,358+h);
  }
  myGLCD.setColor(WHITE_STD);
  myGLCD.setBackColor(BLACK_STD);
  ctGUI.printNumF(azimuth, 1, 283, 194+h, caveatron.FONT_52, 6, RIGHT_J);
  ctGUI.printNumF(inclination, 1, 283, 274+h, caveatron.FONT_52, 6, RIGHT_J);
  if (currentMode==modeManual) {
    ctGUI.printNumF(roll, 1, 131, 336+h, caveatron.FONT_22, 3, RIGHT_J);
    ctGUI.printNumF(temperature, 1, 278, 336+h, caveatron.FONT_22, 3, RIGHT_J);
  }
  if (LRFFlag==true) {
    if (LRFstatus==1) ctGUI.printNumF(distance, 2, 265, 114+h, caveatron.FONT_52, 6, RIGHT_J);
    else if ((LRFstatus==2)||(LRFstatus==4)) ctGUI.print("ERR", 265, 114+h, caveatron.FONT_52, 6, RIGHT_J, RED_STD, BLACK_STD);
    else if (LRFstatus==3) ctGUI.print("FAIL", 265, 114+h, caveatron.FONT_52, 6, RIGHT_J, WHITE_STD, RED_STD);
  }
  delay(100);
}

// Setup for static text on live data display
void DataDisplaySetup() {
  int h=0;
  if (currentMode==modeManual) h=-25;
  myGLCD.setColor(GRAY_025);
  myGLCD.drawLine(15,90+h,304,90+h);
  myGLCD.drawLine(15,91+h,304,91+h);
  myGLCD.drawLine(15,170+h,304,170+h);
  myGLCD.drawLine(15,171+h,304,171+h);
  myGLCD.drawLine(15,250+h,304,250+h);
  myGLCD.drawLine(15,251+h,304,251+h);
  myGLCD.drawLine(15,330+h,304,330+h);
  myGLCD.drawLine(15,331+h,304,331+h);
  ctGUI.print("DISTANCE",15, 96+h, caveatron.FONT_28, 3, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("AZIMUTH",15, 176+h, caveatron.FONT_28, 3, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("INCLINATION",15, 256+h, caveatron.FONT_28, 3, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("m",304, 125+h, caveatron.FONT_43, 4, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print(String((char)176),304, 196+h, caveatron.FONT_43, 4, RIGHT_J, WHITE_STD, BLACK_STD);
  ctGUI.print(String((char)176),304, 276+h, caveatron.FONT_43, 4, RIGHT_J, WHITE_STD, BLACK_STD);
  if (currentMode==modeManual) {
    ctGUI.print("ROLL", 15, 336+h, caveatron.FONT_22, 3, LEFT_J, YELLOW_STD, BLACK_STD);
    ctGUI.print(String((char)176), 143, 336+h, caveatron.FONT_22, 3, RIGHT_J, WHITE_STD, BLACK_STD);
    ctGUI.print("TEMP", 160, 336+h, caveatron.FONT_22, 3, LEFT_J, YELLOW_STD, BLACK_STD);
    ctGUI.print(String((char)176), 290, 336+h, caveatron.FONT_22, 3, RIGHT_J, WHITE_STD, BLACK_STD);
    ctGUI.print("C", 305, 336+h, caveatron.FONT_22, 3, RIGHT_J, WHITE_STD, BLACK_STD);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          File Name Entry Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void FileNameEntrySetup() {
  switch(currentMode) {
    case modeCalRec1000:
      drawStatusBar("Rec 1000 Raw Reads");
      break;
    case modeCalRecCont:
      drawStatusBar("Rec Cont Raw Reads");
      break;
  }
  drawInfoBar("");
  CreateLetterPad(117);
  CreateNumberPad(315);
  btn1 = ctGUI.addButton(234,24,84,84,BLUE_STD,"ENTER",caveatron.FONT_22,optVisible,1);
  btn2 = ctGUI.addButton(1,24,84,84,BLUE_STD,"DELETE",caveatron.FONT_22,optVisible,2); 
  btn3 = ctGUI.addButton(1,24,84,84,RED_STD,"CANCEL",caveatron.FONT_22,optInvisible,3); 
  txtbox1 = ctGUI.addTextInputBox(90,75,140,33,"",caveatron.FONT_22,2,CENTER_J,BLACK_STD,7,optVisible,0);
}

void FileNameEntryHandler(int URN) {  
  int i;
  char entryChar;
  String textBoxText;
  switch (URN) {
    case 1:
      fileName = ctGUI.GUIobject_text[txtbox1]+".txt";
      if ((currentMode==modeCalRec1000)||(currentMode==modeCalRecCont)) {
        CreateScreen(screenRawRecord);
      }
      break;
    case 2:
      ctGUI.delcharTextInputBox(txtbox1);
      if (ctGUI.GUIobject_data1[txtbox1]==0) {
        ctGUI.toggleButton(btn3, btn2);
      }
      break;
    case 3:
      if ((currentMode==modeCalRec1000)||(currentMode==modeCalRecCont)) {
        currentMode = modeRawLive;
        CreateScreen(screenAdvancedCal);
      }
      break;  
    case 4:
      pageLtrKey = !pageLtrKey;    
      if (pageLtrKey==true) {
        for (i=0; i<14; i++) ctGUI.toggleButton(btn[i+14], btn[i]);
      } else {
        for (i=0; i<14; i++) ctGUI.toggleButton(btn[i], btn[i+14]);
      }
      break;
    default:
      entryChar = (char) URN;
      if (ctGUI.GUIobject_data1[txtbox1]==0) {
        ctGUI.toggleButton(btn2, btn3);
      }
      ctGUI.addcharTextInputBox(txtbox1, String(entryChar)); 
      break;
  }
}

//

// ====== double2s(): print out up to 16 digits of input double-precision value
// This version enables double-precision for Teensy 3, etc.
// by J.Beale March 2013
// modified from original float2s() posted by davekw7x on December, 2010
// http://arduino.cc/forum/index.php/topic,46931.0.html

char * double2s(double f, unsigned int digits) {    
int d;
static char s[26];  // formated number as string
int index=0;

 // handle sign
 if (f < 0.0)
 {
   s[index++] = '-';
   f = -f;
 }

 int exponent = int(log10(f));
 double g = f / pow(10, exponent);
 if ((g < 1.0) && (g != 0.0))      
 {
   g *= 10;
   exponent--;
 }
 g += 0.5 / pow(10,digits);
 
 d = g;  
 sprintf(&s[index++],"%d",d);  
 if (digits > 0) sprintf(&s[index++],".");
 for (int i=0;i<digits;i++) {
     g = (g - d) * 10.0;  // shift one decimal place to the left
     d = int(g);
     sprintf(&s[index++],"%d",d);
 }
 sprintf(&s[index], "E%02.2d", exponent);
 return s;    
} // ===== end double2s()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Dialog Box Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

boolean DialogBox(int numButtons, char msgStr1[], char msgStr2[], char msgStr3[], char buttonMsg1[], char buttonMsg2[], int d) {
  boolean result, quit = false;
  int x,y;
  int bx;
  if (numButtons==2) bx = 50;
  else bx = 115;
  myGLCD.setColor(127, 127, 0);
  myGLCD.setBackColor(127, 127, 0);
  myGLCD.fillRect(30, 90, 290, 304);
  myGLCD.setColor(BLACK_STD);
  myGLCD.drawRect(30, 90, 290, 304);
  myGLCD.setColor(WHITE_STD);
  ctGUI.print(msgStr1, 160, 110, caveatron.FONT_22, 2, CENTER_J);
  ctGUI.print(msgStr2, 160, 132, caveatron.FONT_22, 2, CENTER_J);
  ctGUI.print(msgStr3, 160, 154, caveatron.FONT_22, 2, CENTER_J);
  if (numButtons > 0) {
    myGLCD.setColor(BUTTON_STD);
    myGLCD.fillRoundRect(bx,194,bx+90,284);
    if (numButtons==2) myGLCD.fillRoundRect(180,194,270,284);
    myGLCD.setColor(WHITE_STD);
    myGLCD.drawRoundRect(bx,194,bx+90,284);
    if (numButtons==2) myGLCD.drawRoundRect(180,194,270,284);
    myGLCD.setColor(GRAY_025);
    myGLCD.drawRoundRect(bx+1,195,bx+89,283);
    if (numButtons==2) myGLCD.drawRoundRect(181,195,269,283);
    ctGUI.print(buttonMsg1, bx+45, 229, caveatron.FONT_22, 2, CENTER_J, WHITE_STD, BUTTON_STD);
    if (numButtons==2) ctGUI.print(buttonMsg2, 225, 229, caveatron.FONT_22, 2, CENTER_J, WHITE_STD, BUTTON_STD);
    while (quit==false) {
      if (myTouch.dataAvailable()) { 
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
        if ((y>=212) && (y<=302)) {
          myGLCD.setColor(RED_STD);
          if ((x>=bx) && (x<=bx+90)) {
            myGLCD.drawRoundRect(bx,194,bx+90,284);
            myGLCD.drawRoundRect(bx+1,195,bx+89,283);
            result = true;
            quit = true;
          } else if ((numButtons==2) && (x>=180) && (x<=270)) {
            myGLCD.drawRoundRect(180,194,270,284);
            myGLCD.drawRoundRect(181,195,269,283);
            result = false;
            quit = true;
          }
          delay(500);
        }
      }
    }
  }
  else {
    delay(d*1000);
    result = true;
  }
  return result;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Error Box Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void ErrorBox(char msgStr1[], char msgStr2[], char msgStr3[], int resetType) {
  if (resetType==99) CloseSurveyFiles();
  myGLCD.setColor(255, 255, 128);
  myGLCD.setBackColor(255, 255, 128);
  myGLCD.fillRect(30, 80, 290, 400);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setFont(BigFont);  
  if (resetType==98) myGLCD.print("WARNING:", CENTER, 100);
  else myGLCD.print("ERROR!", CENTER, 100);
  myGLCD.print(msgStr1, CENTER, 150); myGLCD.print(msgStr2, CENTER, 180); myGLCD.print(msgStr3, CENTER, 210);
  ctGUI.addButton(100,280,120,80,BLUE_STD,"OK",caveatron.FONT_28,optVisible,resetType);
  while (errorFlag==true) delay(10);
}

void ErrorBoxHandler(int resetType) {
  if (resetType==99) {                  //Error 99 is reset to main screen
    errorFlag = false; 
    currentMode = modeNull;    
    CreateScreen(screenMainMenu);
  }
  else if (resetType==98) {             //Error 98 is continue
    errorFlag = false; 
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(30, 80, 290, 400);
  }
 
}

