/*
  Caveatron_GUI.cpp 
  Version 0.99
  Joe Mitchell
  2018-01-25
  
  This library contains the functions related to the Caveatron GUI including creating standard user interface controls and displays, and printing text.
  The library also has functions to handle general GUI operations such as setting/clearing displays of GUI object and setting GUI object parameters.
*/


#include "Arduino.h"
#include "Caveatron_GUI.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   	Library Instancing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

Caveatron_GUI::Caveatron_GUI()
{}

Caveatron_GUI::Caveatron_GUI(Caveatron_Hardware * ctHW, UTFT * lcd, URTouch * touch) 
{
  	caveatronHW = (Caveatron_Hardware *) ctHW;
	mylcd = (UTFT *) lcd;
  	mytouch = (URTouch *) touch;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   Font and Image Functions for SPI Flash
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Print regular text with text and background color set seperately
void Caveatron_GUI::print(String st, int x, int y, int font_number, int font_spacing, int justify)
{
	int text_length;

	caveatronHW->LCD_SetTextSpacing(font_spacing);
	
	if (justify==RIGHT_J)
	{
		text_length = caveatronHW->LCD_GetTextLength(st, font_number);	
		caveatronHW->LCD_PrintText(st, x-text_length, y, font_number);
	}
	else if (justify==CENTER_J)
	{
		text_length = caveatronHW->LCD_GetTextLength(st, font_number);
		caveatronHW->LCD_PrintText(st, x-(text_length/2), y, font_number);
	}
	else	caveatronHW->LCD_PrintText(st, x, y, font_number);

}

// Print regular text with defined text and background colors
void Caveatron_GUI::print(String st, int x, int y, int font_number, int font_spacing, int justify, word font_color, word back_color)
{
	int text_length;

	mylcd->setColor(font_color);
	mylcd->setBackColor(back_color);
	
	caveatronHW->LCD_SetTextSpacing(font_spacing);
	
	if (justify==RIGHT_J)
	{
		text_length = caveatronHW->LCD_GetTextLength(st, font_number);	
		caveatronHW->LCD_PrintText(st, x-text_length, y, font_number);
	}
	else if (justify==CENTER_J)
	{
		text_length = caveatronHW->LCD_GetTextLength(st, font_number);
		caveatronHW->LCD_PrintText(st, x-(text_length/2), y, font_number);
	}
	else	caveatronHW->LCD_PrintText(st, x, y, font_number);
	
}

// Print integer variable with text and background color set seperately
void Caveatron_GUI::printNumI(long num, int x, int y, int font_number, int font_spacing, int justify)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		st[0]=48;
		st[1]=0;
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y, font_number, font_spacing, justify);
}

// Print integer variable with defined text and background colors
void Caveatron_GUI::printNumI(long num, int x, int y, int font_number, int font_spacing, int justify, word font_color, word back_color)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
		if (num==0)
	{
		st[0]=48;
		st[1]=0;
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y, font_number, font_spacing, justify, font_color, back_color);
}

// Print floating point variable, with defined number of digits after decimal and with text and background color set seperately
void Caveatron_GUI::printNumF(double num, byte dec, int x, int y, int font_number, int font_spacing, int justify)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	mylcd->_convert_float(st, num, 0, dec);

	print(st,x,y, font_number, font_spacing, justify);
}

// Print floating point variable, with defined number of digits after decimal and with defined text and background colors
void Caveatron_GUI::printNumF(double num, byte dec, int x, int y, int font_number, int font_spacing, int justify, word font_color, word back_color)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	mylcd->_convert_float(st, num, 0, dec);

	print(st,x,y, font_number, font_spacing, justify, font_color, back_color);
}

// Get display height of a given font in pixels
int Caveatron_GUI::getFontHeight(int font_number)
{
	int font_y;
	if (font_number==caveatronHW->FONT_13) font_y=13;
	else if (font_number==caveatronHW->FONT_15) font_y=15;
	else if (font_number==caveatronHW->FONT_19) font_y=19;
	else if (font_number==caveatronHW->FONT_22) font_y=22;
	else if (font_number==caveatronHW->FONT_28) font_y=28;
	else if (font_number==caveatronHW->FONT_34) font_y=34;
	else if (font_number==caveatronHW->FONT_43) font_y=43;
	else if (font_number==caveatronHW->FONT_52) font_y=52;
	else if (font_number==caveatronHW->FONT_74) font_y=74;
	else if (font_number==caveatronHW->FONT_112) font_y=112;
	return font_y;
}

