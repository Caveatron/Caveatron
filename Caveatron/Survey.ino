/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Line Plot Viewer Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Setup and handler for the Line Plot screen
void LinePlotSetup() {
  ctGUI.addButton(224,421,86,58,BLUE_STD,"DONE",caveatron.FONT_22,optVisible,1);
  btn1 = ctGUI.addButton(12,354,86,58,BUTTON_STD,"PLAN",caveatron.FONT_22,optInvisible,2);
  btn2 = ctGUI.addButton(118,354,86,58,BUTTON_STD,"PROFILE","South", caveatron.FONT_22,optVisible,3);
  btn3 = ctGUI.addButton(224,354,86,58,BUTTON_STD,"PROFILE","East", caveatron.FONT_22,optVisible,4);
  ctGUI.addButton(12,421,86,58,BUTTON_STD,"TOGGLE","LABELS", caveatron.FONT_19,optVisible,5);
  btn4 = ctGUI.addButton(118,421,86,58,BUTTON_STD,"SWITCH","GROUP", caveatron.FONT_19,optInvisible,6);
  myGLCD.setColor(255,255,255);
  myGLCD.drawLine(0,19,319,19);
  myGLCD.drawLine(0,346,319,346);
  myGLCD.drawLine(0,19,0,346);
  myGLCD.drawLine(319,19,319,346);
  plotView=0;
  plotGroup=1;
  plotLabels=true;
  PlotSurvey();
}

void LinePlotHandler(int URN) {
  switch (URN) { 
    case 1:
      CreateScreen(screenSurveyMenu);
      break;
    case 2:                            //Plan View
      ctGUI.makeObjectInvisible(btn1);
      ctGUI.makeObjectVisible(btn2);
      ctGUI.makeObjectVisible(btn3);
      plotView=0;
      PlotSurvey();
      break;
    case 3:                            //Profile View - from South
      ctGUI.makeObjectVisible(btn1);
      ctGUI.makeObjectInvisible(btn2);
      ctGUI.makeObjectVisible(btn3);
      plotView=1;
      PlotSurvey();
      break;
    case 4:                            //Profile View - from East
      ctGUI.makeObjectVisible(btn1);
      ctGUI.makeObjectVisible(btn2);
      ctGUI.makeObjectInvisible(btn3);
      plotView=2;
      PlotSurvey();
      break;
    case 5:                            //Toggle Labels On or Off
      plotLabels = !plotLabels;
      PlotSurvey();
      break;
    case 6:                            //Select Station Group
      plotGroup++;
      PlotSurvey();
      break;
  }
}

