/******************************************************************/
//                        CAVEATRON                               //
//                       Version 1.31                             //
/******************************************************************/
// Joe Mitchell, 2019-02-25

// Change Notes(1.31):
// CRITICAL: If you updated the Caveatron_Hardware library to v1.3, replace it with v1.31. If not, no change is required.
// Improvements to ensure data is written to SD card in the event of a system freeze.
// Fixed bug that can potentially cause a system freeze when ending a scan with the SWEEP LIDAR (requires updating SWEEP library to 2019-03-02 version).
// Changes to sequence of events when LIDAR scan ended to ensure log file is properly updated in the event of a system freeze.


// Hardware configuration option - set to 0 for manual, 1 to load from EEPROM
#define AUTO_CONFIG 1

// For manual hardware configuration, set hardware revision and code
char hardwareRev = 'A';
char hardwareCode[] = "12221011110";
uint8_t lidarModuleType;
uint8_t lidarModuleSubType=0;
uint8_t lidarModuleConfig=0;

String softwareVersion = "1.31";

#include <Eigen3210.h>     // Calls main Eigen matrix class library
#include <Eigenvalues>             // Calls inverse, determinant, LU decomp., etc.
using namespace Eigen;    // Eigen related statement; simplifies syntax for declaration of matrices

#include <Wire.h>
#include <UTFT.h>
#include <URTouch.h>
#include <Caveatron_Hardware.h>
#include <UTFT_CTE.h>
#include <UTFT_GHL.h>
#include <Caveatron_GUI.h>
#include <SdFat.h>
#include <MAX17043.h>
#include <SparkFunBQ27441.h>
#include <Sweep.h>
#include <RPLidar.h>
#if defined(_SAM3XA_)
  #include <DueTimer.h>
#endif

#if defined(_SAM3XA_)
  UTFT myGLCD(25,26,27,28);
  URTouch myTouch(6,5,32,3,2);
  SdFat sd;
#endif
Caveatron_Hardware caveatron(&myGLCD, &myTouch);
Caveatron_GUI ctGUI(&caveatron, &myGLCD, &myTouch);

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

//Variables for GUI timer
#define GUI_check_frequency 8
volatile boolean buttonFlag = false;
volatile boolean interruptLoop = false;
volatile int buttonPressed = -1;
int updateStatusBarCount = 1;

// variables for screens etc
#define screenMainMenu 1
#define screenSettingsMenu 2
#define screenShotMode 3
#define screenPassageMode 4
#define screenRoomMode 5
#define screenManualMode 6
#define screenSurveyMenu 7
#define screenStationEntry 8
#define screenTraverseEntry 9
#define screenSplayEntry 10
#define screenTimeUnits 11
#define screenSDCard 12
#define screenCalibrate 13
#define screenLIDARView 14
#define screenAbout 15
#define screenShotStationSelect 16
#define screenSetTimeDate 21
#define screenTimeDateEntry 22
#define screenSDCardInfo 23
#define screenCompassCalibrate 24
#define screenFileNameEntry 25
#define screenCaveNameEntry 26
#define screenSurveyStats 31
#define screenLinePlot 32
#define screenShotViewer 33
#define screenScanViewer 34
#define screenShotScanReview 35
#define screenAdvancedCal 40
#define screenRawRecord 41
#define screenCalShots 42
#define screenCheckCalibration 43

#define modeNull 0
#define modeShot 1
#define modePassage 2
#define modeRoom 3
#define modeManual 4
#define modeSurvey 5
#define modeSetTime 11
#define modeLIDARView 12
#define modeRawLive 30
#define modeCalRec1000 31
#define modeCalRecCont 32
#define modeCalShots 33
#define modeEditSurvey 40
#define modeViewScans 41

int currentScreen=0;
int currentMode=0;

//Variables for GUI
#define URNnull 0
int pnlTitle, btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, txtbox1, pbar1, pbar2, list1;
extern uint8_t BigFont[];
extern uint8_t SmallFont[];
extern const int sin_table[];
extern const int cos_table[];
int btn[50];
int guiStep = 0;
int numOfKeys;
boolean pageLtrKey = false;
boolean shotFlag = false;
boolean holdFlag = false;
boolean errorFlag = false;

//Variables for LCD Screen calibration
unsigned long screenCal[3];

// Variables for RTC
int newSecond, newMinute, newHour, newDay, newMonth, newYear;
String timeSecond, timeMinute, timeHour, dateDay, dateMonth, dateYear;
int timeSetType = 0;
boolean newTimeDate = false;
boolean liveTimeDate = true;

