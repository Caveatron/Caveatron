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
      /*Serial.print(vector[vectorCount].st1);
      Serial.print(",");
      Serial.print(vector[vectorCount].st2);
      Serial.print(",");
      Serial.print(vector[vectorCount].di);
      Serial.print(",");
      Serial.print(vector[vectorCount].az);
      Serial.print(",");
      Serial.println(vector[vectorCount].in);*/
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
          //Serial.println();
          //Serial.println();

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
//                                         Shot & Scan Review Menu
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Setup and handler for the Shot & Scan Review Menu screen
void ShotScanReviewSetup() {
  drawStatusBar("REVIEW SHOTS & SCANS");
  drawInfoBar("SURVEY: "+caveName);
  ctGUI.addButton(20,33,280,66,BUTTON_STD,"VIEW/DELETE SHOTS",caveatron.FONT_28,optVisible,1);
  ctGUI.addButton(20,114,280,66,BUTTON_STD,"VIEW LIDAR SCANS",caveatron.FONT_28,optVisible,2);
  ctGUI.addButton(20,364,280,84,BLUE_STD,"RETURN TO","SURVEY MENU",caveatron.FONT_28,optVisible,5);
}

void ShotScanReviewHandler(int URN) {  
  switch (URN) {
      case 1:                            //View and Delete Shots
        CreateScreen(screenShotViewer);
        break;
      case 2:                            //View LIDAR Scans
        currentMode = modeViewScans;
        CreateScreen(screenScanViewer);
        break;
      case 5:                            //Exit to Survey Menu
        guiStep--;
        CreateScreen(screenSurveyMenu);
        break;
  }
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
      CreateScreen(screenShotScanReview);
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
  if ((URN > 3) && (URN <= (numVectors+3))) {
    LoadShots(URN-3);
    DeleteShot();
  }
}

