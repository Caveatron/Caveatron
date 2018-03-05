/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         Live LIDAR View Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//Setup screen and buttons for Live LIDAR display
void LIDARViewSetup() {
  btn6=ctGUI.addButton(224,421,86,58,BLUE_STD,"DONE",caveatron.FONT_22,optVisible,1);
  btn2=ctGUI.addButton(10,421,86,58,BUTTON_STD,"Zoom +",caveatron.FONT_22,optVisible,2);
  btn3=ctGUI.addButton(117,421,86,58,BUTTON_STD,"Zoom -",caveatron.FONT_22,optVisible,3);
  myGLCD.setColor(WHITE_STD);
  myGLCD.drawLine(0,19,319,19);
  myGLCD.drawLine(0,346,319,346);
  myGLCD.drawLine(0,19,0,346);
  myGLCD.drawLine(319,19,319,346);
  //Draw center position dot
  myGLCD.setColor(0,225,0);
  myGLCD.fillCircle(160,180,3);
  myGLCD.setColor(YELLOW_STD);
  
  lidarErrorFlag = false;
  rot_count = 0;
  ctGUI.print("Box Size (m):",10, 360, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.redrawAllObjects();
  if (lidarModuleType==1) LIDARViewDisplay_XV11();
  else if (lidarModuleType==2) LIDARViewDisplay_SWEEP();
  guiStep--;
  currentMode = modeNull;
  CreateScreen(screenSettingsMenu);
}

//Read and display for XV11 LIDAR
void LIDARViewDisplay_XV11() {
  boolean done = false;
  boolean zoomChange = false;
  boolean overRange = false;
  int scaleValue = 32; //pixels per 1 meter
  uint16_t lidarDistance; 
  int lidarAngle, lidarDataX, lidarDataY, inByte, i, j;
  unsigned char b_speed[2], b_data[4][4], b_checksum[2];
  int init_level = 0;
  int index = 0;

  String pscale = String(320/float(scaleValue),1)+"     ";
  ctGUI.print(pscale, 130, 360, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("4m LIDAR Connected",10, 390, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  InitializeLIDAR();
  delay(2500);
  
  while (done==false) {
  
    // Read LIDAR Data
    if (Serial2.available()) { 
      if (init_level == 0) {
        inByte = Serial2.read();  
        if (inByte == 250) init_level = 1;
        else init_level = 0;
      }
      else if (init_level == 1) {
        inByte = Serial2.read();
        if ((inByte >= 160) && (inByte <= 249)) {
            index = inByte - 160;
            init_level = 2;
        }
        else if (inByte != 250) init_level = 0;
      }
      else if (init_level == 2) {
        unsigned char b_data_sum = 0;
        
        for (i=0; i<2; i++) b_speed[i] = Serial2.read();
        
        for (j=0; j<4; j++) {
          for (i=0; i<4; i++) b_data[j][i] = Serial2.read();
          b_data_sum += b_data[j][i];
        }
    
        for (i=0; i<2; i++) b_checksum[i] = Serial2.read();

        // Parse and plot LIDAR Data
        for (j=0; j<4; j++) {
          lidarAngle = ((index * 4 + j) + LIDAROrientCal) % 360;
                    
          lidarDistance = word(b_data[j][1], b_data[j][0]);

          //if (!((lidarDistance>0) && (lidarDistance<10000))) delay(1);

          //Plot data on screen
          if ((lidarDistance > caveatron.minLIDARrange) && (lidarDistance < caveatron.maxLIDARrange)) {
            lidarDataX = 160 + ((lidarDistance * sin_table[lidarAngle])/10000) * scaleValue/1000;
            lidarDataY = 180 - ((lidarDistance * cos_table[lidarAngle])/10000) * scaleValue/1000;
            if ((lidarDataX > 0) && (lidarDataX < 319) && (lidarDataY > 20) && (lidarDataY < 339)) {
              myGLCD.drawPixel(lidarDataX, lidarDataY);
            }
            else overRange = true;
          } else delay(1);
        }
  
        init_level = 0; // reset and wait for the next packet
      }   
    }
    
    if ((lidarAngle-lastangle)<0) {
      rot_count++;
      
      if(overRange){
        myGLCD.setColor(RED_STD);
        myGLCD.fillCircle(160,180,3);
        myGLCD.setColor(YELLOW_STD);
      }
      
      if (rot_count>3) {
        myGLCD.setColor(BLACK_STD);
        myGLCD.fillRect(1,20,318,345);
        myGLCD.setColor(0,225,0);
        myGLCD.fillCircle(160,180,3);
        myGLCD.setColor(YELLOW_STD);
        if (rot_count>=9) {
          pscale = String(320/float(scaleValue),1)+"     ";
          ctGUI.print(pscale, 130, 360, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
        }
        else zoomChange = false;
        rot_count=0; 
      }
    }
    lastangle = lidarAngle;

    // Check for button presses
    if (((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)) && (zoomChange==false)){
      btnFound=ctGUI.checkAllButtons(OnRelease);
      switch(btnFound) {
        case 1:           //Done
          CloseLIDAR();
          done = true;
          break;
        case 2:           //Zoom In
          zoomChange=true;
          rot_count=9;
          scaleValue=scaleValue*2;
          if (scaleValue>=256) ctGUI.makeObjectInvisible(btn2);
          if (scaleValue>4) ctGUI.makeObjectVisible(btn3);
          break;
        case 3:           //Zoom Out
          zoomChange=true;
          rot_count=9;
          scaleValue=scaleValue/2;
          if (scaleValue<=4) ctGUI.makeObjectInvisible(btn3);
          if (scaleValue<256) ctGUI.makeObjectVisible(btn2);
          break;
      }
    }
       
  }
}


//Read and display for SWEEP LIDAR
void LIDARViewDisplay_SWEEP() {
  boolean done = false;
  boolean zoomChange = false;
  boolean failedLIDAR = false;
  boolean result;
  int scaleValue = 32; //pixels per 1 meter
  int lidarDataX, lidarDataY;
  bool syncValues;         // 1 -> first reading of new scan, 0 otherwise
  int lidarAngle;            // in degrees (accurate to the millidegree)
  uint16_t lidarDistance;      // in cm
  uint8_t lidarSignal; // 0:255, higher is better
  boolean overRange = false;

  String pscale = String(320/float(scaleValue),1)+"     ";
  ctGUI.print(pscale, 130, 360, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  ctGUI.print("25m LIDAR Connected",10, 390, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
  if (InitializeLIDAR()) {
    delay(500);
    if (!device.startScanning()) {
      FailedLIDAR(8);
      failedLIDAR = true;
    }
  } else failedLIDAR = true;

  //Loop to acquire scan reading and check for button presses
  while (done==false) {
    if (failedLIDAR==false) {
      // Read LIDAR Data
      ScanPacket reading = device.getReading(result);
      if (result)
      {
        // check if this reading was the very first reading of a new 360 degree scan
        if (reading.isSync()) {
          rot_count++;
          if(overRange){
            myGLCD.setColor(RED_STD);
            myGLCD.fillCircle(160,180,3);
            myGLCD.setColor(YELLOW_STD);
          }
          
          if (rot_count>3) {
            overRange=false;
            myGLCD.setColor(BLACK_STD);
            myGLCD.fillRect(1,21,318,338);
            myGLCD.setColor(GREEN_DRK);
            myGLCD.fillCircle(160,180,3);
            myGLCD.setColor(YELLOW_STD);
            if (rot_count>=9) {
              pscale = String(320/float(scaleValue),1)+"     ";
              ctGUI.print(pscale, 130, 360, caveatron.FONT_22, 2, LEFT_J, YELLOW_STD, BLACK_STD);
            }
            else zoomChange = false;
            rot_count=0; 
          }
        }
       
        // store the info for this sample
        syncValues = reading.isSync();
        lidarAngle = int(reading.getAngleDegrees()+0.5);
        lidarDistance = reading.getDistanceCentimeters()*10;
        lidarSignal = reading.getSignalStrength();
  
        // Plot LIDAR Data       
        //if ((lidarDistance > caveatron.minLIDARrange) && (lidarDistance < caveatron.maxLIDARrange)) {
        if 
        (lidarDistance > 130) {
          lidarDataX = 160 + ((lidarDistance * sin_table[lidarAngle])/10000) * scaleValue/1000;
          lidarDataY = 180 - ((lidarDistance * cos_table[lidarAngle])/10000) * scaleValue/1000;
          if ((lidarDataX > 0) && (lidarDataX < 319) && (lidarDataY > 20) && (lidarDataY < 339)) {
            myGLCD.drawPixel(lidarDataX, lidarDataY);
          }
          else overRange = true;      
        } else delay(1);
      }   
    }

    // Check for button presses
    if (((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)) && (zoomChange==false)){
      btnFound=ctGUI.checkAllButtons(OnRelease);
      switch(btnFound) {
        case 1:           //Done
          device.stopScanning();
          delay(250);
          CloseLIDAR();
          done = true;
          break;
        case 2:           //Zoom In
          zoomChange=true;
          rot_count=9;
          scaleValue=scaleValue*2;
          if (scaleValue>=256) ctGUI.makeObjectInvisible(btn2);
          if (scaleValue>4) ctGUI.makeObjectVisible(btn3);
          break;
        case 3:           //Zoom Out
          zoomChange=true;
          rot_count=9;
          scaleValue=scaleValue/2;
          if (scaleValue<=4) ctGUI.makeObjectInvisible(btn3);
          if (scaleValue<256) ctGUI.makeObjectVisible(btn2);
          break;
      }
    }
      
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                    PASSAGE AND ROOM MODE LIDAR Scanning Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GUI setup and display functions for Passage and Room modes are in the Modes Tab

// In Passage mode, when button to start scan is pressed, this function initializes the variables, sets up a new traverse in the LIDAR file on the SD card, 
// sets the recording display, and starts the LIDAR hardware.
void InitializeTraverse() {
  uint8_t LRFstatus;
  char traverseToChar[8], traverseNumChar[8];
  char timeHourChar[3], timeMinuteChar[3], timeSecondChar[3];
  lidarErrorFlag = false;
  buzzerFlag = false;
  rot_count = 0;
  pos_count = 0;
  data_count = 0;
  save_count = 0;
  lastangle = 0;
  idx=0;
  Serial1Flush();
  delay(500);
  //currentMode=0;
  caveatron.LRF_Fire();
  ReadCompassData(300);
  LRFstatus = caveatron.LRF_GetRange();
  //distance = caveatron.LRFdistance - caveatron.boxRearDist - caveatron.LIDARFrontDist;
  distance = caveatron.LRFdistance - caveatron.LIDARFrontDist;
  if ((LRFstatus != 1) || (distance <= 0)) FailedLIDAR(LRFstatus);
  else {
    traverseTo.toCharArray(traverseToChar, 8);
    traverseNum.toCharArray(traverseNumChar, 4);
    sta.lastTraverseNum = traverseNum.toInt();
    numTraverses++;
    timeHour.toCharArray(timeHourChar, 3); timeMinute.toCharArray(timeMinuteChar, 3); timeSecond.toCharArray(timeSecondChar, 3);
    lidarFile << endl << "#Station: " << traverseToChar << endl;
    lidarFile << "#Traverse: " << traverseNumChar << endl;
    lidarFile << "#Lidar Type: " << int(lidarModuleType) << endl;
    lidarFile << "#Time: " << timeHourChar << ":" << timeMinuteChar << ":" << timeSecondChar << endl << endl;
    lidarFile << 'S' <<  '\t' << distance << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
    lastLIDARdistance = distance;
    initLIDARdistance = distance + caveatron.LIDARFrontDist + caveatron.boxRearDist;
    lastLIDARazi = azimuth;
    lastLIDARinc = inclination;
    caveatron.LRF_SetMode(1);
    delay(2500);
    BuzzerBEEP(800, 500);
    myGLCD.setColor(BLACK_STD);
    myGLCD.fillRect(40, 372, 280, 428);
    ctGUI.print("RECORDING   . . .", 160, 372, caveatron.FONT_34, 3, CENTER_J, WHITE_STD, BLACK_STD);
    //if (LRFstatus==1) ctGUI.printNumF(distance, 2, 10, 410, caveatron.FONT_22, 6, LEFT_J);
    //else ctGUI.print("------  ", 10, 410, caveatron.FONT_22, 6, LEFT_J);
    myGLCD.setColor(WHITE_STD);
    myGLCD.setBackColor(BLACK_STD);
    lastLRFTime = millis();
    caveatron.LRF_Fire();
    switch (lidarModuleType) {
      case 1:
        RecordLIDAR_XV11();
        break;
      case 2:
        boolean result = device.startScanning();
        if(result==false) FailedLIDAR(8);
        else RecordLIDAR_SWEEP();
        break;
    }
  }
}

// In Room mode, takes the initial splay shot and resets system if failed
boolean TakeSplayShot() {
  uint8_t LRFstatus;
  float dis[2];
  float azi = 0;
  float inc = 0;
  float rol = 0;
  Serial1Flush();
  delay(500);
  
  for (int i = 0; i < 2; i++) {
    caveatron.LRF_Fire();
    ReadCompassData(300);
    LRFstatus = caveatron.LRF_GetRange();
    dis[i] = caveatron.LRFdistance;
    azi += azimuth;
    inc += inclination;
    rol += roll;
    if (LRFstatus != 1) {
      FailedLIDAR(LRFstatus);
      return false;
    }
  }
  distance = (dis[0] + dis[1]) / 2;
  azimuth = azi / 2;
  inclination = inc / 2;
  roll = rol / 2;
  distance = distance - caveatron.boxRearDist - caveatron.LIDARFrontDist;
  
  if ((LRFstatus != 1) || (fabs(dis[1] - dis[0]) > 0.1) || (distance <= 0)) {
    FailedLIDAR(5);
    return false;
  }
  else {
    caveatron.LRF_LaserOff();
    caveatron.LRF_PowerOff();
    return true;
  }
}

// In Room mode, when button to start scan is pressed, this function initializes the variables, sets up a new scan in the LIDAR file on the SD card, 
// sets the recording display, and starts the LIDAR hardware.
void InitializeSplay() {
  char splayStationChar[8], splayNumChar[8];
  char timeHourChar[3], timeMinuteChar[3], timeSecondChar[3];
  lidarErrorFlag = false;
  rot_count = 0;
  data_count = 0;
  save_count = 0;
  idx=0;
  splayStation.toCharArray(splayStationChar, 8);
  splayNum.toCharArray(splayNumChar, 4);
  sta.lastSplayNum = splayNum.toInt();
  numSplays++;
  timeHour.toCharArray(timeHourChar, 3); timeMinute.toCharArray(timeMinuteChar, 3); timeSecond.toCharArray(timeSecondChar, 3);
  lidarFile << endl << "#Station: " << splayStationChar << endl;
  lidarFile << "#Splay: " << splayNumChar << endl;
  lidarFile << "#Lidar Type: " << int(lidarModuleType) << endl;
  lidarFile << "#Time: " << timeHourChar << ":" << timeMinuteChar << ":" << timeSecondChar << endl << endl;
  lidarFile << 'S' <<  '\t' << distance << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
  delay(1000);
  BuzzerBEEP(800, 500);
  myGLCD.setColor(BLACK_STD);
  myGLCD.fillRect(40, 372, 280, 428);
  ctGUI.print("RECORDING   . . .", 160, 372, caveatron.FONT_34, 3, CENTER_J, WHITE_STD, BLACK_STD);
  myGLCD.setColor(WHITE_STD);
  myGLCD.setBackColor(BLACK_STD);
  if (lidarModuleType==1) RecordLIDAR_XV11();
  else if (lidarModuleType==2) {
    boolean result = device.startScanning();
    if(result==false) FailedLIDAR(8);
    else RecordLIDAR_SWEEP();
  }
}


//LIDAR hardware initialization routine (takes several seconds, so is called early from Mode setup functions)
boolean InitializeLIDAR() {
  int tryCount=0;
  boolean result=false;
  digitalWrite(LIDARPowerPin, HIGH);
  switch (lidarModuleType) {
    case 1:
      delay(250);
      Serial2.begin(115200);
      result = true;
      break;
    case 2:
      ctGUI.print("LIDAR Initializing", 160, 170, caveatron.FONT_28, 3, CENTER_J, YELLOW_STD, BLACK_STD);
      ctGUI.print("Please Wait ...", 160, 208, caveatron.FONT_28, 3, CENTER_J, YELLOW_STD, BLACK_STD);
      delay(4000);
      Serial2.begin(115200);
      while ((tryCount<8)&&(result==false)) {
        delay(500);
        result = device.setSampleRate(SAMPLE_RATE_CODE_1000_HZ);
        tryCount++;
      }
      myGLCD.setColor(BLACK_STD);
      myGLCD.fillRect(10,170,310,236);
      if (result==false) FailedLIDAR(6);
      else {
        ctGUI.print("LIDAR Calibrating", 160, 170, caveatron.FONT_28, 3, CENTER_J, YELLOW_STD, BLACK_STD);
        ctGUI.print("Please Wait ...", 160, 208, caveatron.FONT_28, 3, CENTER_J, YELLOW_STD, BLACK_STD);
        delay(3000);
        tryCount=0;
        result=false;
        while ((tryCount<16)&&(result==false)) {
          delay(500);
          result = device.getMotorReady();
          tryCount++;
        }
        myGLCD.setColor(BLACK_STD);
        myGLCD.fillRect(10,170,310,236);
        if (result==false) FailedLIDAR(7);
      }
      break;
  }
  return result;
}

//Recording function for XV LIDAR
//Decodes LIDAR data, gets compass reading once per rotation, checks for new LRF reading, and writes data to SD card as soon as it is received.
//Checks for button presses to end scan
void RecordLIDAR_XV11() {
  boolean done = false;
  int inByte, i, j, numSerialBytes;
  unsigned char b_speed[2], b_data[4][4], b_checksum[2];
  int init_level = 0;
  int index = 0;
  byte loop_count = 0;
  int compass_count = 0;
  int lidarData[360][2];
  int deltaTime=0;
  float deltaAzi;
  LIDAR_LRF_Freq = (caveatron.LRFperiod/210)+1; //The max freq of LIDAR rotation that the LRF can be checked. 210 is msec per rotation.

  distance=0; azimuth = 0; inclination = 0; roll = 0;
  LIDARstartTime = millis();
  while (done==false) {
  
    // Read LIDAR Data
    numSerialBytes = Serial2.available();
    //if (Serial2.available()) { 
    if (numSerialBytes>0) {
      if (init_level == 0) {
        inByte = Serial2.read();  
        if (inByte == 250) init_level = 1;
        else init_level = 0;
      }
      else if (init_level == 1) {
        inByte = Serial2.read();
        if ((inByte >= 160) && (inByte <= 249)) {
            index = inByte - 160;
            init_level = 2;
        }
        else if (inByte != 250) init_level = 0;
      }
      else if (init_level == 2) {
        unsigned char b_data_sum = 0;
        
        for (i=0; i<2; i++) b_speed[i] = Serial2.read();
        
        for (j=0; j<4; j++) {
          for (i=0; i<4; i++) b_data[j][i] = Serial2.read();
          b_data_sum += b_data[j][i];
        }
    
        for (i=0; i<2; i++) b_checksum[i] = Serial2.read();

        // Parse and plot LIDAR Data
        for (j=0; j<4; j++) {
          angle = ((index * 4 + j) + LIDAROrientCal) % 360;
                    
          lidarData[angle][0] = word(b_data[j][1], b_data[j][0]);
          lidarData[angle][1] = (b_data[j][2] | (b_data[j][3] << 8));
          data_count++;
          
          if ((lidarData[angle][0] > caveatron.minLIDARrange) && (lidarData[angle][0] < caveatron.maxLIDARrange)) {
            if ((lidarData[angle][1] > 0) && (lidarData[angle][1] < caveatron.maxLIDARsignal)) {
              lidarFile << angle << "\t" << lidarData[angle][0] << "\t" << lidarData[angle][1] << endl;
              save_count++;
            }
          }
          else delay(1);
        }
        loop_count++;
        if (loop_count > 3) {
          if (currentMode==modeRoom) {
            if (angle >= 175) {
              ReadCompassData(-1);
              compass_count++;
            }
          }
          else {
            ReadCompassData(-1);
            compass_count++;
          }
          loop_count = 0;
        }

        init_level = 0; // reset and wait for the next packet
      }   
    }

    if ((angle-lastangle)<-270) {
 
      AverageCompassData(compass_count);

      //Check if enclosure pointing direction suddenly changes too much
      if (currentMode==modePassage)  {
        if (fabs(lastLIDARinc-inclination) > 20) { //Check for excessive acceleration or tilt
          FailedLIDAR(9);
          return;
        }
        deltaAzi = fabs(lastLIDARazi-azimuth);
        if (deltaAzi > 300) deltaAzi = 360-deltaAzi;
        if (deltaAzi > 20) {                      //Check for excessive azimuth
          FailedLIDAR(10);
          return;
        }
        lastLIDARazi = azimuth; lastLIDARinc = inclination;
      }
      if ((currentMode==modePassage) && ((millis()-lastLRFTime) > caveatron.LRFperiod)) {
        
        if(GetLIDARDistance()==false) return;
        if (distance > 0) {
          lidarFile << 'D' <<  '\t' << distance << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
        }
        else {
          lidarFile << 'R' << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;      
        }
      }
      else {
        lidarFile << 'R' << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
      }
      azimuth = 0; inclination = 0; roll = 0;
      compass_count = 0;
      rot_count++;
    }
    lastangle = angle;

    // Check for button presses
    if ((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)) {
      btnFound=ctGUI.checkAllButtons(OnPress);
      if (btnFound==3) {                            //End Traverse/Scan
          if ((currentMode==modePassage) && ((millis()-lastLRFTime) > 5000)) FailedLIDAR(12);
          else EndLIDARScan();
          done = true;
      }
    }
       
  }
}

//Recording function for SWEEP LIDAR
//Decodes LIDAR data, gets compass reading once per rotation, checks for new LRF reading, and writes data to SD card as soon as it is received.
//Checks for button presses to end scan
void RecordLIDAR_SWEEP() {
  boolean done = false;
  boolean result;
  
  bool syncValues;         // 1 -> first reading of new scan, 0 otherwise
  float lidarAngle=0;        
  // in degrees (accurate to the millidegree)
  uint16_t lidarDistance;      // in cm
  uint8_t lidarSignal; // 0:255, higher is better
  int rrr=0;
  byte loop_count = 0;
  int compass_count = 0;
  int lidarErrorCount = 0;
  int deltaTime=0;
  float deltaAzi;
  int lastCompassTime, numSerialBytes;
  int compassRate = 20; // in msec
  LIDAR_LRF_Freq = (caveatron.LRFperiod/250)+1; //The max freq of LIDAR rotation that the LRF can be checked. 250 is msec per rotation.
  
  distance=0; azimuth = 0; inclination = 0; roll = 0;
  LIDARstartTime = millis();
  while (done==false) {
  
    // Read LIDAR Data
    numSerialBytes = Serial2.available();
    ScanPacket reading = device.getReading(result);
    if (result)
    {
      // check if this reading was the very first reading of a new 360 degree scan
      if (reading.isSync()) {
        AverageCompassData(compass_count);
        //Check if enclosure pointing direction suddenly changes too much
        if (currentMode==modePassage)  {
          if (fabs(lastLIDARinc-inclination) > 20) { //Check for excessive acceleration or tilt
            FailedLIDAR(9);
            return;
          }
          deltaAzi = fabs(lastLIDARazi-azimuth);
          if (deltaAzi > 300) deltaAzi = 360-deltaAzi;
          if (deltaAzi > 20) {                      //Check for excessive azimuth
            FailedLIDAR(10);
            return;
          }
          lastLIDARazi = azimuth; lastLIDARinc = inclination;
        }
        
        if ((currentMode==modePassage) && ((millis()-lastLRFTime) > caveatron.LRFperiod)) {  
          if(GetLIDARDistance()==false) return;
          lidarFile << 'D' <<  '\t' << distance << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
          data_count=0;     
        }
        else {
          lidarFile << 'R' << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
          data_count=0;
        }
        azimuth = 0; inclination = 0; roll = 0;
        compass_count = 0;
        rot_count++;
      }

      // store the info for this sample
      syncValues = reading.isSync();
      lidarAngle = reading.getAngleDegrees();
      lidarDistance = reading.getDistanceCentimeters()*10;
      lidarSignal = reading.getSignalStrength();
      
      //if ((lidarDistance > 100) && (lidarDistance < 25000) && (lidarAngle < 360)) {
      if ((lidarDistance > 130) && (lidarAngle < 360)) {
          // Write LIDAR reading to SD card
          lidarFile << lidarAngle << "\t" << lidarDistance << "\t" << int(lidarSignal) << endl;
          save_count++;
      }

      data_count++;      
      loop_count++;
      //if (loop_count > 3) {
      if ((millis()-lastCompassTime) > compassRate) {
        if (currentMode==modeRoom) {
          if (lidarAngle >= 175) {
            ReadCompassData(-1);
            compass_count++;
          }
        }
        else {
          ReadCompassData(-1);
          compass_count++;
        }
        loop_count = 0;
        lastCompassTime = millis();
      } 
    }
    // Deal with checksum errors
    else {
      if (reading.isSync()) { //Success=false & isSync=true means a checksum error
        Serial2.read(); //Step forward 1 byte to attempt to resync with packet
      }
      if ((millis()-lastCompassTime) > 500) { //If too much time elapses since good reading, fail scan
        FailedLIDAR(13);
        return;
      }
    }

    // Check for button presses
    if ((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)) {
      btnFound=ctGUI.checkAllButtons(OnPress);
      if (btnFound==3) {                          //End Traverse/Scan
          if ((currentMode==modePassage) && ((millis()-lastLRFTime) > 5000)) FailedLIDAR(12);
          else EndLIDARScan();
          done = true;
      }
    }
       
  }
}

//Check if new LRF reading is available once per rotation. Read and determine if distance and speed are within valid range.
//Determine if too much time has passed since last valid reading.
boolean GetLIDARDistance() {
  uint8_t LRFstatus = caveatron.LRF_Read();
  caveatron.LRF_Fire();
  if (LRFstatus==1) {        //Value returned from LRF             
      distance = caveatron.LRFdistance - caveatron.LIDARFrontDist;
      //Check that LRF distance is within range
      if ((distance < 0.04) || (distance > initLIDARdistance)) {  //Invalid distance
        distance = -1;
        if ((millis()-lastLRFTime) > 5000) if ((buzzerFlag==false)&&(pos_count>0)) BuzzerSTART(575);
      }
      else {        

        //Valid distance
        pos_count++;
        //Check if speed since last distance is too fast forward or back
        float velocityCheck = 1000*(lastLIDARdistance - distance)/(millis()-lastLRFTime);
        if ((velocityCheck > 1.00) || (velocityCheck < -0.25)) {  //Velocity since last reading too high (can result from erroneous readings)
          if (buzzerFlag==false) BuzzerSTART(575);
        } else {                                                  //Velocity ok - turn off buzzer, save distance, and reset timer
          if (buzzerFlag==true) BuzzerSTOP();
          lastLIDARdistance = distance;
          lastLRFTime = millis();
        }
      }
      //Serial1Flush();
  } else {                //LRF Error or Timeout
    distance = -1;
    if ((millis()-lastLRFTime) > 5000) {
      if ((buzzerFlag==false)&&(pos_count>0)) BuzzerSTART(575);
    }
  }
  //If >10 sec has elapsed since last valid LRF reading, fail the scan
  if (buzzerFlag==true) { 
    if ((millis()-lastLRFTime) > 10000) {
      FailedLIDAR(11);
      return false;
    }
  }
  return true;
}

//When scan was successful and ended by pressing the stop scan button
//Turn off hardware, close file, and display scan statistics
void EndLIDARScan() {
  lidarFile << flush;
  if (lidarModuleType==2) device.stopScanning();
  delay(250);
  CloseLIDAR();
  caveatron.LRF_LaserOff();
  caveatron.LRF_PowerOff();
  laserFlag = false;
  LRFFlag = false;
  caveatron.LRF_SetMode(0);
  UpdateSettingsFile();
  ctGUI.makeObjectInvisible(btn3);
  myGLCD.setColor(BLACK_STD);
  myGLCD.fillRect(14,72,306,93);
  myGLCD.fillRect(40, 372, 280, 428);

  int save_percent = (save_count*100)/(rot_count*caveatron.ptsLIDARrot);
  if (currentMode==modeRoom) pos_count = rot_count;
  //int pos_percent = (pos_count*100)/((lastLRFTime-LIDARstartTime)/caveatron.LRFperiod);
  int pos_percent = 100*pos_count/(rot_count/LIDAR_LRF_Freq);

  if ((pos_percent==0) || (save_percent==0)) {
    ctGUI.print("FAILED", 160, 73, caveatron.FONT_28, 2, CENTER_J, RED_STD, BLACK_STD);
    BuzzerBEEP(350, 1500);
  }
  else {
    ctGUI.print("COMPLETE", 160, 73, caveatron.FONT_28, 2, CENTER_J, WHITE_STD, BLACK_STD);
    BuzzerBEEP(800, 1000);
  }

  ctGUI.makeObjectVisible(btn4);
  ctGUI.makeObjectVisible(btn5);
  ctGUI.makeObjectVisible(btn6);
  drawInfoBar("");

  if (currentMode == modePassage) {
    ctGUI.print("Valid LRF %: ", 20, 153, caveatron.FONT_22, 2, LEFT_J, WHITE_STD, BLACK_STD);
    if (pos_percent < 30) myGLCD.setColor(RED_STD);
    else if (pos_percent < 60) myGLCD.setColor(YELLOW_STD);
    else myGLCD.setColor(GREEN_STD);
    ctGUI.printNumI(pos_percent, 200, 153, caveatron.FONT_22, 2, LEFT_J);
  }
  ctGUI.print("LIDAR Data %: ", 20, 121, caveatron.FONT_22, 2, LEFT_J, WHITE_STD, BLACK_STD);
  if (save_percent < 15) myGLCD.setColor(RED_STD);
  else if (save_percent < 30) myGLCD.setColor(YELLOW_STD);
  else myGLCD.setColor(GREEN_STD);
  ctGUI.printNumI(save_percent, 200, 121, caveatron.FONT_22, 2, LEFT_J);
  ctGUI.print("Total Scans: ", 20, 185, caveatron.FONT_22, 2, LEFT_J, WHITE_STD, BLACK_STD);
  ctGUI.printNumI(rot_count, 200, 185, caveatron.FONT_22, 2, LEFT_J, WHITE_STD, BLACK_STD);
}

//If LIDAR scan failed, stop hardware, close files, and display error code
void FailedLIDAR(int errorCode) {
  //In Room or Passage Mode, marked scan failed and flush data to SD card 
  if (currentMode != modeLIDARView) {   
    if (errorCode > 8) {
      lidarFile << 'D' <<  '\t' << distance << '\t' << azimuth << '\t' << inclination << '\t' << roll << '\t' << millis() << endl;
    }
    if (errorCode > 5) lidarFile << "#FAILED" << endl;
    lidarFile << flush;
  }
  //Shutdown LIDAR and LRF
  if (lidarModuleType==2) device.stopScanning();
  delay(250);
  CloseLIDAR();
  caveatron.LRF_LaserOff();
  caveatron.LRF_PowerOff();
  laserFlag = false;
  LRFFlag = false;
  caveatron.LRF_SetMode(0);
  //In Room or Passage Mode, update settings file
  if (currentMode != modeLIDARView) UpdateSettingsFile();
  BuzzerBEEP(350, 1500);
  //Configure screen and display failure message
  myGLCD.setColor(BLACK_STD);
  if (currentMode != modeLIDARView) myGLCD.fillRect(40, 372, 280, 428);
  ctGUI.makeObjectInvisible(btn1);
  ctGUI.makeObjectInvisible(btn2);
  ctGUI.makeObjectInvisible(btn3);
  ctGUI.makeObjectVisible(btn4);
  ctGUI.makeObjectVisible(btn5);
  ctGUI.makeObjectVisible(btn6);
  if (currentMode==modePassage) ctGUI.print("TRAVERSE", 160, 100, caveatron.FONT_43, 3, CENTER_J, RED_STD, BLACK_STD);
  else if (currentMode==modeRoom) ctGUI.print("SPLAY", 160, 100, caveatron.FONT_43, 3, CENTER_J, RED_STD, BLACK_STD);
  ctGUI.print("FAILED", 160, 150, caveatron.FONT_43, 3, CENTER_J, RED_STD, BLACK_STD);
  switch (errorCode) {
    case 2: 
      ctGUI.print("LRF Error", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 3: 
      ctGUI.print("LRF Timed Out", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 4: 
      ctGUI.print("LRF Out-of-Range", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 5: 
      ctGUI.print("Mismatched readings", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 6:
      ctGUI.print("LIDAR comms error", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 7:
      ctGUI.print("LIDAR motor init failed", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 8:
      ctGUI.print("LIDAR did not start", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 9:
      ctGUI.print("Excess acceleration/tilt", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 10:
      ctGUI.print("Excess azimuth shift", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 11:
      ctGUI.print("No recent LRF reading", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 12:
      ctGUI.print("No recent LRF reading", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;
    case 13:
      ctGUI.print("LIDAR data error", 160, 200, caveatron.FONT_22, 2, CENTER_J, RED_STD, BLACK_STD);
      break;

  }
}

void CloseLIDAR() {
  Serial2.end();
  digitalWrite(LIDARPowerPin, LOW);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        Sine and Cosine Look-up Tables - speeds calculations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

const int sin_table[360] = {0,175,349,523,698,872,1045,1219,1392,1564,1736,1908,2079,2250,2419,2588,
2756,2924,3090,3256,3420,3584,3746,3907,4067,4226,4384,4540,4695,4848,5000,5150,5299,5446,5592,5736,5878,6018,
6157,6293,6428,6561,6691,6820,6947,7071,7193,7314,7431,7547,7660,7771,7880,7986,8090,8192,8290,8387,8480,8572,
8660,8746,8829,8910,8988,9063,9135,9205,9272,9336,9397,9455,9511,9563,9613,9659,9703,9744,9781,9816,9848,9877,
9903,9925,9945,9962,9976,9986,9994,9998,10000,9998,9994,9986,9976,9962,9945,9925,9903,9877,9848,9816,9781,9744,
9703,9659,9613,9563,9511,9455,9397,9336,9272,9205,9135,9063,8988,8910,8829,8746,8660,8572,8480,8387,8290,8192,
8090,7986,7880,7771,7660,7547,7431,7314,7193,7071,6947,6820,6691,6561,6428,6293,6157,6018,5878,5736,5592,5446,
5299,5150,5000,4848,4695,4540,4384,4226,4067,3907,3746,3584,3420,3256,3090,2924,2756,2588,2419,2250,2079,1908,
1736,1564,1392,1219,1045,872,698,523,349,175,0,-175,-349,-523,-698,-872,-1045,-1219,-1392,-1564,-1736,-1908,
-2079,-2250,-2419,-2588,-2756,-2924,-3090,-3256,-3420,-3584,-3746,-3907,-4067,-4226,-4384,-4540,-4695,-4848,
-5000,-5150,-5299,-5446,-5592,-5736,-5878,-6018,-6157,-6293,-6428,-6561,-6691,-6820,-6947,-7071,-7193,-7314,
-7431,-7547,-7660,-7771,-7880,-7986,-8090,-8192,-8290,-8387,-8480,-8572,-8660,-8746,-8829,-8910,-8988,-9063,
-9135,-9205,-9272,-9336,-9397,-9455,-9511,-9563,-9613,-9659,-9703,-9744,-9781,-9816,-9848,-9877,-9903,-9925,
-9945,-9962,-9976,-9986,-9994,-9998,-10000,-9998,-9994,-9986,-9976,-9962,-9945,-9925,-9903,-9877,-9848,-9816,
-9781,-9744,-9703,-9659,-9613,-9563,-9511,-9455,-9397,-9336,-9272,-9205,-9135,-9063,-8988,-8910,-8829,-8746,
-8660,-8572,-8480,-8387,-8290,-8192,-8090,-7986,-7880,-7771,-7660,-7547,-7431,-7314,-7193,-7071,-6947,-6820,
-6691,-6561,-6428,-6293,-6157,-6018,-5878,-5736,-5592,-5446,-5299,-5150,-5000,-4848,-4695,-4540,-4384,-4226,
-4067,-3907,-3746,-3584,-3420,-3256,-3090,-2924,-2756,-2588,-2419,-2250,-2079,-1908,-1736,-1564,-1392,-1219,
-1045,-872,-698,-523,-349,-175};

const int cos_table[360] = {10000,9998,9994,9986,9976,9962,9945,9925,9903,9877,9848,9816,9781,9744,
9703,9659,9613,9563,9511,9455,9397,9336,9272,9205,9135,9063,8988,8910,8829,8746,8660,8572,8480,8387,8290,8192,
8090,7986,7880,7771,7660,7547,7431,7314,7193,7071,6947,6820,6691,6561,6428,6293,6157,6018,5878,5736,5592,5446,
5299,5150,5000,4848,4695,4540,4384,4226,4067,3907,3746,3584,3420,3256,3090,2924,2756,2588,2419,2250,2079,1908,
1736,1564,1392,1219,1045,872,698,523,349,175,0,-175,-349,-523,-698,-872,-1045,-1219,-1392,-1564,-1736,-1908,
-2079,-2250,-2419,-2588,-2756,-2924,-3090,-3256,-3420,-3584,-3746,-3907,-4067,-4226,-4384,-4540,-4695,-4848,
-5000,-5150,-5299,-5446,-5592,-5736,-5878,-6018,-6157,-6293,-6428,-6561,-6691,-6820,-6947,-7071,-7193,-7314,
-7431,-7547,-7660,-7771,-7880,-7986,-8090,-8192,-8290,-8387,-8480,-8572,-8660,-8746,-8829,-8910,-8988,-9063,
-9135,-9205,-9272,-9336,-9397,-9455,-9511,-9563,-9613,-9659,-9703,-9744,-9781,-9816,-9848,-9877,-9903,-9925,
-9945,-9962,-9976,-9986,-9994,-9998,-10000,-9998,-9994,-9986,-9976,-9962,-9945,-9925,-9903,-9877,-9848,-9816,
-9781,-9744,-9703,-9659,-9613,-9563,-9511,-9455,-9397,-9336,-9272,-9205,-9135,-9063,-8988,-8910,-8829,-8746,
-8660,-8572,-8480,-8387,-8290,-8192,-8090,-7986,-7880,-7771,-7660,-7547,-7431,-7314,-7193,-7071,-6947,-6820,
-6691,-6561,-6428,-6293,-6157,-6018,-5878,-5736,-5592,-5446,-5299,-5150,-5000,-4848,-4695,-4540,-4384,-4226,
-4067,-3907,-3746,-3584,-3420,-3256,-3090,-2924,-2756,-2588,-2419,-2250,-2079,-1908,-1736,-1564,-1392,-1219,
-1045,-872,-698,-523,-349,-175,0,175,349,523,698,872,1045,1219,1392,1564,1736,1908,2079,2250,2419,2588,2756,
2924,3090,3256,3420,3584,3746,3907,4067,4226,4384,4540,4695,4848,5000,5150,5299,5446,5592,5736,5878,6018,6157,
6293,6428,6561,6691,6820,6947,7071,7193,7314,7431,7547,7660,7771,7880,7986,8090,8192,8290,8387,8480,8572,8660,
8746,8829,8910,8988,9063,9135,9205,9272,9336,9397,9455,9511,9563,9613,9659,9703,9744,9781,9816,9848,9877,9903,
9925,9945,9962,9976,9986,9994,9998};