// Variables for Laser Rangefinder
volatile boolean laserFlag = false;
long lastLRFTime=0;
volatile boolean LRFFlag = false;
int LRFRangeCal;

// Variables for LIDAR
Sweep device(Serial2);
RPLidar rplidar;
//ScanPacket reading;
boolean LIDARModule = false;
int LIDARPowerPin, LIDARSensePin1, LIDARSensePin2;
int lidarPacket[4][3];
int angle, lastangle, rot_count, pos_count, data_count, save_count, LIDARstartTime, LIDAR_LRF_Freq;
float lastLIDARdistance, lastLIDARazi, lastLIDARinc, initLIDARdistance;
boolean incShiftFlag, aziShiftFlag, lidarErrorFlag = false;
int idx, LIDAROrientCal;

//Variables for Measurement
float ax, ay, az, rtilt;
float distance, azimuth, inclination, roll, temperature;
float accelCal[12], hardIronCal[3], softIronCal[9], mmAmp[4], mmOff[4];
int16_t firstIMU[6], sphereCenter[3];
int rawIMU[6], ztilt;
boolean rightFlag = false;

//Variables for Station Codes
String stationFrom, stationTo, stationFromNew, stationToNew, stationNext;
String traverseNum, traverseTo, splayNum, splayStation;
float lengthTrav, lengthHoriz, lengthVert;
int numStations, numTraverses, numSplays;
boolean backSight;
typedef struct
  {
    String code;
    int lastTraverseNum;
    int lastSplayNum;
  } scan_list;
  scan_list sta;

//Variables for File and Cave Names
String caveName, caveCode, fileName, fileNameBase, infoBarMsg;
int surveySecond, surveyMinute, surveyHour, surveyDay, surveyMonth, surveyYear;

//Variables for Station Plotting
int plotView, plotGroup;
boolean plotLabels;

//Variables for Shot List
int vectorPage, numVectors;
typedef struct
  {
      String st1_e;
      String st2_e;
      String di_e;
      String az_e;
      String in_e;
      boolean bk_e;
  }  editvector_type;
  editvector_type editVector;

//Variables for Scan List
int scanPage, numScans;

//Variables for SD Card
SdFile theFile, settingsFile, scanFileA, logFile;
ifstream inFile, scanFile;
ofstream outFile, lidarFile;
uint32_t filePosition, streamPositionData, streamPositionScans, streamPositionScanLine, readPosition;
boolean SDFlag, settingsFlag;
boolean fileFlag = false;

