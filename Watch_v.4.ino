
#include "DHT.h"
#include "U8glib.h"
#include <avr/sleep.h>

#define DHTPIN 7
#define DHTTYPE DHT11


U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);
DHT dht(DHTPIN, DHTTYPE);


int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0; 
int smokeAX = A0;
int sensorThreshold = 680;
int analogSensor = 0;
const int pin = 3;
const int pwm = 4 ;

String SerialData;
String myDate;
String myTime;
String myTime2;
String myPhone = "phone";
String myText = "message";
String mySteps;
String myDistance;
String mySTime;

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

void setup() { 
  u8g.firstPage();  
    do 
     {    
      
        u8g.drawBitmapP(43, 0, 5,56, obake);
                                   
      }while(u8g.nextPage());

  delay(5000);
  pinMode(pin, INPUT_PULLUP);
  Serial.begin(9600);
  dht.begin();
  pinMode(pwm, OUTPUT);
}

void loop() {
  
  buttonCount();
  int analogSensor = analogRead(smokeAX);
  if(Serial.available() > 0){  
    
    myDate = Serial.readStringUntil('|');
    
    myTime = Serial.readStringUntil('|');
    
    myPhone = Serial.readStringUntil('|');
    
    myText  = Serial.readStringUntil('|');

    mySteps = Serial.readStringUntil('|');

    myDistance = Serial.readStringUntil('|');
    
    mySTime  = Serial.readStringUntil('\n');
    
    
  }
  
  if(analogSensor > sensorThreshold){
    GasSensor();
  }
  else if(myPhone != "phone"){
    phone();
    digitalWrite(pwm, HIGH);
    delay(500);
    digitalWrite(pwm, LOW);
    delay(500);
  }
  else if(buttonPushCounter == 0){
    menu1();
  }
  else if(buttonPushCounter == 1){
    menu2(); 
  }
  else if(buttonPushCounter == 2){
    menu3();
  }
  else if(buttonPushCounter == 3){
    buttonPushCounter = -1;
    buttonState = 0;
    lastButtonState = 0; 
  
  }
  else{
    
  }
  
}

void menu1(){
  u8g.firstPage();  
  do 
    {    
     
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(1, 1, myDate.c_str());
      u8g.drawLine(1, 10, 128, 10);
      
      u8g.setFont(u8g_font_helvB24n);
      u8g.setPrintPos(0, 45);
      u8g.print(myTime);
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(34, 55, "PM");
      u8g.setFont(u8g_font_helvB24n);
         
    }while(u8g.nextPage());
}
void menu2(){
  float humi  = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);
  String tempC2 = "";
  String tempF2 = ""; 
  String humi2 = "";  
  tempC2.concat(tempC);
  tempF2.concat(tempF);
  humi2.concat(humi);
  String humi3 = humi2 + "%";
   u8g.firstPage();  
   do 
     {  
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawLine(1, 31, 128, 31);
      String totalTemp = tempC2+" C"+"|"+tempF2+" F";    
      u8g.drawStr(1, 1, "Temperature");
      u8g.drawStr(1, 15, totalTemp.c_str());
      u8g.drawStr(1, 33, "Humidity");
      u8g.setPrintPos(1, 48);
      u8g.print(humi3);
      
      
     }while(u8g.nextPage());
}
void menu3(){
   u8g.firstPage();  
   do 
     {  
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
        
      u8g.drawStr(1, 1, "Pedometer");
      String mySteps2 = "Steps:" + mySteps;
      String myDistance2 = "Distance:" + myDistance;
      String mySTime2 = "Time:" + mySTime;
      u8g.drawStr(1, 20, mySteps2.c_str());
      u8g.drawStr(1, 31, myDistance2.c_str());
      u8g.drawStr(1, 42, mySTime2.c_str());
      
      
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
      u8g.drawStr(67, 16 , myText.c_str());
      u8g.drawStr(67, 26, "texted...");
  }while(u8g.nextPage());
                           
}
void phone(){
  u8g.firstPage();  
    do 
      {    
        u8g.setFont(u8g_font_6x10);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawBitmapP(16, 16, 4, 32, ph_15);
        u8g.drawStr(57, 21 , myPhone.c_str());
        u8g.drawStr(57, 31, "called...");
                           
      }while(u8g.nextPage());
}
void GasSensor(){
  //int analogSensor = analogRead(smokeAX);
  u8g.firstPage();  
  do 
    {    
      u8g.setFont(u8g_font_6x10);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(26, 28 , "Dangerous Gases");
      u8g.drawStr(45, 40 , "Detected!");
    }while(u8g.nextPage());
  
}
void buttonCount(){
  if(digitalRead(pin) == LOW){
    buttonState++;
    buttonPushCounter++;
    }
   
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      buttonPushCounter++;   
    }else{
    }
    delay(10);
  }
  lastButtonState = buttonState;
}