void Caveatron_GUI::showImage(int x, int y, int iadd) {
    caveatronHW->LCD_ShowImage(x,y,iadd);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            BUTTON FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Standard button
//
// function add BUTTON
//
// variable			  	stored in			description
//
// int x				[x]				{ location x }
// int y				[y]				{ location y }
// int xs				[xs]				{ size of the panel x in pixels }
// int ys				[ys]				{ size of the panel y in pixels }
// long bcolor				[bcolor]			{ colour of the button }
// String btext				[btext]				{ text line 1 in button }
// String btext1			[btext1]			{ text line 2 in button (optional)}
// int bfont				[bfont]				{ font for text }
// int URN				[URN]				{ unique reference number for this button }
// bool visible   	    		[visible]    			{ visibility flag }
// bool changed				[changed]			{ object has changed since drawn or not drawn at all }
//

// Single text line button
int Caveatron_GUI::addButton(int x, int y, int xs, int ys, word bcolor, String btext, int bfont, bool visible, int URN)
{
  //
  // "Button" is a child of "Panel" call panel then add the additional attributes.
  //
  addPanel(x,y,xs,ys,bcolor,WHITE_STD,WHITE_STD,btext,bfont,visible,URN);;
  GUIobject_type[GUI_objects]=GUI_Type_button;
  GUIobject_pressedcolour[GUI_objects]=GRAY_025;
  GUIobject_pressedtextcolour[GUI_objects]=WHITE_STD;
  GUIobject_currentstate[GUI_objects]=GUI_State_Unpressed;
  GUIobject_data4[GUI_objects] = 1;
  return GUI_objects;
}

// Two text line button
int Caveatron_GUI::addButton(int x, int y, int xs, int ys, word bcolor, String btext, String btext1, int bfont, bool visible, int URN)
{
    //
    // "Button" is a child of "Panel" call panel then add the additional attributes.
    //
    addPanel(x,y,xs,ys,bcolor,WHITE_STD,WHITE_STD,btext,bfont,visible,URN);
    GUIobject_type[GUI_objects]=GUI_Type_button;
    GUIobject_pressedcolour[GUI_objects]=GRAY_025;
    GUIobject_pressedtextcolour[GUI_objects]=WHITE_STD;
    GUIobject_currentstate[GUI_objects]=GUI_State_Unpressed;
    GUIobject_text1[GUI_objects]=btext1;
    GUIobject_data4[GUI_objects] = 2;
    return GUI_objects;
}

void Caveatron_GUI::drawButton(int objectNumber){
  if (GUIobject_visible[objectNumber]){

    // Draw the outer filled button
      if (GUIobject_currentstate[objectNumber]==GUI_State_Unpressed)
      {
		mylcd->setColor(GUIobject_colour[objectNumber]);
    	mylcd->fillRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_borcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
		int font_height = getFontHeight(GUIobject_font[objectNumber]);
        if (GUIobject_data4[objectNumber]==2) {
            print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/3)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_pressedtextcolour[objectNumber], GUIobject_colour[objectNumber]);
            print(GUIobject_text1[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(2*GUIobject_ys[objectNumber]/3)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_pressedtextcolour[objectNumber], GUIobject_colour[objectNumber]);
        } else {
            print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_pressedtextcolour[objectNumber], GUIobject_colour[objectNumber]);
        }
      }
      else 
      {
        mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
        mylcd->fillRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1, GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
		int font_height = getFontHeight(GUIobject_font[objectNumber]);
    	if (GUIobject_data4[objectNumber]==2) {
            print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/3)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
            print(GUIobject_text1[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(2*GUIobject_ys[objectNumber]/3)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
        } else {
            print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
        }
      }

  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            ICONBUTTON FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Button with an icon graphic loaded from SPI Flash IC
//
// function addIconButton
//
// variable			  	stored in			description
//
// int x				[x]					{ location x }
// int y				[y]					{ location y }
// int xs				[xs]				{ size of the panel x in pixels }
// int ys				[ys]				{ size of the panel y in pixels }
// long bcolor			[bcolor]			{ colour of the button }
// String btext			[btext]				{ text in button }
// int bfont			[bfont]				{ font for text }
// int yt				[yt]				{y position for text relative to button top}
// int iadd				[iadd]				{icon address}
// int ixsize			[ixsize]			{icon x size}
// int iy				[iy]				{y position for icon relative to button top}
// bool visible   	    [visible]    		{ visibility flag }
// int URN				[URN]				{ unique reference number for this button }
//
int Caveatron_GUI::addIconButton(int x, int y, int xs, int ys, word bcolor, String btext, int bfont, int yt, int iadd, int ixsize, int iy, bool visible, int URN) 
{
  //
  // "IconButton" is a child of "Panel" call panel then add the additional attributes.
  //
  addPanel(x,y,xs,ys,bcolor,WHITE_STD,WHITE_STD,btext,bfont,visible,URN);
  GUIobject_type[GUI_objects]=GUI_Type_iconbutton;
  GUIobject_pressedcolour[GUI_objects]=GRAY_025;
  GUIobject_pressedtextcolour[GUI_objects]=WHITE_STD;
  GUIobject_texty[GUI_objects]=yt;
  GUIobject_iconaddress[GUI_objects]=iadd;
  GUIobject_iconx[GUI_objects]=x+(xs-ixsize)/2;
  GUIobject_icony[GUI_objects]=y+iy;
  GUIobject_currentstate[GUI_objects]=GUI_State_Unpressed;
  return GUI_objects;
}


void Caveatron_GUI::drawIconButton(int objectNumber){
  if (GUIobject_visible[objectNumber]){

    // Draw the outer filled button
      if (GUIobject_currentstate[objectNumber]==GUI_State_Unpressed)
      {
	    mylcd->setColor(GUIobject_colour[GUI_objects]);
    	mylcd->fillRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_borcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
    	print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+GUIobject_texty[objectNumber],GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_pressedtextcolour[objectNumber], GUIobject_colour[objectNumber]);
        caveatronHW->LCD_ShowImage(GUIobject_iconx[objectNumber],GUIobject_icony[objectNumber],GUIobject_iconaddress[objectNumber]);
	  
	  } 
      else 
      {
        mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
        mylcd->fillRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1, GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
    	print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+GUIobject_texty[objectNumber],GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
      }

  }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            HIDDENBUTTON FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Button that has no on-screen display but does have a position and size defined
//
// function addHiddenButton
//
// variable			  	stored in			description
//
// int x				[x]					{ location x }
// int y				[y]					{ location y }
// int xs				[xs]				{ size of the panel x in pixels }
// int ys				[ys]				{ size of the panel y in pixels }
// bool visible   	    [visible]    		{ visibility flag }
// int URN				[URN]				{ unique reference number for this button }
//
int Caveatron_GUI::addHiddenButton(int x, int y, int xs, int ys, bool visible, int URN) 
{
  //
  // "HiddenButton" is a child of "Panel" call panel then add the additional attributes.
  //
  addPanel(x,y,xs,ys,BLACK_STD,BLACK_STD,BLACK_STD,"",caveatronHW->FONT_13,visible,URN);;
  GUIobject_type[GUI_objects]=GUI_Type_hiddenbutton;
  GUIobject_currentstate[GUI_objects]=GUI_State_Unpressed;
  return GUI_objects;
}


void Caveatron_GUI::drawHiddenButton(int objectNumber){
  if (GUIobject_visible[objectNumber]){

    // Draw the outer filled button
      if (GUIobject_currentstate[objectNumber]==GUI_State_Unpressed)
      {
 
	} 
      else 
      {

      }

  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            TOGGLESWITCH FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Two position toggle switch control
//
// function add TOGGLESWITCH
//
// variable			  	stored in			description
//
// int x				[x]				{ location x }
// int y				[y]				{ location y }
// int xs				[xs]				{ size of the panel x in pixels }
// int ys				[ys]				{ size of the panel y in pixels }
// int yt				[yt]				{ height of the toggle indicator in pixels }
// long bcolor				[bcolor]			{ colour of the button }
// long tcolor				[tcolor]			{ color of the toggle indicator }
// String btext				[btext]				{ text line in button }
// String btexttop			[btexttop]			{ text at top position }
// String btextbot			[btexttop]			{ text at bottom position }
// int bfont				[bfont]				{ font for text }
// int initposition		[initposition]				{ initial position for switch (down or up) }
// int URN				[URN]				{ unique reference number for this button }
// bool visible   	    		[visible]    			{ visibility flag }
// bool changed				[changed]			{ object has changed since drawn or not drawn at all }
//
int Caveatron_GUI::addToggleSwitch(int x, int y, int xs, int ys, int yt, word bcolor, word tcolor, String btext, String btexttop, String btextbot, int bfont, int initposition, bool visible, int URN)
{
  //
  // "Button" is a child of "Panel" call panel then add the additional attributes.
  //
  addPanel(x,y,xs,ys,bcolor,WHITE_STD,WHITE_STD,btext,bfont,visible,URN);
  GUIobject_type[GUI_objects]=GUI_Type_toggleswitch;
  GUIobject_pressedcolour[GUI_objects]=GRAY_025;
  GUIobject_pressedtextcolour[GUI_objects]=WHITE_STD;
  GUIobject_currentstate[GUI_objects]=GUI_State_Unpressed;
  GUIobject_currentposition[GUI_objects]=initposition;
  GUIobject_data1[GUI_objects]=yt;
  GUIobject_colour1[GUI_objects]=tcolor;
  GUIobject_text1[GUI_objects]=btexttop;
  GUIobject_text2[GUI_objects]=btextbot;
  return GUI_objects;
}

void Caveatron_GUI::drawToggleSwitch(int objectNumber){
  if (GUIobject_visible[objectNumber]){

    // Draw the button
	if (GUIobject_currentstate[objectNumber]==GUI_State_Unpressed)
	{
      if (GUIobject_currentposition[objectNumber]==Switch_Down)
      {
		//Draw main button
		mylcd->setColor(GUIobject_colour[objectNumber]);
    	mylcd->fillRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_borcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
		//Draw slider indicator
		mylcd->setColor(GUIobject_colour1[objectNumber]);
    	mylcd->fillRoundRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-GUIobject_data1[objectNumber]-2,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-2);
    	mylcd->setColor(GUIobject_borcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-GUIobject_data1[objectNumber]-2,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-2);
		//Draw text
		int font_height = getFontHeight(GUIobject_font[objectNumber]);
        print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2), GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_colour[objectNumber]);
        print(GUIobject_text1[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_data1[objectNumber]/2)-(font_height/2)+2, GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_pressedcolour[objectNumber], GUIobject_colour[objectNumber]);
		print(GUIobject_text2[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-(GUIobject_data1[objectNumber]/2)-(font_height/2)-2, GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_colour1[objectNumber]);
      }
	  else if (GUIobject_currentposition[objectNumber]==Switch_Up) 
	  {
		//Draw main button
	  	mylcd->setColor(GUIobject_colour[objectNumber]);
    	mylcd->fillRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_borcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    	mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
		//Draw slider indicator
		mylcd->setColor(GUIobject_colour1[objectNumber]);
    	mylcd->fillRoundRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+2,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2,GUIobject_y[objectNumber]+GUIobject_data1[objectNumber]+2);
    	mylcd->setColor(GUIobject_borcolour[objectNumber]);
    	mylcd->drawRoundRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+2,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2,GUIobject_y[objectNumber]+GUIobject_data1[objectNumber]+2);
		//Draw text
		int font_height = getFontHeight(GUIobject_font[objectNumber]);
        print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2), GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_colour[objectNumber]);
        print(GUIobject_text1[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_data1[objectNumber]/2)-(font_height/2)+2, GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_colour1[objectNumber]);
		print(GUIobject_text2[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-(GUIobject_data1[objectNumber]/2)-(font_height/2)-2, GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_pressedcolour[objectNumber], GUIobject_colour[objectNumber]);
	  }
	}
    else 
    {
      mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
      mylcd->fillRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1, GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
	  int font_height = getFontHeight(GUIobject_font[objectNumber]);
      print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
      print(GUIobject_text1[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_data1[objectNumber]/2)-(font_height/2)+2,GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
	  print(GUIobject_text2[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-(GUIobject_data1[objectNumber]/2)-(font_height/2)-2,GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_pressedcolour[objectNumber]);
    }

  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            TextInputBox FUNCTION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Text input box graphic for use with keypad
//
// function add TextInputBox
//
// add variable		  	stored in			description
//	
// int x				[x]					{ location x }
// int y				[y]					{ location y }
// int xs				[xs]				{ size of the panel x in pixels }
// int ys				[ys]				{ size of the panel y in pixels }
//String initial_text	[initial_text]		{initial text for textbox}
// int font				[font]				{ font for textbox text }
// int font_spacing		[font_spacing]		{ font spacing for textbox text }
// int justify			[justify]			{textbox text justification}
// long textcolour		[textcolour]		{ colour of text }
// int maxinputlength	[maxinputlength]	{ maximum input length }
// bool visible   	    [visible]    		{ visibility flag }
// int URN				[URN]				{ unique reference number for this panel }
//
int  Caveatron_GUI::addTextInputBox(int x,int y,int xs,int ys,String initial_text,int font,int font_spacing,int justify,long textcolour,int maxinputlength,bool visible,int URN){
  
  addPanel(x,y,xs,ys,GRAY_025,WHITE_STD,BLACK_STD,initial_text,font,visible,URN);

  GUIobject_type[GUI_objects]=GUI_Type_TextInputBox;  // use definition of button but change type to correct value.
  GUIobject_justify[GUI_objects] = justify;
  GUIobject_data1[GUI_objects] = initial_text.length();
  GUIobject_data2[GUI_objects] = maxinputlength;
  GUIobject_data3[GUI_objects] = font_spacing;
  return GUI_objects;
}

void Caveatron_GUI::drawTextInputBox(int objectNumber){
  int textoffset;  
  if (GUIobject_visible[objectNumber]){
	mylcd->setColor(GUIobject_colour[GUI_objects]);
	mylcd->fillRect(GUIobject_x[GUI_objects]+1,GUIobject_y[GUI_objects]+1,GUIobject_x[GUI_objects]+GUIobject_xs[GUI_objects]-1,GUIobject_y[GUI_objects]+GUIobject_ys[GUI_objects]-1); 
	mylcd->setColor(GUIobject_borcolour[objectNumber]);
	mylcd->drawRect(GUIobject_x[GUI_objects],GUIobject_y[GUI_objects],GUIobject_x[GUI_objects]+GUIobject_xs[GUI_objects],GUIobject_y[GUI_objects]+GUIobject_ys[GUI_objects]);
  	mylcd->setColor(GUIobject_textcolour[GUI_objects]);
  	mylcd->setBackColor(GUIobject_colour[GUI_objects]);
  	switch (GUIobject_justify[objectNumber]) {
  	case LEFT_J: 
	  textoffset = 5;
	  break;
  	case RIGHT_J: 
	  textoffset = -5;
	  break;
  	default:
	  textoffset = GUIobject_xs[objectNumber]/2;
	  break;
  	}
	int font_height = getFontHeight(GUIobject_font[objectNumber]);
  	print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+textoffset,GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2),GUIobject_font[objectNumber], GUIobject_data3[objectNumber], GUIobject_justify[objectNumber], GUIobject_textcolour[objectNumber], GUIobject_colour[GUI_objects]);
  }
}

void Caveatron_GUI::addcharTextInputBox(int objectNumber, String newChar){
	if (GUIobject_data1[objectNumber] < GUIobject_data2[objectNumber])
	{
		GUIobject_text[objectNumber] += newChar;
		GUIobject_data1[objectNumber]++;
		drawTextInputBox(objectNumber);
	} 
}

void Caveatron_GUI::delcharTextInputBox(int objectNumber){
	if (GUIobject_data1[objectNumber] > 0)
	{
		GUIobject_text[objectNumber] = GUIobject_text[objectNumber].substring(0, GUIobject_text[objectNumber].length() - 1);
		GUIobject_data1[objectNumber]--;
		drawTextInputBox(objectNumber);
	} 
}

void Caveatron_GUI::clearTextInputBox(int objectNumber){
    //updatingScreen();

	GUIobject_text[objectNumber] = "";
	GUIobject_data1[objectNumber] = GUIobject_text[objectNumber].length();
	drawTextInputBox(objectNumber);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            LISTBOX FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Listbox graphic for displaying lists of text in columns
//
// function add LISTBOX
//
// variable			  	stored in			description
//
// int x				[x]				{ location x }
// int y				[y]				{ location y }
// int xs				[xs]				{ width of the panel x in pixels }
// int data1				[data1]				{ maximum number of entry lines }
// int bfont				[bfont]				{ font for text }
// int URN				[URN]				{ unique reference number for this button }
// bool visible   	    		[visible]    			{ visibility flag }
// bool changed				[changed]			{ object has changed since drawn or not drawn at all }
//
int Caveatron_GUI::addListBox(int x, int y, int xs, int data1, int bfont, bool visible, int URN)
{
  addPanel(x,y,xs,(50*data1)+22,BUTTON_DRK,WHITE_STD,WHITE_STD,"",bfont,visible,URN);
  GUIobject_type[GUI_objects]=GUI_Type_listbox;
  GUIobject_textcolour[GUI_objects]=WHITE_STD;
  GUIobject_colour1[GUI_objects]=GRAY_025;
  GUIobject_data1[GUI_objects]=data1;
  for (int i=0; i<data1; i++) {
	addPanel(x+2,y+(50*i)+22,xs-4,49,BUTTON_DRK,WHITE_STD,WHITE_STD,"",bfont,visible,URN+i);
	GUIobject_type[GUI_objects]=GUI_Type_listboxrow;
	GUIobject_pressedcolour[GUI_objects]=GRAY_025;
	GUIobject_pressedtextcolour[GUI_objects]=WHITE_STD;
	GUIobject_currentstate[GUI_objects]=GUI_State_Unpressed;
  }
  return GUI_objects;
}


void Caveatron_GUI::drawListBox(int objectNumber){
  if (GUIobject_visible[objectNumber]){

    // Draw the listbox
	//Draw box
	mylcd->setColor(GUIobject_borcolour[objectNumber]);
	mylcd->drawRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
	mylcd->setColor(GUIobject_colour1[objectNumber]);
	mylcd->drawRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
	mylcd->setColor(GUIobject_colour[objectNumber]);
	//mylcd->fillRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+2,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-2);

	//Draw interior lines
	mylcd->setColor(GUIobject_colour1[objectNumber]);
	for (int i=0; i<GUIobject_data1[objectNumber]; i++) {
		mylcd->drawLine(GUIobject_x[objectNumber]+2,(GUIobject_y[objectNumber]+21) + i*50, GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2, (GUIobject_y[objectNumber]+21) +i*50);
	}
		
  }
}
  
void Caveatron_GUI::drawListBoxRow(int objectNumber){
  if (GUIobject_visible[objectNumber]){
	  // Draw list box element in unpressed state
      if (GUIobject_currentstate[objectNumber]==GUI_State_Unpressed) {
		mylcd->setColor(GUIobject_colour[objectNumber]);
	  } 
	// Draw list box element in pressed state
	 else {
		mylcd->setColor(GUIobject_pressedcolour[objectNumber]);
	  }
	mylcd->fillRect(GUIobject_x[objectNumber], GUIobject_y[objectNumber], GUIobject_x[objectNumber]+GUIobject_xs[objectNumber], GUIobject_y[objectNumber]+15); 
    mylcd->fillRect(GUIobject_x[objectNumber], GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-16, GUIobject_x[objectNumber]+GUIobject_xs[objectNumber], GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);   
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            PANEL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generic graphic panel used by all GUI elements
//
// function add PANEL
//
// variable			 stored in		  description
//
// word x			[x]			{ location of label x }
// word y			[y]			{ location of label y }
// word xs			[xs]			{ size of the panel x in pixels }
// word ys			[ys]			{ size of the panel y in pixels }
// long colour			[colour]		{ colour of background }
// long borcolour		[borcolour]		{ colour of the border }
// long textcolour		[textcolour]		{ colour of text }
// String text			[text]			{ text of label }
// int font			[font]			{ font for text }
// bool visible     		[visible]    		{ visibility flag }
// int URN			[URN]			{ unique reference number for this button }
// bool changed			[changed]		{ object has changed since drawn or not drawn at all }
//
int Caveatron_GUI::addPanel(word x,word y,word xs,word ys,long colour,long borcolour,long textcolour,String text, int font,bool visible,int URN){
  GUI_objects+=1;
  GUIobject_type[GUI_objects]=GUI_Type_panel;
  GUIobject_x[GUI_objects]=x;
  GUIobject_y[GUI_objects]=y;
  GUIobject_xs[GUI_objects]=xs;
  GUIobject_ys[GUI_objects]=ys;

  GUIobject_colour[GUI_objects]=colour;
  GUIobject_borcolour[GUI_objects]=borcolour;
  GUIobject_textcolour[GUI_objects]=textcolour;
  GUIobject_text[GUI_objects]=text;
  GUIobject_font[GUI_objects]=font;

  GUIobject_UniqueReference[GUI_objects]=URN;  
  GUIobject_visible[GUI_objects]=visible;
  GUIobject_changed[GUI_objects]=true;
  return GUI_objects;
}

void Caveatron_GUI::drawPanel(int objectNumber){

  if (GUIobject_visible[objectNumber]){
    mylcd->setColor(GUIobject_colour[objectNumber]);
    mylcd->fillRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    mylcd->setColor(GUIobject_borcolour[objectNumber]);
    mylcd->drawRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    int font_height = getFontHeight(GUIobject_font[objectNumber]);
    print(GUIobject_text[objectNumber],GUIobject_x[objectNumber]+(GUIobject_xs[objectNumber]/2),GUIobject_y[objectNumber]+(GUIobject_ys[objectNumber]/2)-(font_height/2),GUIobject_font[objectNumber], 2, CENTER_J, GUIobject_textcolour[objectNumber], GUIobject_colour[objectNumber]);
    
    GUIobject_changed[objectNumber]=false;
  } 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                           ProgressBar FUNCTION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Displays progress bar that can be gradually filled as an event occurs
//
// function add ProgressBar
//
// variable			 stored in		  description
//
// int x			[x]			{ location of bar x }
// int y			[y]			{ location of bar y }
// int xs			[xs]			{ size of the bar x in pixels }
// int ys			[ys]			{ size of the bar y in pixels }
// int pbarlength	[pbl]			{ numeric length of progress bar}
// String text			[text]			{ text of label }
// int font			[font]			{ font for text }
// long textcolour		[textcolour]		{ colour of label text }
// bool visible     		[visible]    		{ visibility flag }
// int URN			[URN]			{ unique reference number for this button }
//
int Caveatron_GUI::addProgressBar(int x, int y, int xs, int ys, int pbl, String text, int font,long textcolour,bool visible,int URN){
  GUI_objects+=1;
  GUIobject_type[GUI_objects]=GUI_Type_ProgressBar;
  GUIobject_x[GUI_objects]=x;
  GUIobject_y[GUI_objects]=y;
  GUIobject_xs[GUI_objects]=xs;
  GUIobject_ys[GUI_objects]=ys;
  GUIobject_data6[GUI_objects]=pbl;
  GUIobject_text[GUI_objects]=text;
  GUIobject_font[GUI_objects]=font;  
  GUIobject_textcolour[GUI_objects]=textcolour;

  GUIobject_UniqueReference[GUI_objects]=URN;  
  GUIobject_visible[GUI_objects]=visible;
  GUIobject_changed[GUI_objects]=true;
  return GUI_objects;
}

void Caveatron_GUI::drawProgressBar(int objectNumber){

  if (GUIobject_visible[objectNumber]){
    mylcd->setColor(WHITE_STD);
    mylcd->drawRoundRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
    mylcd->setColor(GRAY_025);
    mylcd->drawRoundRect(GUIobject_x[objectNumber]+1,GUIobject_y[objectNumber]+1,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-1,GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-1);
    int font_height = getFontHeight(GUIobject_font[objectNumber]);
    print(GUIobject_text[objectNumber],GUIobject_x[objectNumber],GUIobject_y[objectNumber]-font_height-5,GUIobject_font[objectNumber], 2, LEFT_J, GUIobject_textcolour[objectNumber], BLACK_STD);
	
	int progressbarpercent = 100*GUIobject_progressbarfill[objectNumber]/GUIobject_data6[objectNumber];
	String progressbarpercentvalue = "   "+String(progressbarpercent)+" %";
	if ((progressbarpercent < 100) && (progressbarpercent > 0)) {
		mylcd->setColor(BLACK_STD);
		mylcd->fillRect(GUIobject_x[objectNumber]+2+(progressbarpercent*(GUIobject_xs[objectNumber]-4)/100), GUIobject_y[objectNumber]+2, GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2, GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-2);
		mylcd->setColor(BLUE_BAR);
		mylcd->fillRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+2,GUIobject_x[objectNumber]+2+(progressbarpercent*(GUIobject_xs[objectNumber]-4)/100), GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-2);
		print(progressbarpercentvalue,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]-font_height-5,GUIobject_font[objectNumber], 2, RIGHT_J, BLUE_BAR, BLACK_STD);
	}
	else if (progressbarpercent>=100) {
		mylcd->setColor(GREEN_BAR);
		mylcd->fillRect(GUIobject_x[objectNumber]+2,GUIobject_y[objectNumber]+2,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]-2, GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]-2);
		print(progressbarpercentvalue,GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]-font_height-5,GUIobject_font[objectNumber], 2, RIGHT_J, GREEN_BAR, BLACK_STD);
	}
    
  } 
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         GUI INITIALIZATION, UPDATE, AND BUTTON PRESS CHECK FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Caveatron_GUI::InitGUI(){

  //
  // Variables for the GUI
  //
  anyButtonPressed=false;
  GUI_firstObject=0;
  GUI_objects=0;
}

void Caveatron_GUI::preserveColors(){
  GUI_br=bcolorr;
  GUI_bg=bcolorg;
  GUI_bb=bcolorb;
  GUI_fr=fcolorr;
  GUI_fg=fcolorg;
  GUI_Fb=fcolorb;
}

void Caveatron_GUI::restoreColors(){
  mylcd->setBackColor(GUI_br,GUI_bg,GUI_bb);
  mylcd->setColor(GUI_fr,GUI_fg,GUI_Fb);
}


void Caveatron_GUI::updatingScreen(){
  //stopTimer();
  preserveColors();
}

void Caveatron_GUI::finishedUpdatingScreen(){
  restoreColors();
  //restartTimer();
}

bool Caveatron_GUI::withinBounds(int chkX,int chkY,int X, int Y, int X2, int Y2){
  if ( (chkX>X) && (chkY>Y) && (chkX<X2) && (chkY<Y2) ){ 
    return true; 
  } else {
    return false;
  }
}

bool Caveatron_GUI::checkButton(int objectNumber, bool pressType){
		bool inbounds = (withinBounds(TP_X,TP_Y,GUIobject_x[objectNumber],GUIobject_y[objectNumber],(GUIobject_x[objectNumber]+GUIobject_xs[objectNumber]),(GUIobject_y[objectNumber]+GUIobject_ys[objectNumber])));
		//Check whether the press was inside the button

		if (inbounds){   
			if (GUIobject_currentstate[objectNumber]==GUI_State_Unpressed){
				//Serial.println("no / yes");
				// Button is currently shown as NOT pressed BUT has been pressed
				// Change state to pressed

				GUIobject_currentstate[objectNumber]=GUI_State_Pressed;
				redrawObject(objectNumber);
				anyButtonPressed=true;
				if (pressType==OnRelease) return false;
				return true;
			} else {
				//Serial.println("yes / yes");
				// Button is currently shown as pressed and has been pressed
				// No need to change anything on screen but still need to return true as it was pressed
				anyButtonPressed=true;
				return true;  
			}
		} else {
			if (GUIobject_currentstate[objectNumber]==GUI_State_Pressed){
				//Serial.println("yes / no");
				// Button is currently show as pressed AND has NOT been pressed
				// Change state to unpressed
				GUIobject_currentstate[objectNumber]=GUI_State_Unpressed;
				GUIobject_currentposition[objectNumber]=!GUIobject_currentposition[objectNumber];
				redrawObject(objectNumber);        
				anyButtonPressed=false;
				return true; 
			} else {
				//Serial.println("no / no");
				// Button is currently show as NOT pressed AND has NOT been pressed
				// No need to change anything on screen but still need to return false as it isn't pressed
				anyButtonPressed=false;        
				return false;   
			}
		}
}

int Caveatron_GUI::checkAllButtons(bool pressType){
	int found=-1;
	bool butfound;
	touchRead();
	// Then check all normal buttons
    
	for (int i = GUI_firstObject+1; i <= GUI_objects; i++) {
		if (GUIobject_type[i]!=GUI_Type_listbox) {
			if (GUIobject_visible[i]) {
				butfound = checkButton(i, pressType);
				if (pressType==OnRelease) {
					if (anyButtonPressed==true) {
						if (butfound==true) {
							found = -1;
							break;
						} else {
							found = -1;
							break;
						}
					} else {
						if (butfound==true) {
							found = i;
							break;
						}
					}
				} else {
					found = i;
					break;
				}
			}
		}
	}
	return found;
}

void Caveatron_GUI::touchRead(){
  
  if (mytouch->dataAvailable()) {
    mytouch->read();
    TP_X = mytouch->getX();
    TP_Y = mytouch->getY();

  } else {
    TP_X=-1;
    TP_Y=-1;
  }
 
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                         GENERAL OBJECT FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Caveatron_GUI::ClearObjects(int first,int last){
  for (byte i = first; i < last; i++) {
    GUIobject_type[i]=0;
    GUIobject_currentstate[i]=0;
    GUIobject_x[i]=0;
    GUIobject_y[i]=0;
    GUIobject_xs[i]=0;
    GUIobject_ys[i]=0;
    GUIobject_colour[i]=0;
	GUIobject_colour1[i]=0;
    GUIobject_borcolour[i]=0;
    GUIobject_textcolour[i]=0;
    GUIobject_pressedcolour[i]=0;
    GUIobject_pressedtextcolour[i]=0;
    GUIobject_text[i]="";
	GUIobject_text1[i]="";
	GUIobject_text2[i]="";
	GUIobject_texty[i]=0;
	GUIobject_justify[i]=0;
	GUIobject_iconaddress[i]=0;
	GUIobject_iconx[i]=0;
	GUIobject_icony[i]=0;
    GUIobject_font[i]=0;
	GUIobject_data1[i]=0;
	GUIobject_data2[i]=0;
	GUIobject_data3[i]=0;
	GUIobject_data4[i]=0;
	GUIobject_data5[i]=0;
	GUIobject_data6[i]=0;
	GUIobject_progressbarfill[i]=0;
    GUIobject_link[i]=0;
    GUIobject_linkoption[i]=0;
    GUIobject_visible[i]=optInvisible;
    GUIobject_changed[i]=false;
  }

}

void Caveatron_GUI::clearAllObjects(){
  ClearObjects(0,maxobjects);
  anyButtonPressed=false;
  GUI_firstObject=0;
  GUI_objects=0;
}

void Caveatron_GUI::redrawAllObjects(){
  int objectNumber=GUI_firstObject;
  while (objectNumber<GUI_objects){
    objectNumber+=1;
    if(GUIobject_visible[objectNumber]){
      redrawObject(objectNumber);
    };
  } 
}

void Caveatron_GUI::clearObjectArea(int objectNumber){
  mylcd->setColor(0,0,0);
  mylcd->fillRect(GUIobject_x[objectNumber],GUIobject_y[objectNumber],GUIobject_x[objectNumber]+GUIobject_xs[objectNumber],GUIobject_y[objectNumber]+GUIobject_ys[objectNumber]);
}

void Caveatron_GUI::redrawObject(int objectNumber){
  
if (GUIobject_visible[objectNumber] && ((objectNumber+1)>GUI_firstObject)){
    switch (GUIobject_type[objectNumber]){
    
    case GUI_Type_button:
		// Object type is a button.
		drawButton(objectNumber); 
		break;
	case GUI_Type_iconbutton:
		// Object type is a iconButton.
		drawIconButton(objectNumber); 
		break;
	case GUI_Type_hiddenbutton:
		// Object type is a hiddenButton.
		drawHiddenButton(objectNumber); 
		break;
	case GUI_Type_toggleswitch:
		// Object type is a toggle switch.
		drawToggleSwitch(objectNumber); 
		break;
	case GUI_Type_TextInputBox:
      	// Object type is a TextInputBox.
		drawTextInputBox(objectNumber); 
		break;
	case GUI_Type_ProgressBar:
      	// Object type is a ProgressBar.
		drawProgressBar(objectNumber); 
		break;
	case GUI_Type_listbox:
      	// Object type is a ProgressBar.
		drawListBox(objectNumber); 
		break;
	case GUI_Type_listboxrow:
      	// Object type is a ProgressBar.
		drawListBoxRow(objectNumber); 
		break;  
        
    } // end of switch

  }
}

void Caveatron_GUI::makeObjectInvisible(int objectNumber){
  GUIobject_visible[objectNumber]=optInvisible;
  GUIobject_currentstate[objectNumber]=GUI_State_Unpressed;
  anyButtonPressed=false;
  clearObjectArea(objectNumber);
}

void Caveatron_GUI::makeObjectVisible(int objectNumber){
  GUIobject_visible[objectNumber]=optVisible;
  redrawObject(objectNumber);
}

void Caveatron_GUI::toggleButton(int objectON, int objectOFF) {
	GUIobject_visible[objectOFF]=optInvisible;
	GUIobject_currentstate[objectOFF]=GUI_State_Unpressed;
	anyButtonPressed=false;
	GUIobject_visible[objectON]=optVisible;
	redrawObject(objectON);
}

void Caveatron_GUI::updateProgressBar(int objectNumber, int fillLevel) {
    GUIobject_progressbarfill[objectNumber] = fillLevel;
    redrawObject(objectNumber);
}
