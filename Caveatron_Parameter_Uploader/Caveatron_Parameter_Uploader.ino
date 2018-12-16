/*****************************************************************/
//              Caveatron Parameter Uploader                     //
//                       Version 1.2                             //
/*****************************************************************/
// Joe Mitchell, 2018-11-28
// Used to Upload Calibration Parameters to the Caveatron EEPROM
// Fill in your calibration parameters in the "Calibration_Parameters" tab

#include <Wire.h>
#include <UTFT.h>

UTFT myGLCD(CTE35IPS,25,26,27,28);

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

// Variables for AT24C32 EEPROM
#define DS3231_ADDRESS 0x68
#define AT24C32_EEPROM_ADDR 0x57

// Switches for Parameter Upload
// Set to 1 to load, 0 to ignore
//*******************************

boolean loadSerialNum = 0;          // Unit Serial Number, 5 char
boolean loadHardwareCode = 0;       // Unit Hardware Code, 11 char
boolean loadScreenCal = 0;          // LCD Display Calibration, 3x1 array of 10 char

boolean loadAccelCal = 0;           // Accelerometer Offsets and Alignment, 3x4 float array

boolean loadMagAlignCalNoLid = 0;   // Magnetometer Alignment, 8x1 float array, No LIDAR
boolean loadMagAlignCalLidarXV = 1; // Magnetometer Alignment, 8x1 float array, XV11 LIDAR
boolean loadMagAlignCalLidarSW = 1; // Magnetometer Alignment, 8x1 float array, SWEEP LIDAR
boolean loadMagAlignCalLidarRP = 1; // Magnetometer Alignment, 8x1 float array, RP LIDAR

boolean loadMagHSCalNoLid = 1;      // **Loaded through internal calibration - Magnetometer Hard and Soft Iron, 3x4 float array, No LIDAR
boolean loadMagHSCalLidarXV = 1;    // **Loaded through internal calibration - Magnetometer Hard and Soft Iron, 3x4 float array, XV11 LIDAR
boolean loadMagHSCalLidarSW = 1;    // **Loaded through internal calibration - Magnetometer Hard and Soft Iron, 3x4 float array, SWEEP LIDAR
boolean loadMagHSCalLidarRP = 1;    // **Loaded through internal calibration - Magnetometer Hard and Soft Iron, 3x4 float array, RP LIDAR

boolean loadLidarXVOrientCal = 0;   // LIDAR Rotation Orientation, 1 float, XV11 LIDAR
boolean loadLidarSWOrientCal = 0;   // LIDAR Rotation Orientation, 1 float, SWEEP LIDAR
boolean loadLidarRPOrientCal = 0;   // LIDAR Rotation Orientation, 1 float, RP LIDAR

boolean loadLRFRangeCal = 0;        // LRF Distance, 1 float

//*******************************

// EEPROM Addresses
// 1 page is 0x020 in length
#define ADDR_SERIAL_NUM     0x00        //Uses 1 page
#define ADDR_HARDWARE_CODE    0x020     //Uses 1 page
#define ADDR_SCREEN_CAL     0x080       //Uses 3 pages
#define ADDR_ACC_CAL      0x200         //Uses 4 pages
#define ADDR_MAG_ALIGNCAL_NOLID 0x300   //Uses 2 pages
#define ADDR_MAG_ALIGNCAL_LIDXV 0x340   //Uses 2 pages
#define ADDR_MAG_ALIGNCAL_LIDSW 0x380   //Uses 2 pages
#define ADDR_MAG_ALIGNCAL_LIDRP 0x420   //Uses 2 pages
#define ADDR_MAG_HSCAL_NOLID  0x500     //Uses 4 pages
#define ADDR_MAG_HSCAL_LIDXV  0x580     //Uses 4 pages
#define ADDR_MAG_HSCAL_LIDSW  0x660     //Uses 4 pages
#define ADDR_MAG_HSCAL_LIDRP  0x740     //Uses 4 pages
#define ADDR_LIDXV_ORIENTCAL   0x820     //Uses 1 page
#define ADDR_LIDSW_ORIENTCAL   0x840     //Uses 1 page
#define ADDR_LIDRP_ORIENTCAL   0x860     //Uses 1 page
#define ADDR_LRF_RANGECAL   0x880     //Uses 1 page

