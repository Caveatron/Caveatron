/*
  Caveatron_GUI.h 
  Version 0.95a
  Joe Mitchell
  2017-10-17
  
  This library contains the functions related to the Caveatron GUI including creating standard user interface controls and displays, and printing text.
  The library also has functions to handle general GUI operations such as setting/clearing displays of GUI object and setting GUI object parameters.
*/


#ifndef Caveatron_GUI_h
#define Caveatron_GUI_h

#include "Caveatron_Hardware.h"
#include <URTouch.h> //For firmware 0.9g, 0.95b and later
//#include <UTouch_Caveatron.h> //Edited for firmware 0.9d, 0.9e, 0.9f, and 0.95a
//#include <UTouch.h> //Uncomment for firmware 0.9c and earlier
#include <UTFT.h>
#include <UTFT_CTE.h>
//#include <UTFT_GHL.h>


#include "Arduino.h"


//#define GUI_Handler TC3_Handler
//#define GUI_timer TC3_IRQn


#define LEFT_J 0
#define RIGHT_J 1
#define CENTER_J 2

#define maxobjects 100
#define ticksPerSecond 8

#define optVisible true
#define optInvisible false

#define GUI_State_Unpressed false
#define GUI_State_Pressed true
#define Switch_Down false
#define Switch_Up true

#define OnPress false

#define OnRelease true

#define GUI_Type_void 0
#define GUI_Type_button 1
#define GUI_Type_iconbutton 2
#define GUI_Type_hiddenbutton 3
#define GUI_Type_panel 4
#define GUI_Type_TextInputBox 5
#define GUI_Type_toggleswitch 6
#define GUI_Type_ProgressBar 7
#define GUI_Type_listbox 8
#define GUI_Type_listboxrow 9


//*********************************
// COLORS
//*********************************
// Caveatron color palette
#define BLACK_STD		0x0000
#define WHITE_STD		0xFFFF
#define RED_STD			0xF800
#define RED_DRK			0xC800
#define RED_BLK			0x6000
#define RED_GRY			0x89E7
#define GREEN_STD		0x07E0
#define GREEN_DRK		0x04A0
#define BLUE_STD		0x001F
#define YELLOW_STD		0xFFE0
//#define BUTTON_STD		0x6414
#define BUTTON_DRK		0x3A6B	//58,76,94
#define BUTTON_STD		0x5371	//85,110,136
#define BUTTON_LGT		0xADFA
#define BAR_TOP			0x8BB6
#define GRAY_025		0xBDF7
#define GRAY_050		0x7BEF
#define GRAY_075		0x39E7
#define BLUE_BAR		0x761E //117,194,244
#define BLUE_SEL		0x7D19 //125,162,206
#define GREEN_BAR		0x2F28 //43,229,64
#define TAN_STD			0xC5B3 //194,181,155
#define	TAN_LGT			0xFF79 //255,238,204


class Caveatron_GUI
{
	public:
		Caveatron_GUI();		
		//Caveatron_GUI(UTFT * lcd, UTouch * touch, UTFT_CTE * fontIC);
		Caveatron_GUI(Caveatron_Hardware * ctHW, UTFT * lcd, URTouch * touch);

// Text Printing Functions


		void print(String st, int x, int y, int font_number, int font_spacing, int justify=0);
		void print(String st, int x, int y, int font_number, int font_spacing, int justify, word font_color, word back_color);
		void printNumI(long num, int x, int y, int font_number, int font_spacing, int justify=0);
		void printNumI(long num, int x, int y, int font_number, int font_spacing, int justify, word font_color, word back_color);
		void printNumF(double num, byte dec, int x, int y, int font_number, int font_spacing, int justify=0);
		void printNumF(double num, byte dec, int x, int y, int font_number, int font_spacing, int justify, word font_color, word back_color);
		int getFontHeight(int font_number);
		void showImage(int x, int y, int iadd);


// GUI Functions

