/*
  Caveatron_Hardware.cpp
  Version 1.0
  Joe Mitchell
  2018-03-04

  This library contains all functions to interface between the main code and the hardware used for the Caveatron.
  The library is setup to allow for the use of different hardware which is set by a hardware code stored on the EEPROM with the calibration parameters.
*/


//#include "Arduino.h"

#include "Caveatron_Hardware.h"
#include <Wire.h>

// Library instancing function to UTFT, URTouch, UTFT_CTE, and UTFT_GHL libraries
Caveatron_Hardware::Caveatron_Hardware(UTFT * lcd, URTouch * touch)
{
  	mylcd = (UTFT *) lcd;
  	mytouch = (URTouch *) touch;
	myFont_CTE = new UTFT_CTE(mylcd);
	myFont_GHL = new UTFT_GHL(mylcd);
}

// Initialize library from serial number and hardware code
void Caveatron_Hardware::Init()
{
	char hwCode[11], hwRev;
	//Load Serial Number
	serialNumber = EEPROM_readCharArray(ADDR_SERIAL_NUM, 5);
	hwRev = serialNumber.charAt(1);
	//Load Hardware Parameters
	String hwCodeStr = EEPROM_readCharArray(ADDR_HARDWARE_CODE, 11);
	hwCodeStr.toCharArray(hwCode, sizeof(hwCode));

	Init(hwRev, hwCode);
}