extern char serialNumber[];
extern char hardwareCode[];
extern char screenCal[3][11];
extern float accCal[4][3];
extern float magAlignCalNoLid[8];
extern float magAlignCalLidarXV[8];
extern float magAlignCalLidarSW[8];
extern float magAlignCalLidarRP[8];
extern float magCalNoLid[4][3];
extern float magCalLidarXV[4][3];
extern float magCalLidarSW[4][3];
extern float magCalLidarRP[4][3];
extern float lidarXVOrientCal[1];
extern float lidarSWOrientCal[1];
extern float lidarRPOrientCal[1];
extern float LRFRangeCal[1];


void setup() {
  //Init Serial
  Serial.begin(250000);
  
  //Init SPI
  Wire1.begin();

  //Init GUI
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
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
  if (loadMagAlignCalNoLid) {
    UploadMagnetometerAlignCalNoLid();
    delay(100);
    if (CheckMagnetometerAlignCalNoLid()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagA NL Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagA NL Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagAlignCalLidarXV) {
    UploadMagnetometerAlignCalLidarXV();
    delay(100);
    if (CheckMagnetometerAlignCalLidarXV()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagA LXV Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagA LXV Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagAlignCalLidarSW) {
    UploadMagnetometerAlignCalLidarSW();
    delay(100);
    if (CheckMagnetometerAlignCalLidarSW()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagA LSW Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagA LSW Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagAlignCalLidarRP) {
    UploadMagnetometerAlignCalLidarRP();
    delay(100);
    if (CheckMagnetometerAlignCalLidarRP()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagA LRP Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagA LRP Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagHSCalNoLid) {
    UploadMagnetometerHSCalNoLid();
    delay(100);
    if (CheckMagnetometerHSCalNoLid()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagHS NL Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagHS NL Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagHSCalLidarXV) {
    UploadMagnetometerHSCalLidarXV();
    delay(100);
    if (CheckMagnetometerHSCalLidarXV()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagHS LXV Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagHS LXV Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagHSCalLidarSW) {
    UploadMagnetometerHSCalLidarSW();
    delay(100);
    if (CheckMagnetometerHSCalLidarSW()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagHS LSW Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagHS LSW Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadMagHSCalLidarRP) {
    UploadMagnetometerHSCalLidarRP();
    delay(100);
    if (CheckMagnetometerHSCalLidarRP()) {      
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("MagHS LRP Cal Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("MagHS LRP Cal Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLidarXVOrientCal) {
    UploadLidarXVOrientCal();
    delay(100);
    if (CheckLidarXVOrientCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("LIDARXV Orient Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("LIDARXV Orient Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLidarSWOrientCal) {
    UploadLidarSWOrientCal();
    delay(100);
    if (CheckLidarSWOrientCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("LIDARSW Orient Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("LIDARSW Orient Failed", CENTER, y);
    }
    y=y+20;
  }
  if (loadLidarRPOrientCal) {
    UploadLidarRPOrientCal();
    delay(100);
    if (CheckLidarRPOrientCal()) {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("LIDARRP Orient Loaded", CENTER, y);
    } else {
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("LIDARRP Orient Failed", CENTER, y);
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
  EEPROM_writeCharArray(ADDR_HARDWARE_CODE, hardwareCode, 11);
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

void UploadMagnetometerAlignCalNoLid() {
  float arrayBuffer[4];
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalNoLid[i];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_NOLID, arrayBuffer, 4);
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalNoLid[i+4];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_NOLID+0x20, arrayBuffer, 4);
}

void UploadMagnetometerAlignCalLidarXV() {
  float arrayBuffer[4];
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalLidarXV[i];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDXV, arrayBuffer, 4);
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalLidarXV[i+4];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDXV+0x20, arrayBuffer, 4);
}

void UploadMagnetometerAlignCalLidarSW() {
  float arrayBuffer[4];
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalLidarSW[i];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDSW, arrayBuffer, 4);
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalLidarSW[i+4];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDSW+0x20, arrayBuffer, 4);
}

void UploadMagnetometerAlignCalLidarRP() {
  float arrayBuffer[4];
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalLidarRP[i];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDRP, arrayBuffer, 4);
  for(int i=0;i<4;i++) arrayBuffer[i] = magAlignCalLidarRP[i+4];
  EEPROM_writeFloatArray(ADDR_MAG_ALIGNCAL_LIDRP+0x20, arrayBuffer, 4);
}

void UploadMagnetometerHSCalNoLid() {
  float arrayBuffer[3];
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) arrayBuffer[j] = magCalNoLid[i][j];
    EEPROM_writeFloatArray(ADDR_MAG_HSCAL_NOLID+(i*0x20), arrayBuffer, 3);
  }
}

void UploadMagnetometerHSCalLidarXV() {
  float arrayBuffer[3];
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) arrayBuffer[j] = magCalLidarXV[i][j];
    EEPROM_writeFloatArray(ADDR_MAG_HSCAL_LIDXV+(i*0x20), arrayBuffer, 3);
  }
}

void UploadMagnetometerHSCalLidarSW() {
  float arrayBuffer[3];
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) arrayBuffer[j] = magCalLidarSW[i][j];
    EEPROM_writeFloatArray(ADDR_MAG_HSCAL_LIDSW+(i*0x20), arrayBuffer, 3);
  }
}

void UploadMagnetometerHSCalLidarRP() {
  float arrayBuffer[3];
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) arrayBuffer[j] = magCalLidarRP[i][j];
    EEPROM_writeFloatArray(ADDR_MAG_HSCAL_LIDRP+(i*0x20), arrayBuffer, 3);
  }
}