// Function for computing the line plot from the survey data and displaying it on screen
void PlotSurvey() {
  int vectorCount=0;
  int nextStation=0;
  int sX, sY, cX, cY, offsetText;
  float sXscale, sYscale, sXoffset, sYoffset, plotScale;
  String plotTypeText, dirText, groupStr;

  float maxCoordX[10];
  float minCoordX[10];
  float maxCoordY[10];
  float minCoordY[10];
  float maxCoordZ[10];
  float minCoordZ[10];
  
  int currentStation;
  int groupCount=1;
  int vectorsProcessed=0;
  char station1[8], station2[8];
  typedef struct
  {
      String st1;
      String st2;
      float di;
      float az;
      float in;
      int group;
      int st1index;
      int st2index;
  }  vector_type;
  vector_type vector[100];
  
  typedef struct
  {
      String code;
      float coords[3];
      int group;
      boolean closure;
      boolean overlap;
  }  station_type;
  station_type station[100];
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,20,318,345);
  myGLCD.fillRect(200,0,319,18);
  
  uint32_t curReadPos;
  char linebuffer[66];
  fileFlag = OpenSurveyFiles();
  for (int i=0;i<7;i++) inFile.getline(linebuffer, 65, '\n');
  while((inFile.eof() == false)&&(inFile.peek()>=0)) { 
    if (inFile.peek()!=';') {
      inFile >> station1 >> station2 >> vector[vectorCount].di >> vector[vectorCount].az >> vector[vectorCount].in;
      vector[vectorCount].st1 = String(station1);
      vector[vectorCount].st2 = String(station2);
      vector[vectorCount].group = 0;
      Serial.print(vector[vectorCount].st1);
      Serial.print(",");
      Serial.print(vector[vectorCount].st2);
      Serial.print(",");
      Serial.print(vector[vectorCount].di);
      Serial.print(",");
      Serial.print(vector[vectorCount].az);
      Serial.print(",");
      Serial.println(vector[vectorCount].in);
      if (vector[vectorCount].di >= 0) vectorCount++;
    }
    inFile.getline(linebuffer, 65, '\n');
  }
  inFile.close();
  
  currentStation=0;
  while(vectorsProcessed<vectorCount) {
    maxCoordX[groupCount]=0;
    minCoordX[groupCount]=0;
    maxCoordY[groupCount]=0;
    minCoordY[groupCount]=0;
    maxCoordZ[groupCount]=0;
    minCoordZ[groupCount]=0;
          Serial.println();
          Serial.println();

    /*for (int z=0; z<vectorCount; z++) {
      Serial.print(vector[z].st1);
      Serial.print(",");
      Serial.print(vector[z].st2);
      Serial.print(",");
      Serial.print(vector[z].di);
      Serial.print(",");
      Serial.print(vector[z].az);
      Serial.print(",");
      Serial.print(vector[z].in);
      Serial.print(",");
      Serial.println(vector[z].group);
    }*/
  
    for(int i=0;i<vectorCount;i++) {
      if (vector[i].group==0) {
        station[nextStation].code = vector[i].st1;
        station[nextStation].coords[0] = 0;
        station[nextStation].coords[1] = 0;
        station[nextStation].coords[2] = 0;
        station[nextStation].group = groupCount;
        i=vectorCount;
            /*Serial.println();
            Serial.println();
            Serial.print(nextStation);
            Serial.print(",");
            Serial.print(station[nextStation].code);
            Serial.print(",");
            Serial.print(station[nextStation].coords[0]);
            Serial.print(",");
            Serial.print(station[nextStation].coords[1]);
            Serial.print(",");
            Serial.print(station[nextStation].coords[2]);
            Serial.print(",");
            Serial.println(station[nextStation].group);*/
            nextStation++;
      }
    }
  
    for (int k=0; k<vectorCount; k++) {
      //int nextStation = stationCount;
      for (int j=currentStation; j<nextStation; j++) {
        for (int i=k; i<vectorCount; i++) {
          
          //Check all frontshots
          if ((station[j].code==vector[i].st1) && (vector[i].group==0)) {
            station[nextStation].code=vector[i].st2;
            station[nextStation].coords[0] = station[j].coords[0] + vector[i].di*cos(vector[i].in/57.2958) * sin(vector[i].az/57.2958);
            station[nextStation].coords[1] = station[j].coords[1] + vector[i].di*cos(vector[i].in/57.2958) * cos(vector[i].az/57.2958);
            station[nextStation].coords[2] = station[j].coords[2] + vector[i].di*sin(vector[i].in/57.2958);
            station[nextStation].group = groupCount;
            vector[i].group = groupCount;
            vector[i].st1index = j;
            vector[i].st2index = nextStation;
            vectorsProcessed++;
            //Chack if new station is farthest from the center on any axis (used for setting display scale)
            if(station[nextStation].coords[0]>maxCoordX[groupCount]) maxCoordX[groupCount] = station[nextStation].coords[0];
            else if(station[nextStation].coords[0]<minCoordX[groupCount]) minCoordX[groupCount] = station[nextStation].coords[0];
            if(station[nextStation].coords[1]>maxCoordY[groupCount]) maxCoordY[groupCount] = station[nextStation].coords[1];
            else if(station[nextStation].coords[1]<minCoordY[groupCount]) minCoordY[groupCount] = station[nextStation].coords[1];
            if(station[nextStation].coords[2]>maxCoordZ[groupCount]) maxCoordZ[groupCount] = station[nextStation].coords[2];
            else if(station[nextStation].coords[2]<minCoordZ[groupCount]) minCoordZ[groupCount] = station[nextStation].coords[2];
                /*Serial.print(nextStation);
                Serial.print(",");
                Serial.print(station[nextStation].code);
                Serial.print(",");
                Serial.print(station[nextStation].coords[0]);
                Serial.print(",");
                Serial.print(station[nextStation].coords[1]);
                Serial.print(",");
                Serial.print(station[nextStation].coords[2]);
                Serial.print(",");
                Serial.println(station[nextStation].group);*/
            nextStation++; 
          }
          
          //Check all backshots
          else if ((station[j].code==vector[i].st2) && (vector[i].group==0)) {
            station[nextStation].code=vector[i].st1;
            station[nextStation].coords[0] = station[j].coords[0] - vector[i].di*cos(vector[i].in/57.2958) * sin(vector[i].az/57.2958);
            station[nextStation].coords[1] = station[j].coords[1] - vector[i].di*cos(vector[i].in/57.2958) * cos(vector[i].az/57.2958);
            station[nextStation].coords[2] = station[j].coords[2] - vector[i].di*sin(vector[i].in/57.2958);
            station[nextStation].group = groupCount;
            vector[i].group = groupCount;
            vector[i].st2index = j;
            vector[i].st1index = nextStation;
            vectorsProcessed++;
            //Chack if new station is farthest from the center on any axis (used for setting display scale)
            if(station[nextStation].coords[0]>maxCoordX[groupCount]) maxCoordX[groupCount] = station[nextStation].coords[0];
            else if(station[nextStation].coords[0]<minCoordX[groupCount]) minCoordX[groupCount] = station[nextStation].coords[0];
            if(station[nextStation].coords[1]>maxCoordY[groupCount]) maxCoordY[groupCount] = station[nextStation].coords[1];
            else if(station[nextStation].coords[1]<minCoordY[groupCount]) minCoordY[groupCount] = station[nextStation].coords[1];
            if(station[nextStation].coords[2]>maxCoordZ[groupCount]) maxCoordZ[groupCount] = station[nextStation].coords[2];
            else if(station[nextStation].coords[2]<minCoordZ[groupCount]) minCoordZ[groupCount] = station[nextStation].coords[2];
                /*Serial.print("_");
                Serial.print(nextStation);
                Serial.print(",");
                Serial.print(station[nextStation].code);
                Serial.print(",");
                Serial.print(station[nextStation].coords[0]);
                Serial.print(",");
                Serial.print(station[nextStation].coords[1]);
                Serial.print(",");
                Serial.print(station[nextStation].coords[2]);
                Serial.print(",");
                Serial.print(station[nextStation].group);
                Serial.print(",  ");
                Serial.println(station[j].code);*/
            nextStation++; 
          }

        }
      }
      currentStation=nextStation-1;
      //stationCount = nextStation;
    }
    
    groupCount++;
  }
  // Station and vector sorting complete

 
  /*Serial.println();
  Serial.println(currentStation);
  Serial.println();*/

  /*for (int i=0; i<vectorCount; i++) {
    Serial.print(vector[i].st1);
    Serial.print(",");
    Serial.print(vector[i].st2);
    Serial.print(",");
    Serial.print(vector[i].di);
    Serial.print(",");
    Serial.print(vector[i].az);
    Serial.print(",");
    Serial.print(vector[i].in);
        Serial.print(",");
    Serial.print(vector[i].group);
        Serial.print(",");
    Serial.print(vector[i].st1index);
        Serial.print(",");
    Serial.println(vector[i].st2index);
  }
  Serial.println(vectorCount);*/
  
  if (groupCount>2) {
    ctGUI.makeObjectVisible(btn4);
    if (plotGroup >= groupCount) plotGroup = 1;
    groupStr = "G: " + String(plotGroup);
  }
  else groupStr = "";
  
  myGLCD.setBackColor(0,0,0);
  myGLCD.setColor(0,255,255);
  
  //Select view direction and detemine size needed to display line plot
  switch (plotView) {
    case 0:
      sX = 0;
      sY = 1;     
      sXscale = maxCoordX[plotGroup] - minCoordX[plotGroup];
      sYscale = maxCoordY[plotGroup] - minCoordY[plotGroup];
      sXoffset = -minCoordX[plotGroup];
      sYoffset = -minCoordY[plotGroup];
      plotTypeText = "Plan View";
      dirText = "N";
      offsetText = 12;
      break;
    case 1:
      sX = 0;
      sY = 2;
      sXscale = maxCoordX[plotGroup] - minCoordX[plotGroup];
      sYscale = maxCoordZ[plotGroup] - minCoordZ[plotGroup];
      sXoffset = -minCoordX[plotGroup];
      sYoffset = -minCoordZ[plotGroup];
      plotTypeText = "Profile View (S)";
      dirText = "E";
      offsetText = 118;
      break;
    case 2:
      sX = 1;
      sY = 2;
      sXscale = maxCoordY[plotGroup] - minCoordY[plotGroup];
      sYscale = maxCoordZ[plotGroup] - minCoordZ[plotGroup];
      sXoffset = -minCoordY[plotGroup];
      sYoffset = -minCoordZ[plotGroup];
      plotTypeText = "Profile View (E)";
      dirText = "N";
      offsetText = 224;
      break;
  }
  
  //Set scale for display
  if (sXscale>sYscale) {
    plotScale = 310/sXscale;
    cX = 0;
    cY = 160-(sYscale*plotScale/2);
  } else {
    plotScale = 310/sYscale;
    cX = 160-(sXscale*plotScale/2);
    cY = 0;
  }
  
  //Identify loop closure stations and stations where label text will overlap
  for (int i=0; i<=currentStation; i++) {
    station[i].closure = false;
    station[i].overlap = false;
    /*for (int j=0; j<=currentStation; j++) {
        if ((station[i].code==station[j].code)&&(j!=i)) station[i].closure = true;
    }*/
    for (int j=0; j<i; j++) {
      if (station[i].code==station[j].code) {
        station[i].closure = true;
        station[j].closure = true;
      }
      float sXoverlap = fabs(station[i].coords[sX] - station[j].coords[sX])*plotScale;
      float sYoverlap = fabs(station[i].coords[sY] - station[j].coords[sY])*plotScale;
      //if (sqrt(pow(sXoverlap, 2)+pow(sYoverlap, 2)) < 10+(18*atan(sXoverlap/sYoverlap))) station[i].overlap = true;
      if (sXoverlap+sYoverlap < 9+(6*atan(sXoverlap/sYoverlap))) station[i].overlap = true;  
    }
  }

  //Plot vectors on screen
  for (int i=0; i<vectorCount; i++) {
    if (vector[i].group==plotGroup) { 
      int screenX1 = 5 + cX + (sXoffset+station[vector[i].st1index].coords[sX]) * plotScale;
      int screenY1 = 333 - cY - (sYoffset+station[vector[i].st1index].coords[sY]) * plotScale;
      int screenX2 = 5 + cX + (sXoffset+station[vector[i].st2index].coords[sX]) * plotScale;
      int screenY2 = 333 - cY - (sYoffset+station[vector[i].st2index].coords[sY]) * plotScale;
      myGLCD.drawLine(screenX1, screenY1, screenX2, screenY2);
    }
  }
  
  //Plot stations on screen
  for (int i=0; i<=currentStation; i++) {
    if (station[i].group==plotGroup) {
      int screenX = 5 + cX + (sXoffset+station[i].coords[sX]) * plotScale;
      int screenY = 333 - cY - (sYoffset+station[i].coords[sY]) * plotScale;
      myGLCD.setColor(255,255,255);
      if ((station[i].overlap==false) && (plotLabels==true)) {
        if (screenX<230) ctGUI.print(station[i].code, screenX+2, screenY, caveatron.FONT_13, 1, LEFT_J);
        else ctGUI.print(station[i].code, screenX-2, screenY, caveatron.FONT_13, 1, RIGHT_J);
      }     
      if (station[i].code==vector[vectorCount-1].st2) {
        if (station[i].closure==true) myGLCD.setColor(255,0,0);
        else myGLCD.setColor(0,255,0);
        myGLCD.drawLine(screenX-2,screenY+2,screenX,screenY-4);
        myGLCD.drawLine(screenX,screenY-4,screenX+2,screenY+2);
        myGLCD.drawLine(screenX-2,screenY+2,screenX+2,screenY+2);
        myGLCD.drawLine(screenX-1,screenY+1,screenX+1,screenY+1);
        myGLCD.fillRect(screenX,screenY-4,screenX,screenY+1);
      } else {
        if (station[i].closure==true) myGLCD.setColor(255,0,0);
        else if (i==0) myGLCD.setColor(255,255,255);
        else myGLCD.setColor(255,255,0);
        myGLCD.fillRect(screenX-1, screenY-1, screenX+1, screenY+1);
      }
    }
  }
  
  //Plot scale bar
  myGLCD.setColor(255,255,0);
  myGLCD.drawLine(offsetText+25,365,offsetText+84,365);
  myGLCD.drawLine(offsetText+25,359,offsetText+25,371);
  myGLCD.drawLine(offsetText+84,359,offsetText+84,371);
  String scaleText = String(60/plotScale) + " m";
  ctGUI.print(scaleText, offsetText+55, 371, caveatron.FONT_15, 2, CENTER_J);
  ctGUI.print(dirText, offsetText+10, 390, caveatron.FONT_15, 1, CENTER_J);
  myGLCD.drawCircle(offsetText+10,397,10);
  ctGUI.print(groupStr, offsetText+84, 390, caveatron.FONT_15, 2, RIGHT_J);
  if (plotView==0) {
    myGLCD.drawLine(offsetText+10,387,offsetText+10,359);
    myGLCD.drawLine(offsetText+6,366,offsetText+10,359);
    myGLCD.drawLine(offsetText+14,366,offsetText+10,359);
  } else {
    myGLCD.drawLine(offsetText+20,397,offsetText+48,397);
    myGLCD.drawLine(offsetText+41,393,offsetText+48,397);
    myGLCD.drawLine(offsetText+41,401,offsetText+48,397);
  }
  
  //Plot Title
  myGLCD.setColor(255,255,255);
  ctGUI.print(caveName, 0, 0, caveatron.FONT_19, 2, LEFT_J);
  ctGUI.print(plotTypeText, 317, 0, caveatron.FONT_19, 2, RIGHT_J);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           View & Delete Shots
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Setup and handler for the viewing shots
void ShotViewerSetup(int m) {
  drawStatusBar("Review/Delete Shots");
  ctGUI.addButton(224,405,86,50,BLUE_STD,"DONE",caveatron.FONT_22,optVisible,1);
  btn1 = ctGUI.addButton(118,405,86,50,BUTTON_STD,"NEXT",caveatron.FONT_22,optInvisible,2);
  btn2 = ctGUI.addButton(12,405,86,50,BUTTON_STD,"PREV",caveatron.FONT_22,optInvisible,3);
  int listboxTop = 26;
  ctGUI.addListBox(5,listboxTop,310,7,caveatron.FONT_13,optVisible,4);

  myGLCD.setColor(GRAY_075);
  myGLCD.fillRect(7,listboxTop+2,313,listboxTop+22);
  
  myGLCD.setColor(WHITE_STD);
  ctGUI.print(" From     To          Dist       Azi          Inc", 10, 29, caveatron.FONT_19, 2 , LEFT_J, WHITE_STD, GRAY_075);
  myGLCD.setColor(GRAY_050);
  for (int j=1;j<5;j++) {
    myGLCD.drawLine(j*61+7,listboxTop+2,j*61+7,listboxTop+22);
  }

  vectorPage = m;
  LoadShots(0);
}

