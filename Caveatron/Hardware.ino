/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           LSM303 Compass Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Read and average the specified number of calibrated azimuth, inclination and roll measurements
void ReadCompassData (int numPoints) { 
  int i;
  int p = 1;
  if (numPoints > 0) {azimuth = 0; inclination = 0; roll = 0; ztilt = 0; p = numPoints;}
  for (int j=0;j<6;j++) rawIMU[j]=0;
  float singleInclin, singleRoll, singleAzimuth;
  for (i=0; i<p; i++) {
    GetCompassReading();
    singleRoll = measureRoll(singleInclin);
    singleInclin = measureTilt(singleRoll);
    singleAzimuth = measureHeading(singleInclin, singleRoll);
    roll = SumRoll(roll, singleRoll);
    inclination += -singleInclin; 
    azimuth = SumAzimuth(azimuth, singleAzimuth);
    rawIMU[0] += caveatron.IMU_a_x; rawIMU[1] += caveatron.IMU_a_y; rawIMU[2] += caveatron.IMU_a_z;
    rawIMU[3] += caveatron.IMU_m_x; rawIMU[4] += caveatron.IMU_m_y; rawIMU[5] += caveatron.IMU_m_z;
  }
  if (numPoints > 0) AverageCompassData(numPoints);
}

// Perform compass reading averaging and convert back to degrees
void AverageCompassData (int numPoints) {
  inclination = (inclination/numPoints) * 57.29578;
  roll = (roll/numPoints) * 57.29578;
  azimuth = AverageAzimuth(azimuth, numPoints);
  for (int j=0;j<6;j++) rawIMU[j] /= numPoints;
}
  
// Obtain a single reading from the compass and compute the calibrated accelerometer values for each axis
void GetCompassReading() {
  //Calibration Parameters
  float acc10 = accelCal[0];
  float acc20 = accelCal[1];
  float acc30 = accelCal[2];
  float acc11 = accelCal[3];
  float acc12 = accelCal[4];
  float acc13 = accelCal[5];
  float acc21 = accelCal[6];
  float acc22 = accelCal[7];
  float acc23 = accelCal[8];
  float acc31 = accelCal[9];
  float acc32 = accelCal[10];
  float acc33 = accelCal[11];

  //Take a single compass reading
  caveatron.IMU_Read();
  
  //Correct accelerometer for scaling, offset, and misalignment
  ax = acc11*caveatron.IMU_a_x + acc12*caveatron.IMU_a_y + acc13*caveatron.IMU_a_z + acc10;
  ay = acc21*caveatron.IMU_a_x + acc22*caveatron.IMU_a_y + acc23*caveatron.IMU_a_z + acc20;
  az = acc31*caveatron.IMU_a_x + acc32*caveatron.IMU_a_y + acc33*caveatron.IMU_a_z + acc30;
  rtilt = sqrt(pow(ax,2) + pow(ay,2) + pow(az,2));
}

// Compute the inclination angle value
float measureTilt(float aroll) {
  float xtilt = atan2(-ax/rtilt,(ay* sin(aroll) + az*cos(aroll))/rtilt);
  return xtilt;
} 

// Compute the roll angle value
float measureRoll(float apitch) {
  float yrot = atan2(ay/rtilt,az/rtilt);
  return yrot;
}