void UploadLidarXVOrientCal() {
  EEPROM_writeFloatArray(ADDR_LIDXV_ORIENTCAL, lidarXVOrientCal, 1);
}

void UploadLidarSWOrientCal() {
  EEPROM_writeFloatArray(ADDR_LIDSW_ORIENTCAL, lidarSWOrientCal, 1);
}

void UploadLidarRPOrientCal() {
  EEPROM_writeFloatArray(ADDR_LIDRP_ORIENTCAL, lidarRPOrientCal, 1);
}

void UploadLRFRangeCal() {
  EEPROM_writeFloatArray(ADDR_LRF_RANGECAL, LRFRangeCal, 1);
}


boolean CheckSerialNumber() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_SERIAL_NUM, 5);
  if (arr!=serialNumber) return 0;
  return 1;
}

boolean CheckHardwareCode() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_HARDWARE_CODE, 11);
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

boolean CheckMagnetometerAlignCalNoLid() {
  float arr[8];
  for(int i=0;i<4;i++) {
    arr[i] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_NOLID+(4*i));
    arr[i+4] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_NOLID+0x20+(4*i));
    delay(50);
  }
  for(int i=0;i<8;i++) if (arr[i]!=magAlignCalNoLid[i]) return 0;
  return 1;
}

boolean CheckMagnetometerAlignCalLidarXV() {
  float arr[8];
  for(int i=0;i<4;i++) {
    arr[i] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDXV+(4*i));
    arr[i+4] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDXV+0x20+(4*i));
    delay(50);
  }
  for(int i=0;i<8;i++) if (arr[i]!=magAlignCalLidarXV[i]) return 0;
  return 1;
}

boolean CheckMagnetometerAlignCalLidarSW() {
  float arr[8];
  for(int i=0;i<4;i++) {
    arr[i] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDSW+(4*i));
    arr[i+4] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDSW+0x20+(4*i));
    delay(50);
  }
  for(int i=0;i<8;i++) if (arr[i]!=magAlignCalLidarSW[i]) return 0;
  return 1;
}

boolean CheckMagnetometerAlignCalLidarRP() {
  float arr[8];
  for(int i=0;i<4;i++) {
    arr[i] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDRP+(4*i));
    arr[i+4] = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDRP+0x20+(4*i));
    delay(50);
  }
  for(int i=0;i<8;i++) if (arr[i]!=magAlignCalLidarRP[i]) return 0;
  return 1;
}

