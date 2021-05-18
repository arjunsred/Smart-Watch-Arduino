#include "U8glib.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <avr/sleep.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
const int pin = 2;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
int Battery;
int batteryLevel;
int arduinoVoltage;
int month;
int day;
int year;
int hour;
int minute;
int second;
int centisecond;
int dist;
int recNum = 0;
int distance;
int i1;
int i2;
int i3;
int i4;
int i5;
int i6;
int i7;

const double EARTH_RADIUS_IN_METERS = 6372797.5;
double data1;
double data2;
double lat = 10.4683174;;
double lon = 76.389833;

float voltage;
float degreesF;
float degreesC;

String district;
String state;
String extra;
String country;
String Line;
String Data1;
String Data2;
String Data3;
String Data4;
String Data5;
String Data6;
String Data7;

bool error = false;


File myFile;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup() {
  ss.begin(GPSBaud);
  SD.begin(10);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(pin, INPUT_PULLUP);
  
}

void loop() {
  GPSLocator(); 
}

void BatteryMonitor() {
 
  u8g.firstPage();
  do
  {
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();
    u8g.drawFrame(112, 1, 12, 7);
    u8g.drawFrame(124, 3, 2, 3);

    batteryLevel = Battery / 10;
    for (int i = 0; i <= batteryLevel; i++) {
      u8g.drawLine((112 + i), 1, (112 + i), 7);
    }
  } while (u8g.nextPage());
}

void BatteryTime() {
  //Battery
  arduinoVoltage = analogRead(A0);
  arduinoVoltage = arduinoVoltage * 5.0 / 1023;
  Battery = map(arduinoVoltage, 3.6, 4.2, 0, 100);
  //Time
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      if (gps.location.isValid()) {
        lat = (gps.location.lat() , 6);
        lon = (gps.location.lng() , 6);
      }
      else {
        error = true;
      }
      if (gps.date.isValid())
      {
        month = gps.date.month();
        day = gps.date.day();
        year = gps.date.year();
      }
      else {
        error = true;
      }
      if (gps.time.isValid())
      {
        if (gps.time.hour() > 10) {
          hour = gps.time.hour();
        }
        if (gps.time.minute() > 10) {
          minute = gps.time.minute();
        }
        if (gps.time.second() > 10) {
          second = gps.time.second();
        }
        if (gps.time.centisecond() > 10) {
          centisecond = gps.time.centisecond();
        }
      }
      else {
        error = true;
      }
    }
  }

  if (error != true) {
    u8g.firstPage();
    do
    {
      //Battery
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawFrame(112, 1, 12, 7);
      u8g.drawFrame(124, 3, 2, 3);

      batteryLevel = Battery / 10;
      for (int i = 0; i <= batteryLevel; i++) {
        Serial.println(i);
        u8g.drawLine((112 + i), 1, (112 + i), 7);
      }
      //Time
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.setFont(u8g_font_6x10);
      int date = day + month + year;
      u8g.setPrintPos(18, 3);
      //u8g.drawStr(1, 1, date);
      u8g.print(date);
      u8g.drawLine(1, 10, 128, 10);
      int time = hour + minute + second + centisecond;
      //u8g.drawStr(26, 45, time);
      u8g.setPrintPos(26, 4);
      u8g.print(time);
    } while (u8g.nextPage());
  }
}
void GPSLocator() {
  myFile = SD.open("MYFILE~1.TXT");
  
  if (myFile) {
    
    while (myFile.available()) {
      
      Line = myFile.readStringUntil('\r');

      i1 = Line.indexOf(',');
      Data1 = Line.substring(0, i1);
      i2 = Line.indexOf(',', i1 + 1);
      Data2 = Line.substring(i1 + 1, i2);
      i3 = Line.indexOf(',', i2 + 1 );
      Data3 = Line.substring(i2 + 1, i3);
      i4 = Line.indexOf(',', i3 + 1 );
      Data4 = Line.substring(i3 + 1, i4);
      i5 = Line.indexOf(',', i4 + 1 );
      Data5 = Line.substring(i4 + 1, i5 );
      i6 = Line.indexOf(',', i5 + 1 );
      Data6 = Line.substring(i5 + 1);
      data1 = Data1.toDouble();
      data2 = Data2.toDouble();

      dist = EARTH_RADIUS_IN_METERS * haversine(lat, lon, data1, data2) / 1000;

      if (recNum == 0) {
        district = Data3;
        state = Data4;
        country = Data6;
        extra = Data5;
        distance = dist;
      }
      else {
        if (dist < distance) {
          distance = dist;
          district = Data3;
          state = Data4;
          extra = Data5;
          country = Data6;
        }
      }
      recNum++;
      if (recNum == 3434) {
        recNum = 0;
        break;
        myFile.close();
      }
    }
    myFile.close();
  }
  else {
    digitalWrite(13, HIGH);
  }
  u8g.firstPage();
  do{
    u8g.setFont(u8g_font_6x10);
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();
    u8g.drawStr(1, 1, district.c_str());
    Serial.println(district);
    
    
  }while(u8g.nextPage());
}
void Temperature() {
  voltage = analogRead(A0) * 0.004882813;
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0 / 5.0) + 32.0;
  float Dis = degreesC;
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();
    u8g.drawLine(1, 31, 128, 31);
    u8g.drawStr(1, 1, "Temperature");
    u8g.setPrintPos(1, 15);
    u8g.print(Dis);
    u8g.drawStr(1, 33, "Humidity");
    u8g.drawStr(1, 48, "Not known now");

  } while (u8g.nextPage());

}
double haversine(double fromLat, double fromLng, double toLat, double toLng) {
  double lat_arc = (fromLat - toLat) * DEG_TO_RAD;
  double lon_arc = (fromLng - toLng) * DEG_TO_RAD;
  double lat_h = sin(lat_arc * 0.5)  ;
  lat_h *= lat_h;
  double lon_h = sin(lon_arc * 0.5);
  lon_h *= lon_h;
  double tmp = cos(fromLat * DEG_TO_RAD) * cos(toLat * DEG_TO_RAD);
  return 2.0 * asin(sqrt(lat_h + tmp * lon_h));
}
