#include <Arduino.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include "GyverUART.h"
#include "defines.h"
//#include <Fonts/FreeSans18pt7b.h>
//#include <Fonts/FreeMono12pt7b.h>
#include <FreeDefaultFonts.h>
//#include <Fonts/FreeMonoBold12pt7b.h>
#include <FontsRus/TimesNRCyr10.h> 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
MCUFRIEND_kbv tft;
String inString="";

/*
int i=0;
int tw;
int th;
int prevx=0;
int prevy=0;
int x=0;
int y=0;
int step=1;
int speed = 0;
char cstr[80];

String g="";
*/

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



void bar(String b){
        String part01 = getValue(b,' ',1);
        String part02 = getValue(b,' ',2);
        String part03 = getValue(b,' ',3);
        String part04 = getValue(b,' ',4);
        String part05 = getValue(b,' ',5);
        tft.fillRect(part01.toInt(),part02.toInt(),part03.toInt(),part04.toInt(),part05.toInt());
}
void clrscr(void){
 tft.fillScreen(BLACK);
 tft.drawFastHLine(0,0,tft.width()-1,GREEN) ;// ok
 tft.drawFastHLine(0,tft.height()-1,tft.width()-1,GREEN) ;// ok
 tft.drawFastVLine(0,0,tft.height()-1,GREEN) ;// ok
 tft.drawFastVLine(tft.width()-1,0,tft.width()-1,GREEN) ;// ok
}
String pr="";
int px;
int py;
int ps;
void text(String b){
/////// стираем старое черным цветом
  tft.setFont(&TimesNRCyr10pt8b);
  tft.setCursor(px, py);
  tft.setTextColor(BLACK);
  tft.setTextSize(ps);
  tft.print(pr);
/////// запоминаем что стирать и рисуем новое
  String part01 = getValue(b,' ',1);
  String part02 = getValue(b,' ',2);
  String part03 = getValue(b,' ',3);
  String part04 = getValue(b,' ',4);
  String part05 = getValue(b,' ',5);
  px = part01.toInt();
  py = part02.toInt();
  ps = part04.toInt();
  tft.setFont(&TimesNRCyr10pt8b);
  tft.setCursor(px, py);
  tft.setTextColor(part03.toInt());
  tft.setTextSize(ps);//;
  tft.print(part05);
  pr = part05;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  uartBegin();
  uartPrintln("+ Started Video"); 
  uint16_t ID = tft.readID(); uartPrintln(ID, HEX);
  tft.begin(ID); 
  //tft.cp437(true);
  tft.setRotation(LANDSCAPE);
  clrscr();
 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  
  while (uartAvailable() > 0) {
    char inChar = uartRead();
    inString += inChar;
    if (inChar == '\n') {
      switch (inString.charAt(0))
      {
       case '0':uartPrintln("+ 0");
        break;
       case 'b':uartPrintln("+ bar");
        bar(inString);
        break;
       case 'c':uartPrintln("+ clrscr");
        clrscr();
        break;
       case 't':uartPrintln("+ text");
        text(inString);
        break;   
       default: uartPrintln("- unknown");
        break; 
      }
      inString="";   
    }
    else {
        ;
    }      
  }
}
