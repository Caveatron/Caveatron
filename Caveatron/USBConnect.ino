/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                  USB Connection Mode Functions for File List Display, File Transfer and Deletion
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void USBConnectMode() { 
  String sOut;
  char inChar;
  byte checksum;
  int i,j,k,numbytes,numpackets,lastnumbytes,packetcount,packetsize;
  char data[3610];
    
  InitSD_USBConnectMode();

  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("SD CARD FILE", CENTER, 60);
  myGLCD.print("TRANSFER MODE", CENTER, 90);
  delay(100);
  sOut = "*"+caveatron.serialNumber+softwareVersion;
  Serial.print(sOut);
  
  while (true) {
    if (Serial.available()) { // If data is available to read,
      inChar = Serial.read();
      char filename[13]="";
      
      switch (inChar) {   
        
        // List Files
        case 'L':
          delay(100);
          myGLCD.print(" Listing Files ", CENTER, 170);
          myGLCD.print("                ", CENTER, 200);
          myGLCD.print("                ", CENTER, 230);
          Serial.print("@");
          Serial.flush();
          delay(5);
          j=0;
          sd.vwd()->rewind();
          while (theFile.openNext(sd.vwd(), O_READ)) {
            if (theFile.isFile()==true) {
              theFile.getName(filename, sizeof(filename));
              //String filenameStr = filename;
              //int idx = filenameStr.indexOf('.');
              //if(filenameStr.substring(idx+1, idx+4)=="TXT") {
                Serial.print(filename);
                Serial.print("\t");
                int fsize = ceil(float (theFile.fileSize())/1000);
                Serial.print(fsize, DEC);
                Serial.print("\t");
                theFile.printCreateDateTime(&Serial);
                Serial.println();
              //}
            } 
            //else {
              //FileTransferError("Corrupt Files", "");
            //}
            theFile.close();
          }
          Serial.println("%");
          Serial.flush();
          inChar = (char)0;
          myGLCD.print("    Ready    ", CENTER, 170);
          break;
     
        // Setup for File Transfer
        case 'T':
          myGLCD.print("Transfer File", CENTER, 170);
          delay(100);
          while (!Serial.available()) {}
          Serial.readBytesUntil('\n', filename, 13);
          myGLCD.print(filename, CENTER, 200);
          if(!theFile.open(filename, O_READ)) {
            FileTransferError("File Not Found", filename);
          } 
          else { 
            delay(100);
            numbytes = theFile.available();
            numpackets = (numbytes/3600) + 1;
            lastnumbytes = numbytes%3600;
            
            packetcount = 0;
            Serial.println(numbytes);
            Serial.println(numbytes);
            Serial.print("!");
          }
          break;
            
         
          // Send Next Packet
          case 'P':
            packetcount++;  
            checksum = 0;
            if (packetcount < numpackets) packetsize = 3600;
            else packetsize = numbytes%3600;
            for (i=0; i<packetsize; i++) {
              data[i] = theFile.read();
              checksum = checksum^(byte(data[i]));
            }
            for (i=0; i<packetsize; i++) Serial.write(data[i]);
            Serial.write(char(checksum));
            break;


          // Resend Last Packet
          case 'R':
            for (i=0; i<packetsize; i++) Serial.write(data[i]);
            Serial.write(checksum);
            break;


          // Transmit Error
          case 'E':
            theFile.close();
            FileTransferError("Transfer Failed", filename);
          break;

          // Transmit Complete
          case 'C':
            theFile.close();
            myGLCD.print("Transfer Done", CENTER, 170);
            myGLCD.print("                ", CENTER, 200);
            delay(500);
          break;
          
        // Delete File
        case 'D':
          myGLCD.print(" Delete File ", CENTER, 170);
          delay(100);
          while (!Serial.available()) {}
          Serial.readBytesUntil('\n', filename, 13);
          myGLCD.print(filename, CENTER, 200);
          if(!theFile.open(filename, O_READ)) {
            FileTransferError("File Not Found", filename);
          } 
          else { 
            theFile.close();
            if(!theFile.remove(sd.vwd(), filename)) {
              FileTransferError("File Not Deleted", filename);
            } 
            else {
              myGLCD.print(" Deletion Done ", CENTER, 170);
              delay(500);
              Serial.print("$");
            }    
          }
          break;
     
        //Check for Connection 
        case '?':
          delay(100);
          Serial.print("?");
          break;
      }
    }
  }
}

// Initialize SD card for USB connection mode
void InitSD_USBConnectMode() {
  char lineBuffer[25], c1;
  #if defined(_SAM3XA_)
    SDFlag = sd.begin(caveatron.SD_CSpin, SPI_FULL_SPEED);
  #endif
}

// Display error messages
void FileTransferError(String s1, String s2) {
  myGLCD.print("    ERROR!    ", CENTER, 170);
  myGLCD.print(s1, CENTER, 200);
  myGLCD.print(s2, CENTER, 230);
  Serial.print("^");
}