// Set each hardware type from code
void Caveatron_Hardware::Init(char hRev, char hCode[])
{

	/* Digit Position Definitions
		0=SD Card Data Storage
		1=Display
		2=Font/Graphics Storage
		3=LRF
		4=Compass
		5=IMU/Gyro
		6=Battery
		7=Battery Gauge
		8=Clock
		9=Piezo Buzzer
		10=Wireless
	*/

	hardwareRev = hRev;

	sdType=hCode[0];
	displayType=hCode[1];
	graphicsType=hCode[2];
	lrfType=hCode[3];
	compassType=hCode[4];
	imuType=hCode[5];
	batteryType=hCode[6];
	battGaugeType=hCode[7];
	clockType=hCode[8];
	buzzerType=hCode[9];
	wirelessType=hCode[10];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          DISPLAY FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Initialize Display parameters
void Caveatron_Hardware::LCD_Init()
{
	// Initialize TFT with additional parameters
	switch(displayType) {
		case '1':
			mylcd->setDisplayModel(CTE35IPS);
			mylcd->invert_colors = false;
			mylcd->InitLCD(PORTRAIT);
			break;
		case '2':						//Invert display colors
			mylcd->setDisplayModel(CTE35IPS);
			mylcd->invert_colors = true;
			mylcd->InitLCD(PORTRAIT);
			break;
	}

	// Initialize Font IC
	switch(graphicsType) {
		case '1':
			myFont_CTE->SPI_Flash_init(52);
			//BVS= Bitstream vera sans, suffix = font size in pixel (height)
			FONT_13 = CBVS_13;
			FONT_15 = CBVS_15;
			FONT_19 = CBVS_19;
			FONT_22 = CBVS_22;
			FONT_28 = CBVS_28;
			FONT_34 = CBVS_34;
			FONT_43 = CBVS_43;
			FONT_52 = CBVS_52;
			FONT_74 = CBVS_74;
			FONT_112 = CBVS_112;
			//Icon and Graphic address locations
			iSHOT = 500;
			iPASSAGE = 502;
			iROOM = 504;
			iMANUAL = 508;
			iSURVEY = 512;
			iSETTINGS = 510;
			iTIME = 514;
			iSD = 516;
			iCALIBRATE = 518;
			iLIDAR = 506;
			iABOUT = 520;
			iRETURN = 522;
			gLOGO = 550;
			break;
		case '2':
			myFont_GHL->SPI_Flash_init(52);
			//BVS= Bitstream vera sans, suffix = font size in pixel (height)
			FONT_13 = BVS_13;
			FONT_15 = BVS_15;
			FONT_19 = BVS_19;
			FONT_22 = BVS_22;
			FONT_28 = BVS_28;
			FONT_34 = BVS_34;
			FONT_43 = BVS_43;
			FONT_52 = BVS_56;
			FONT_74 = BVS_74;
			FONT_112 = BVS_112;
			//Icon and Graphic address locations
			iSHOT = 255;
			iPASSAGE = 256;
			iROOM = 257;
			iMANUAL = 258;
			iSURVEY = 259;
			iSETTINGS = 260;
			iTIME = 261;
			iSD = 262;
			iCALIBRATE = 263;
			iLIDAR = 264;
			iABOUT = 265;
			iRETURN = 266;
			gLOGO = 254;
			break;
	}
}


//Print text to screen
void Caveatron_Hardware::LCD_PrintText(String st, int x, int y, int font_number)
{
	switch(graphicsType) {
		case '1':
			myFont_CTE->Put_Text(st, x, y, font_number);
			break;
		case '2':
			myFont_GHL->Put_Text(st, x, y, font_number);
			break;
	}
}

//Set character spacing in font
void Caveatron_Hardware::LCD_SetTextSpacing(int font_spacing)
{
	switch(graphicsType) {
		case '1':
			myFont_CTE->Set_character_spacing(font_spacing);
			break;
		case '2':
			myFont_GHL->Set_character_spacing(font_spacing);
			break;
	}
}

//Get length of text on x axis
int Caveatron_Hardware::LCD_GetTextLength(String st, int font_number)
{
	int tlength;
	switch(graphicsType) {
		case '1':
			tlength = myFont_CTE->Get_Text_size_x(st, font_number);
			break;
		case '2':
			char buf[st.length()+1];
			st.toCharArray(buf, st.length()+1);
			tlength = myFont_GHL->GetTextWidth(buf, font_number);
			break;
	}
	return tlength;
}

//Load image from SPI flash and display
void Caveatron_Hardware::LCD_ShowImage(int x, int y, int addr)
{
	switch(graphicsType) {
		case '1':
			myFont_CTE->Load_image(x, y, addr);
			break;
		case '2':
			myFont_GHL->Load_image(x, y, addr);
			break;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            LRF FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//UT390B Serial Commands
// *00084553#   Rapid Fire Mode
// *00004#      Single Shot Mode
// *00002#      Three Shot Mode
// *100515#     Turn on laser
// *00003#      Turn off laser

//Initialize LRF parameters
void Caveatron_Hardware::LRF_Init()
{
	// Set control pins and enclosure geometry for LRF
	switch (hardwareRev) {
		case 'A':
		case 'B':
			LRFOnPin = 9;
			LRFPowerPin = 10;
			//Distances from LRF measurement point to corners of box in meters
			boxRearDist = 0.05;
			boxLeftDist = 0.03;
			boxRightDist = 0.09;
			boxBaseDist = 0.02;
			//LRF minimum range in meters
			LRFminRange = 0.20;
			LRFmaxRange = 45.0;
			break;
	}

	// Initialize LRF control pins and ports
	switch(lrfType) {
		case '1':
		case '2':
			pinMode(LRFOnPin, OUTPUT);
			digitalWrite(LRFOnPin, LOW);
			pinMode(LRFPowerPin, OUTPUT);
			digitalWrite(LRFPowerPin, LOW);
			Serial1.begin(115200);
			break;
		case '3':
			pinMode(LRFPowerPin, OUTPUT);
			digitalWrite(LRFPowerPin, LOW);
			Serial1.begin(19200);
			break;
	}

}

//Turn on power to LRF
void Caveatron_Hardware::LRF_PowerOn()
{
	digitalWrite(LRFPowerPin, HIGH);
	delay(100);
	switch(lrfType) {
		case '1':
		case '2':
			digitalWrite(LRFOnPin, HIGH);
			delay(400);
			digitalWrite(LRFOnPin, LOW);
			break;
	}
}

//Turn off power to LRF
void Caveatron_Hardware::LRF_PowerOff()
{
	digitalWrite(LRFPowerPin, LOW);
}

//Turn on LRF laser
void Caveatron_Hardware::LRF_LaserOn()
{
	switch(lrfType) {
		case '1':
		case '2':
			Serial1.print("*100515#");
			break;
		case '3':
			Serial1.print("O");
			break;
	}
}

//Turn off LRF laser
void Caveatron_Hardware::LRF_LaserOff()
{
	switch(lrfType) {
		case '1':
		case '2':
			Serial1.print("*00003#");
			break;
		case '3':
			Serial1.print("C");
			break;
	}
}

//Initiate an LRF range measurement
void Caveatron_Hardware::LRF_Fire()
{
	switch(lrfType) {
		case '1':
		case '2':
			Serial1.print("*00004#");
			break;
		case '3':
			Serial1.print("D");
			break;
	}
}

//Set serial and LRF timeouts for shot (normal) or LIDAR modes
void Caveatron_Hardware::LRF_SetMode(boolean mode)
{
	//mode = 1 - LIDAR mode
	//mode = 0 - Normal mode
	LRFmode = mode;
	switch(lrfType) {
		case '1':
		case '2':
			if (LRFmode==1) {
				Serial1.setTimeout(25);
				//LRFtimeout = 175;
				LRFtimeout = 125;
			}
			else {
				Serial1.setTimeout(1000);
				LRFtimeout = 4000;
			}
			LRFperiod = 1175;
			break;
		case '3':
			if (LRFmode==1) {
				Serial1.setTimeout(25);
				LRFtimeout = 50;
			}
			else {
				Serial1.setTimeout(1000);
				LRFtimeout = 4000;
			}
			LRFperiod = 575;
			break;
	}
}

//Read raw distance from LRF and return read status code
uint8_t Caveatron_Hardware::LRF_Read()
{
	char buf[256];

	switch(lrfType) {
		case '1':

			if (Serial1.available()) {
				char *comma;
				int dist, rc;

				if (LRFmode==1) LRFstartTime = millis();
				while ((millis() - LRFstartTime) < LRFtimeout) {

					rc = Serial1.readBytesUntil('\n', buf, sizeof(buf));
					//if ((LRFmode==1) && (rc == 0))
						//return 0; //No LRF data found - Return 0 (only during LIDAR scan)
					buf[rc] = '\0';

					if (LRFmode==0) {
						if (strstart(buf, "MEDIUM1") || strstart(buf, "MEDIUM2") || strstart(buf, "THICK") || strstart(buf, "BIASVolate"))
							return 2;  //LRF Error - Return 2
					}

					if (!strstart(buf, "Dist: ")) {
						comma = strchr(buf, ',');
						if (comma == NULL) continue;
						*comma = '\0';
						dist = atoi(buf + strlen("Dist: "));
						memset(buf,0,sizeof(buf));
						LRFdistance = (float) dist/1000;
						return 1; //Good LRF Reading - Return 1
					}
				}
				return 3; //Timeout has occurred - Return 3
			}
			break;

		case '2':

			if (Serial1.available()) {
				int dist, rc;
				char diststr[10];

				if (LRFmode==1) LRFstartTime = millis();
				while ((millis() - LRFstartTime) < LRFtimeout) {

					rc = Serial1.readBytesUntil('\n', buf, sizeof(buf));
					//if ((LRFmode==1) && (rc == 0))
						//return 0; //No LRF data found - Return 0 (only during LIDAR scan)
					buf[rc] = '\0';

					if (LRFmode==0) {
						if (strstart(buf, "MEDIUM1") || strstart(buf, "MEDIUM2") || strstart(buf, "THICK") || strstart(buf, "BIASVolate"))
							return 2;  //LRF Error - Return 2
					}

					if (strstart(buf, "*00064")) {
						strncpy(diststr, buf+strlen("*00064"), 9);
						diststr[9] = '\0';
						dist = atoi(diststr);
						memset(buf,0,sizeof(buf));
						LRFdistance = (float) dist/1000;
						return 1; //Good LRF Reading - Return 1
					}
				}
				return 3; //Timeout has occurred - Return 3
			}
			break;

		case '3':

			if (Serial1.available()) {
				int rc;
				char diststr[8];
				char *pchs, *pche;

				if (LRFmode==1) LRFstartTime = millis();
				while ((millis() - LRFstartTime) < LRFtimeout) {

					rc = Serial1.readBytesUntil('\n', buf, sizeof(buf));
					//if ((LRFmode==1) && (rc == 0))
						//return 0; //No LRF data found - Return 0 (only during LIDAR scan)
					buf[rc] = '\0';

					pchs = strchr(buf, ':');
					if (pchs != NULL) {
						if (pchs[1]=='E')
							return 2;  //Error - Return 2
						pche = strchr(buf, 'm');
						if (pche != NULL) {
							strncpy(diststr, &pchs[1], pche-&pchs[1]);
							LRFdistance = atof(diststr);
							memset(buf,0,sizeof(buf));
							return 1; //Good LRF Reading - Return 1
						}
					}
				}
				return 3; //Timeout has occurred - Return 3
			}
			break;
	}
	return 0; //Timeout has occurred - Return 3
}

//Read distance from LRF, filter for distance, correct for mechanical dimensions and return status code
uint8_t Caveatron_Hardware::LRF_GetRange ()
{
	uint8_t LRFresult = 0;
	LRFstartTime = millis();
	while(LRFresult==0) {
		LRFresult = LRF_Read();
		if ((millis() - LRFstartTime) > LRFtimeout) LRFresult = 3;
	}
	if (LRFresult != 1) LRFdistance = -1;
	else {
		LRFdistance = LRFdistance + boxRearDist;
		if ((LRFdistance < LRFminRange) || (LRFdistance > LRFmaxRange)) {
			LRFdistance = -1;
			LRFresult = 4; //Short or long distance error - 4
		}
	}
	return LRFresult;
}

//Finds characters at beginning of char array
int Caveatron_Hardware::strstart(const char *s1, const char * PROGMEM s2)
{
    return strncmp(s1, s2, strlen(s2)) == 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      LIDAR CONFIGURATION FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize SD Hardware Parameters
void Caveatron_Hardware::LIDAR_Init(uint8_t lidarType)
{
	// LIDAR hardware geometry related to enclosure and LRF (Distances in m)
	switch (hardwareRev) {
		case 'A':
			LIDARFrontDist = 0.13;
			LIDARHeight = 0.11;
			LIDARLateral = 0.02;
			break;
	}

	// Setup LIDAR range limits (Distance in mm)
	switch(lidarType) {
		case 1:
			minLIDARrange = 130;
			maxLIDARrange = 4500;
			maxLIDARsignal = 66000;
			ptsLIDARrot = 360;
			break;
		case 2:
			minLIDARrange = 100;
			maxLIDARrange = 25000;
			maxLIDARsignal = 256;
			ptsLIDARrot = 262;
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                         IMU COMPASS FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize Compass IMU Hardware
void Caveatron_Hardware::IMU_Init()
{
	//Initialize hardware
	switch(compassType) {
		case '1':
			compass.setI2CPort(Wire1);
			compass.init();
			compass.enableDefault();
			break;
	}

	// Set bounds for hard & soft iron calibration data collection
	switch(compassType) {
		case '1':
			switch (compass.getDeviceType()) {
				case LSM303::device_D:
					IMU_hypmin = 800;
					IMU_hypmax = 8000;
					IMU_hypUbound = 16383;
					break;
				default:
					IMU_hypmin = 200;
					IMU_hypmax = 1000;
					IMU_hypUbound = 2047;
					break;
			}
			break;
	}
}

// Reads compass data and loads it into variables
void Caveatron_Hardware::IMU_Read()
{
	switch(compassType) {
		case '1':
			compass.read();
			IMU_a_x = compass.a.x;
			IMU_a_y = compass.a.y;
			IMU_a_z = compass.a.z;
			IMU_m_x = compass.m.x;
			IMU_m_y = compass.m.y;
			IMU_m_z = compass.m.z;
			break;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          BATTERY FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize Battery Gauge Hardware
void Caveatron_Hardware::BATT_Init()
{
	// Set parameters for battery
	switch(batteryType) {
		case '1':
			BATTERY_CAPACITY = 4400;
			CUTOFF_VOLTAGE = 2900;
			break;
	}

	// Load libraries and initialize battery gauge hardware
	switch(battGaugeType) {
		case '1':
			MAXbatt.setI2CPort(Wire1);
			MAXbatt.reset();
			MAXbatt.quickStart();
			break;
		case '2':
			lipo.setI2CPort(Wire1);
			lipo.begin();
			lipo.setCapacity(BATTERY_CAPACITY);
			lipo.setTerminateVoltage(CUTOFF_VOLTAGE);
			break;
	}
}

// Compute and Return Battery Level Estimate
unsigned int Caveatron_Hardware::BATT_GetLevel()
{
	int bLevel;
	float bVolts;
	switch(battGaugeType) {
		case '1':
			bVolts = MAXbatt.getVCell()*1000;
			if (bVolts >= 3100) bLevel = 6 + (bVolts - 3100)/8.2;
			else bLevel = 0.5 + (bVolts - 2750)/70;
			if (bLevel > 100) bLevel = 100;
			if (bLevel < 0) bLevel = 0;
			break;
		case '2':
			bLevel = lipo.soc();
			break;
	}
	return bLevel;
}

// Get Battery State of Charge
unsigned int Caveatron_Hardware::BATT_GetSOC()
{
	unsigned int bCharge;
	switch(battGaugeType) {
		case '1':
			bCharge = MAXbatt.getSoC();
			break;
		case '2':
			bCharge = lipo.soc();
			break;
	}
	return bCharge;
}

// Get Battery Voltage
unsigned int Caveatron_Hardware::BATT_GetVolts()
{
	unsigned int bVolts;
	switch(battGaugeType) {
		case '1':
			bVolts = MAXbatt.getVCell()*1000;
			break;
		case '2':
			bVolts = lipo.voltage();
			break;
	}
	return bVolts;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     REAL TIME CLOCK FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Get date and time from RTC
void Caveatron_Hardware::RTC_GetDateTime()
{
	switch(clockType) {
		case '1':
			byte zero = 0x00;

			// Reset the register pointer
			Wire1.beginTransmission(DS3231_ADDRESS);
			Wire1.write(zero);
			Wire1.endTransmission();
			Wire1.requestFrom((byte)DS3231_ADDRESS, (byte)7);

			// Read values
			RTCsecond = bcdToDec(Wire1.read());
			RTCminute = bcdToDec(Wire1.read());
			RTChour = bcdToDec(Wire1.read() & 0b111111); //24 hour time

			int weekDay = bcdToDec(Wire1.read()); //0-6 -> sunday - Saturday
			RTCday = bcdToDec(Wire1.read());
			RTCmonth = bcdToDec(Wire1.read());
			RTCyear = bcdToDec(Wire1.read());

			break;
	}
}

// Set date and time on RTC
void Caveatron_Hardware::RTC_SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute)
{
	switch(clockType) {
		case '1':
			byte byteSecond =      00; //0-59
			byte byteMinute =      nMinute; //0-59
			byte byteHour =        nHour; //0-23
			byte byteWeekDay =     1; //1-7
			byte byteDay =         nDay; //1-31
			byte byteMonth =       nMonth; //1-12
			byte byteYear  =       nYear; //0-99
			byte zero = 0x00;

			Wire1.beginTransmission(DS3231_ADDRESS);
			Wire1.write(zero); //stop Oscillator

			Wire1.write(decToBcd(byteSecond));
			Wire1.write(decToBcd(byteMinute));
			Wire1.write(decToBcd(byteHour));
			Wire1.write(decToBcd(byteWeekDay));
			Wire1.write(decToBcd(byteDay));
			Wire1.write(decToBcd(byteMonth));
			Wire1.write(decToBcd(byteYear));
			Wire1.write(zero); //start
			Wire1.endTransmission();
			break;
	}
}

// Get temperature from RTC sensor
float Caveatron_Hardware::RTC_GetTemperature()
{
  	switch(clockType) {
		case '1':
			//temp registers (11h-12h) get updated automatically every 64s
			Wire1.beginTransmission(DS3231_ADDRESS);
			Wire1.write(0x11);
			Wire1.endTransmission();
			Wire1.requestFrom((byte)DS3231_ADDRESS, (byte)2);
			if(Wire1.available()) {
				byte tMSB = Wire1.read(); //2's complement int portion
				byte tLSB = Wire1.read(); //fraction portion
				float temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
				temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
				return temp3231;
			}
			return 0;
			break;
	}
}

//Value conversion functions
byte Caveatron_Hardware::bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

byte Caveatron_Hardware::decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                       PIEZO BUZZER FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize Piezo Buzzer Hardware
void Caveatron_Hardware::BUZZ_Init()
{
	// Set parameters for piezo buzzer
	switch(buzzerType) {
		case '1':
			BUZZpin=12;
			break;
	}
	pinMode(BUZZpin, OUTPUT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      SD CARD STORAGE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize SD Hardware Parameters
void Caveatron_Hardware::SD_Init()
{
	// Set chip select for storage
	switch(sdType) {
		case '1':
			SD_CSpin = 53;
			SPIspeed = 1; // 1 = FULL_SPEED, 2 = HALF_SPEED, etc.
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                         EEPROM FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Write array of floating point values to EEPROM
void Caveatron_Hardware::EEPROM_writeFloatArray(uint16_t address, float values[], int numvalues)
{
  byte aBytes[30];
  #if defined(_SAM3XA_)
	union u_tag {
		byte b[4];
		float fval;
	} u;
	for (int i=0;i<numvalues;i++) {
		u.fval = values[i];
		for(int j=0;j<4;j++) aBytes[j+i*4] = u.b[j];
	}
	Write_AT24Cxx_Page(address, aBytes, numvalues*4);
  #endif
}

// Read single floating point value from EEPROM
float Caveatron_Hardware::EEPROM_readFloat(uint16_t address)
{
  float value;
  #if defined(_SAM3XA_)
	union u_tag {
		byte b[4];
		float fval;
	} u;
	for(int i=0;i<4;i++) u.b[i] = Read_AT24Cxx_Byte(address+i);
	value = u.fval;
  #endif
  return value;
}

// Read character array from EEPROM
String Caveatron_Hardware::EEPROM_readCharArray(uint16_t address, int numvalues)
{
  char b[11];
  #if defined(_SAM3XA_)
	for(int i=0;i<numvalues;i++) b[i] = Read_AT24Cxx_Byte(address+i);
	b[numvalues+1] = '\0';
  #endif
  String value(b);
  return value;
}

// Data page write to EEPROM
void Caveatron_Hardware::Write_AT24Cxx_Page(int iAddr, byte* iData, int iLength)
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

// Data page read from EEPROM
int Caveatron_Hardware::Read_AT24Cxx_Byte(int iAddr) {
  uint8_t rdata = 0xFF;
  Wire1.beginTransmission(AT24C32_EEPROM_ADDR);
  Wire1.write((int)(iAddr >> 8)); // MSB
  Wire1.write((int)(iAddr & 0xFF)); // LSB
  Wire1.endTransmission();
  Wire1.requestFrom(AT24C32_EEPROM_ADDR, 1);
  if (Wire1.available()) rdata = Wire1.read();
  return rdata;
}
