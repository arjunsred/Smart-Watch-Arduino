#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include "U8glib.h"
#include <avr/sleep.h>

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);
TMRpcm audio;

#define interruptPin 2 //Pin we are going to use to wake up the Arduino
#define SD_ChipSelectPin 10   
Sd2Card card;
SdVolume volume;
SdFile root;

bool DisplayFileSymbol = true;
int Battery = 50;
int batteryLevel;
String SerialData;
String myFiles[] = {".PythonProjects",".Audiotest.py",".Important.txt",".HJ.txt", ".Bling.ino", ".ultimatum.exe","dancer.mp4", ".juggler.mp4"};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SD.begin(SD_ChipSelectPin);
  audio.CSPin = SD_ChipSelectPin; 
  pinMode(LED_BUILTIN,OUTPUT);//We use the led on pin 13 to indecate when Arduino is A sleep
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  digitalWrite(LED_BUILTIN,HIGH);//turning LED on 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available() > 0){
    
    SerialData = Serial.readString();
  
  }

  if(SerialData == "FileManager"){
    menu3pt2();
  }
  else if(SerialData == "WeatherReport"){
    menu2();
  }
  else if(SerialData == "NormalDisplay"){
    menu1();
  }
  else if(SerialData == "Sleep"){
    
    sleep();
  }
  else{
    menu1();
  }
  
}

void menu1(){
  u8g.firstPage();  
  do 
    {    
      u8g.drawFrame(112, 1, 12, 7);
      u8g.drawFrame(124, 3, 2, 3);
      
      batteryLevel = Battery/10;
      for(int i=0; i<=batteryLevel; i++){
          Serial.println(i); 
          u8g.drawLine((112+i), 1, (112+i), 7);  
        }   
      u8g.drawLine(105, 1, 105, 7);
      u8g.drawLine(105, 1, 108, 2);
      u8g.drawLine(108, 2, 102, 6);
      u8g.drawLine(102, 3, 108, 6);
      u8g.drawLine(108, 6, 105, 7);
      
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(1, 1, "11 MAR 2020 FRI");
      u8g.drawLine(1, 10, 128, 10);
      
      u8g.setFont(u8g_font_helvB24n);
      u8g.drawStr(26, 45, "12:28");
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(34, 55, "PM");
      u8g.setFont(u8g_font_helvB24n);
         
    }while(u8g.nextPage());
}
void menu2(){
   u8g.firstPage();  
   do 
     {  
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawLine(1, 31, 128, 31);    
      u8g.drawStr(1, 1, "Temperature");
      u8g.drawStr(1, 15, "30.00 C | 86.00 F");
      u8g.drawStr(1, 33, "Humidity");
      u8g.drawStr(1, 48, "60%");
      
     }while(u8g.nextPage());
}
void menu3pt1(){
  u8g.firstPage();  
   do 
     {
      u8g.drawFrame(36, 4, 26, 5);
      u8g.drawFrame(36, 8, 56, 48);
      u8g.drawLine(45, 55, 45, 34);
      u8g.drawLine(45, 34, 83, 34);
      u8g.drawLine(83, 34, 83, 55);
      u8g.drawLine(54, 55, 54, 43);
      u8g.drawLine(54, 43, 74, 43);
      u8g.drawLine(74, 43, 74, 55);   
      }while(u8g.nextPage());
}
void menu3pt2(){
   if(DisplayFileSymbol == true){
        menu3pt1();
        delay(3000);
        DisplayFileSymbol=false;
      }
  u8g.firstPage();  
   do 
     { 
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      int n = 1;
      int size = (sizeof(myFiles)/6);
      for(int y = 0; y < size; y++){
        u8g.drawStr(1, n, myFiles[y].c_str());  
        n = n + 13;   
      }
      
     }while(u8g.nextPage());
}
void sleep(){
    u8g.firstPage();  
    do 
      {        
      }while(u8g.nextPage());
    sleep_enable();
    attachInterrupt(0, wake, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000); 
    sleep_cpu();
}
void wake(){
  SerialData = "NormalDisplay";
  sleep_disable();
  detachInterrupt(0); 
}
