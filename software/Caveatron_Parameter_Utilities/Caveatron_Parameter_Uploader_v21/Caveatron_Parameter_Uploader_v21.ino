/*****************************************************************/
//             Caveatron Parameter Uploader                      //
//                       Version 2.1                             //
/*****************************************************************/
// Joe Mitchell, 2020-12-02
// Used to Upload Calibration Parameters to the Caveatron EEPROM
// Fill in your calibration parameters in the "Calibration_Parameters" tab

#include <UTFT.h>
#include <EEPROM.h>

UTFT myGLCD(CTE35IPS,23,22,3,4);

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

// Switches for Parameter Upload
// Set to 1 to load, 0 to ignore
//*******************************

boolean loadSerialNum = 0;          // Unit Serial Number, 5 char
boolean loadHardwareCode = 0;       // Unit Hardware Code, 12 char
boolean loadScreenCal = 0;          // LCD Display Calibration, 3x1 array of 10 char

boolean loadAccelCal = 0;           // Accelerometer Offsets and Alignment, 3x4 float array

boolean loadMagAlignCal = 0;   // Magnetometer Alignment, 8x1 float array
boolean loadMagHSCal = 0;      // **Loaded through internal calibration - Magnetometer Hard and Soft Iron, 3x4 float array

boolean loadGyroCal = 0;      //Gyroscope Zero Rate Offset, 3x1 float array

boolean loadLidarWindowCorners = 0;      //Gyroscope Zero Rate Offset, 3x1 float array
boolean loadLidarOrientCal = 0;   // LIDAR Rotation Orientation, 1 float
boolean loadLidarWindowCorrection = 0;   // LIDAR Rotation Orientation, 1 float
boolean loadLRFRangeCal = 0;        // LRF Distance, 1 float

boolean initalizePrefs = 0;   //Set preference values to "1"
boolean loadFileName = 0;   //Used to set current file name

//*******************************

// EEPROM Calibration Parameter Addresses
// 1 page is 0x020 in length
#define ADDR_SERIAL_NUM     0x00        //Uses 1 page
#define ADDR_HARDWARE_CODE    0x020     //Uses 1 page
#define ADDR_SCREEN_CAL     0x080       //Uses 3 pages
#define ADDR_ACC_CAL      0x200         //Uses 4 pages
#define ADDR_MAG_ALIGNCAL 0x300   //Uses 2 pages
#define ADDR_MAG_HSCAL  0x400     //Uses 4 pages
#define ADDR_GYR_CAL  0x600     //Uses 4 pages
#define ADDR_LID_CORNERS 0x700    //Users 4 pages
#define ADDR_LID_ORIENTCAL   0x800     //Uses 1 page
#define ADDR_LRF_RANGECAL   0x860     //Uses 1 page
#define ADDR_LID_WINCORRECT   0xA00   //Uses 722 bytes

// EEPROM Preferences Addresses
#define ADDR_LIDARSPEED_PREF  0x900
#define ADDR_LIDARRATE_PREF 0x901
#define ADDR_LRFRATE_PREF 0x905
#define ADDR_SHUTDOWN_PREF  0x910

#define ADDR_FILENAME   0xF00     //Uses 34 bytes

extern char serialNumber[];
extern char hardwareCode[];
extern char screenCal[3][11];
extern float accCal[4][3];
extern float magAlignCal[8];
extern float magCal[4][3];
extern float gyrCal[3];
extern float lidarCorners[4][2];
extern float lidarOrientCal[1];
extern float LRFRangeCal[1];
extern int8_t lidarWindowCorrectSettings[2];
extern int8_t lidarWindowCorrection[720];
extern char fileName[];


