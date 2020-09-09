/*****************************************************************/
//             Caveatron Parameter Downloader                    //
//                       Version 2.0                             //
/*****************************************************************/
// Joe Mitchell, 2019-12-15
// Used to download calibration parameters from the Caveatron EEPROM
// After loading, open a Serial Monitor window at 250000 baud to view the parameters

#include <EEPROM.h>

// EEPROM Calibration Parameter Addresses
// 1 page is 0x020 in length
#define ADDR_SERIAL_NUM     0x00        //Uses 1 page
#define ADDR_HARDWARE_CODE    0x020     //Uses 1 page
#define ADDR_SCREEN_CAL     0x080       //Uses 3 pages
#define ADDR_ACC_CAL      0x200         //Uses 4 pages
#define ADDR_MAG_ALIGNCAL 0x300   //Uses 2 pages
#define ADDR_MAG_HSCAL  0x400     //Uses 4 pages
#define ADDR_GYR_CAL  0x600     //Uses 4 pages
#define ADDR_LID_CORNERS 0x700    //Users 2 pages
#define ADDR_LID_ORIENTCAL   0x800     //Uses 1 page
#define ADDR_LRF_RANGECAL   0x860     //Uses 1 page
#define ADDR_LID_WINCORRECT   0xA00     //Uses 1 page

// EEPROM Preferences Addresses
#define ADDR_LIDARSPEED_PREF  0x900
#define ADDR_LIDARRATE_PREF 0x901
#define ADDR_LRFRATE_PREF 0x905
#define ADDR_SHUTDOWN_PREF  0x910


void setup() {
  //Init Serial
  Serial.begin(250000);

  delay(500);
  
  Serial.println("CAVEATRON SAVED PARAMETERS");
  Serial.println();
  
  DownloadSerialNumber();
  DownloadHardwareCode();
  DownloadScreenCal();
  DownloadAccelerometerCal();
  DownloadMagnetometerAlignCal();
  DownloadMagnetometerHSCal();
  DownloadGyroscopeCal();
  DownloadLidarCorners();
  DownloadLidarOrientCal();
  DownloadLRFRangeCal();
  DownloadWindowCorrections();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void DownloadSerialNumber() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_SERIAL_NUM, 5);
  Serial.println("Serial Number");
  Serial.println(arr);
  Serial.println();
}

boolean DownloadHardwareCode() {
  String arr;
  arr = EEPROM_readCharArray(ADDR_HARDWARE_CODE, 12);
  Serial.println("Hardware Code");
  Serial.println(arr);
  Serial.println();
}

void DownloadScreenCal() {
  String arr;
  char checkArr[10];
  Serial.println("Screen Calibration");
  for (int i=0; i<3; i++) {
    arr = EEPROM_readCharArray(ADDR_SCREEN_CAL+(i*0x20), 10);
    arr.toCharArray(checkArr, 10);
    Serial.print("0x"+String(checkArr));
    if (i<2) Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadAccelerometerCal() {
  float arr;
  Serial.println("Accelerometer Calibration");
  for(int j=0;j<4;j++) {
    for(int i=0;i<3;i++) {
      arr = EEPROM_readFloat((ADDR_ACC_CAL+(4*i))+(0x20*j));
      delay(50);
      Serial.print(double2s(arr, 5));
      if (i<2) Serial.print(",");
    }
    Serial.println();
  }
  Serial.println();
}

void DownloadMagnetometerAlignCal() {
  float arr;
  Serial.println("Magnetometer Alignment Calibration");
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL+(4*i));
    delay(50);
    Serial.print(arr,1);
    Serial.print(",");
  }
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL+0x20+(4*i));
    delay(50);
    Serial.print(arr,1);
    if (i<3) Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadMagnetometerHSCal() {
  float hardIronCal[3], softIronCal[9];
  Serial.println("Magnetometer Hard Iron Calibration");
  for(int i=0;i<3;i++) hardIronCal[i] = EEPROM_readFloat(ADDR_MAG_HSCAL+(4*i));
  Serial.print(hardIronCal[0],7);Serial.print(",");Serial.print(hardIronCal[1],7);Serial.print(",");Serial.println(hardIronCal[2],7);
  for(int j=0;j<3;j++) {
    for(int i=0;i<3;i++) softIronCal[i+(3*j)] = EEPROM_readFloat(ADDR_MAG_HSCAL+(4*i)+(0x20*(j+1)));
  }
  Serial.println("Magnetometer Soft Iron Calibration");
  Serial.print(softIronCal[0],7);Serial.print(",");Serial.print(softIronCal[1],7);Serial.print(",");Serial.println(softIronCal[2],7);
  Serial.print(softIronCal[3],7);Serial.print(",");Serial.print(softIronCal[4],7);Serial.print(",");Serial.println(softIronCal[5],7);
  Serial.print(softIronCal[6],7);Serial.print(",");Serial.print(softIronCal[7],7);Serial.print(",");Serial.println(softIronCal[8],7);
  Serial.println();
}

