#include <Arduino.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include "GyverUART.h"
#include "defines.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////
 //#include <Fonts/FreeSans18pt7b.h>
 //#include <Fonts/FreeMono12pt7b.h>
 //#include <FreeDefaultFonts.h>
 //#include <Fonts/FreeMonoBold12pt7b.h>
 //#include <FontsRus/bahamas18.h> 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
MCUFRIEND_kbv tft;
String inString="";
String pr="";
int px;
int py;
int ps;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String getValue(String data, char separator, int index)
 {
   int found = 0;
   int strIndex[] = {0, -1};
   int maxIndex = data.length()-1;
   for(int i=0; i<=maxIndex && found<=index; i++){
   if(data.charAt(i)==separator || i==maxIndex){
   found++;
   strIndex[0] = strIndex[1]+1;
   strIndex[1] = (i == maxIndex) ? i+1 : i;
   }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
 } 


String utf8rus(String source)
 {
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
  return target;
 }

void bar(String b) 
 {
        String part01 = getValue(b,' ',1);
        String part02 = getValue(b,' ',2);
        String part03 = getValue(b,' ',3);
        String part04 = getValue(b,' ',4);
        String part05 = getValue(b,' ',5);
        tft.fillRect(part01.toInt(),part02.toInt(),part03.toInt(),part04.toInt(),part05.toInt());
 }
void clrscr(void)
 {
  tft.fillScreen(BLACK);
  tft.drawFastHLine(0,0,tft.width()-1,GREEN) ;// ok
  tft.drawFastHLine(0,tft.height()-1,tft.width()-1,GREEN) ;// ok
  tft.drawFastVLine(0,0,tft.height()-1,GREEN) ;// ok
  tft.drawFastVLine(tft.width()-1,0,tft.width()-1,GREEN) ;// ok
 }



void text(String b)
 {
   /////// стираем старое черным цветом
  tft.setFont();//&Bahamas18pt8b);
  tft.setCursor(px, py);
  tft.setTextColor(BLACK);
  tft.setTextSize(ps);
  tft.print(utf8rus(pr));
   /////// запоминаем что стирать и рисуем новое
  String part01 = getValue(b,' ',1);
  String part02 = getValue(b,' ',2);
  String part03 = getValue(b,' ',3);
  String part04 = getValue(b,' ',4);
  String part05 = getValue(b,' ',5);
  px = part01.toInt();
  py = part02.toInt();
  ps = part04.toInt();
  tft.setFont();//&Bahamas18pt8b);
  tft.setCursor(px, py);
  tft.setTextColor(part03.toInt());
  tft.setTextSize(ps);//;
  tft.print(part05);
  pr = part05;
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
 {
  Serial.begin(115200);
  //Serial.println("+ Started Video"); 
  uint16_t ID = tft.readID(); 
  //Serial.println(ID, HEX);
  text("Start");
  tft.begin(ID); 
  tft.cp437(true);
  tft.setRotation(LANDSCAPE);
  clrscr(); 
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
 {
  while (Serial.available() > 0) {
    char inChar = Serial.readstring();
    inString += inChar;
    if (inChar == '\n') {
      switch (inString.charAt(0))
      {
       case '0':Serial.println("+ 0");
        break;
       case 'b':Serial.println("+ bar - "+inString);
        bar(inString);
        break;
       case 'c':Serial.println("+ clrscr");
        clrscr();
        break;
       case 't':Serial.println("+ text");
        text(inString);
        break;
       case 'a':Serial.println("+ a test");
        tft.fillRect(10,10,80,80,40);
        break;    
       default: Serial.println("- unknown");
        break; 
      }
      inString="";   
    }
    else {
        ;
    }      
  }
  
 }