// Compute the calibrated magnetometer readings for each axis and tilt-compensated azimuth angle value
float measureHeading(float mpitch, float mroll) {

  //Compute the misalignment calibration parameters
  float mm1 = mmOff[0] + mmAmp[0]*cos(mroll);
  float mm2 = mmOff[1] + mmAmp[1]*cos(mroll);
  float mm3 = mmOff[2] + mmAmp[2]*cos(mroll);
  float mrot = mmOff[3] + mmAmp[3]*cos(mroll);

  //Calibration Parameters for hard and soft iron
  float mcc11 = softIronCal[0];
  float mcc12 = softIronCal[1];
  float mcc13 = softIronCal[2];
  float mcc21 = softIronCal[3];
  float mcc22 = softIronCal[4];
  float mcc23 = softIronCal[5];
  float mcc31 = softIronCal[6];
  float mcc32 = softIronCal[7];
  float mcc33 = softIronCal[8];
  float mb1 = hardIronCal[0];
  float mb2 = hardIronCal[1];
  float mb3 = hardIronCal[2]; 

  //Correct mag values for hard and soft iron effects and misalignment
  float mx = mcc11*(caveatron.IMU_m_x-mb1) + mcc12*(caveatron.IMU_m_y-mb2) + mcc13*(caveatron.IMU_m_z-mb3) + mm1;
  float my = mcc21*(caveatron.IMU_m_x-mb1) + mcc22*(caveatron.IMU_m_y-mb2) + mcc23*(caveatron.IMU_m_z-mb3) + mm2;
  float mz = mcc31*(caveatron.IMU_m_x-mb1) + mcc32*(caveatron.IMU_m_y-mb2) + mcc33*(caveatron.IMU_m_z-mb3) + mm3;

  //Perform tilt compensation correction
  float mhx = mx*cos(mpitch) + my * sin(mpitch)*sin(mroll) + mz*sin(mpitch)*cos(mroll);
  float mhy = my*cos(mroll) - mz*sin(mroll);

  float rawHeading = atan2(mhy, mhx);

  //Correct for roll angle
  float correctedHeading = rawHeading + ((mrot/57.3)*cos(mroll));
  
  return (correctedHeading);
}   

//Function for summing multiple azimuth readings (for averaging) to maintain result within 0-360 deg
float SumAzimuth(float sumHeading, float newHeading) {
  if ((newHeading > 1.570796) || (newHeading < -1.570796)) {
    if ((sumHeading*newHeading)<0) {
      if (sumHeading<0) {
        sumHeading += (newHeading - 6.283185);
      } else {
        sumHeading += (newHeading + 6.283185);
      }
    } else {
      sumHeading += newHeading;
    }
  } else {
    sumHeading += newHeading;
  }
  return sumHeading;
}
  
//Function for averaging azimuth readings and maintaining result within 0-360 deg
float AverageAzimuth(float sumHeading, float numPoint) {
  float trueHeading = (sumHeading/numPoint) * 57.29578;
  if (trueHeading>=360) trueHeading -= 360;
  if (trueHeading<0) trueHeading += 360;
  return trueHeading;
}

//Function for summing multiple roll angle readings (for averaging) to avoid problems when upside down (+180/-180 wraparound)
float SumRoll(float sumRoll, float newRoll) {
  //Check if new roll angle has the opposite sign from the current roll sum
  if ((sumRoll > 0) && (newRoll < -1.570796)) newRoll += 6.283185;
  else if ((sumRoll < 0) && (newRoll > 1.570796)) newRoll -= 6.283185;
  sumRoll += newRoll; 
  return sumRoll;
}

//Correct azimuth and inclination for the selected measurement point at the rear bottom corner of the enclosure
//Angle correction is based on enclosure dimensions and distance to target
void ComputeBoxCorrection() {
  if (rightFlag==true) azimuth = azimuth - (atan((caveatron.boxRightDist*cos(roll/57.29578))/distance)*57.29578);
  else azimuth = azimuth + (atan((caveatron.boxLeftDist*cos(roll/57.29578))/distance)*57.29578);
  if (azimuth>=360) azimuth -= 360;
  else if (azimuth<0) azimuth += 360;
  
  float adjustInc = inclination + (atan((caveatron.boxBaseDist*cos(roll/57.29578))/distance)*57.29578);
  if (adjustInc > 90) {
    inclination = 180 - adjustInc;
    azimuth -= 180;
    if (azimuth < 0) azimuth += 360;
  }
  else if (adjustInc < -90) {
    inclination = 180 + adjustInc;
    azimuth -= 180;
    if (azimuth < 0) azimuth += 360;
  }
  else inclination = adjustInc;
}