void DownloadGyroscopeCal() {
  float arr;
  Serial.println("Gyroscope Zero Offset Calibration");
  for(int i=0;i<3;i++) {
    arr = EEPROM_readFloat(ADDR_GYR_CAL+(4*i));
    delay(50);
    Serial.print(arr,1);
    Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadLidarCorners() {
  float arr;
  Serial.println("Lidar Window Corners");
  for(int j=0;j<4;j++) {
    for(int i=0;i<2;i++) {
      arr = EEPROM_readFloat((ADDR_LID_CORNERS+(4*i))+(0x20*j));
      delay(50);
      Serial.print(double2s(arr, 5));
      if (i<1) Serial.print(",");
    }
    Serial.println();
  }
  Serial.println();
}

void DownloadLidarOrientCal() {
  float arr;
  Serial.println("LIDAR Orientation Angle Calibration");
  arr = EEPROM_readFloat(ADDR_LID_ORIENTCAL);
  delay(50);
  Serial.print(arr,1);
  Serial.println();
  Serial.println();
}

void DownloadLRFRangeCal() {
  float arr;
  Serial.println("LRF Range Calibration");
  arr = EEPROM_readFloat(ADDR_LRF_RANGECAL);
  delay(50);
  Serial.print(arr,1);
  Serial.println();
  Serial.println();
}

void DownloadWindowCorrections() {
  int8_t sByte;
  Serial.println("LIDAR Window Distortion Correction");
  for (int i=0; i<2; i++) {
    sByte = EEPROM.read(ADDR_LID_WINCORRECT+i);
    Serial.print(sByte); 
    if (i<1) Serial.print(",");
    delay(2);
  }
  Serial.println();
  for (int i=0; i<720; i++) {
    sByte = EEPROM.read(ADDR_LID_WINCORRECT+2+i);
    Serial.print(sByte); 
    if (i<719) Serial.print(",");
    delay(2);
  }
  Serial.println();
  Serial.println();
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
  char b[13];
  for(int i=0;i<numvalues;i++) b[i] = Read_EEPROM_Byte(address+i);
  b[numvalues+1] = '\0';
  String value(b);
  return value;
}

// Data page read from EEPROM
int Read_EEPROM_Byte(int iAddr) {
  uint8_t rdata = EEPROM.read(iAddr);
  return rdata;
}

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
 // handle infinite values
 if (isinf(f))
 {
   strcpy(&s[index], "INF");
   return s;
 }
 // handle Not a Number
 if (isnan(f))
 {
   strcpy(&s[index], "NaN");
   return s;
 }

 // max digits
 if (digits > 16) digits = 16;
 int exponent = int(log10(f));
 double g = f / pow(10, exponent);
 if ((g < 1.0) && (g != 0.0))      
 {
   g *= 10;
   exponent--;
 }
 if (exponent < -330) {  // lower limit of double-precision on Teensy 3
   g = 0;
   exponent = 0;
 }
 if (digits < 16) {  // display number rounded at last digit
   g += 0.5 / pow(10,digits);
 }
 
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
