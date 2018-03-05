/*
  Caveatron_Hardware.h 
  Version 1.0
  Joe Mitchell
  2018-02-26
  
  This library contains all functions to interface between the main code and the hardware used for the Caveatron. 
  The library is setup to allow for the use of different hardware which is set by a hardware code stored on the EEPROM with the calibration parameters.
*/


#ifndef Caveatron_Hardware_h
#define Caveatron_Hardware_h

#include <UTFT.h>
#include <URTouch.h>
#include <UTFT_CTE.h>
#include <UTFT_GHL.h>

#include <LSM303.h>
#include <MAX17043.h>
#include <SparkFunBQ27441.h>
#include "Arduino.h"

#define DS3231_ADDRESS 0x68
#define AT24C32_EEPROM_ADDR 0x57

// EEPROM Calibration Parameter Addresses
// 1 page is 0x020 in length
#define ADDR_SERIAL_NUM     0x00        //Uses 1 page
#define ADDR_HARDWARE_CODE    0x020     //Uses 1 page
#define ADDR_SCREEN_CAL     0x080       //Uses 3 pages
#define ADDR_ACC_CAL      0x200         //Uses 4 pages
#define ADDR_MAG_ALIGNCAL_NOLID 0x300   //Uses 2 pages
#define ADDR_MAG_ALIGNCAL_LIDXV 0x340   //Uses 2 pages
#define ADDR_MAG_ALIGNCAL_LIDSW 0x380   //Uses 2 pages
#define ADDR_MAG_HSCAL_NOLID  0x500     //Uses 4 pages
#define ADDR_MAG_HSCAL_LIDXV  0x580     //Uses 4 pages
#define ADDR_MAG_HSCAL_LIDSW  0x660     //Uses 4 pages
#define ADDR_LIDXV_ORIENTCAL   0x820     //Uses 1 page
#define ADDR_LIDSW_ORIENTCAL   0x840     //Uses 1 page
#define ADDR_LRF_RANGECAL   0x880     //Uses 1 page


class Caveatron_Hardware
{
	public:
		Caveatron_Hardware(UTFT * lcd, URTouch * touch);		

		// Hardware Initialization
		void Init();
		void Init(char hRev, char hCode[]);
		
		// TFT Touchscreen Display Functions
		void LCD_Init();
		void LCD_PrintText(String st, int x, int y, int font_number);
		void LCD_SetTextSpacing(int font_spacing);
		int LCD_GetTextLength(String st, int font_number);
		void LCD_ShowImage(int x, int y, int addr);
				
		// LRF Functions
		void LRF_Init();
		void LRF_PowerOn();
		void LRF_PowerOff();
		void LRF_LaserOn();
		void LRF_LaserOff();
		void LRF_SetMode(boolean mode);
		void LRF_Fire();
		uint8_t LRF_Read();
		uint8_t LRF_GetRange();
		
		// LRF Configuration Functions
		void LIDAR_Init(uint8_t lidarType);
		
		// IMU Compass Functions
		void IMU_Init();
		void IMU_Read();
		
		// 	Battery Monitor Functions
		void BATT_Init();
		unsigned int BATT_GetLevel();
		unsigned int BATT_GetSOC();
		unsigned int BATT_GetVolts();

		// 	Clock and Temperature Functions
		void RTC_GetDateTime();
		void RTC_SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute);
		float RTC_GetTemperature();
		
		// 	Piezo Buzzer Functions
		void BUZZ_Init();
		
		// 	Storage Functions
		void SD_Init();
		
		// 	EEPROM Functions
		void EEPROM_writeFloatArray(uint16_t address, float values[], int numvalues);
		float EEPROM_readFloat(uint16_t address);
		String EEPROM_readCharArray(uint16_t address, int numvalues);
		
		
		//Font definition variables
		int FONT_13;										 
		int FONT_15;										 
		int FONT_19;											 
		int FONT_22;											 
		int FONT_28;											 
		int FONT_34;											 
		int FONT_43;											 
		int FONT_52;											 
		int FONT_74;											 
		int FONT_112;
		
		//Icon and Graphic address variables
		int iSHOT;
		int iPASSAGE;
		int iROOM;
		int iMANUAL;
		int iSURVEY;
		int iSETTINGS;
		int iTIME;
		int iSD;
		int iCALIBRATE;
		int iLIDAR;
		int iABOUT;
		int iRETURN;
		int gLOGO;
		
		// Hardware variables
        float LRFdistance; //Distance in m
		int LRFtimeout;	//Time to LRF time out in msec
		int LRFperiod;	//Minimum time between LRF reading attempts in msec
		float LIDARFrontDist, LIDARHeight, LIDARLateral;
		int LIDARAngleOffset, minLIDARrange, maxLIDARrange, maxLIDARsignal, ptsLIDARrot;
		int	IMU_a_x, IMU_a_y, IMU_a_z, IMU_m_x, IMU_m_y, IMU_m_z, IMU_g_x, IMU_g_y, IMU_g_z; //Variables to store IMU readings
		int IMU_hypmin, IMU_hypmax, IMU_hypUbound; //Bounds for magnetometer calibration
		float boxRearDist, boxLeftDist, boxRightDist, boxBaseDist; //Distances from LRF measurement point to edges of the enclosure
		int RTCsecond, RTCminute, RTChour, RTCday, RTCmonth, RTCyear;
		uint8_t BUZZpin, SD_CSpin, SPIspeed;
		
		String serialNumber;

	private:

		int strstart(const char *s1, const char * PROGMEM s2);
		byte bcdToDec(byte val);
		byte decToBcd(byte val);
		void Write_AT24Cxx_Page(int iAddr, byte* iData, int iLength);
		int Read_AT24Cxx_Byte(int iAddr);
	
		// Hardware variables
        char hardwareRev;
		char sdType;
		char displayType;
		char graphicsType;
		char lrfType;
		char compassType;
		char imuType;
		char batteryType;
		char battGaugeType;
		char clockType;
		char buzzerType;
		char wirelessType;
		
		uint8_t LRFPowerPin;
		uint8_t LRFOnPin;
		float LRFminRange;
		float LRFmaxRange;
		boolean LRFmode;
		int LRFstartTime;
		
		
		unsigned int BATTERY_CAPACITY;
		unsigned int CUTOFF_VOLTAGE;
	

	protected:
		UTFT    * mylcd;
        URTouch  * mytouch;
		UTFT_CTE * myFont_CTE;
		UTFT_GHL * myFont_GHL;
		LSM303 compass;
};

#endif


