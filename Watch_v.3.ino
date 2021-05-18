
#include "U8glib.h"
#include <avr/sleep.h>

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);


#define interruptPin 2 //Pin we are going to use to wake up the Arduino

extern volatile unsigned long timer0_millis;
unsigned long Time;
int seconds;
int minutes;
int hours;
int seconds3;
int minutes3;
int hours3;
int Battery = 50;
int batteryLevel;
String SerialData;
bool TimerPlay;
bool DisplayTriangle;
bool CV;
const uint8_t ph_15[] PROGMEM = {
0x06, 0x00, 0x70, 0x00, 0x0E, 0x00, 0xFE, 0x00, 0x0F, 0x00, 0xFF, 0x80, 0x1F, 0x80, 0x07, 0xC0,
0x3F, 0x80, 0x01, 0xE0, 0x7F, 0xC0, 0x00, 0xF0, 0xFF, 0xC0, 0x00, 0x38, 0xFF, 0xE0, 0xF0, 0x3C,
0xFF, 0xE1, 0xFC, 0x1C, 0x7F, 0xC0, 0xFE, 0x0E, 0x7F, 0x80, 0x0F, 0x0E, 0x7F, 0x00, 0x07, 0x06,
0x7F, 0x00, 0x03, 0x87, 0x3F, 0x00, 0x03, 0x87, 0x3F, 0x80, 0x03, 0x87, 0x1F, 0x80, 0x03, 0x82,
0x0F, 0xC0, 0x01, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00,
0x01, 0xF8, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0xFE, 0x01, 0xC0, 0x00, 0x7F, 0x83, 0xF0,
0x00, 0x3F, 0xFF, 0xF8, 0x00, 0x1F, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xFF, 0x00, 0x03, 0xFF, 0xFE,
0x00, 0x01, 0xFF, 0xFC, 0x00, 0x00, 0x7F, 0xF8, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x03, 0xC0
};
static const byte PHONE_WIDTH = 16;
static const byte PHONE_HEIGHT = 16;