//Load shots from Survey file on SD card
void LoadShots(int k) {
  int listboxTop = 26;
  int numVectorPages, vectorsPerPage=7, vectorCount=0;
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
  vector_type vector[92];
  
  uint32_t curReadPos;
  char linebuffer[66];

  fileFlag = OpenSurveyFiles();
  for (int i=0;i<8;i++) inFile.getline(linebuffer, 65, '\n');
  while((inFile.eof() == false)&&(inFile.peek()>=0)&&(vectorCount<91)) { 
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
    if (vectorPage==numVectorPages) vectorsPerPage = numVectors%7;
    for (int i=0; i<vectorsPerPage; i++) {
      ctGUI.print(vector[numVectors-(i+((vectorPage-1)*7))-1].st1, 13, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[numVectors-(i+((vectorPage-1)*7))-1].st2, 74, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[numVectors-(i+((vectorPage-1)*7))-1].di, 135, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[numVectors-(i+((vectorPage-1)*7))-1].az, 196, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.print(vector[numVectors-(i+((vectorPage-1)*7))-1].in, 257, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           LIDAR Scan Viewer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// Setup and handler for the viewing list of LIDAR scans
void ScanViewerSetup(int m) {
  drawStatusBar("Review LIDAR Scans");
  btn1 = ctGUI.addButton(224,405,86,50,BLUE_STD,"DONE",caveatron.FONT_22,optVisible,1);
  btn2 = ctGUI.addButton(118,405,86,50,BUTTON_STD,"NEXT",caveatron.FONT_22,optInvisible,2);
  btn3 = ctGUI.addButton(12,405,86,50,BUTTON_STD,"PREV",caveatron.FONT_22,optInvisible,3);

  btn4=ctGUI.addButton(224,421,86,58,BLUE_STD,"RETURN",caveatron.FONT_22,optInvisible,4);
  btn5=ctGUI.addButton(10,421,86,58,BUTTON_STD,"Zoom +",caveatron.FONT_22,optInvisible,5);
  btn6=ctGUI.addButton(117,421,86,58,BUTTON_STD,"Zoom -",caveatron.FONT_22,optInvisible,6);
  btn7=ctGUI.addButton(10,354,86,58,BUTTON_STD,"Profile",caveatron.FONT_22,optInvisible,7);
  btn8=ctGUI.addButton(10,354,86,58,BUTTON_STD,"Plan", caveatron.FONT_22,optInvisible,8);
  
  int listboxTop = 26;
  list1 = ctGUI.addListBox(5,listboxTop,310,7,caveatron.FONT_13,optVisible,11); 
  myGLCD.setColor(GRAY_075);
  myGLCD.fillRect(7,listboxTop+2,313,listboxTop+22);
  
  myGLCD.setColor(WHITE_STD);
  ctGUI.print("Station    Scan   #Points                       Ok", 13, 29, caveatron.FONT_19, 2 , LEFT_J, WHITE_STD, GRAY_075);
  myGLCD.setColor(GRAY_050);
  myGLCD.drawLine(77,listboxTop+2,77,listboxTop+22);
  myGLCD.drawLine(127,listboxTop+2,127,listboxTop+22);
  myGLCD.drawLine(283,listboxTop+2,283,listboxTop+22);

  scanPage = m;
  LoadScans(0);
}

void ScanViewerHandler(int URN) {
  switch (URN) { 
    case 1:
      currentMode = modeSurvey;
      CreateScreen(screenShotScanReview);
      break;
    case 2:                            //Next Page
      scanPage++;
      LoadScans(0);
      break;
    case 3:                            //Previous Page
      scanPage--;
      LoadScans(0);
      break;
  }
  if (URN > 10) {
    LoadScans(URN+(7*(scanPage-1))-10);
  }
}

//Load scans from Survey file on SD card
void LoadScans(int k) {
  int listboxTop = 26;
  int scansPerPage = 7;
  int numScanPages, scanCount=0;
  uint32_t filePCount = 0;
  
  typedef struct
  {
      char scanStation[8];
      char scanType;
      char scanNum[3];
      int numScanPoints;
      int numScanRots;
      int scanStatus;
      uint32_t filep; 
      char scanRedo;
  }  scan_type;
  scan_type scanList[85];
  
  uint32_t curReadPos;
  char linebuffer[51], c[2];
  float f;

  fileFlag = OpenSurveyFiles();
  while (theFile.fgets(linebuffer, 50)>0) {
    if (linebuffer[0]=='#') {
      if (linebuffer[1]=='#') {
        scanCount++;
        scanList[scanCount].numScanPoints=0;
        ibufstream ibuf(linebuffer);
        ibuf >> c >> scanList[scanCount].scanStation >> scanList[scanCount].scanType >> scanList[scanCount].scanNum >> c >> scanList[scanCount].filep;
      }
      else if (linebuffer[1]=='!') {
        ibufstream ibuf(linebuffer);
        ibuf >> c >> scanList[scanCount].scanStatus >> scanList[scanCount].numScanPoints >> scanList[scanCount].numScanRots >> f >> scanList[scanCount].scanRedo;
        //Serial.print(scanList[scanCount].scanStatus); Serial.print(","); Serial.print(scanList[scanCount].numScanPoints); Serial.print(","); Serial.print(scanList[scanCount].numScanRots); Serial.print(",");
        //Serial.print(f); Serial.print(","); Serial.print(scanList[scanCount].scanRedo); Serial.println();
      }
    }
  }
  theFile.close();
  
  numScans = scanCount;
  numScanPages = int(ceil(float(numScans)/7));
  if (k > 0) {
    String s = "Station:  "+String(scanList[numScans-k+1].scanStation) + "  /  Scan:  " + String(scanList[numScans-k+1].scanType) + String(scanList[numScans-k+1].scanNum);
    ViewScan(scanList[numScans-k+1].filep, scanList[numScans-k+1].numScanRots, scanList[numScans-k+1].scanType, s);
  } else {
    if (scanPage < numScanPages) ctGUI.makeObjectVisible(btn2);
    else ctGUI.makeObjectInvisible(btn2);
    if (scanPage > 1) ctGUI.makeObjectVisible(btn3);
    else ctGUI.makeObjectInvisible(btn3);
    
    myGLCD.setColor(BUTTON_DRK);
    for (int i=0;i<7;i++) {
      myGLCD.fillRect(7,50*i+(listboxTop+38),313,50*i+(listboxTop+38)+16);
    }
  
    myGLCD.setColor(GRAY_050);
    for (int i=0;i<7;i++) {
        myGLCD.drawLine(77,50*i+(listboxTop+38),77,50*i+(listboxTop+38)+16);
        myGLCD.drawLine(127,50*i+(listboxTop+38),127,50*i+(listboxTop+38)+16);
        myGLCD.drawLine(283,50*i+(listboxTop+38),283,50*i+(listboxTop+38)+16);
    }
    
    myGLCD.setColor(WHITE_STD);
    myGLCD.setBackColor(BUTTON_DRK);
    if (scanPage==numScanPages) scansPerPage = 1+((numScans-1)%7);
    if (numScans==0) scansPerPage = 0; 
    for (int i=0; i<scansPerPage; i++) {
      String outStr;
      ctGUI.print(scanList[numScans-(i+((scanPage-1)*7))].scanStation, 12, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      outStr = String(scanList[numScans-(i+((scanPage-1)*7))].scanType) + String(scanList[numScans-(i+((scanPage-1)*7))].scanNum);
      ctGUI.print(outStr, 82, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      ctGUI.printNumI(scanList[numScans-(i+((scanPage-1)*7))].numScanPoints,132, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      if (scanList[numScans-(i+((scanPage-1)*7))].scanStatus > 0) ctGUI.print("F", 288, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      else if (scanList[numScans-(i+((scanPage-1)*7))].scanStatus < 0) ctGUI.print("E", 288, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      if (scanList[numScans-(i+((scanPage-1)*7))].scanRedo == 'R') ctGUI.print("R", 298, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
      else if (scanList[numScans-(i+((scanPage-1)*7))].scanRedo == 'C') ctGUI.print("C", 298, 50*i+(listboxTop+37), caveatron.FONT_19, 2);
    }
    
   drawInfoBar("Page "+String(scanPage)+"/"+String(numScanPages)+"     ");
  }
}


void ViewScan (uint32_t fp, const int nr, char stype, String stext) {
  char linebuffer[51], cin;
  float scanDis, scanAzi, scanInc, scanRol, firstScanDis, lastScanDis, firstScanAzi, firstScanInc, lastScanInc, lastScanAzi, ptx, pty;
  int16_t scanPointUp[321][2], scanPointDn[321][2], scanPointLt[321][2], scanPointRt[321][2];
  int16_t scanCenterProfile[321][2], scanCenterPlan[321][2];
  boolean scanFixedPoint[321];
  int16_t planMax[2], planMin[2], profileMax[2], profileMin[2]; 
  int ptRange, ptSense;
  float ptAngle, rangeScale=32.0, rangeSpan;
  int rangeCenter[2], rotCount, fixedRotCount, iin, radius=2;
  boolean scanPlanView=true, refreshScanPlot=true, done=false;
  String scaleText, dirText;

  fileName = fileNameBase+".cvl";
  char _fileName[sizeof(fileName)+1];
  fileName.toCharArray(_fileName, sizeof(_fileName));
  if (!theFile.open(_fileName, O_READ)) {
    ErrorBox("Could Not Open", "LIDAR File", "", 99);
  }

  myGLCD.clrScr();
  ctGUI.makeObjectInvisible(btn1);
  ctGUI.makeObjectInvisible(btn2);
  ctGUI.makeObjectInvisible(btn3);
  ctGUI.makeObjectInvisible(list1);

  //Draw display box
  myGLCD.setColor(WHITE_STD);
  myGLCD.drawLine(0,19,319,19);
  myGLCD.drawLine(0,346,319,346);
  myGLCD.drawLine(0,19,0,346);
  myGLCD.drawLine(319,19,319,346);
  ctGUI.print(stext, 10, 0, caveatron.FONT_19, 2, LEFT_J, WHITE_STD, BLACK_STD);
  ctGUI.print("LOADING ...", 160, 170, caveatron.FONT_28, 3, CENTER_J, YELLOW_STD, BLACK_STD);

  //Load scan from file
  theFile.rewind();
  theFile.seekSet(fp);
  if (theFile.fgets(linebuffer, 50)>2) {
    if (linebuffer[0]=='S') {
      ibufstream ibuf(linebuffer);
      ibuf >> cin >> firstScanDis >> firstScanAzi >> firstScanInc >> scanRol >> iin;
      lastScanDis = firstScanDis; lastScanAzi = firstScanAzi; lastScanInc = firstScanInc;
    }
  }
  rotCount=0;
  scanFixedPoint[rotCount]=false;
  for (int i=0;i<2;i++) {scanPointUp[rotCount][i]=0; scanPointDn[rotCount][i]=0; scanPointRt[rotCount][i]=0; scanPointLt[rotCount][i]=0;}
  fixedRotCount=0;
  while ((theFile.fgets(linebuffer, 50)>3) && (rotCount<320)) {
    if (linebuffer[0]=='D') {
      rotCount++;
      scanFixedPoint[rotCount]=false;
      ibufstream ibuf(linebuffer);
      ibuf >> cin >> scanDis >> scanAzi >> scanInc >> scanRol >> iin;
      //If fixed point, interpolate distance for all scans back to last fixed point
      if (scanDis>0) {
        for(int i=0;i<(rotCount-fixedRotCount);i++) {
          int pointScanDis = 1000*(lastScanDis-i*((lastScanDis-scanDis)/(rotCount-fixedRotCount)));
          float pointScanInc = lastScanInc-i*((lastScanInc-scanInc)/(rotCount-fixedRotCount));
          float pointScanAzi = lastScanAzi-i*((lastScanAzi-scanAzi)/(rotCount-fixedRotCount));
          int j=i+fixedRotCount;
          scanCenterProfile[j][0] = pointScanDis*cos(pointScanInc/57.2958); scanCenterProfile[j][1] = pointScanDis*sin(-pointScanInc/57.2958);
          scanPointUp[j][0] = scanPointUp[j][0]+scanCenterProfile[j][0]; scanPointUp[j][1] = scanPointUp[j][1]+scanCenterProfile[j][1];
          scanPointDn[j][0] = scanPointDn[j][0]+scanCenterProfile[j][0]; scanPointDn[j][1] = scanPointDn[j][1]+scanCenterProfile[j][1];
          scanCenterPlan[j][0] = pointScanDis*sin(pointScanAzi/57.2958); scanCenterPlan[j][1] = pointScanDis*cos(pointScanAzi/57.2958);
          scanPointRt[j][0] = scanPointRt[j][0]+scanCenterPlan[j][0]; scanPointRt[j][1] = scanPointRt[j][1]+scanCenterPlan[j][1];
          scanPointLt[j][0] = scanPointLt[j][0]+scanCenterPlan[j][0]; scanPointLt[j][1] = scanPointLt[j][1]+scanCenterPlan[j][1];
        }
        fixedRotCount = rotCount;
        lastScanDis = scanDis;
        lastScanAzi = scanAzi;
        lastScanInc = scanInc;
        scanFixedPoint[rotCount]=true;
      }
      for (int i=0;i<2;i++) {scanPointUp[rotCount][i]=0; scanPointDn[rotCount][i]=0; scanPointRt[rotCount][i]=0; scanPointLt[rotCount][i]=0;}
    }
    else if (linebuffer[0]=='R') {
      rotCount++;
      scanFixedPoint[rotCount]=false;
      ibufstream ibuf(linebuffer);
      ibuf >> cin >> scanAzi >> scanInc >> scanRol >> iin;
      for (int i=0;i<2;i++) {scanPointUp[rotCount][i]=0; scanPointDn[rotCount][i]=0; scanPointRt[rotCount][i]=0; scanPointLt[rotCount][i]=0;}
    }
    else {
      ibufstream ibuf(linebuffer);
      ibuf >> ptAngle >> ptRange >> ptSense;
      ptAngle = ptAngle-scanRol;
      if (ptAngle<0) ptAngle=360+ptAngle; if (ptAngle>360) ptAngle=ptAngle-360;
      if (((ptAngle <= 1) || (ptAngle >= 359)) && (ptSense > 16)) {
        scanPointUp[rotCount][0] = ptRange * sin(scanInc/57.2958); scanPointUp[rotCount][1] = ptRange * cos(scanInc/57.2958);
      }
      if ((abs(90 - ptAngle) <= 1) && (ptSense > 16)) {
        scanPointLt[rotCount][0] = ptRange * sin((scanAzi-90)/57.2958); scanPointLt[rotCount][1] = ptRange * cos((scanAzi-90)/57.2958);
      }
      if ((abs(180 - ptAngle) <= 1) && (ptSense > 16)) {
        scanPointDn[rotCount][0] = ptRange * sin((scanInc+180)/57.2958); scanPointDn[rotCount][1] = ptRange * cos((scanInc+180)/57.2958);
      }
      if ((abs(270 - ptAngle) <= 1) && (ptSense > 16)) {
        scanPointRt[rotCount][0] = ptRange * sin((scanAzi+90)/57.2958); scanPointRt[rotCount][1] = ptRange * cos((scanAzi+90)/57.2958);
      }
    }
  }
  theFile.close();

  //Draw button, scale, and labels
  int offsetText=224;
  ctGUI.makeObjectVisible(btn4);
  ctGUI.makeObjectVisible(btn5);
  ctGUI.makeObjectVisible(btn6);
  if (stype=='T') {
    ctGUI.makeObjectVisible(btn7);
    ctGUI.print("Traverse", 160, 359, caveatron.FONT_19, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  } else ctGUI.print("Splay", 160, 359, caveatron.FONT_19, 2, CENTER_J, YELLOW_STD, BLACK_STD);

  myGLCD.setColor(YELLOW_STD);
  myGLCD.drawLine(offsetText+25,365,offsetText+88,365);
  myGLCD.drawLine(offsetText+25,359,offsetText+25,371);
  myGLCD.drawLine(offsetText+88,359,offsetText+88,371);
  myGLCD.drawLine(offsetText+10,387,offsetText+10,359);
  myGLCD.drawLine(offsetText+6,366,offsetText+10,359);
  myGLCD.drawLine(offsetText+14,366,offsetText+10,359);
  ctGUI.print("Plan View", 160, 388, caveatron.FONT_19, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  ctGUI.print(dirText, offsetText+10, 390, caveatron.FONT_15, 1, CENTER_J, YELLOW_STD, BLACK_STD);
  scaleText = String(64/rangeScale) + " m";
  dirText = "N";
  ctGUI.print(scaleText, offsetText+57, 371, caveatron.FONT_15, 2, CENTER_J, YELLOW_STD, BLACK_STD);
  ctGUI.print(dirText, offsetText+10, 390, caveatron.FONT_15, 1, CENTER_J, YELLOW_STD, BLACK_STD);
  myGLCD.drawCircle(offsetText+10,397,10);  

  while (done==false) {
    if (refreshScanPlot==true) {
      refreshScanPlot=false;
      myGLCD.setColor(BLACK_STD);
      myGLCD.fillRect(1,20,318,345);
   
      if (stype=='T') {
        //Draw Passage mode walls and traverse path for Profile view
        if (scanPlanView==false) {
          rangeCenter[0]=159+(156*cos(firstScanInc/57.2958));
          rangeCenter[1]=19+159+(156*sin(-firstScanInc/57.2958));
          for (int i=0; i<=rotCount; i++) {
            myGLCD.setColor(GREEN_STD);
            ptx = rangeCenter[0]-(scanPointUp[i][0]/rangeScale); pty = rangeCenter[1]-(scanPointUp[i][1]/rangeScale);
            //if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.drawPixel(ptx, pty);
            if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.fillCircle(ptx, pty, radius);
            ptx = rangeCenter[0]-(scanPointDn[i][0]/rangeScale); pty = rangeCenter[1]-(scanPointDn[i][1]/rangeScale);
            //if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.drawPixel(ptx, pty);
            if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.fillCircle(ptx, pty, radius);
            myGLCD.setColor(YELLOW_STD);
            ptx = rangeCenter[0]-(scanCenterProfile[i][0]/rangeScale); pty = rangeCenter[1]-(scanCenterProfile[i][1]/rangeScale);
            if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) {
              if (scanFixedPoint[i]==true) myGLCD.setColor(YELLOW_STD);
              else myGLCD.setColor(BLACK_STD);
              //myGLCD.drawPixel(ptx, pty);
              myGLCD.fillCircle(ptx, pty, radius);
            }
          }
        //Draw Passage mode walls and traverse path for Plan view
        } else {
          rangeCenter[0]=159+(156*sin(firstScanAzi/57.2958));
          rangeCenter[1]=19+159-(156*cos(firstScanAzi/57.2958));
          for (int i=0; i<=rotCount; i++) {
            myGLCD.setColor(GREEN_STD);
            ptx = rangeCenter[0]-(scanPointLt[i][0]/rangeScale); pty = rangeCenter[1]+(scanPointLt[i][1]/rangeScale);
            //if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.drawPixel(ptx, pty);
            if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.fillCircle(ptx, pty, radius);
            ptx = rangeCenter[0]-(scanPointRt[i][0]/rangeScale); pty = rangeCenter[1]+(scanPointRt[i][1]/rangeScale);
            //if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.drawPixel(ptx, pty);
            if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.fillCircle(ptx, pty, radius);
            myGLCD.setColor(YELLOW_STD);
            ptx = rangeCenter[0]-(scanCenterPlan[i][0]/rangeScale); pty = rangeCenter[1]+(scanCenterPlan[i][1]/rangeScale);
            if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) {
              if (scanFixedPoint[i]==true) myGLCD.setColor(YELLOW_STD);
              else myGLCD.setColor(BLACK_STD);
              //myGLCD.drawPixel(ptx, pty);
              myGLCD.fillCircle(ptx, pty, radius);
            }
          }
        }
        //Draw station
        myGLCD.setColor(RED_STD);
        myGLCD.fillCircle(rangeCenter[0],rangeCenter[1],radius+2);
      } 
      else if (stype=='S') {
        myGLCD.setColor(GREEN_STD);
        for (int i=0; i<=rotCount; i++) {
          ptx = 159-(scanPointLt[i][0]/rangeScale); pty = 178+(scanPointLt[i][1]/rangeScale);
          //if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.drawPixel(ptx, pty);
          if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.fillCircle(ptx, pty, radius);
          
        }
        //Draw station and rotation point
        myGLCD.setColor(YELLOW_STD);
        myGLCD.fillCircle(159,178,radius+2);
        ptx = 159+((1000*firstScanDis*sin(firstScanAzi/57.2958))/rangeScale); pty = 178+((1000*firstScanDis*cos(firstScanAzi/57.2958))/rangeScale);
        myGLCD.setColor(RED_STD);
        if (((ptx>0)&&(ptx<319))&&((pty>19)&&(pty<346))) myGLCD.fillCircle(ptx,pty,radius+2);
      }
    }

    // Check for button presses
    if ((myTouch.dataAvailable()==true) || (ctGUI.anyButtonPressed==true)) {
      btnFound=ctGUI.checkAllButtons(OnRelease);
      switch(btnFound) {
        case 4:           //Done
          done = true;
          break;
        case 5:           //Zoom In
          refreshScanPlot=true;
          rangeScale=rangeScale/2;
          if (rangeScale<=4) ctGUI.makeObjectInvisible(btn5);
          if (rangeScale<256) ctGUI.makeObjectVisible(btn6);
          break;
        case 6:           //Zoom Out
          refreshScanPlot=true;
          rangeScale=rangeScale*2;
          if (rangeScale>4) ctGUI.makeObjectVisible(btn5);
          if (rangeScale>=256) ctGUI.makeObjectInvisible(btn6);
          break;
        case 7:           //Switch to Profile View
          refreshScanPlot=true;
          scanPlanView=false;
          ctGUI.makeObjectInvisible(btn7);
          ctGUI.makeObjectVisible(btn8);
          dirText = "U";
          ctGUI.print("Profile View", 160, 388, caveatron.FONT_19, 2, CENTER_J, YELLOW_STD, BLACK_STD);
          break;
        case 8:           //Switch to Plan View
          refreshScanPlot=true;
          scanPlanView=true;
          ctGUI.makeObjectInvisible(btn8);
          ctGUI.makeObjectVisible(btn7);
          dirText = "N";
          ctGUI.print("   Plan View   ", 160, 388, caveatron.FONT_19, 2, CENTER_J, YELLOW_STD, BLACK_STD);
          break;
      }
      scaleText = String(rangeScale/16) + " m";
      ctGUI.print(scaleText, offsetText+57, 371, caveatron.FONT_15, 2, CENTER_J, YELLOW_STD, BLACK_STD);
      ctGUI.print(dirText, offsetText+10, 390, caveatron.FONT_15, 1, CENTER_J, YELLOW_STD, BLACK_STD);
      myGLCD.setColor(YELLOW_STD);
      myGLCD.drawCircle(offsetText+10,397,10);
    }
  }
  ctGUI.clearAllObjects();
  myGLCD.clrScr();
  ScanViewerSetup(scanPage);
  ctGUI.redrawAllObjects();
}