//Variables for Buzzer
bool buzOn = false;
boolean buzzerFlag = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Startup Setup Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void setup() {
  //Setup LIDAR pins
  LIDARSensePin1 = 21;
  LIDARSensePin2 = 20;
  LIDARPowerPin = 11;
  pinMode(LIDARPowerPin, OUTPUT); 
  pinMode(LIDARSensePin1, INPUT_PULLUP);
  pinMode(LIDARSensePin2, INPUT_PULLUP);
  
  //Setup Serial
  Serial.begin(250000);
  //Serial2.begin(115200);

  //Init SPI
  Wire.begin();
  Wire1.begin();

  delay(500);
  
  //Check for LIDAR Module
  boolean LIDARsense1 = digitalRead(LIDARSensePin1);
  boolean LIDARsense2 = digitalRead(LIDARSensePin2);
  if ((LIDARsense1==LOW)&&(LIDARsense2==LOW)) {
    LIDARModule = true;
    lidarModuleType = 3;
  } else if (LIDARsense1==LOW) {
    LIDARModule = true;
    lidarModuleType = 1
    ;
  } else if (LIDARsense2==LOW) {
    LIDARModule = true;
    lidarModuleType = 2;
  } else LIDARModule = false;

  if (LIDARModule==true) digitalWrite(LIDARPowerPin, HIGH);

  //Set Calibration Parameters and Hardware Version Specific Settings
  if (AUTO_CONFIG==1) {
    LoadCalibrationParameters();
    caveatron.Init();
  }
  else {
    LoadTestCalibrationParameters();
    caveatron.Init(hardwareRev, hardwareCode);
  }
  
  //Setup SD card
  caveatron.SD_Init();
  
  //Setup Laser Rangefinder
  caveatron.LRF_Init();
  caveatron.LRF_SetMode(0);

  //Setup Battery Gauge
  caveatron.BATT_Init();
  
  //Setup IMU module
  caveatron.IMU_Init();
  
  //Init Buzzer
  caveatron.BUZZ_Init();
  delay(100);

  //Init GUI
  myTouch.InitTouch(PORTRAIT, screenCal[0], screenCal[1], screenCal[2]);
  caveatron.LCD_Init();
  myGLCD.clrScr();
  delay(500);
  ctGUI.InitGUI();
  ctGUI.print("", 160, 0, caveatron.FONT_19, 2, CENTER_J, BLACK_STD, BLACK_STD);
  
  delay(500);

  //Configure LIDAR
  if (LIDARModule==true) caveatron.LIDAR_Init(lidarModuleType);
  digitalWrite(LIDARPowerPin, LOW);

  //If USB connected from Caveatron Connect software, start USB connect mode
  if (Serial.available() > 0) {}
  if (Serial.read() == '?') USBConnectMode();

  //Init SD
  InitSD();

  delay(100);
  CreateScreen(screenMainMenu);

  //Start GUI Interrupt Timer  
  Timer4.attachInterrupt(GUI_Handler).setFrequency(GUI_check_frequency).start();
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Main Function Loop
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void loop() {
  
  //If button pressed, send to handler
  if (buttonFlag==true){
    Timer4.stop();
    OnButtonPress(buttonPressed);
    Timer4.start();
  }

  //If interrupt timer triggers (@8 Hz), take action depending on mode
  if (interruptLoop==true) {
    switch (currentMode) {
      case modeShot:
        if (shotFlag==true) {
          TakeShot();
        }
        break;
      case modeManual:
        if (holdFlag==false) {
          if (shotFlag==true) {
            holdFlag = true;
            TakeShot();
          }
          else {
            Timer4.stop();
            UpdateLiveData();
            Timer4.start();
          }
        }
        break;
      case modeSetTime:
        DisplayTimeDate();
        break;
      case modeRawLive:
      case modeCalRec1000:
      case modeCalRecCont:
        if (currentScreen!=screenFileNameEntry) UpdateRawDisplay();
        break;
      case modeCalShots:
        UpdateCalShotsDisplay();
        break;
    }
  }
  //Set false to await next interrupt
  interruptLoop = false;  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          GUI General and Main Menu Setup Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Function for calling functions to setup new menu screens depending on screen code
void CreateScreen(int screen) {
  // Set global variable to record which screen is in view
  currentScreen=screen; 
  // Clear all the data and the screen
  ctGUI.clearAllObjects();
  myGLCD.clrScr();
  
  //Select screen to setup
  boolean fileFlag = false;
  switch (currentScreen) {
    case screenMainMenu: 
      MainMenuSetup();
      break;
    case screenSettingsMenu: 
      SettingsMenuSetup();
      break;
    case screenShotStationSelect:
      if (SDFlag==false) ErrorBox("SD Card", "Not Found", "", 99);
      else fileFlag = OpenSurveyFiles();
      if ((SDFlag==true) && (fileFlag==true)) ShotStationSetup();
      break;
    case screenStationEntry:   
      if (currentMode!=modeShot) {
        if (SDFlag==false) ErrorBox("SD Card", "Not Found", "", 99);
        else fileFlag = OpenSurveyFiles();
        if ((SDFlag==true) && (fileFlag==true)) StationEntrySetup();
      } else StationEntrySetup();
      break;
    case screenTraverseEntry:
      TraverseEntrySetup();
      break;
    case screenSplayEntry:
      SplayEntrySetup();
      break;
    case screenShotMode:
      ShotModeSetup();
      break;
    case screenPassageMode:
      PassageModeSetup();
      break;
    case screenRoomMode:
      RoomModeSetup();
      break;
    case screenManualMode:
      ManualModeSetup();
      break;
    case screenSurveyMenu:
      if (SDFlag==false) ErrorBox("SD Card", "Not Found", "", 99);
      else SurveyMenuSetup();
      break;
    case screenCaveNameEntry:
      CaveNameEntrySetup();
      break;
    case screenSurveyStats:
      if (settingsFlag==false) ErrorBox("Settings File", "Not Found -", "Start New Survey", 99);
      else SurveyStatsSetup();
      break;
    case screenLinePlot:
      LinePlotSetup();
      break;
    case screenShotScanReview:
      ShotScanReviewSetup();
      break;
    case screenShotViewer:
      ShotViewerSetup(1);
      break;
    case screenScanViewer:
      ScanViewerSetup(1);
      break;
    case screenTimeUnits:
      TimeUnitsSetup();
      break;
    case screenSetTimeDate:
      SetTimeDateSetup();
      break;
    case screenTimeDateEntry:
      TimeDateEntrySetup();
      break;
    case screenSDCard:
      if (SDFlag==false) ErrorBox("SD Card", "Not Found", "", 99);
      else SDCardMenuSetup();
      break;
    case screenCalibrate:
      CalibrateMenuSetup();
      break;
    case screenAbout:
      AboutScreenSetup();
      break;
    case screenCompassCalibrate:
      CompassCalibrateSetup();
      break;
    case screenCheckCalibration:
      CheckCalibrationSetup();
      break;
    case screenAdvancedCal:
      AdvancedCalMenuSetup();
      break;
    case screenRawRecord:
      RawRecordSetup();
      break;
    case screenCalShots:
      CalShotsSetup();
      break;
    case screenFileNameEntry:
      FileNameEntrySetup();
      break;
    case screenLIDARView:
      LIDARViewSetup();
      break;
  }
  
  // Draw screen to LCD
  ctGUI.redrawAllObjects();
}

// Setup Main Menu Screen
void MainMenuSetup(){
  drawStatusBar("CAVEATRON");
  drawInfoBar(infoBarMsg);
  guiStep = 0;
  ctGUI.addIconButton(12,30,140,130,BUTTON_STD,"SHOT",caveatron.FONT_28,10,caveatron.iSHOT,62,50,optVisible,1);
  if(LIDARModule) {
    ctGUI.addIconButton(167,30,140,130,BUTTON_STD,"PASSAGE",caveatron.FONT_28,10,caveatron.iPASSAGE,62,50,optVisible,2);
    ctGUI.addIconButton(167,171,140,130,BUTTON_STD,"ROOM",caveatron.FONT_28,10,caveatron.iROOM,62,50,optVisible,3);
    ctGUI.addIconButton(12,171,140,130,BUTTON_STD,"MANUAL",caveatron.FONT_28,10,caveatron.iMANUAL,62,50,optVisible,4);
  }
  else {
    ctGUI.addIconButton(167,30,140,130,BUTTON_STD,"MANUAL",caveatron.FONT_28,10,caveatron.iMANUAL,62,50,optVisible,4);
  }
  ctGUI.addIconButton(12,321,140,130,BUTTON_STD,"SURVEY",caveatron.FONT_28,10,caveatron.iSURVEY,62,50,optVisible,5);
  ctGUI.addIconButton(167,321,140,130,BUTTON_STD,"SETTINGS",caveatron.FONT_28,10,caveatron.iSETTINGS,62,50,optVisible,6);
  myGLCD.setColor(GRAY_025);
  myGLCD.drawLine(12,311,307,311);
}

//Setup Settings Menu Screen
void SettingsMenuSetup(){
  drawStatusBar("SETTINGS");
  drawInfoBar("SURVEY: "+caveName);

  ctGUI.addIconButton(12,30,140,130,BUTTON_STD,"TIME",caveatron.FONT_28,10,caveatron.iTIME,62,50,optVisible,1);
  ctGUI.addIconButton(167,30,140,130,BUTTON_STD,"SD CARD",caveatron.FONT_28,10,caveatron.iSD,62,50,optVisible,2);
  ctGUI.addIconButton(12,171,140,130,BUTTON_STD,"CALIBRATE",caveatron.FONT_28,10,caveatron.iCALIBRATE,62,50,optVisible,3);
  if(LIDARModule) {
    ctGUI.addIconButton(167,171,140,130,BUTTON_STD,"LIDAR",caveatron.FONT_28,10,caveatron.iLIDAR,62,50,optVisible,4);
  }
  ctGUI.addIconButton(12,321,140,130,BUTTON_STD,"ABOUT",caveatron.FONT_28,10,caveatron.iABOUT,62,50,optVisible,5);
  ctGUI.addIconButton(167,321,140,130,BUTTON_STD,"MAIN MENU",caveatron.FONT_28,10,caveatron.iRETURN,62,50,optVisible,6);
  myGLCD.setColor(GRAY_025);
  myGLCD.drawLine(12,311,307,311);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 GUI Interupt Routines
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// fast ticks for Due GUI interupt
volatile int btnFound;
volatile unsigned long int GUI_InterruptTime;
unsigned long int GUI_ButtonLastPressedTime=0;
//
//  DueGUI Interupt handler routine
//
void GUI_Handler(){
  //
  // We don't want timer called whilst it's being handled.
  //
  GUI_InterruptTime=millis();
  //
  // routine is called several times per second ie "ticksPerSecond".
  if ((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)){
    if ((GUI_ButtonLastPressedTime+200)<GUI_InterruptTime){
      GUI_ButtonLastPressedTime=GUI_InterruptTime;
      btnFound=ctGUI.checkAllButtons(OnRelease);
      //If button found set button flag
      if (btnFound!=-1){
        buttonFlag=true;
        buttonPressed = btnFound;
      }
    }
  }
  //Update status bar every 10 seconds
  if (updateStatusBarCount>(GUI_check_frequency*10)) {
    if ((currentMode!=modePassage) && (currentMode!=modeRoom) && (currentScreen!=screenLinePlot)) { //Don't update under these modes and screens
      drawStatusBar("");
      updateStatusBarCount=1;
    }
  }
  updateStatusBarCount++;
  
  interruptLoop = true;
  // Ok interupt handler is finished. 
}


// Function for calling button press handler depending on current screen code
void OnButtonPress(int btnFound){
  int URN=ctGUI.GUIobject_UniqueReference[btnFound];
  if ((URN==98)||(URN==99)) ErrorBoxHandler(URN);
  switch(currentScreen) {
    case screenMainMenu: 
      MainMenuHandler(URN);
      break;
    case screenSettingsMenu:
      SettingsMenuHandler(URN);
      break;
    case screenStationEntry:
    case screenTraverseEntry: 
    case screenSplayEntry:
    case screenCaveNameEntry: 
    case screenTimeDateEntry:
      KeyPadHandler(URN);
      break;
    case screenShotStationSelect:
      ShotStationSelectHandler(URN);
      break;
    case screenShotMode: 
      ShotModeHandler(URN);
      break;
    case screenPassageMode: 
      PassageModeHandler(URN);
      break;
    case screenRoomMode: 
      RoomModeHandler(URN);
      break;
    case screenManualMode: 
      ManualModeHandler(URN);
      break;
    case screenSurveyMenu: 
    case screenSurveyStats:
      SurveyMenuHandler(URN);
      break;
    case screenShotScanReview:
      ShotScanReviewHandler(URN);
      break;
    case screenShotViewer:
      ShotViewerHandler(URN);
      break;
    case screenScanViewer:
      ScanViewerHandler(URN);
      break;
    case screenLinePlot:
      LinePlotHandler(URN);
      break;
    case screenTimeUnits:
      TimeUnitsMenuHandler(URN);
      break;
    case screenSetTimeDate:
      SetTimeDateHandler(URN);
      break;
    case screenAbout:
    case screenSDCard:
      SDCardMenuHandler(URN);
      break;
    case screenCalibrate:
      CalibrateMenuHandler(URN);
      break;
    case screenCompassCalibrate:
      CompassCalibrateHandler(URN);
      break;
    case screenCheckCalibration:
      CheckCalibrationHandler(URN);
      break;
    case screenAdvancedCal:
      AdvancedCalMenuHandler(URN);
      break;
    case screenRawRecord:
      RawRecordHandler(URN);
      break;
    case screenCalShots:
      CalShotsHandler(URN);
      break;
    case screenFileNameEntry:
      FileNameEntryHandler(URN);
      break;
  }
  //Reset button flag have buttons handled
  buttonFlag=false;  
}  
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Main Menu and Settings Menu Handlers
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Button press handler for Main Menu screen
void MainMenuHandler(int URN) {  
  switch (URN) {
    case 1:
      currentMode = modeShot; 
      guiStep++;
      CreateScreen(screenShotStationSelect);
      break;
    case 2:
      currentMode = modePassage; 
      guiStep++;
      CreateScreen(screenStationEntry);
      break;
    case 3:
      currentMode = modeRoom; 
      guiStep++;
      CreateScreen(screenStationEntry);
      break;
    case 4:
      currentMode = modeManual;
      CreateScreen(screenManualMode);
      break;
    case 5:
      currentMode = modeSurvey; 
      guiStep++;    
      CreateScreen(screenSurveyMenu);
      break;
    case 6:
      CreateScreen(screenSettingsMenu);
      break;
  }
}

//Button press handler for Settings Menu screen
void SettingsMenuHandler(int URN) {  
  switch (URN) {
    case 1:
      guiStep++;
      CreateScreen(screenTimeUnits);
      break;
    case 2:
      guiStep++;
      CreateScreen(screenSDCard);
      break;
    case 3:
      guiStep++;
      CreateScreen(screenCalibrate);
      break;
    case 4:
      currentMode = modeLIDARView;
      guiStep++;
      CreateScreen(screenLIDARView);
      break;
    case 5:
      guiStep++;
      CreateScreen(screenAbout);
      break;
    case 6:
      CreateScreen(screenMainMenu);
      break;
  }
}

