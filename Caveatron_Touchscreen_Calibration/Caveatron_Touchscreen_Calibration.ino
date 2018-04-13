// Caveatron_Touchscreen_Calibration
// 2018-04-11
//
// Adapted from URTouch_Calibration 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program can be used to calibrate the touchscreen
// of the display modules.
// This program requires the UTFT library and a touch
// screen module that is compatible with UTFT.
//
// It is assumed that the display module is connected to an
// appropriate shield or that you know how to change the pin 
// numbers in the setup.
//
// Instructions will be given on the display.
//
// Parameters set for use with Caveatron with options for supported touchscreen types

#include <UTFT.h>
#include <URTouch.h>

// ************************************
// CHOOSE TOUCHSCREEN TYPE
// 1 = Original Coldtears 3.5” 480x320 LCD (With original Font IC. No longer produced – legacy only)
// 2 = Updated Coldtears 3.5” 480x320 LCD (With new Font IC and using UTFT_GHL library)
// 3 = Coldtears 4.0" 480x320 LCD
// ************************************

#define DISPLAY_TYPE 2


// Initialize display
// 
UTFT    myGLCD(25,26,27,28);

// Initialize touchscreen
//
URTouch  myTouch( 6, 5, 32, 3, 2);

// ************************************
// DO NOT EDIT ANYTHING BELOW THIS LINE
// ************************************

// Declare which fonts we will be using
extern uint8_t SmallFont[];

uint32_t cx, cy;
uint32_t rx[8], ry[8];
uint32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center;
uint32_t calx, caly, cals;
char buf[13];

void setup()
{
  switch(DISPLAY_TYPE) {
    case 1:
      myGLCD.setDisplayModel(CTE35IPS);
      myGLCD.invert_colors = false;
    break;
    case 2:
      myGLCD.setDisplayModel(CTE35IPS);
      myGLCD.invert_colors = true;
    break;
    case 3:
      myGLCD.setDisplayModel(CTE40);
      myGLCD.invert_colors = false;
    break;
  }
    
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  myTouch.InitTouch(PORTRAIT);
  dispx=myGLCD.getDisplayXSize();
  dispy=myGLCD.getDisplayYSize();
  text_y_center=(dispy/2)-6;
}

void drawCrossHair(int x, int y)
{
  myGLCD.drawRect(x-10, y-10, x+10, y+10);
  myGLCD.drawLine(x-5, y, x+5, y);
  myGLCD.drawLine(x, y-5, x, y+5);
}

void readCoordinates()
{
  int iter = 5000;
  int failcount = 0;
  int cnt = 0;
  uint32_t tx=0;
  uint32_t ty=0;
  boolean OK = false;
  
  while (OK == false)
  {
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("*  PRESS  *", CENTER, text_y_center);
    while (myTouch.dataAvailable() == false) {}
    myGLCD.print("*  HOLD!  *", CENTER, text_y_center);
    while ((myTouch.dataAvailable() == true) && (cnt<iter) && (failcount<10000))
    {
      myTouch.calibrateRead();
      if (!((myTouch.TP_X==65535) || (myTouch.TP_Y==65535)))
      {
        tx += myTouch.TP_X;
        ty += myTouch.TP_Y;
        cnt++;
      }
      else
        failcount++;
    }
    if (cnt>=iter)
    {
      OK = true;
    }
    else
    {
      tx = 0;
      ty = 0;
      cnt = 0;
    }
    if (failcount>=10000)
      fail();
  }

  cx = tx / iter;
  cy = ty / iter;
}

void calibrate(int x, int y, int i)
{
  myGLCD.setColor(255, 255, 255);
  drawCrossHair(x,y);
  myGLCD.setBackColor(255, 0, 0);
  readCoordinates();
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("* RELEASE *", CENTER, text_y_center);
  myGLCD.setColor(80, 80, 80);
  drawCrossHair(x,y);
  rx[i]=cx;
  ry[i]=cy;
  while (myTouch.dataAvailable() == true) {}
}

void waitForTouch()
{
  while (myTouch.dataAvailable() == true) {}
  while (myTouch.dataAvailable() == false) {}
  while (myTouch.dataAvailable() == true) {}
}

void toHex(uint32_t num)
{
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = 'U';
  buf[11] = 'L';
  buf[12] = 0;
  for (int zz=9; zz>1; zz--)
  {
    if ((num & 0xF) > 9)
      buf[zz] = (num & 0xF) + 55;
    else
      buf[zz] = (num & 0xF) + 48;
    num=num>>4;
  }
}