boolean CheckMagnetometerHSCalNoLid() {
  float arr[12];
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr[i+(3*j)] = EEPROM_readFloat((ADDR_MAG_HSCAL_NOLID+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      if (arr[i+(3*j)]!=magCalNoLid[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckMagnetometerHSCalLidarXV() {
  float arr[12];
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr[i+(3*j)] = EEPROM_readFloat((ADDR_MAG_HSCAL_LIDXV+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      if (arr[i+(3*j)]!=magCalLidarXV[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckMagnetometerHSCalLidarSW() {
  float arr[12];
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr[i+(3*j)] = EEPROM_readFloat((ADDR_MAG_HSCAL_LIDSW+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      if (arr[i+(3*j)]!=magCalLidarSW[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckMagnetometerHSCalLidarRP() {
  float arr[12];
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr[i+(3*j)] = EEPROM_readFloat((ADDR_MAG_HSCAL_LIDRP+(4*i))+(0x20*j));
      delay(50);
    }
  }
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      if (arr[i+(3*j)]!=magCalLidarRP[j][i]) return 0;
    }
  }
  return 1;
}

boolean CheckLidarXVOrientCal() {
  float arr[1];
  arr[0] = EEPROM_readFloat(ADDR_LIDXV_ORIENTCAL);
  delay(50);
  if (arr[0]!=lidarXVOrientCal[0]) return 0;
  return 1;
}

boolean CheckLidarSWOrientCal() {
  float arr[1];
  arr[0] = EEPROM_readFloat(ADDR_LIDSW_ORIENTCAL);
  delay(50);
  if (arr[0]!=lidarSWOrientCal[0]) return 0;
  return 1;
}

boolean CheckLidarRPOrientCal() {
  float arr[1];
  arr[0] = EEPROM_readFloat(ADDR_LIDRP_ORIENTCAL);
  delay(50);
  if (arr[0]!=lidarRPOrientCal[0]) return 0;
  return 1;
}

boolean CheckLRFRangeCal() {
  float arr[1];
  arr[0] = EEPROM_readFloat(ADDR_LRF_RANGECAL);
  delay(50);
  if (arr[0]!=LRFRangeCal[0]) return 0;
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
  Write_AT24Cxx_Page(address, aBytes, numvalues*4);
}

void EEPROM_writeCharArray(uint16_t address, char values[], int numvalues)
{
  byte aBytes[30];
  for (int i=0;i<numvalues;i++) aBytes[i] = values[i];
  Write_AT24Cxx_Page(address, aBytes, numvalues);
}

float EEPROM_readFloat(uint16_t address)
{
  float value;
  union u_tag {
      byte b[4];
      float fval;
  } u;
  for(int i=0;i<4;i++) u.b[i] = Read_AT24Cxx_Byte(address+i);
  value = u.fval;
  return value;
}

String EEPROM_readCharArray(uint16_t address, int numvalues)
{
  char b[11];
  for(int i=0;i<numvalues;i++) b[i] = Read_AT24Cxx_Byte(address+i);
  b[numvalues+1] = '\0';
  String value(b);
  return value;
}

void Write_AT24Cxx_Page(int iAddr, byte* iData, int iLength)
{
  Wire1.beginTransmission(AT24C32_EEPROM_ADDR);
  Wire1.write((int)(iAddr >> 8)); // Address MSB
  Wire1.write((int)(iAddr & 0xFF));  // Address LSB
  for (byte c = 0; c < iLength; c++) {
    Wire1.write(iData[c]);
    delay(10);
  }
  Wire1.endTransmission();
}

int Read_AT24Cxx_Byte(int iAddr) {
  uint8_t rdata = 0xFF;
  Wire1.beginTransmission(AT24C32_EEPROM_ADDR);
  Wire1.write((int)(iAddr >> 8)); // MSB
  Wire1.write((int)(iAddr & 0xFF)); // LSB
  Wire1.endTransmission();
  Wire1.requestFrom(AT24C32_EEPROM_ADDR, 1);
  if (Wire1.available()) rdata = Wire1.read();
  return rdata;
}
