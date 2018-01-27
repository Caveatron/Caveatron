//LIST OF COMMANDS TO CHANGE COMPASS UPDATE RATE
//compass.writeMagReg(LSM303_1::CRA_REG_M, 0x18); //75Hz Update Rate
//compass.writeMagReg(LSM303::CRA_REG_M, 0x14); //30Hz Update Rate
//compass.writeMagReg(LSM303::CRA_REG_M, 0x0C); //Default 7.5Hz Update Rate

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Calibration Menu Setup and Handler
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CalibrateMenuSetup() {
  drawStatusBar("Calibration");
  drawInfoBar("");
  btn1 = ctGUI.addButton(20,33,280,66,BUTTON_STD,"COMPASS CALIBRATION",caveatron.FONT_22,optVisible,1);
  btn2 = ctGUI.addButton(20,114,280,66,BUTTON_STD,"View Calibration Values",caveatron.FONT_22,optVisible,2);
  btn3 = ctGUI.addButton(20,195,280,99,BLACK_STD,"Advanced Calibration", "For Expert Use Only",caveatron.FONT_22,optVisible,3);
  ctGUI.addButton(20,364,280,84,BLUE_STD,"RETURN TO SETTINGS",caveatron.FONT_28,optVisible,5);
}

void CalibrateMenuHandler(int URN) {
    switch (URN) {
      case 1:                            //User Compass Calibration
        CreateScreen(screenCompassCalibrate);
        break;
      case 2:                            //View Compass Calibration Parameters
        ViewCalParameters();
        break;
      case 3:                            //Advanced Calibration - for system setup and verification
        currentMode = modeRawLive;
        CreateScreen(screenAdvancedCal);
        break;
      case 5:                            //Exit
        guiStep--;
        currentMode = modeNull;
        CreateScreen(screenSettingsMenu);
        break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       User Compass Calibration Routines
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void CompassCalibrateSetup() {
  drawStatusBar("Compass Calibration");
  drawInfoBar("");
  btn1 = ctGUI.addButton(20,145,280,200,GREEN_DRK,"START", "CALIBRATION", caveatron.FONT_28, optVisible, 1);
  pbar1 = ctGUI.addProgressBar(20,300,280,40,64,"Points Collected", caveatron.FONT_22, WHITE_STD, optInvisible, 2);
  pbar2 = ctGUI.addProgressBar(20,300,280,40,4040,"Time Remaining", caveatron.FONT_22, WHITE_STD, optInvisible, 3);
  btn4 = ctGUI.addButton(20,145,280,200,GREEN_DRK,"CONTINUE", "CALIBRATION", caveatron.FONT_28, optInvisible, 4);
  btn2 = ctGUI.addButton(20,364,130,84,RED_DRK,"CANCEL",caveatron.FONT_22, optVisible, 5);
  btn3 = ctGUI.addButton(170,364,130,84,BLUE_STD,"SAVE","& EXIT",caveatron.FONT_22, optInvisible, 6);
  myGLCD.setColor(YELLOW_STD);
  myGLCD.setBackColor(BLACK_STD);
  ctGUI.print("Step 1: Find Mag Center", 160, 30, caveatron.FONT_22, 2, CENTER_J);
  ctGUI.print("Continuously rotate box in", 160, 55, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.print("all possible orientations", 160, 75, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.print("to keep blue progress bar", 160, 95, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.print("full as long as possible", 160, 115, caveatron.FONT_19, 2, CENTER_J);
}

void CompassCalibrateHandler(int URN) {
    switch (URN) {
      case 1:                            //Start Calibration - Step 1: Find approximate center of magnetic sphere
        ctGUI.makeObjectInvisible(btn1);
        ctGUI.makeObjectInvisible(btn2);    
        //theFile.open("EIGDATA.TXT", O_RDWR | O_CREAT | O_AT_END);
        //theFile.println("**********");theFile.println("NEW DATASET");
        ctGUI.print("FINDING CENTER...", 160, 200, caveatron.FONT_34, 3, CENTER_J, YELLOW_STD, BLACK_STD);
        ctGUI.print("Center:", 20, 380, caveatron.FONT_19, 2, LEFT_J, YELLOW_STD, BLACK_STD);
        ctGUI.makeObjectVisible(pbar2);
        Timer4.stop();
        ctGUI.updateProgressBar(pbar2, 4000);
        BuzzerBEEP(800, 200);
        delay(1300);
        FindMagSphereCenter();
        myGLCD.setColor(BLACK_STD);
        myGLCD.fillRect(20,30,300,415);
        ctGUI.makeObjectInvisible(pbar2);
        ctGUI.makeObjectVisible(btn2);
        ctGUI.makeObjectVisible(btn4);
        myGLCD.setColor(YELLOW_STD);
        myGLCD.setBackColor(BLACK_STD);
        BuzzerBEEP(800, 200);
        ctGUI.print("Step 2: Get Calibration Pts", 160, 30, caveatron.FONT_22, 2, CENTER_J);
        ctGUI.print("Continuously rotate box in", 160, 55, caveatron.FONT_19, 2, CENTER_J);
        ctGUI.print("all possible orientations", 160, 75, caveatron.FONT_19, 2, CENTER_J);
        ctGUI.print("to fill blue progress bar", 160, 95, caveatron.FONT_19, 2, CENTER_J);
        Timer4.start();
        break;
      case 4:                            //Continue Calibration - Step 2: Gather the hard and soft iron data, calculate the eigenvectors, and soft for the coefficients
        ctGUI.makeObjectInvisible(btn4);   
        ctGUI.makeObjectInvisible(btn2);  
        //theFile.open("EIGDATA.TXT", O_RDWR | O_CREAT | O_AT_END);
        //theFile.println("**********");theFile.println("NEW DATASET");
        ctGUI.print("CALIBRATING...", 160, 200, caveatron.FONT_34, 3, CENTER_J, YELLOW_STD, BLACK_STD);
        ctGUI.makeObjectVisible(pbar1);
        Timer4.stop();
        BuzzerBEEP(800, 200);
        delay(1300);
        EigenCalibration();
        myGLCD.setColor(BLACK_STD);
        myGLCD.fillRect(20,30,300,235);
        ctGUI.makeObjectVisible(btn2);
        ctGUI.makeObjectVisible(btn3);
        myGLCD.setBackColor(BLACK_STD);
        myGLCD.setColor(YELLOW_STD);
        ctGUI.print("Hard Iron Calibration", 160, 40, caveatron.FONT_22, 2, CENTER_J);
        ctGUI.print("Soft Iron Calibration", 160, 150, caveatron.FONT_22, 2, CENTER_J);
        ctGUI.print("X=", 80, 70, caveatron.FONT_19, 2, LEFT_J);
        ctGUI.print("Y=", 80, 90, caveatron.FONT_19, 2, LEFT_J);
        ctGUI.print("Z=", 80, 110, caveatron.FONT_19, 2, LEFT_J);
        myGLCD.setColor(WHITE_STD);
        ctGUI.printNumF(hardIronCal[0], 4, 240, 70, caveatron.FONT_19, 2, RIGHT_J);
        ctGUI.printNumF(hardIronCal[1], 4, 240, 90, caveatron.FONT_19, 2, RIGHT_J);
        ctGUI.printNumF(hardIronCal[2], 4, 240, 110, caveatron.FONT_19, 2, RIGHT_J);
        ctGUI.printNumF(softIronCal[0], 6, 5, 180, caveatron.FONT_19, 2, LEFT_J);
        ctGUI.printNumF(softIronCal[1], 6, 160, 180, caveatron.FONT_19, 2, CENTER_J);
        ctGUI.printNumF(softIronCal[2], 6, 315, 180, caveatron.FONT_19, 2, RIGHT_J);
        ctGUI.printNumF(softIronCal[3], 6, 5, 200, caveatron.FONT_19, 2, LEFT_J);
        ctGUI.printNumF(softIronCal[4], 6, 160, 200, caveatron.FONT_19, 2, CENTER_J);
        ctGUI.printNumF(softIronCal[5], 6, 315, 200, caveatron.FONT_19, 2, RIGHT_J);
        ctGUI.printNumF(softIronCal[6], 6, 5, 220, caveatron.FONT_19, 2, LEFT_J);
        ctGUI.printNumF(softIronCal[7], 6, 160, 220, caveatron.FONT_19, 2, CENTER_J);
        ctGUI.printNumF(softIronCal[8], 6, 315, 220, caveatron.FONT_19, 2, RIGHT_J);
        BuzzerBEEP(800, 1000);
        Timer4.start();
        break;
      case 6:                            //Save & Exit
        SaveCalibration();
      case 5:                            //Cancel
        guiStep--;
        currentMode = modeNull;
        CreateScreen(screenSettingsMenu);
        break;
  }
}


//FIND CENTER OF MAGNETIC SPHERE FOR HARD AND SOFT IRON CALIBRATION
void FindMagSphereCenter() {
  int16_t running_min[3] = {32767, 32767, 32767}, running_max[3] = {-32768, -32768, -32768}, running_center[3] = {0, 0, 0}, last_running_center[3] = {0, 0, 0};
  int lastChangeTime = millis();

  //Look for new min and max magnetic values. If more than 4 sec since last new min and max, end collection
  while ((millis() - lastChangeTime) < 4000) {
    caveatron.IMU_Read();
    
    running_min[0] = min(running_min[0], caveatron.IMU_m_x);
    running_min[1] = min(running_min[1], caveatron.IMU_m_y);
    running_min[2] = min(running_min[2], caveatron.IMU_m_z);
  
    running_max[0] = max(running_max[0], caveatron.IMU_m_x);
    running_max[1] = max(running_max[1], caveatron.IMU_m_y);
    running_max[2] = max(running_max[2], caveatron.IMU_m_z);

    //Update x,y,z center estimate from min and max
    for (int i=0; i<3; i++) {
      last_running_center[i] = running_center[i];  
      running_center[i] = running_max[i] - ((running_max[i]-running_min[i])/2);
      ctGUI.printNumI(running_center[i], 100+75*i, 380, caveatron.FONT_19, 2, LEFT_J, WHITE_STD, BLACK_STD);
    }

    //Check for change in center values since last reading and if so, reset time counter
    if ((running_center[0] != last_running_center[0]) || (running_center[1] != last_running_center[1]) || (running_center[2] != last_running_center[2])) {
      lastChangeTime = millis();
    }
    ctGUI.updateProgressBar(pbar2, 4000-(millis()-lastChangeTime));
  }

  for (int i=0; i<3; i++) sphereCenter[i] = running_center[i];
}


//HARD AND SOFT IRON CALIBRATION ROUTINE
void EigenCalibration() { 
  int calNumCount=0;
  int combo=-1, combo_sector=-1, XAxis, YAxis, ZAxis;
  int s=1000;
  float p;

  int sphereSectorCount[8];
  for(int i=0;i<8;i++) sphereSectorCount[i]=8;
  /*int sphereSectorCount[8][4];
  for(int i=0;i<8;i++) for(int j=0;j<4;j++) sphereSectorCount[i][j]=2;*/
  
  //Initialize Matrices
    ArrayXf x(64,1);
    ArrayXf y(64,1);
    ArrayXf z(64,1);
    MatrixXf D(64,9);
    /*ArrayXf x(40,1);
    ArrayXf y(40,1);
    ArrayXf z(40,1);
    MatrixXf D(40,9);*/
    MatrixXf VA(9,9);
    MatrixXf VB(9,1);
    MatrixXf V(9,1);
    MatrixXf A(4,4);
    MatrixXf CA(3,3);
    MatrixXf CB(3,1);
    MatrixXf C(3,1);
    MatrixXf T(4,4);
    MatrixXf R(4,4);
    MatrixXf E(3,3);
    MatrixXf EVEC(3,3);
    MatrixXf RADII(3,1);
    MatrixXf SCALE(3,3);
    MatrixXf SOFTIRON(3,3);
   
  // Find points around sphere and load array for calibration
  // The objective is to obtain a roughly equal distribution of points around the magnetic sphere
  while	(calNumCount<64) {
  //while (calNumCount<40) {

    delay(200);
    caveatron.IMU_Read();
    XAxis = caveatron.IMU_m_x;	
    YAxis = caveatron.IMU_m_y;
    ZAxis = caveatron.IMU_m_z;

    //Subdivide sphere into 8 quadrants and detemine which quadrant new point falls within 
    if ((XAxis > sphereCenter[0]) && (YAxis > sphereCenter[1]) && (ZAxis > sphereCenter[2])) combo = 0; 
    if ((XAxis > sphereCenter[0]) && (YAxis > sphereCenter[1]) && (ZAxis < sphereCenter[2])) combo = 1; 
    if ((XAxis > sphereCenter[0]) && (YAxis < sphereCenter[1]) && (ZAxis > sphereCenter[2])) combo = 2; 
    if ((XAxis > sphereCenter[0]) && (YAxis < sphereCenter[1]) && (ZAxis < sphereCenter[2])) combo = 3; 
    if ((XAxis < sphereCenter[0]) && (YAxis > sphereCenter[1]) && (ZAxis > sphereCenter[2])) combo = 4; 
    if ((XAxis < sphereCenter[0]) && (YAxis > sphereCenter[1]) && (ZAxis < sphereCenter[2])) combo = 5;
    if ((XAxis < sphereCenter[0]) && (YAxis < sphereCenter[1]) && (ZAxis > sphereCenter[2])) combo = 6;
    if ((XAxis < sphereCenter[0]) && (YAxis < sphereCenter[1]) && (ZAxis < sphereCenter[2])) combo = 7;

    //Subdivide each quadrant into 4 sectors and determine which sector the point falls within
    if (XAxis > YAxis) {
        if (ZAxis > XAxis) combo_sector = 1;
        else combo_sector = 0;
      } else {
        if (ZAxis > YAxis) combo_sector = 3;
        else combo_sector = 2;
      }

    // Bad data filter - reject out of bounds (not used at this time)
    /*int Hyp =  sqrt(ZAxis*ZAxis + YAxis*YAxis + XAxis*XAxis);
    if ( (Hyp > (caveatron.IMU_hypmax)) || (Hyp < (caveatron.IMU_hypmin)) ) combo =-1;
    if ( (abs(XAxis) > caveatron.IMU_hypUbound) || (abs(YAxis) > caveatron.IMU_hypUbound) || (abs(ZAxis) > caveatron.IMU_hypUbound)) combo =-1;*/
    
    //We want to get 8 points within each quadrant for 64 total points
    
    //Optional approach for getting finer point distribution by subdividing each quandrant into 4 sector
    /*if ((combo>=0)&&(combo_sector>=0)) {
      if (sphereSectorCount[combo][combo_sector]>0) {
        x(calNumCount) = XAxis; y(calNumCount) = YAxis; z(calNumCount) = ZAxis;
        //theFile.print(XAxis);theFile.print("\t");theFile.print(YAxis);theFile.print("\t");theFile.println(ZAxis);
        sphereSectorCount[combo][combo_sector]--;
        calNumCount++;
        ctGUI.updateProgressBar(pbar1, calNumCount);
      }
    }*/
    if (combo>=0) {
      if (sphereSectorCount[combo]>0) {
        x(calNumCount) = XAxis; y(calNumCount) = YAxis; z(calNumCount) = ZAxis;
        //theFile.print(XAxis);theFile.print("\t");theFile.print(YAxis);theFile.print("\t");theFile.println(ZAxis);
        sphereSectorCount[combo]--;
        calNumCount++;
        ctGUI.updateProgressBar(pbar1, calNumCount);
      }
    }
  }

  // Perform the eigenvector computation
  D << x * x,
          y * y,
          z * z,
      2 * (x * y),
      2 * (x * z),
      2 * (y * z),
      2 * x,
      2 * y, 
      2 * z;

  D = D/s;
  VA = ( D.transpose() * D ); 
  VB = ( D.transpose() * VectorXf::Ones(64));
  
  
  V = VA.lu().solve(VB);
  A <<  V(0),V(3),V(4),V(6),
        V(3),V(1),V(5),V(7),
        V(4),V(5),V(2),V(8),
        V(6),V(7),V(8),-s;
        
  CA = -A.topLeftCorner(3,3);
  CB << V(6),V(7),V(8);
  
  C = CA.lu().solve(CB); //Hard Iron Result
  
  T <<  1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        C(0),C(1),C(2),1;     
  
  R = T * A * T.transpose();
  
  E = R.topLeftCorner(3,3)/R(3,3);
  
  EigenSolver<MatrixXf> eigensolver(E);
  MatrixXf EVE = eigensolver.pseudoEigenvectors();
  MatrixXf EVA = eigensolver.pseudoEigenvalueMatrix();
  
  RADII << sqrt(-1/EVA(2,2)), sqrt(-1/EVA(0,0)), sqrt(-1/EVA(1,1));
  EVEC << -EVE.col(2), EVE.col(0), -EVE.col(1);
    
  float r_min = RADII.minCoeff();
  SCALE <<  (1/RADII(0))*r_min,0,0,
            0,(1/RADII(1))*r_min,0,
            0,0,(1/RADII(2))*r_min;
  
  SOFTIRON = EVEC * SCALE * EVEC.transpose(); //Soft Iron Result
  
  for (int i=0; i<3; i++) {
    hardIronCal[i] = C(i);
    for (int j=0; j<3; j++) softIronCal[3*i+j] = SOFTIRON(i, j);
  }
}

// View Current Hard & Soft Iron User Calibration Parameters
void ViewCalParameters() {
  ctGUI.makeObjectInvisible(btn1);
  ctGUI.makeObjectInvisible(btn2);
  ctGUI.makeObjectInvisible(btn3);
  ctGUI.makeObjectInvisible(btn4);
  myGLCD.setBackColor(BLACK_STD);
  myGLCD.setColor(YELLOW_STD);
  ctGUI.print("Hard Iron Calibration", 160, 40, caveatron.FONT_22, 2, CENTER_J);
  ctGUI.print("Soft Iron Calibration", 160, 150, caveatron.FONT_22, 2, CENTER_J);
  ctGUI.print("X=", 80, 70, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.print("Y=", 80, 90, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.print("Z=", 80, 110, caveatron.FONT_19, 2, LEFT_J);
  myGLCD.setColor(WHITE_STD);
  ctGUI.printNumF(hardIronCal[0], 4, 240, 70, caveatron.FONT_19, 2, RIGHT_J);
  ctGUI.printNumF(hardIronCal[1], 4, 240, 90, caveatron.FONT_19, 2, RIGHT_J);
  ctGUI.printNumF(hardIronCal[2], 4, 240, 110, caveatron.FONT_19, 2, RIGHT_J);
  ctGUI.printNumF(softIronCal[0], 6, 5, 180, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumF(softIronCal[1], 6, 160, 180, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.printNumF(softIronCal[2], 6, 315, 180, caveatron.FONT_19, 2, RIGHT_J);
  ctGUI.printNumF(softIronCal[3], 6, 5, 200, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumF(softIronCal[4], 6, 160, 200, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.printNumF(softIronCal[5], 6, 315, 200, caveatron.FONT_19, 2, RIGHT_J);
  ctGUI.printNumF(softIronCal[6], 6, 5, 220, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.printNumF(softIronCal[7], 6, 160, 220, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.printNumF(softIronCal[8], 6, 315, 220, caveatron.FONT_19, 2, RIGHT_J);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//            **** ADVANCED CALIBRATION FUNCTIONS FOR SYSTEM SETUP AND VALIDATION **** //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void AdvancedCalMenuSetup() {
  drawStatusBar("Advanced Calibration");
  drawInfoBar("Data Recording Functions");
  btn1 = ctGUI.addButton(5,30,310,80,BLACK_STD, "1000 Raw Mag & Acc Points", "For Accelometer Calibration",caveatron.FONT_19,optVisible,1);
  btn2 = ctGUI.addButton(5,120,310,80,BLACK_STD,"Continuous Raw Mag & Acc", caveatron.FONT_19,optVisible,2);
  btn3 = ctGUI.addButton(5,210,310,80,BLACK_STD,"Uncorrected Compass Shots", "For Mag Alignment Calibration", caveatron.FONT_19,optVisible,3);
  ctGUI.addButton(5,390,310,64,BLUE_STD,"RETURN TO SETTINGS",caveatron.FONT_28,optVisible,5);
  SetupRawDisplay();
}

void AdvancedCalMenuHandler(int URN) {
    switch (URN) {
      case 1:                            //Record 1000 Raw Magnetometer and Accelerometer Readings
        currentMode = modeCalRec1000;
        CreateScreen(screenFileNameEntry);
        break;
      case 2:                            //Record Continuous Raw Magnetometer and Accelerometer Readings
        currentMode = modeCalRecCont;
        CreateScreen(screenFileNameEntry);
        break;
      case 3:                            //Take Uncorrected & Raw Compass Shots
        currentMode = modeNull;
        CreateScreen(screenCalShots);
        break;
      case 5:                            //Exit
        guiStep--;
        currentMode = modeNull;
        CreateScreen(screenSettingsMenu);
        break;
  }
}

void SetupRawDisplay() {
  myGLCD.setColor(WHITE_STD);
  myGLCD.setBackColor(BLACK_STD);
  ctGUI.print("X", 140, 302, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.print("Y", 200, 302, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.print("Z", 270, 302, caveatron.FONT_19, 2, CENTER_J);
  ctGUI.print("Raw ACC:", 10, 328, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.print("Raw MAG:", 10, 354, caveatron.FONT_19, 2, LEFT_J);
}

void UpdateRawDisplay() {
    caveatron.IMU_Read();  
    myGLCD.setColor(BLACK_STD);
    myGLCD.fillRect(120, 328, 300, 373);
    myGLCD.setBackColor(BLACK_STD);
    myGLCD.setColor(YELLOW_STD);
    ctGUI.printNumI(caveatron.IMU_a_x, 140, 328, caveatron.FONT_19, 2, CENTER_J);
    ctGUI.printNumI(caveatron.IMU_a_y, 200, 328, caveatron.FONT_19, 2, CENTER_J);
    ctGUI.printNumI(caveatron.IMU_a_z, 270, 328, caveatron.FONT_19, 2, CENTER_J);
    ctGUI.printNumI(caveatron.IMU_m_x, 140, 354, caveatron.FONT_19, 2, CENTER_J);
    ctGUI.printNumI(caveatron.IMU_m_y, 200, 354, caveatron.FONT_19, 2, CENTER_J);
    ctGUI.printNumI(caveatron.IMU_m_z, 270, 354, caveatron.FONT_19, 2, CENTER_J);
    delay(125);
}


// Acquire Raw Accelerometer and Magnetometer Readings Functions

// Screen setup and handler
void RawRecordSetup() {
  caveatron.LRF_PowerOn();
  if (currentMode==modeCalRec1000) drawStatusBar("Rec 1000 Raw Reads");
  else drawStatusBar("Rec Cont Raw Reads");
  drawInfoBar("Filename: "+fileName);
  
  char _fileName[sizeof(fileName)];
  fileName.toCharArray(_fileName, sizeof(_fileName));
  if (!theFile.open(_fileName, O_RDWR | O_CREAT | O_AT_END)) {   
    ctGUI.print("File Open Failed", 160, 230, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
    while(true) {}
  }
  GetCurrentTime();
  if (theFile.fileSize()==0) theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
  if (currentMode==modeCalRec1000) theFile.println("1000 RAW ACCELEROMETER & MAGNETOMETER READINGS");
  else theFile.println("CONTINUOUS RAW ACCELEROMETER & MAGNETOMETER READINGS");
  theFile.print("Date: "); theFile.print(dateYear); theFile.print("-"); theFile.print(dateMonth); theFile.print("-"); theFile.print(dateDay);
  theFile.print("     Time: "); theFile.print(timeHour); theFile.print(":"); theFile.print(timeMinute); theFile.print(":"); theFile.println(timeSecond);
  theFile.println();

  btn1 = ctGUI.addButton(11,28,300,265,GREEN_DRK,"START",caveatron.FONT_34,optVisible,1);
  btn2 = ctGUI.addButton(11,28,300,265,RED_DRK,"STOP",caveatron.FONT_34,optInvisible,2);
  btn3 = ctGUI.addButton(11,390,194,64,RED_STD,"Turn Laser Off",caveatron.FONT_28,optVisible,3);
  btn4 = ctGUI.addButton(11,390,194,64,RED_DRK,"Turn Laser On",caveatron.FONT_28,optInvisible,4);
  btn5 = ctGUI.addButton(215,390,94,64,BLUE_STD,"Done",caveatron.FONT_28,optVisible,5);
  SetupRawDisplay();
  delay(250);
  caveatron.LRF_LaserOn();
  laserFlag = true;
}

void RawRecordHandler(int URN) {
    switch (URN) {
      case 1:                            //Start Recording
        ctGUI.makeObjectInvisible(btn1);
        if (laserFlag==true) ctGUI.makeObjectInvisible(btn3);
        else ctGUI.makeObjectInvisible(btn4);
        ctGUI.makeObjectInvisible(btn5);
        BuzzerBEEP(800, 200);
        if (currentMode==modeCalRecCont) ctGUI.makeObjectVisible(btn2);
        AcquireRawAccMag();
        break;
      case 3:                            //Turn Laser Off
        ctGUI.makeObjectInvisible(btn3);
        ctGUI.makeObjectVisible(btn4);
        caveatron.LRF_LaserOff();
        laserFlag = false;
        break;
      case 4:                            //Turn Laser On
        ctGUI.makeObjectInvisible(btn4);
        ctGUI.makeObjectVisible(btn3);
        caveatron.LRF_LaserOn();
        laserFlag = true; 
        break;
      case 5:                            //Done
        caveatron.LRF_LaserOff();
        caveatron.LRF_PowerOff();
        laserFlag=false;
        theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
        theFile.close();
        currentMode = modeRawLive;
        CreateScreen(screenAdvancedCal);
        break;
  }
}

// Data Record Function
void AcquireRawAccMag() {
  boolean done=false;

  //Record Raw Data
  if (currentMode==modeCalRec1000) {
    for (int i=0; i<1000; i++) {
      caveatron.IMU_Read();
      theFile.print(caveatron.IMU_a_x);theFile.print("\t");theFile.print(caveatron.IMU_a_y);theFile.print("\t");theFile.print(caveatron.IMU_a_z);theFile.print("\t");theFile.print(caveatron.IMU_m_x);theFile.print("\t");theFile.print(caveatron.IMU_m_y);theFile.print("\t");theFile.print(caveatron.IMU_m_z);theFile.print("\t");
      theFile.println();  
    }
  }
  else {
    while (done==false) {
      caveatron.IMU_Read();
      theFile.print(caveatron.IMU_a_x);theFile.print("\t");theFile.print(caveatron.IMU_a_y);theFile.print("\t");theFile.print(caveatron.IMU_a_z);theFile.print("\t");theFile.print(caveatron.IMU_m_x);theFile.print("\t");theFile.print(caveatron.IMU_m_y);theFile.print("\t");theFile.print(caveatron.IMU_m_z);theFile.print("\t");
      theFile.println(); 
      // Check for button presses
      if ((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)) {
        btnFound=ctGUI.checkAllButtons(OnRelease);
        if (btnFound==2) {                          //Stop Recording          
            done = true;
            ctGUI.makeObjectInvisible(btn2);
        }
      } 
    }
  }
   
  //End Recording Process
  BuzzerBEEP(800, 1000);
  ctGUI.makeObjectVisible(btn1);
  if (laserFlag==true) ctGUI.makeObjectVisible(btn3);
  else ctGUI.makeObjectVisible(btn4);
  ctGUI.makeObjectVisible(btn5);
  Timer4.start();
}


// Take Uncorrected & Raw Compass Shots Functions

void CalShotsSetup() {
  drawStatusBar("Calibration Shots");
  drawInfoBar("");
  btn1 = ctGUI.addButton(11,393,86,60,GREEN_DRK,"BEGIN",caveatron.FONT_28,optVisible,1);
  ctGUI.addButton(117,393,86,60,BUTTON_STD,"NEW",caveatron.FONT_28,optVisible,2);
  btn3 = ctGUI.addHiddenButton(10, 30, 310, 343, optInvisible, 3);
  ctGUI.addButton(223,393,86,60,BLUE_STD,"QUIT",caveatron.FONT_28,optVisible,5);
  if (!theFile.open("CalShots.txt", O_RDWR | O_CREAT)) {   
    ctGUI.print("File Open Failed", 160, 230, caveatron.FONT_22, 2, CENTER_J, YELLOW_STD, BLACK_STD);
    sd.errorHalt("File opening failed");
    while(true) {}
  }
  if (theFile.fileSize()==0) theFile.timestamp(T_CREATE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
  currentMode = modeCalShots;
  LoadCalShots();
}

void CalShotsHandler(int URN) {
boolean waiting=true;
  String datetimeStr, outStr;
  switch (URN) {
      case 1:             //Begin
        ctGUI.GUIobject_visible[btn1] = optInvisible;
        ctGUI.GUIobject_visible[btn3] = optVisible;
        caveatron.LRF_PowerOn();
        LRFFlag = false;
        delay(250);
        caveatron.LRF_LaserOn();
        laserFlag = true;   
        break;
      case 2:               //New
        theFile.println("*");
        theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
        LoadCalShots();
        break;
      case 3:               //Take Shot
        float singleInclin, singleRoll, singleAzimuth;
        int axSum, aySum, azSum, mxSum, mySum, mzSum, axAvg, ayAvg, azAvg, mxAvg, myAvg, mzAvg;
        axSum=0; aySum=0; azSum=0; mxSum=0; mySum=0; mzSum=0;
        azimuth=0; inclination=0; roll=0; ztilt = 0;
        BuzzerBEEP(800, 200);
        GetCurrentTime();
        for (int i=0; i<300; i++) {
          GetCompassReading();
          singleRoll = measureRoll(singleInclin);
          singleInclin = measureTilt(singleRoll);
          singleAzimuth = measureHeading(singleInclin, singleRoll);
          axSum += caveatron.IMU_a_x; aySum += caveatron.IMU_a_y; azSum += caveatron.IMU_a_z; mxSum += caveatron.IMU_m_x; mySum += caveatron.IMU_m_y; mzSum += caveatron.IMU_m_z;
          roll += singleRoll;
          inclination += -singleInclin; 
          azimuth = SumAzimuth(azimuth, singleAzimuth);
        }
        AverageCompassData(300);
        datetimeStr = "20"+dateYear+"-"+dateMonth+"-"+dateDay+"_"+timeHour+":"+timeMinute+":"+timeSecond;
        axAvg=axSum/300; ayAvg=aySum/300; azAvg=azSum/300; mxAvg=mxSum/300; myAvg=mySum/300; mzAvg=mzSum/300;
        outStr = String(azimuth)+", "+String(inclination)+", "+String(roll)+",#,"+String(axAvg)+","+String(ayAvg)+","+String(azAvg)+","+String(mxAvg)+","+String(myAvg)+","+String(mzAvg)+","+datetimeStr;
        theFile.timestamp(T_WRITE, 2000+surveyYear, surveyMonth, surveyDay, surveyHour, surveyMinute, surveySecond);
        theFile.println(outStr);
        BuzzerBEEP(800, 1000);
        LoadCalShots();
        break;
      case 5:               //Done
        theFile.close();
        laserFlag = false;
        caveatron.LRF_PowerOff();
        currentMode = modeRawLive;
        CreateScreen(screenAdvancedCal);
        break;
  }
}

//Load list of cal shots stored on SD card
void LoadCalShots() {
  int linelength = 1;
  int linecount = 0; int i=0;
  int numlines, endstr;
  char bufin[110];
  String linestring;
  myGLCD.setColor(BLACK_STD);
  myGLCD.fillRect(160, 25, 300, 390);
  myGLCD.setBackColor(BLACK_STD);
  myGLCD.setColor(WHITE_STD);
  theFile.rewind();
  while(linelength > 0) {
    linelength = theFile.fgets(bufin, 109);
    linecount++;
  }
  numlines = linecount;
  Serial.println("Num lines");
  Serial.println(numlines);
  linecount = 0;
  linelength = 1;
  theFile.rewind();
  while (linelength > 0) {
    linelength = theFile.fgets(bufin, 109);
    linecount++;
    if ((numlines-linecount) <= 20) {
      Serial.println("Line count");
      Serial.println(linecount);
      linestring = String(bufin);
      endstr = linestring.indexOf("#");  
      ctGUI.print(linestring.substring(0,endstr-2), 160, 25+(i*18), caveatron.FONT_15, 2);
      i++;
    }
  }
  theFile.seekEnd(0);
}

//Display list of cal shots
void UpdateCalShotsDisplay() {
    ReadCompassData(1);  
    myGLCD.setBackColor(BLACK_STD);
    myGLCD.setColor(BLACK_STD);
    myGLCD.fillRect(20, 50, 70, 178);
    myGLCD.setColor(YELLOW_STD);
    ctGUI.printNumI(azimuth, 20, 50, caveatron.FONT_28, 2);
    ctGUI.printNumI(inclination, 20, 100, caveatron.FONT_28, 2);
    ctGUI.printNumI(roll, 20, 150, caveatron.FONT_28, 2);
    delay(125);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      **** LOAD & SAVE CALIBRATION PARAMETERS FROM EEPROM **** //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Load calibration parameters from EEPROM
void LoadCalibrationParameters() {
  String strArr;
  char cArr[9];
  int addrMagAlignCal, addrMagHSCal, addrLidarOrientCal, addrLidarDistCal;
  float magAlignCal[8];
  
  //Load Screen Calibration
  for (int i=0; i<3; i++) {
    strArr = caveatron.EEPROM_readCharArray(ADDR_SCREEN_CAL+(i*0x20), 10);
    strArr.toCharArray(cArr, 9);
    screenCal[i] = strtoul(cArr, NULL, 16);
    delay(5);
  }
  //Load Accelerometer Calibration
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      accelCal[i+(3*j)] = caveatron.EEPROM_readFloat((ADDR_ACC_CAL+(4*i))+(0x20*j));
      delay(5);
    }
  }
  if (LIDARModule) {
    switch (lidarModuleType) {
      case 1:
        addrMagAlignCal = ADDR_MAG_ALIGNCAL_LIDXV;
        addrMagHSCal = ADDR_MAG_HSCAL_LIDXV;
        addrLidarOrientCal = ADDR_LIDXV_ORIENTCAL;
        addrLidarDistCal = ADDR_LIDXV_DISTCAL;
        break;
      case 2:
        addrMagAlignCal = ADDR_MAG_ALIGNCAL_LIDSW;
        addrMagHSCal = ADDR_MAG_HSCAL_LIDSW;
        addrLidarOrientCal = ADDR_LIDSW_ORIENTCAL;
        addrLidarDistCal = ADDR_LIDSW_DISTCAL;
        break;
    }
  } else {
    addrMagAlignCal = ADDR_MAG_ALIGNCAL_NOLID;
    addrMagHSCal = ADDR_MAG_HSCAL_NOLID;
  }
  //Load Magnetometer Alignment Calibration
  for(int i=0;i<4;i++) {
    magAlignCal[i] = caveatron.EEPROM_readFloat(addrMagAlignCal+(4*i));
    magAlignCal[i+4] = caveatron.EEPROM_readFloat(addrMagAlignCal+0x20+(4*i));
    delay(5);
    mmAmp[i] = (magAlignCal[i] - magAlignCal[i+4]) / 2;
    mmOff[i] = (magAlignCal[i] + magAlignCal[i+4]) / 2;
  }
  //Load Hard & Soft Magnetometer Alignment Calibration
  for(int i=0;i<3;i++) {
    hardIronCal[i] = caveatron.EEPROM_readFloat(addrMagHSCal+(4*i));
    delay(5);
  }
  for(int j=0;j<3;j++) {
    for(int i=0;i<3;i++) {
      softIronCal[i+(3*j)] = caveatron.EEPROM_readFloat(addrMagHSCal+(4*i)+(0x20*(j+1)));
      delay(5);
    }
  }
  //Load LIDAR Rotation Orientation Calibration
  LIDAROrientCal = int(caveatron.EEPROM_readFloat(addrLidarOrientCal));
  delay(5);

  //Load LIDAR Distance Calibration
  LIDARDistCal = int(caveatron.EEPROM_readFloat(addrLidarDistCal));
  delay(5);

  //Load LRF Range Calibration
  LRFRangeCal = int(caveatron.EEPROM_readFloat(ADDR_LRF_RANGECAL));
  delay(5);
}

//Save new Hard and Soft Iron calibration parameters to EEPROM
void SaveCalibration() {
  float calBuffer1[3], calBuffer2[3], calBuffer3[3];
  int addrMagHSCal;
  if (LIDARModule) {
    switch (lidarModuleType) {
      case 1:
        addrMagHSCal = ADDR_MAG_HSCAL_LIDXV;
        break;
      case 2:
        addrMagHSCal = ADDR_MAG_HSCAL_LIDSW;
        break;
    }
  } else addrMagHSCal = ADDR_MAG_HSCAL_NOLID;
  caveatron.EEPROM_writeFloatArray(addrMagHSCal, hardIronCal, 3);
  for(int i=0;i<3;i++) calBuffer1[i] = softIronCal[i];
  for(int i=0;i<3;i++) calBuffer2[i] = softIronCal[3+i];
  for(int i=0;i<3;i++) calBuffer3[i] = softIronCal[6+i];
  caveatron.EEPROM_writeFloatArray(addrMagHSCal+0x20, calBuffer1, 3);
  caveatron.EEPROM_writeFloatArray(addrMagHSCal+0x40, calBuffer2, 3);
  caveatron.EEPROM_writeFloatArray(addrMagHSCal+0x60, calBuffer3, 3);  
}