//CREATE AN IMAGE FOR INCOMING TEXTS
const uint8_t txt_ico [] PROGMEM ={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1F, 0xFF, 0xFF, 0xF8, 0x3F, 0xFF, 0xFF, 0xFC, 0x38, 0x00, 0x00, 0x1C, 0x3E, 0x00, 0x00, 0x7C,
0x37, 0x00, 0x00, 0xEC, 0x33, 0xC0, 0x03, 0xCC, 0x30, 0xE0, 0x07, 0x0C, 0x30, 0x78, 0x1E, 0x0C,
0x30, 0x1C, 0x38, 0x0C, 0x30, 0x0F, 0xF0, 0x0C, 0x30, 0x03, 0xC0, 0x0C, 0x30, 0x01, 0x80, 0x0C,
0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C,
0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C,
0x3F, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const byte TEXT_WIDTH = 16;
static const byte TEXT_HEIGHT = 16;
const uint8_t obake [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x80,
0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x02, 0x00, 0x00, 0xE0, 0x00, 0x07, 0x00, 0x01, 0xC0, 0x00,
0x03, 0x80, 0x03, 0x80, 0x00, 0x01, 0x80, 0x03, 0x80, 0x00, 0x01, 0xC0, 0x07, 0x00, 0x18, 0x00,
0xE0, 0x0E, 0x01, 0xFF, 0xC0, 0x70, 0x1F, 0x07, 0xFF, 0xE0, 0xF8, 0x1F, 0x9F, 0xFF, 0xF9, 0xF8,
0x3F, 0xFF, 0xFF, 0xFF, 0xFC, 0x3F, 0xFC, 0xFF, 0x3F, 0xF8, 0x1F, 0xF8, 0x7F, 0x1F, 0xF8, 0x0F,
0xF0, 0xE7, 0x07, 0xF0, 0x0F, 0xE0, 0xE7, 0x07, 0xE0, 0x07, 0xF0, 0xFF, 0x0F, 0xC0, 0x1B, 0xF8,
0x7E, 0x1F, 0xF8, 0x1F, 0xFC, 0x18, 0x3F, 0xF8, 0x1F, 0xFF, 0x00, 0xFF, 0xF8, 0x1F, 0xFF, 0xE7,
0xFF, 0xF8, 0x07, 0xFF, 0xFF, 0xFF, 0xE0, 0x03, 0xFE, 0xFE, 0x7F, 0x80, 0x00, 0xFE, 0x38, 0x7E,
0x00, 0x00, 0x1F, 0x7F, 0xF8, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00,
0x00, 0x0F, 0xA5, 0xF0, 0x00, 0x00, 0x0D, 0x24, 0xB0, 0x00, 0x00, 0x0D, 0x00, 0xB0, 0x00, 0x00,
0x09, 0x00, 0x10, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x08,
0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, 0x10, 0x18, 0x08, 0x00, 0x00, 0x12, 0x3C,
0x98, 0x00, 0x00, 0x1B, 0x3C, 0xD8, 0x00, 0x00, 0x0F, 0x3C, 0xF0, 0x00, 0x00, 0x03, 0x3C, 0xC0,
0x00, 0x00, 0x00, 0x3C, 0x10, 0x00, 0x00, 0x00, 0x3C, 0x78, 0x00, 0x00, 0x00, 0x3D, 0xE4, 0x00,
0x00, 0x00, 0x1F, 0xC4, 0x00, 0x00, 0x00, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
static const byte LOGO_WIDTH = 54;
static const byte LOGO_HEIGHT = 77;

const int pin = 3;
const int pin2 = 4;


int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

int buttonPushCounter2 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;
int x =0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  u8g.firstPage();  
    do 
     {    
      
        u8g.drawBitmapP(0, 0, 5,56, obake);
                                   
      }while(u8g.nextPage());
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  delay(3000);
  pinMode(pin, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);//We use the led on pin 13 to indecate when Arduino is A sleep
  
  digitalWrite(LED_BUILTIN,HIGH);//turning LED on 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(pin) == LOW){
    buttonState++;
  
    buttonPushCounter++;
    }
   
  if (buttonState != lastButtonState) {
    
    if (buttonState == LOW) {
      buttonPushCounter++;
     
    }else{
    }

    delay(50);
  }
  
 if(digitalRead(pin2) == LOW){
  buttonState2++;

  buttonPushCounter2++;
  }
 
if (buttonState2 != lastButtonState2) {
  
  if (buttonState2 == LOW) {
    buttonPushCounter2++; 
    Serial.println(buttonPushCounter2);
   
  }else{
  }

  delay(50);
}

  lastButtonState = buttonState;
  lastButtonState2 = buttonState2;
  
 
  if(Serial.available() > 0){  
    SerialData = Serial.readString();
  }
  else if(buttonPushCounter == 0){
    noInterrupts();
    timer0_millis = 0;
    interrupts();    
    menu1();
    minutes=0;
    hours=0;
  }
  else if(buttonPushCounter == 1){
    noInterrupts();
    timer0_millis = 0;
    interrupts();    
    menu2();
    minutes=0;
    hours=0;
  }
  else if(buttonPushCounter == 2){
    noInterrupts();
    timer0_millis = 0;
    interrupts();    
    menu3();
    minutes=0;
    hours=0;
    x = buttonPushCounter;
  }
  else if (x == 2 and buttonPushCounter == (x+1)){
    TimerPlay = true;
    DisplayTriangle = false;
    menu3();
    
  }
  
  else if(buttonPushCounter == 4){
   buttonPushCounter = 0;
   buttonState = 0;
   lastButtonState = 0;
  }
  else if(SerialData == "WeatherReport"){
    noInterrupts();
    timer0_millis = 0;
    interrupts();    
    menu2();
    minutes=0;
    hours=0;
  }
  else if(SerialData == "phone"){
    u8g.firstPage();  
    do 
      {    
        u8g.setFont(u8g_font_6x10);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawBitmapP(16, 16, 4, 32, ph_15);
        u8g.drawStr(57, 21 , "EverybodyBlahBlah");
        u8g.drawStr(57, 31, "texted...");
                           
        
       
      }while(u8g.nextPage());
  
   }
  else if(SerialData == "message"){
    u8g.firstPage();  
    do 
      {    
        u8g.setFont(u8g_font_6x10);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawBitmapP(48, 16, 4, 32, txt_ico);
        u8g.drawStr(32, 48 , "New Message");
      }while(u8g.nextPage());
      delay(3000);
      message();
  
   }
  else if(SerialData == "NormalDisplay"){
    noInterrupts();
    timer0_millis = 0;
    interrupts();    
    menu1();
    minutes=0;
    hours=0;
  }
  else if(SerialData == "Sleep"){  
    noInterrupts();
    timer0_millis = 0;
    interrupts();  
    sleep();
    minutes=0;
    hours=0;
  }
  else if(SerialData == "Timer"){
    noInterrupts();
    timer0_millis = 0;
    interrupts();  
    minutes=0;
    hours=0;
    DisplayTriangle = true;
    menu3();
  }
  else if(SerialData == "Play"){
    
    TimerPlay = true;
    DisplayTriangle = false;
    menu3();
    
  }
  else if(SerialData == "Stop"){
    noInterrupts();
    timer0_millis = 0;
    interrupts();  
    minutes=0;
    hours=0;
    TimerPlay = false;
    DisplayTriangle = true;
    menu3;
  }
  else if(SerialData == "Pause"){
    seconds3 = seconds;
    minutes3 = minutes;
    hours3 = hours;
    noInterrupts();
    timer0_millis = seconds3;
    interrupts();  
    seconds = seconds3;
    minutes=minutes3;
    hours=hours3;
    menu3;
    TimerPlay = false;
    CV = true;
    
  }
  else if(SerialData == "Resume"){
    if(CV == true){
      noInterrupts();
      timer0_millis = seconds3*1000;
      interrupts();  
      minutes=minutes3;
      hours=hours3;
      CV = false;
    }
    
    TimerPlay = true;
    DisplayTriangle = false;
    menu3();  
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
void message(){
   
  u8g.firstPage();  
  do 
    {    
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  u8g.drawStr(67, 16 , "EverybodyBlahBlah");
  u8g.drawStr(67, 26, "texted...");
  }while(u8g.nextPage());
                           
}
void menu3(){
  
  if(TimerPlay == true){
    
    Time = millis();
    seconds = Time/1000;
    
    if(seconds == 60){
      
      noInterrupts();
      timer0_millis = 0;
      interrupts();
      minutes += 1;
    }
    if(minutes > 60){
      minutes = 0;
      hours += 1;
    }
    if(hours == 24){
      noInterrupts();
      timer0_millis = 0;
      interrupts();   
    }
    //Serial.print(seconds);
    //Serial.println(minutes);
    //Serial.print(hours);
  }
  
  u8g.firstPage();  
  do 
    {        
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(38,0,"StopWatch");
      u8g.setFont(u8g_font_helvB24n);
      u8g.setPrintPos(0,37);
      
      String seconds2 = getPadded(seconds); 
      String minutes2 = getPadded(minutes); 
      String hours2 =   getPadded(hours); 
      u8g.print(hours2+":"+minutes2+":"+seconds2);
      
      if(DisplayTriangle == true){
        u8g.drawTriangle(55, 42, 69, 51, 55, 60);  
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
String getPadded(int num) {
  char buff[6];
  char padded[7];
  
  //sprintf function will convert the long to a string
  sprintf(buff, "%.2u", num); // buff will be "01238"

  padded[0] = buff[0];
  padded[1] = buff[1];
  padded[2] = buff[2];
  padded[3] = buff[3];
  padded[4] = buff[4];
  padded[5] = buff[5];
  padded[6] = '\0'; // The terminating NULL

  return String(padded);
}