void startup()
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, dispx-1, 13);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, dispx-1, 14);
  myGLCD.print("Touchscreen Calibration for Caveatron", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);

  if (dispx==220)
  {  
    myGLCD.print("Use a stylus or something", LEFT, 30);
    myGLCD.print("similar to touch as close", LEFT, 42);
    myGLCD.print("to the center of the", LEFT, 54);
    myGLCD.print("highlighted crosshair as", LEFT, 66);
    myGLCD.print("possible. Keep as still as", LEFT, 78);
    myGLCD.print("possible and keep holding", LEFT, 90);
    myGLCD.print("until the highlight is", LEFT, 102);
    myGLCD.print("removed. Repeat for all", LEFT, 114);
    myGLCD.print("crosshairs in sequence.", LEFT, 126);
    myGLCD.print("Touch screen to continue", CENTER, 162);
  }
  else
  {
    myGLCD.print("INSTRUCTIONS", CENTER, 30);
    myGLCD.print("Use a stylus or something similar to", LEFT, 50);
    myGLCD.print("touch as close to the center of the", LEFT, 62);
    myGLCD.print("highlighted crosshair as possible. Keep", LEFT, 74);
    myGLCD.print("as still as possible and keep holding", LEFT, 86);
    myGLCD.print("until the highlight is removed. Repeat", LEFT, 98);
    myGLCD.print("for all crosshairs in sequence.", LEFT, 110);

    myGLCD.print("Further instructions will be displayed", LEFT, 134);
    myGLCD.print("when the calibration is complete.", LEFT, 146);

    myGLCD.print("Do NOT use your finger as a calibration", LEFT, 170);
    myGLCD.print("stylus or the result WILL BE imprecise.", LEFT, 182);

    myGLCD.print("Adapted from URTouch_Calibration", LEFT, 290);
    myGLCD.print("by Rinky-Dink Electronics, Henning Karlsen", LEFT, 302);

    myGLCD.setColor(255, 255, 0);
    myGLCD.print("Touch screen to continue", CENTER, 226);
  }

  waitForTouch();
  myGLCD.clrScr();
}

void done()
{
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, dispx-1, 13);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, dispx-1, 14);
  myGLCD.print("URTouch Calibration", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);
  
  if (dispx==220)
  {  
    myGLCD.print("To use the new calibration", LEFT, 30);
    myGLCD.print("settings you must edit the", LEFT, 42);
    myGLCD.setColor(160, 160, 255);
    myGLCD.print("URTouchCD.h", LEFT, 54);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("file and change", 88, 54);
    myGLCD.print("the following values. The", LEFT, 66);
    myGLCD.print("values are located right", LEFT, 78);
    myGLCD.print("below the opening comment.", LEFT, 90);
    myGLCD.print("CAL_X", LEFT, 110);
    myGLCD.print("CAL_Y", LEFT, 122);
    myGLCD.print("CAL_S", LEFT, 134);
    toHex(calx);
    myGLCD.print(buf, 75, 110);
    toHex(caly);
    myGLCD.print(buf, 75, 122);
    toHex(cals);
    myGLCD.print(buf, 75, 134);
  }
  else
  {  
    myGLCD.print("CALIBRATION COMPLETE", CENTER, 30);
    myGLCD.print("Record the values below to load to the Caveatron", LEFT, 62);
    myGLCD.print("using the calibration parameter uploader.", LEFT, 74);

    myGLCD.print("CAL_X", LEFT, 150);
    myGLCD.print("CAL_Y", LEFT, 162);
    myGLCD.print("CAL_S", LEFT, 174);

    toHex(calx);
    myGLCD.print(buf, 75, 150);
    toHex(caly);
    myGLCD.print(buf, 75, 162);
    toHex(cals);
    myGLCD.print(buf, 75, 174);
  }
  
}

void fail()
{
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, dispx-1, 13);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, dispx-1, 14);
  myGLCD.print("URTouch Calibration FAILED", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);
  
  myGLCD.print("Unable to read the position", LEFT, 30);
  myGLCD.print("of the press. This is a", LEFT, 42);
  myGLCD.print("hardware issue and can", 88, 54);
  myGLCD.print("not be corrected in", LEFT, 66);
  myGLCD.print("software.", LEFT, 78);
  
  while(true) {};
}

void loop()
{
  startup();
  
  myGLCD.setColor(80, 80, 80);
  drawCrossHair(dispx-11, 10);
  drawCrossHair(dispx/2, 10);
  drawCrossHair(10, 10);
  drawCrossHair(dispx-11, dispy/2);
  drawCrossHair(10, dispy/2);
  drawCrossHair(dispx-11, dispy-11);
  drawCrossHair(dispx/2, dispy-11);
  drawCrossHair(10, dispy-11);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("***********", CENTER, text_y_center-12);
  myGLCD.print("***********", CENTER, text_y_center+12);

  calibrate(10, 10, 0);
  calibrate(10, dispy/2, 1);
  calibrate(10, dispy-11, 2);
  calibrate(dispx/2, 10, 3);
  calibrate(dispx/2, dispy-11, 4);
  calibrate(dispx-11, 10, 5);
  calibrate(dispx-11, dispy/2, 6);
  calibrate(dispx-11, dispy-11, 7);
  
  cals=(long(dispy-1)<<12)+(dispx-1);

  px = abs(((float(rx[2]+rx[4]+rx[7])/3)-(float(rx[0]+rx[3]+rx[5])/3))/(dispy-20));  // PORTRAIT

  clx = (((rx[0]+rx[3]+rx[5])/3));  // PORTRAIT
  crx = (((rx[2]+rx[4]+rx[7])/3));  // PORTRAIT

  if (clx<crx)
  {
    clx = clx - (px*10);
    crx = crx + (px*10);
  }
  else
  {
    clx = clx + (px*10);
    crx = crx - (px*10);
  }
  
  py = abs(((float(ry[5]+ry[6]+ry[7])/3)-(float(ry[0]+ry[1]+ry[2])/3))/(dispx-20));  // PORTRAIT

  cty = (((ry[5]+ry[6]+ry[7])/3));  // PORTRAIT
  cby = (((ry[0]+ry[1]+ry[2])/3));  // PORTRAIT

  if (cty<cby)
  {
    cty = cty - (py*10);
    cby = cby + (py*10);
  }
  else
  {
    cty = cty + (py*10);
    cby = cby - (py*10);
  }
  
  calx = (long(clx)<<14) + long(crx);
  caly = (long(cty)<<14) + long(cby);

  done();
  while(true) {}
}