	void 	InitGUI();	
	int 	addButton(int x, int y, int xs, int ys, word bcolor, String btext, int bfont, bool visible, int URN);
    int 	addButton(int x, int y, int xs, int ys, word bcolor, String btext, String btext1, int bfont, bool visible, int URN);
	int 	addPanel(word x,word y,word xs,word ys,long colour,long borcolour,long textcolour,String text, int font,bool visible,int URN);
	int		addIconButton(int x, int y, int xs, int ys, word bcolor, String btext, int bfont, int yt, int iadd, int ixsize, int yi, bool visible, int URN);
	int 	addHiddenButton(int x, int y, int xs, int ys, bool visible, int URN);
	int 	addToggleSwitch(int x, int y, int xs, int ys, int yt, word bcolor, word tcolor, String btext, String btexttop, String btextbot, int bfont, int initposition, bool visible, int URN);
	int  	addTextInputBox(int x, int y,int xs,int ys,String textbox_text,int font,int font_spacing,int justify,long textcolour,int inputlength,bool visible,int URN);
	int 	addProgressBar(int x, int y, int xs,int ys, int pbl, String text, int font, long textcolour, bool visible, int URN);
	int 	addListBox(int x, int y, int xs, int data1, int bfont, bool visible, int URN);
	void    drawPanel(int objectnumber);	
	void    preserveColors();
  	void    restoreColors();
  	void    updatingScreen();
  	void    finishedUpdatingScreen();
  	void    startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);
  	void    stopTimer();
  	void    restartTimer();
  	void    ClearObjects(int first,int last);
  	void    clearAllObjects(); 
	void	clearObjectArea(int objectNumber);
	void 	redrawAllObjects();
  	void    redrawObject(int objectNumber);
	void    makeObjectInvisible(int objectNumber);
	void    makeObjectVisible(int objectNumber);
	void 	toggleButton(int objectON, int objectOFF);
	void 	updateProgressBar(int objectNumber, int fillLevel);
	bool    withinBounds(int chkX,int chkY,int X, int Y, int XS, int YS);
  	void    drawButton(int objectNumber);
	void    drawIconButton(int objectNumber);
	void 	drawTextInputBox(int objectNumber);
  	void    drawHiddenButton(int objectNumber);
	void	drawToggleSwitch(int objectNumber);
	void	drawProgressBar(int objectNumber);
	void	drawListBox(int objectNumber);
	void	drawListBoxRow(int objectNumber);
	void	addcharTextInputBox(int objectNumber,String newChar);
	void	delcharTextInputBox(int objectNumber);
	void	clearTextInputBox(int objectNumber);
  	bool    checkButton(int objectNumber, bool pressType);
  	int     checkAllButtons(bool pressType);
	void	touchRead();

// GUI variables
        
  byte    GUIobject_type[maxobjects];
  int     GUIobject_link[maxobjects];
  byte    GUIobject_linkoption[maxobjects];
  int     GUIobject_UniqueReference[maxobjects];

  int     GUIobject_x[maxobjects];
  int     GUIobject_y[maxobjects];
  int     GUIobject_xs[maxobjects];
  int     GUIobject_ys[maxobjects];
  long    GUIobject_xo[maxobjects];
  long    GUIobject_yo[maxobjects];
  long    GUIobject_colour[maxobjects];
  long    GUIobject_colour1[maxobjects];
  long    GUIobject_borcolour[maxobjects];
  long    GUIobject_textcolour[maxobjects];
  long    GUIobject_pressedcolour[maxobjects];
  long    GUIobject_pressedtextcolour[maxobjects];
  int     GUIobject_font[maxobjects];
  String  GUIobject_text[maxobjects];
  String  GUIobject_text1[maxobjects];
  String  GUIobject_text2[maxobjects];
  String  GUIobject_lbtext[maxobjects][7];
  int 	  GUIobject_texty[maxobjects];
  int     GUIobject_justify[maxobjects];
  int	  GUIobject_iconaddress[maxobjects];
  int	  GUIobject_iconx[maxobjects];
  int	  GUIobject_icony[maxobjects];
  int     GUIobject_currentstate[maxobjects];
  int     GUIobject_currentposition[maxobjects];
  bool    GUIobject_visible[maxobjects];
  bool    GUIobject_changed[maxobjects];
  int     GUIobject_data1[maxobjects];
  int     GUIobject_data2[maxobjects];
  int     GUIobject_data3[maxobjects];
  int     GUIobject_data4[maxobjects];
  int     GUIobject_data5[maxobjects];
  int     GUIobject_data6[maxobjects];
  int	  GUIobject_progressbarfill[maxobjects];
  String  GUIobject_Stringdata1[maxobjects];
  String  *GUIobject_StringPointer[maxobjects];

  int     GUI_firstObject;
  int     GUI_objects;
  byte    GUI_br,GUI_bg,GUI_bb,GUI_fr,GUI_fg,GUI_Fb;
  
  unsigned long int  GUI_prevTime;
  unsigned long int  GUI_thisTime;


// End of GUI Functions and variables  
 
       long	touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;
		long	touch_disp_x_size, touch_disp_y_size;
		byte    set_rs,set_wr,set_rst,set_cs;
		bool    anyButtonPressed;
		word	TP_X ,TP_Y;



	private:


	protected:
		
		Caveatron_Hardware    * caveatronHW;
		UTFT    * mylcd;
        URTouch  * mytouch;
		byte fcolorr,fcolorg,fcolorb;
		byte bcolorr,bcolorg,bcolorb;

};

#endif