void ShotViewerHandler(int URN) {
  switch (URN) { 
    case 1:
      CreateScreen(screenSurveyMenu);
      break;
    case 2:                            //Next Page
      vectorPage++;
      LoadShots(0);
      break;
    case 3:                            //Previous Page
      vectorPage--;
      LoadShots(0);
      break;
  }
  if ((URN > 3) && (URN < 90)) {
    LoadShots(URN-3);
    DeleteShot();
    
  }
}

//Load shots from Survey file on SD card
void LoadShots(int k) {
  int listboxTop = 26;
  int numVectorPages, vectorCount=0;
  char st1_in[8], st2_in[8];
  float di_in, az_in, in_in;
  
  typedef struct
  {
      String st1;
      String st2;
      String di;
      String az;
      String in;
      uint32_t filep; 
  }  vector_type;
  vector_type vector[100];
  
  uint32_t curReadPos;
  char linebuffer[66];

  fileFlag = OpenSurveyFiles();
  for (int i=0;i<8;i++) inFile.getline(linebuffer, 65, '\n');
  while((inFile.eof() == false)&&(inFile.peek()>=0)) { 
    vector[vectorCount].filep = inFile.tellg();
    if (inFile.peek()!=';') {
      inFile >> st1_in >> st2_in >> di_in >> az_in >> in_in;
      vector[vectorCount].st1 = String(st1_in);
      vector[vectorCount].st2 = String(st2_in);
      vector[vectorCount].di = String(di_in, 2);
      vector[vectorCount].az = String(az_in, 1);
      vector[vectorCount].in = String(in_in, 1);
      vectorCount++;
    }
    inFile.getline(linebuffer, 65, '\n');
  }
  inFile.close();
  numVectors = vectorCount;
  numVectorPages = int(ceil(float(numVectors)/7));

  if (k > 0) {
    editVector.st1_e = vector[(k-1)+((vectorPage-1)*7)].st1;
    editVector.st2_e = vector[(k-1)+((vectorPage-1)*7)].st2;
    editVector.di_e = vector[(k-1)+((vectorPage-1)*7)].di;
    editVector.az_e = vector[(k-1)+((vectorPage-1)*7)].az;
    editVector.in_e = vector[(k-1)+((vectorPage-1)*7)].in;
    readPosition = vector[(k-1)+((vectorPage-1)*7)].filep;
  } else {
    if (vectorPage < numVectorPages) ctGUI.makeObjectVisible(btn1);
    else ctGUI.makeObjectInvisible(btn1);
    if (vectorPage > 1) ctGUI.makeObjectVisible(btn2);
    else ctGUI.makeObjectInvisible(btn2);
    
    myGLCD.setColor(BUTTON_DRK);
    for (int i=0;i<7;i++) {
      myGLCD.fillRect(7,50*i+(listboxTop+38),313,50*i+(listboxTop+38)+16);
    }
  
    myGLCD.setColor(GRAY_050);
    for (int i=0;i<7;i++) {
      for (int j=1;j<5;j++) {
        myGLCD.drawLine(j*61+7,50*i+(listboxTop+38),j*61+7,50*i+(listboxTop+38)+16);
      }
    }
    
    myGLCD.setColor(WHITE_STD);
    myGLCD.setBackColor(BUTTON_DRK);
    for (int i=0; i<7; i++) {
      ctGUI.print(vector[i+((vectorPage-1)*7)].st1, 13, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[i+((vectorPage-1)*7)].st2, 74, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[i+((vectorPage-1)*7)].di, 135, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[i+((vectorPage-1)*7)].az, 196, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[i+((vectorPage-1)*7)].in, 257, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
    }
    
   drawInfoBar("Page "+String(vectorPage)+"/"+String(numVectorPages)+"     Select to Edit/Delete");
  }
}

// Delete a shot from the Survey file on the SD card
void DeleteShot() {
  boolean quit = false;
  int x,y;
  currentMode = modeEditSurvey;
  myGLCD.setColor(127, 127, 0);
  myGLCD.setBackColor(127, 127, 0);
  myGLCD.fillRect(30, 80, 290, 270);
  myGLCD.setColor(WHITE_STD);
  ctGUI.print("Really delete", 160, 90, caveatron.FONT_22, 2, CENTER_J);
  ctGUI.print("shot "+editVector.st1_e+" - "+editVector.st2_e+"?", 160, 122, caveatron.FONT_22, 2, CENTER_J);
  myGLCD.setColor(BUTTON_STD);
  myGLCD.fillRoundRect(180,160,270,250);
  myGLCD.fillRoundRect(50,160,140,250);
  myGLCD.setColor(WHITE_STD);
  myGLCD.drawRoundRect(180,160,270,250);
  myGLCD.drawRoundRect(50,160,140,250);
  myGLCD.setColor(GRAY_025);
  myGLCD.drawRoundRect(181,161,269,249);
  myGLCD.drawRoundRect(51,161,139,249);
  ctGUI.print("DELETE", 225, 195, caveatron.FONT_22, 2, CENTER_J, WHITE_STD, BUTTON_STD);
  ctGUI.print("CANCEL", 95, 195, caveatron.FONT_22, 2, CENTER_J, WHITE_STD, BUTTON_STD);
  while (quit==false) {
    if (myTouch.dataAvailable()) { 
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      if ((y>=120) && (y<=210)) {
        if ((x>=180) && (x<=270)) {
          myGLCD.setColor(RED_STD);
          myGLCD.drawRoundRect(180,160,270,250);
          myGLCD.drawRoundRect(181,161,269,249);
          delay(500);
          fileFlag = OpenSurveyFiles();
          theFile.rewind();
          theFile.seekSet(readPosition);
          theFile.print(";");
          theFile.close();
          quit = true;
        } else if ((x>=50) && (x<=140)) {
          quit = true;
        }
      }
    }
  }
  myGLCD.clrScr();
  currentMode = modeSurvey;
  ShotViewerSetup(vectorPage);
  ctGUI.redrawAllObjects();
}


