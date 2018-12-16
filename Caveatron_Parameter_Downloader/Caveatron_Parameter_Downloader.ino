/*****************************************************************/
//              Caveatron Parameter Downloader                   //
//                       Version 1.2                            //
/*****************************************************************/
// Joe Mitchell, 2018-11-28
// Used to download calibration parameters from the Caveatron EEPROM
// After loading, open a Serial Monitor window at 250000 baud to view the parameters

#include <Wire.h>

// Variables for RTC
#define DS3231_ADDRESS 0x68
#define AT24C32_EEPROM_ADDR 0x57

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


void setup() {
  //Init Serial
  Serial.begin(250000);
  
  //Init SPI
  Wire1.begin();

  delay(500);
  
  Serial.println("CAVEATRON SAVED PARAMETERS");
  Serial.println();
  
  DownloadSerialNumber();
  DownloadHardwareCode();
  DownloadScreenCal();
  DownloadAccelerometerCal();
  DownloadMagnetometerAlignCalNoLid();
  DownloadMagnetometerAlignCalLidarXV();
  DownloadMagnetometerAlignCalLidarSW();
  DownloadMagnetometerAlignCalLidarRP();
  DownloadMagnetometerHSCalNoLid();
  DownloadMagnetometerHSCalLidarXV();
  DownloadMagnetometerHSCalLidarSW();
  DownloadMagnetometerHSCalLidarRP();
  DownloadLidarXVOrientCal();
  DownloadLidarSWOrientCal();
  DownloadLidarRPOrientCal();
  DownloadLRFRangeCal();
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
  arr = EEPROM_readCharArray(ADDR_HARDWARE_CODE, 11);
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

void DownloadMagnetometerAlignCalNoLid() {
  float arr;
  Serial.println("Magnetometer Alignment Calibration - No LIDAR");
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_NOLID+(4*i));
    delay(50);
    Serial.print(arr,1);
    Serial.print(",");
  }
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_NOLID+0x20+(4*i));
    delay(50);
    Serial.print(arr,1);
    if (i<3) Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadMagnetometerAlignCalLidarXV() {
  float arr;
  Serial.println("Magnetometer Alignment Calibration - XV11 LIDAR Installed");
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDXV+(4*i));
    delay(50);
    Serial.print(arr,1);
    Serial.print(",");
  }
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDXV+0x20+(4*i));
    delay(50);
    Serial.print(arr,1);
    if (i<3) Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadMagnetometerAlignCalLidarSW() {
  float arr;
  Serial.println("Magnetometer Alignment Calibration - SWEEP LIDAR Installed");
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDSW+(4*i));
    delay(50);
    Serial.print(arr,1);
    Serial.print(",");
  }
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDSW+0x20+(4*i));
    delay(50);
    Serial.print(arr,1);
    if (i<3) Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadMagnetometerAlignCalLidarRP() {
  float arr;
  Serial.println("Magnetometer Alignment Calibration - RP LIDAR Installed");
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDRP+(4*i));
    delay(50);
    Serial.print(arr,1);
    Serial.print(",");
  }
  for(int i=0;i<4;i++) {
    arr = EEPROM_readFloat(ADDR_MAG_ALIGNCAL_LIDRP+0x20+(4*i));
    delay(50);
    Serial.print(arr,1);
    if (i<3) Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void DownloadMagnetometerHSCalNoLid() {
  float hardIronCal[3], softIronCal[9];
  Serial.println("Magnetometer Hard Iron Calibration - No LIDAR");
  for(int i=0;i<3;i++) hardIronCal[i] = EEPROM_readFloat(ADDR_MAG_HSCAL_NOLID+(4*i));
  Serial.print(hardIronCal[0],7);Serial.print(",");Serial.print(hardIronCal[1],7);Serial.print(",");Serial.println(hardIronCal[2],7);
  for(int j=0;j<3;j++) {
    for(int i=0;i<3;i++) softIronCal[i+(3*j)] = EEPROM_readFloat(ADDR_MAG_HSCAL_NOLID+(4*i)+(0x20*(j+1)));
  }
  Serial.println("Magnetometer Soft Iron Calibration - No LIDAR");
  Serial.print(softIronCal[0],7);Serial.print(",");Serial.print(softIronCal[1],7);Serial.print(",");Serial.println(softIronCal[2],7);
  Serial.print(softIronCal[3],7);Serial.print(",");Serial.print(softIronCal[4],7);Serial.print(",");Serial.println(softIronCal[5],7);
  Serial.print(softIronCal[6],7);Serial.print(",");Serial.print(softIronCal[7],7);Serial.print(",");Serial.println(softIronCal[8],7);
  Serial.println();
}

void DownloadMagnetometerHSCalLidarXV() {
  float hardIronCal[3], softIronCal[9];
  Serial.println("Magnetometer Hard Iron Calibration - XV11 LIDAR Installed");
  for(int i=0;i<3;i++) hardIronCal[i] = EEPROM_readFloat(ADDR_MAG_HSCAL_LIDXV+(4*i));
  Serial.print(hardIronCal[0],7);Serial.print(",");Serial.print(hardIronCal[1],7);Serial.print(",");Serial.println(hardIronCal[2],7);
  for(int j=0;j<3;j++) {
    for(int i=0;i<3;i++) softIronCal[i+(3*j)] = EEPROM_readFloat(ADDR_MAG_HSCAL_LIDXV+(4*i)+(0x20*(j+1)));
  }
  Serial.println("Magnetometer Soft Iron Calibration - XV11 LIDAR Installed");
  Serial.print(softIronCal[0],7);Serial.print(",");Serial.print(softIronCal[1],7);Serial.print(",");Serial.println(softIronCal[2],7);
  Serial.print(softIronCal[3],7);Serial.print(",");Serial.print(softIronCal[4],7);Serial.print(",");Serial.println(softIronCal[5],7);
  Serial.print(softIronCal[6],7);Serial.print(",");Serial.print(softIronCal[7],7);Serial.print(",");Serial.println(softIronCal[8],7);
  Serial.println();
}

void DownloadMagnetometerHSCalLidarSW() {
  float hardIronCal[3], softIronCal[9];
  Serial.println("Magnetometer Hard Iron Calibration - SWEEP LIDAR Installed");
  for(int i=0;i<3;i++) hardIronCal[i] = EEPROM_readFloat(ADDR_MAG_HSCAL_LIDSW+(4*i));
  Serial.print(hardIronCal[0],7);Serial.print(",");Serial.print(hardIronCal[1],7);Serial.print(",");Serial.println(hardIronCal[2],7);
  for(int j=0;j<3;j++) {
    for(int i=0;i<3;i++) softIronCal[i+(3*j)] = EEPROM_readFloat(ADDR_MAG_HSCAL_LIDSW+(4*i)+(0x20*(j+1)));
  }
  Serial.println("Magnetometer Soft Iron Calibration - SWEEP LIDAR Installed");
  Serial.print(softIronCal[0],7);Serial.print(",");Serial.print(softIronCal[1],7);Serial.print(",");Serial.println(softIronCal[2],7);
  Serial.print(softIronCal[3],7);Serial.print(",");Serial.print(softIronCal[4],7);Serial.print(",");Serial.println(softIronCal[5],7);
  Serial.print(softIronCal[6],7);Serial.print(",");Serial.print(softIronCal[7],7);Serial.print(",");Serial.println(softIronCal[8],7);
  Serial.println();
}

void DownloadMagnetometerHSCalLidarRP() {
  float hardIronCal[3], softIronCal[9];
  Serial.println("Magnetometer Hard Iron Calibration - RP LIDAR Installed");
  for(int i=0;i<3;i++) hardIronCal[i] = EEPROM_readFloat(ADDR_MAG_HSCAL_LIDRP+(4*i));
  Serial.print(hardIronCal[0],7);Serial.print(",");Serial.print(hardIronCal[1],7);Serial.print(",");Serial.println(hardIronCal[2],7);
  for(int j=0;j<3;j++) {
    for(int i=0;i<3;i++) softIronCal[i+(3*j)] = EEPROM_readFloat(ADDR_MAG_HSCAL_LIDRP+(4*i)+(0x20*(j+1)));
  }
  Serial.println("Magnetometer Soft Iron Calibration - RP LIDAR Installed");
  Serial.print(softIronCal[0],7);Serial.print(",");Serial.print(softIronCal[1],7);Serial.print(",");Serial.println(softIronCal[2],7);
  Serial.print(softIronCal[3],7);Serial.print(",");Serial.print(softIronCal[4],7);Serial.print(",");Serial.println(softIronCal[5],7);
  Serial.print(softIronCal[6],7);Serial.print(",");Serial.print(softIronCal[7],7);Serial.print(",");Serial.println(softIronCal[8],7);
  Serial.println();
}

void DownloadLidarXVOrientCal() {
  float arr;
  Serial.println("XV11 LIDAR Orientation Angle Calibration");
  arr = EEPROM_readFloat(ADDR_LIDXV_ORIENTCAL);
  delay(50);
  Serial.print(arr,1);
  Serial.println();
  Serial.println();
}

void DownloadLidarSWOrientCal() {
  float arr;
  Serial.println("SWEEP LIDAR Orientation Angle Calibration");
  arr = EEPROM_readFloat(ADDR_LIDSW_ORIENTCAL);
  delay(50);
  Serial.print(arr,1);
  Serial.println();
  Serial.println();
}

void DownloadLidarRPOrientCal() {
  float arr;
  Serial.println("RP LIDAR Orientation Angle Calibration");
  arr = EEPROM_readFloat(ADDR_LIDRP_ORIENTCAL);
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