void setup() {
  //Init Serial
  Serial.begin(250000);

  //Init GUI
  //myGLCD.invert_colors = true;
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  delay(500);

  int y = 20;
  
  if (loadSerialNum) {
    UploadSerialNumber();
    delay(100);
    if (CheckSerialNumber()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Serial Num Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Serial Num Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadHardwareCode) {
    UploadHardwareCode();
    delay(100);
    if (CheckHardwareCode()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Hardware Code Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Hardware Code Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadScreenCal) {
    UploadScreenCal();
    delay(100);
    if (CheckScreenCal()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Screen Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Screen Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadAccelCal) {
    UploadAccelerometerCal();
    delay(100);
    if (CheckAccelerometerCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Accel Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Accel Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagAlignCal) {
    UploadMagnetometerAlignCal();
    delay(100);
    if (CheckMagnetometerAlignCal()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Mag Al Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Mag Al Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagHSCal) {
    UploadMagnetometerHSCal();
    delay(100);
    if (CheckMagnetometerHSCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagHS Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagHS Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadGyroCal) {
    UploadGyroCal();
    delay(100);
    if (CheckGyroCal()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Gyro Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Gyro Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLidarWindowCorners) {
    UploadLidarCornersCal();
    delay(100);
    if (CheckLidarCornersCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Lidar Corners Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Lidar Corners Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLidarOrientCal) {
    UploadLidarOrientCal();
    delay(100);
    if (CheckLidarOrientCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("LIDAR Orient Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("LIDAR Orient Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLRFRangeCal) {
    UploadLRFRangeCal();
    delay(100);
    if (CheckLRFRangeCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("LRF Range Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("LRF Range Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLidarWindowCorrection) {
    UploadLidarWindowCorrection();
    delay(100);
    if (CheckLidarWindowCorrection()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Window Corr Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Window Corr Failed", CENTER, y);
    }
    y=y+20;
  }
  if (initalizePrefs) {
    InitializePrefs();
    delay(100);
    if (CheckPrefs()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Preferences Inited", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Pref Init Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadFileName) {
    UploadFileName();
    delay(100);
    if (CheckFileName()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("File Name Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("File Name Failed", CENTER, y);
    }
    y=y+20;
  }
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("DONE", CENTER, y);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void UploadSerialNumber() {
  EEPROM_writeCharArray(ADDR_SERIAL_NUM, serialNumber, 5);
}

void UploadHardwareCode() {
  EEPROM_writeCharArray(ADDR_HARDWARE_CODE, hardwareCode, 12);
}

void UploadScreenCal() {
  for(int i=0; i<3; i++) EEPROM_writeCharArray(ADDR_SCREEN_CAL+(i*0x20), screenCal[i], 10);
}

void UploadAccelerometerCal() {
  float arrayBuffer[3];
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) arrayBuffer[j] = accCal[i][j];
    EEPROM_writeFloatArray(ADDR_ACC_CAL+(i*0x20), arrayBuffer, 3);
  }
}

void UploadMagnetometerAlignCal() {
  float arrayBuffer[4];
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCal[i];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL, arrayBuffer, 4);
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCal[i+4];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL+0x20, arrayBuffer, 4);
}

void UploadMagnetometerHSCal() {
  float arrayBuffer[3];
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) arrayBuffer[j] = magCal[i][j];
    EEPROM_writeFloatArray(ADDR_MAG_HSCAL+(i*0x20), arrayBuffer, 3);
  }
}

void UploadGyroCal() {
  float arrayBuffer[3];
  for(int i=0;i<3;i++) arrayBuffer[i] = gyrCal[i];
  EEPROM_writeFloatArray(ADDR_GYR_CAL, arrayBuffer, 3);
}

void UploadLidarCornersCal() {
  float arrayBuffer[2];
  for(int i=0;i<4;i++) {
    for(int j=0;j<2;j++) arrayBuffer[j] = lidarCorners[i][j];
    EEPROM_writeFloatArray(ADDR_LID_CORNERS+(i*0x20), arrayBuffer, 2);
  }
}
 
void UploadLidarOrientCal() {
  EEPROM_writeFloatArray(ADDR_LID_ORIENTCAL, lidarOrientCal, 1);
}

void UploadLRFRangeCal() {
  EEPROM_writeFloatArray(ADDR_LRF_RANGECAL, LRFRangeCal, 1);
}

void UploadLidarWindowCorrection() {
  Write_EEPROM_Bytes(ADDR_LID_WINCORRECT, lidarWindowCorrectSettings, 2);
  Write_EEPROM_Bytes(ADDR_LID_WINCORRECT+2, lidarWindowCorrection, 720);
  //Write_EEPROM_Bytes(ADDR_LID_WINCORRECT+0x100+0x2, lidarWindowCorrection2, 256);
  //Write_EEPROM_Bytes(ADDR_LID_WINCORRECT+0x200+0x2, lidarWindowCorrection3, 208);
}

void InitializePrefs() {
  byte b[1] = {1};
  Write_EEPROM_Bytes(ADDR_LIDARSPEED_PREF, b, 1);
  Write_EEPROM_Bytes(ADDR_LIDARRATE_PREF, b, 1);
  Write_EEPROM_Bytes(ADDR_LRFRATE_PREF, b, 1);
  Write_EEPROM_Bytes(ADDR_SHUTDOWN_PREF, b, 1);
}

void UploadFileName() {
  EEPROM_writeCharArray(ADDR_FILENAME, fileName, 33);
}



boolean CheckSerialNumber() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_SERIAL_NUM, 5);
  if (arr!=serialNumber) return 0;
  return 1;
}

boolean CheckHardwareCode() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_HARDWARE_CODE, 12);
  if (arr!=hardwareCode) return 0;
  return 1;
}

boolean CheckScreenCal() {
  String arr[3];
  char checkArr[10];
  for (int i=0; i<3; i++) {
    arr[i] = EEPROM_readCharArray(ADDR_SCREEN_CAL+(i*0x20), 10);
    arr[i].toCharArray(checkArr, 10);
    if (!strcmp(checkArr, screenCal[i])) return 0;
  }
  return 1;
}

boolean CheckAccelerometerCal() {
  float arr[12];
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr[i+(3*j)] = EEPROM_readFloat((ADDR_ACC_CAL+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      if (arr[i+(3*j)]!=accCal[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckMagnetometerAlignCal() {
  float arr[8];
  for(int i=0;i<4;i++) {
    arr[i] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL+(4*i));
    arr[i+4] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL+0x20+(4*i));
    delay(50);
  }
  for(int i=0;i<8;i++) if (arr[i]!=magAlignCal[i]) return 0;
  return 1;
}

boolean CheckMagnetometerHSCal() {
  float arr[12];
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr[i+(3*j)] = EEPROM_readFloat((ADDR_MAG_HSCAL+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      if (arr[i+(3*j)]!=magCal[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckGyroCal() {
  float arr[3];
  for(int i=0;i<3;i++) {
    arr[i] = EEPROM_readFloat(ADDR_GYR_CAL+(4*i));
    delay(50);
  }
  for(int i=0;i<3;i++) if (arr[i]!=gyrCal[i]) return 0;
  return 1;
}

boolean CheckLidarCornersCal() {
  float arr[8];
  for(int j=0;j<4;j++) {
    for(int i=0;i<2;i++) {
      arr[i+(2*j)] = EEPROM_readFloat((ADDR_LID_CORNERS+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<2;i++) {
      if (arr[i+(2*j)]!=lidarCorners[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckLidarOrientCal() {
  float arr[1];
  arr[0] = EEPROM_readFloat(ADDR_LID_ORIENTCAL);
  delay(50);
  if (arr[0]!=lidarOrientCal[0]) return 0;
  return 1;
}

boolean CheckLRFRangeCal() {
  float arr[1];
  arr[0] = EEPROM_readFloat(ADDR_LRF_RANGECAL);
  delay(50);
  if (arr[0]!=LRFRangeCal[0]) return 0;
  return 1;
}

boolean CheckLidarWindowCorrection() {
  int8_t sByte;
  for (int i=0; i<2; i++) {
    sByte = EEPROM.read(ADDR_LID_WINCORRECT+i);
    if (sByte!=lidarWindowCorrectSettings[i]) return 0;
    delay(2);
  }
  for (int i=0; i<720; i++) {
    sByte = EEPROM.read(ADDR_LID_WINCORRECT+2+i);
    if (sByte!=lidarWindowCorrection[i]) return 0;
    delay(2);
  }
  return 1;
}

boolean CheckPrefs() {
  int8_t aByte, bByte, cByte, dByte;
  aByte = EEPROM.read(ADDR_LIDARSPEED_PREF);
  delay(2);
  bByte = EEPROM.read(ADDR_LIDARRATE_PREF);
  delay(2);
  cByte = EEPROM.read(ADDR_LRFRATE_PREF);
  delay(2);
  dByte = EEPROM.read(ADDR_SHUTDOWN_PREF);
  if ((aByte!=1) || (bByte!=1) || (cByte!=1) || (dByte!=1)) return 0; 
  else return 1;
}

boolean CheckFileName() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_FILENAME, 33);
  if (arr!=fileName) return 0;
  return 1;
}


void EEPROM_writeFloatArray(uint16_t address, float values[], int numvalues) {
  byte aBytes[30];
  union u_tag {
      byte b[4];
      float fval;
  } u;
  for (int i=0;i<numvalues;i++) {
    u.fval = values[i];
    for(int j=0;j<4;j++) aBytes[j+i*4] = u.b[j];
  }
  Write_EEPROM_Bytes(address, aBytes, numvalues*4);
}

void EEPROM_writeCharArray(uint16_t address, char values[], int numvalues)
{
  byte aBytes[30];
  for (int i=0;i<numvalues;i++) aBytes[i] = values[i];
  Write_EEPROM_Bytes(address, aBytes, numvalues);
}

float EEPROM_readFloat(uint16_t address)
{
  float value;
  union u_tag {
      byte b[4];
      float fval;
  } u;
  for(int i=0;i<4;i++) u.b[i] = Read_EEPROM_Byte(address+i);
  value = u.fval;
  return value;
}

String EEPROM_readCharArray(uint16_t address, int numvalues)
{
  char b[12];
  for(int i=0;i<numvalues;i++) b[i] = Read_EEPROM_Byte(address+i);
  b[numvalues+1] = '\0';
  String value(b);
  return value;
}

// Data page write to EEPROM
void Write_EEPROM_Bytes(int iAddr, byte* iData, int iLength)
{
  for (int c = 0; c < iLength; c++) {
    EEPROM.write(iAddr + c, iData[c]);
    delay(10);
  }
}

// Data page read from EEPROM
int Read_EEPROM_Byte(int iAddr) {
  uint8_t rdata = EEPROM.read(iAddr);
  return rdata;
}