//Error check for frozen IMU - compare current reading with first reading
uint8_t IMUErrorCheck(boolean startFlag) {
  uint8_t IMUstatus=0;
  int16_t lastIMU[6];
  caveatron.IMU_Read();
  if (startFlag==true) {
    firstIMU[0]=caveatron.IMU_a_x; firstIMU[1]=caveatron.IMU_a_y; firstIMU[2]=caveatron.IMU_a_z;
    firstIMU[3]=caveatron.IMU_m_x; firstIMU[4]=caveatron.IMU_m_y; firstIMU[5]=caveatron.IMU_m_z;
  } else {
    lastIMU[0]=caveatron.IMU_a_x; lastIMU[1]=caveatron.IMU_a_y; lastIMU[2]=caveatron.IMU_a_z;
    lastIMU[3]=caveatron.IMU_m_x; lastIMU[4]=caveatron.IMU_m_y; lastIMU[5]=caveatron.IMU_m_z;
    if ((lastIMU[0]==firstIMU[0])&&(lastIMU[1]==firstIMU[1])&&(lastIMU[2]==firstIMU[2])) {
      ErrorBox("Accelerometer", "may be frozen", "", 98); 
      IMUstatus = 1;
    }
    if ((lastIMU[3]==firstIMU[3])&&(lastIMU[4]==firstIMU[4])&&(lastIMU[5]==firstIMU[5])) {
      ErrorBox("Magnetometer", "may be frozen", "", 98); 
      if (IMUstatus==1) IMUstatus = 3;
      else IMUstatus = 2;
    }
  }
  return IMUstatus;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Laser Rangefinder Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Reset LRF in the event of anomalous values (not currently used)
uint8_t ResetLRF() {
  caveatron.LRF_PowerOff();
  delay(1000);
  if (currentMode==modeManual) ManualModeHandler(3);
  caveatron.LRF_PowerOn();
  delay(250);
  laserFlag = false;
  return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Piezo Buzzer Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Sound buzzer for specified frequency and duration
void BuzzerBEEP(int freq, int dur){   
  Timer3.setFrequency(freq*2);
  Timer3.attachInterrupt(buzzerHandler);
  Timer3.start();
  delay(dur);
  Timer3.stop();
  Timer3.detachInterrupt();
}

// Start buzzer at specified frequency
void BuzzerSTART(int freq){   
  Timer3.setFrequency(freq*2);
  Timer3.attachInterrupt(buzzerHandler);
  buzzerFlag = true;
  Timer3.start();
}

//Stop buzzer
void BuzzerSTOP(){   
  Timer3.stop();
  Timer3.detachInterrupt();
  buzzerFlag = false;
}

//Handles buzzer operation
void buzzerHandler(){
  buzOn = !buzOn;
  digitalWrite(caveatron.BUZZpin, buzOn);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           DS3231 Time and Temperature Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Get time from RTC
void GetCurrentTime() {
  caveatron.RTC_GetDateTime();
  FormatTimeDate(caveatron.RTChour, caveatron.RTCminute, caveatron.RTCsecond, caveatron.RTCyear, caveatron.RTCmonth, caveatron.RTCday);
}

// Format time for display or writing to SD card
void FormatTimeDate(int valHour, int valMinute, int valSecond, int valYear, int valMonth, int valDay) {
  timeSecond = String(valSecond);
  if (valSecond < 10) timeSecond = "0"+timeSecond;
  timeMinute = String(valMinute);
  if (valMinute < 10) timeMinute = "0"+timeMinute;
  timeHour = String(valHour);
  if (valHour < 10) timeHour = "0"+timeHour;
  dateDay = String(valDay);
  if (valDay < 10) dateDay = "0"+dateDay;
  dateMonth = String(valMonth);
  if (valMonth < 10) dateMonth = "0"+dateMonth;
  dateYear = String(valYear);
  if (valYear < 10) dateYear = "0"+dateYear;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Serial Port Buffer Clearing Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void Serial1Flush(){
  while(Serial1.available() > 0) Serial1.read();
} 

void Serial2Flush(){
  while(Serial2.available() > 0) Serial2.read();
} 

