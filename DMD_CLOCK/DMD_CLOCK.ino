#include <SPI.h>
#include  <DMD.h>
#include <TimerOne.h>
#include<Arial_black_16.h>
#include<SystemFont5x7.h>
//#include <Arial14.h>
//#include "clock.h"  
#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68

const unsigned long eventInterval = 1000;
unsigned long previousTime = 0;
int AlarmSetPin = 2;
int SnoozePin = 3;
int buzzer = 12;
int Alarm = 4;
int AlarmSet = 0;
int Snooze = 0;
int a ,b;
int val;
String str_val ;
int tempPin = A0;
char chr[5];
float cel;
int i,Hr ,Min,Sec,num,Interval,x,k = 0;
int set_hr,set_min,set_sec;
String str_val_2;


//Fire up the DMD library as dmd
DMD dmd(2,1);
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}


void setDS1307time(){
  // sets time and date data to DS3231
 byte second = 0;
 byte minute = set_min;
byte  hour = set_hr; // use 24-hour time otherwise day/date calculations will be off
byte  dayOfWeek = 6;
byte  dayOfMonth = 12;
byte  month = 1;
byte  year = 23;

Serial.print(hour, DEC);
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)

  Wire.write(0); //start 

  Wire.endTransmission();
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

void getDateDs1307(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // set DS1307 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());


  
}


void show_time(){
Wire.beginTransmission(DS1307_I2C_ADDRESS);
byte zero = 0x00;
Wire.write(zero);
Wire.endTransmission();
Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  int second = bcdToDec(Wire.read() & 0x7f  );
  int minute = bcdToDec(Wire.read());
  int hour =   bcdToDec(Wire.read() & 0x3f);
  int weekDay=    bcdToDec(Wire.read());
  int monthDay =    bcdToDec(Wire.read());
  int month =   bcdToDec(Wire.read());
  int year =  bcdToDec(Wire.read());

  
   Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
  
  }

void setup()
{
  Wire.begin(); 
   Serial.begin(9600);
 //setDS1307time();
  delay(100);
  pinMode(buzzer,OUTPUT);

  Timer1.initialize(5000);           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()  //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
  dmd.selectFont(Arial_Black_16);
  //dmd.selectFont(SystemFont5x7);
  //dmd.selectFont(Arial14);
  
  
    
   //pinMode(AlarmSetPin, INPUT_PULLUP);
  pinMode(SnoozePin, INPUT_PULLUP);
  pinMode(Alarm, OUTPUT);
  digitalWrite(Alarm, LOW);
  
}


void bluetooth(){

  
  
  if(Serial.available()>0){
    str_val = Serial.readString();
    Serial.println(str_val);
  if (str_val.startsWith("<")){
    str_val.remove(0,1);
    Hr = (str_val.toInt());
    Serial.println(Hr);
    str_val.remove(0,((str_val.indexOf(","))+1));
   
    Min = (str_val.toInt());
    str_val.remove(0,((str_val.indexOf(","))+1));

    set_hr = (str_val.toInt());
    str_val.remove(0,((str_val.indexOf(","))+1));

      set_min = (str_val.toInt());
    str_val.remove(0,((str_val.indexOf(">"))+1));

    
 setDS1307time(); 
 get_temp();
  //showTemp(); 
     }
     
     
}

}
 

void get_temp(){
  
  val = analogRead(tempPin);
  Serial.println(val);
  float mv = (val/1024.0)*5000;
   cel = mv /10;
  dtostrf(cel, 4, 2, chr);
  
  Serial.print("T= ");
  Serial.print(cel);
  Serial.print("*C");
  Serial.println();
  delay(1000);

    dmd.drawString(13,1,chr,4, GRAPHICS_NORMAL );
    dmd.writePixel(43,1, GRAPHICS_NORMAL,1 );
    dmd.drawString(46,1, "c", 1, GRAPHICS_NORMAL );
  }


 void drawMonth()
// shows day of week
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(month)
  {
  case 1:
    dmd.drawString( 2,1, "Jan,", 3, GRAPHICS_NORMAL );    
    break;  case 2:
    dmd.drawString( 2,1, "Feb,", 3, GRAPHICS_NORMAL );        
    break;  case 3:
    dmd.drawString( 2,1, "Mar,", 3, GRAPHICS_NORMAL );        
    break;  case 4:
    dmd.drawString( 2,1, "Apr,", 3, GRAPHICS_NORMAL );        
    break;  case 5:
    dmd.drawString( 2,1, "May,", 3, GRAPHICS_NORMAL );        
    break;  case 6:
    dmd.drawString( 2,1, "Jun,", 3, GRAPHICS_NORMAL );        
    break;  case 7:
    dmd.drawString( 2,1, "jul,", 3, GRAPHICS_NORMAL );        
    break;
    dmd.drawString( 2,1, "Aug,", 3, GRAPHICS_NORMAL );        
    break;
    dmd.drawString( 2,1, "Sep,", 3, GRAPHICS_NORMAL );        
    break;
    dmd.drawString( 2,1, "Oct,", 3, GRAPHICS_NORMAL );        
    break;
    dmd.drawString( 2,1, "Nov,", 3, GRAPHICS_NORMAL );        
    break;
    dmd.drawString( 39,1, "Dec,", 3, GRAPHICS_NORMAL );        
    break;
  }
}
void drawDay()
// shows day of week
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(dayOfWeek)
  {
  case 1:
    dmd.drawString( 4,1, "Sun,", 3, GRAPHICS_NORMAL );    
    break;  case 2:
    dmd.drawString( 4,1, "Mon,", 3, GRAPHICS_NORMAL );        
    break;  case 3:
    dmd.drawString( 4,1, "Tue,", 3, GRAPHICS_NORMAL );        
    break;  case 4:
    dmd.drawString( 4,1, "Wed,", 3, GRAPHICS_NORMAL );        
    break;  case 5:
    dmd.drawString( 4,1, "Thu,", 3, GRAPHICS_NORMAL );        
    break;  case 6:
    dmd.drawString( 4,1, "Fri,", 3, GRAPHICS_NORMAL );        
    break;  case 7:
    dmd.drawString( 4,1, "Sat,", 3, GRAPHICS_NORMAL );        
    break;
  }

}
  void drawYear()
// shows day of week
{
  byte second, minute, hour,  dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(year)
  {
  case 1:
    dmd.drawString( 49,1, "24", 2, GRAPHICS_NORMAL );    
    break;  
    case 2:
    dmd.drawString( 40,1, "24", 2, GRAPHICS_NORMAL );        
    break;  
    case 3:
    dmd.drawString( 40,1, "25", 3, GRAPHICS_NORMAL );        
    break;  
    case 4:
    dmd.drawString( 40,1, "26,", 3, GRAPHICS_NORMAL );        
    break;  
    case 5:
    dmd.drawString( 4,1, "27,", 3, GRAPHICS_NORMAL );        
    break;  
    case 6:
    dmd.drawString( 4,1, "28,", 3, GRAPHICS_NORMAL );        
    break; 
    case 7:
    dmd.drawString( 4,1, "29,", 3, GRAPHICS_NORMAL );        
    break;
  }
}
void drawDate()
// shows numerical date
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(dayOfMonth)
  {
  case 1:
    dmd.drawString( 38,1, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 31,1, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 31,1, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 31,1, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 31,1, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 38,1, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 31,1, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 31,1, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 31,1, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 31,1, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 31,1, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 31,1, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 31,1, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 31,1, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 31,1, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 31,1, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 31,1, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 31,1, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 31,1, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 31,1, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 31,1, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 31,1, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 31,1, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 31,1, "24", 2, GRAPHICS_NORMAL );
    break;
  case 25:
    dmd.drawString( 31,1, "25", 2, GRAPHICS_NORMAL );
    break;
  case 26:
    dmd.drawString( 31,1, "26", 2, GRAPHICS_NORMAL );
    break;
  case 27:
    dmd.drawString( 31,1, "27", 2, GRAPHICS_NORMAL );
    break;
  case 28:
    dmd.drawString( 31,1, "28", 2, GRAPHICS_NORMAL );
    break;
  case 29:
    dmd.drawString( 31,1, "29", 2, GRAPHICS_NORMAL );
    break;
  case 30:
    dmd.drawString( 31,1, "30", 2, GRAPHICS_NORMAL );
    break;
  case 31:
    dmd.drawString( 31,1, "31", 2, GRAPHICS_NORMAL );
    break;
  }
}void drawHour()
// draws the hour hand
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  /*if (hour>12) //Remove this if command for 24 hour time
  {
    hour=hour-12; // This is a 12 hour Clock 
  }
  */
  switch(hour)
  {
   case 0:
    dmd.drawString( 1,1, "00", 2, GRAPHICS_NORMAL );
    break;
  case 1:
    dmd.drawString( 1,1, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 1,1, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 1,1, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 1,1, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 1,1, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 1,1, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 1,1, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString(1,1, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString(1,1, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString(1,1, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString(1,1, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString(1,1, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString(1,1, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString(1,1, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString(1,1, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString(1,1, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString(1,1, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString(1,1, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString(1,1, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString(1,1, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString(1,1, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString(1,1, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString(1,1, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString(1,1, "24", 2, GRAPHICS_NORMAL );
    break;
  }
}void drawMinute()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(minute)
  {
  case 0:
    dmd.drawString( 23,1, "00", 2, GRAPHICS_NORMAL );
    break;
  case 1:
    dmd.drawString( 23,1, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 23,1, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 23,1, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 23,1, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 23,1, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 23,1, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 23,1, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 23,1, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 23,1, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 23,1, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 23,1, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 23,1, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 23,1, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 23,1, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 23,1, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 23,1, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 23,1, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 23,1, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 23,1, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 23,1, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 23,1, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 23,1, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 23,1, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 23,1, "24", 2, GRAPHICS_NORMAL );
    break;
  case 25:
    dmd.drawString( 23,1, "25", 2, GRAPHICS_NORMAL );
    break;
  case 26:
    dmd.drawString( 23,1, "26", 2, GRAPHICS_NORMAL );
    break;
  case 27:
    dmd.drawString( 23,1, "27", 2, GRAPHICS_NORMAL );
    break;
  case 28:
    dmd.drawString( 23,1, "28", 2, GRAPHICS_NORMAL );
    break;
  case 29:
    dmd.drawString( 23,1, "29", 2, GRAPHICS_NORMAL );
    break;
  case 30:
    dmd.drawString( 23,1, "30", 2, GRAPHICS_NORMAL );
    break;
  case 31:
    dmd.drawString( 23,1, "31", 2, GRAPHICS_NORMAL );
    break;
  case 32:
    dmd.drawString( 23,1, "32", 2, GRAPHICS_NORMAL );
    break;
  case 33:
    dmd.drawString( 23,1, "33", 2, GRAPHICS_NORMAL );
    break;
  case 34:
    dmd.drawString( 23,1, "34", 2, GRAPHICS_NORMAL );
    break;
  case 35:
    dmd.drawString( 23,1, "35", 2, GRAPHICS_NORMAL );
    break;
  case 36:
    dmd.drawString( 23,1, "36", 2, GRAPHICS_NORMAL );
    break;
  case 37:
    dmd.drawString( 23,1, "37", 2, GRAPHICS_NORMAL );
    break;
  case 38:
    dmd.drawString( 23,1, "38", 2, GRAPHICS_NORMAL );
    break;
  case 39:
    dmd.drawString( 23,1, "39", 2, GRAPHICS_NORMAL );
    break;
  case 40:
    dmd.drawString( 23,1, "40", 2, GRAPHICS_NORMAL );
    break;
  case 41:
    dmd.drawString( 23,1, "41", 2, GRAPHICS_NORMAL );
    break;
  case 42:
    dmd.drawString( 23,1, "42", 2, GRAPHICS_NORMAL );
    break;
  case 43:
    dmd.drawString( 23,1, "43", 2, GRAPHICS_NORMAL );
    break;
  case 44:
    dmd.drawString( 23,1, "44", 2, GRAPHICS_NORMAL );
    break;
  case 45:
    dmd.drawString( 23,1, "45", 2, GRAPHICS_NORMAL );
    break;
  case 46:
    dmd.drawString( 23,1, "46", 2, GRAPHICS_NORMAL );
    break;
  case 47:
    dmd.drawString( 23,1, "47", 2, GRAPHICS_NORMAL );
    break;
  case 48:
    dmd.drawString( 23,1, "48", 2, GRAPHICS_NORMAL );
    break;
  case 49:
    dmd.drawString( 23,1, "49", 2, GRAPHICS_NORMAL );
    break;
  case 50:
    dmd.drawString( 23,1, "50", 2, GRAPHICS_NORMAL );
    break;
  case 51:
    dmd.drawString( 23,1, "51", 2, GRAPHICS_NORMAL );
    break;
  case 52:
    dmd.drawString( 23,1, "52", 2, GRAPHICS_NORMAL );
    break;
  case 53:
    dmd.drawString( 23,1, "53", 2, GRAPHICS_NORMAL );
    break;
  case 54:
    dmd.drawString( 23,1, "54", 2, GRAPHICS_NORMAL );
    break;
  case 55:
    dmd.drawString( 23,1, "55", 2, GRAPHICS_NORMAL );
    break;
  case 56:
    dmd.drawString( 23,1, "56", 2, GRAPHICS_NORMAL );
    break;
  case 57:
    dmd.drawString( 23,1, "57", 2, GRAPHICS_NORMAL );
    break;
  case 58:
    dmd.drawString( 23,1, "58", 2, GRAPHICS_NORMAL );
    break;
  case 59:
    dmd.drawString( 23,1, "59", 2, GRAPHICS_NORMAL );
    break;
  }
}
void drawSec()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(second)
  {
  case 0:
    dmd.drawString( 45,1, "00", 2, GRAPHICS_NORMAL );
    break;
  case 1:
    dmd.drawString( 45,1, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 45,1, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 45,1, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 45,1, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 45,1, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 45,1, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 45,1, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 45,1, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 45,1, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 45,1, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 45,1, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 45,1, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 45,1, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 45,1, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 45,1, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 45,1, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 45,1, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 45,1, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 45,1, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 45,1, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 45,1, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 45,1, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 45,1, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 45,1, "24", 2, GRAPHICS_NORMAL );
    break;
  case 25:
    dmd.drawString( 4,1, "25", 2, GRAPHICS_NORMAL );
    break;
  case 26:
    dmd.drawString( 45,1, "26", 2, GRAPHICS_NORMAL );
    break;
  case 27:
    dmd.drawString( 45,1, "27", 2, GRAPHICS_NORMAL );
    break;
  case 28:
    dmd.drawString( 45,1, "28", 2, GRAPHICS_NORMAL );
    break;
  case 29:
    dmd.drawString( 45,1, "29", 2, GRAPHICS_NORMAL );
    break;
  case 30:
    dmd.drawString( 45,1, "30", 2, GRAPHICS_NORMAL );
    break;
  case 31:
    dmd.drawString( 45,1, "31", 2, GRAPHICS_NORMAL );
    break;
  case 32:
    dmd.drawString( 45,1, "32", 2, GRAPHICS_NORMAL );
    break;
  case 33:
    dmd.drawString( 45,1, "33", 2, GRAPHICS_NORMAL );
    break;
  case 34:
    dmd.drawString( 45,1, "34", 2, GRAPHICS_NORMAL );
    break;
  case 35:
    dmd.drawString( 45,1, "35", 2, GRAPHICS_NORMAL );
    break;
  case 36:
    dmd.drawString( 45,1, "36", 2, GRAPHICS_NORMAL );
    break;
  case 37:
    dmd.drawString( 45,1, "37", 2, GRAPHICS_NORMAL );
    break;
  case 38:
    dmd.drawString( 45,1, "38", 2, GRAPHICS_NORMAL );
    break;
  case 39:
    dmd.drawString( 45,1, "39", 2, GRAPHICS_NORMAL );
    break;
  case 40:
    dmd.drawString( 45,1, "40", 2, GRAPHICS_NORMAL );
    break;
  case 41:
    dmd.drawString( 45,1, "41", 2, GRAPHICS_NORMAL );
    break;
  case 42:
    dmd.drawString( 45,1, "42", 2, GRAPHICS_NORMAL );
    break;
  case 43:
    dmd.drawString( 45,1, "43", 2, GRAPHICS_NORMAL );
    break;
  case 44:
    dmd.drawString( 45,1, "44", 2, GRAPHICS_NORMAL );
    break;
  case 45:
    dmd.drawString( 45,1, "45", 2, GRAPHICS_NORMAL );
    break;
  case 46:
    dmd.drawString( 45,1, "46", 2, GRAPHICS_NORMAL );
    break;
  case 47:
    dmd.drawString( 45,1, "47", 2, GRAPHICS_NORMAL );
    break;
  case 48:
    dmd.drawString( 45,1, "48", 2, GRAPHICS_NORMAL );
    break;
  case 49:
    dmd.drawString( 45,1, "49", 2, GRAPHICS_NORMAL );
    break;
  case 50:
    dmd.drawString( 45,1, "50", 2, GRAPHICS_NORMAL );
    break;
  case 51:
    dmd.drawString( 45,1, "51", 2, GRAPHICS_NORMAL );
    break;
  case 52:
    dmd.drawString( 45,1, "52", 2, GRAPHICS_NORMAL );
    break;
  case 53:
    dmd.drawString( 45,1, "53", 2, GRAPHICS_NORMAL );
    break;
  case 54:
    dmd.drawString( 45,1, "54", 2, GRAPHICS_NORMAL );
    break;
  case 55:
    dmd.drawString( 45,1, "55", 2, GRAPHICS_NORMAL );
    break;
  case 56:
    dmd.drawString( 45,1, "56", 2, GRAPHICS_NORMAL );
    break;
  case 57:
    dmd.drawString( 45,1, "57", 2, GRAPHICS_NORMAL );
    break;
  case 58:
    dmd.drawString( 45,1, "58", 2, GRAPHICS_NORMAL );
    break;
  case 59:
    dmd.drawString( 45,1, "59", 2, GRAPHICS_NORMAL );
    break;
  } 
}



void drawcircle(){
  dmd.drawCircle(15,11,4,GRAPHICS_NORMAL);
  }
  void drawbox(){
  dmd.drawBox(0,0,63,15,GRAPHICS_NORMAL);
  }
  
void drawCol1()
{
  dmd.drawString( 19,1, ":", 1, GRAPHICS_NORMAL );
  }

void showTemp(){
  //dmd.writePixel(3,11,GRAPHICS_NORMAL,1);
  dmd.drawString( 1,1, "T", 1, GRAPHICS_NORMAL );
  dmd.drawString( 12,1, "=", 1, GRAPHICS_NORMAL );
}

void drawCol2(){
  dmd.drawString( 41,1, ":", 1, GRAPHICS_NORMAL );
}

void runClock()
{
  drawHour();
  drawCol1(); 
  drawMinute();
  drawCol2();
  drawSec();
  CheckAlarm();
 }
 void AlarmSequence() {
  //drawDay();  
  //drawDate();
  drawHour();
 drawCol1();
  drawMinute();
  digitalWrite(Alarm, HIGH);
  delay(1000);
 // dmd.clearScreen( true );
  digitalWrite(Alarm, LOW);
  delay(1000);
}

void CheckAlarm() {
  bluetooth();
  AlarmSet = digitalRead(AlarmSetPin);
  Serial.println(AlarmSet);
 // Snooze = digitalRead(SnoozePin);
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
if(dayOfWeek == 2 || dayOfWeek == 3 || dayOfWeek == 4 || dayOfWeek == 5 ||dayOfWeek == 6) { //This is days ALLOWED to ring alarm
    if(hour == Hr && minute== Min && second <=20) { //Set Time Here (In 24 hour Time) 
      
      AlarmSequence();
      runClock();
        

    }
  /* Only Trigger Alarm If it is a day listed below. This can be changed by changing the numbers to any of the following:
   1 = Sunday
   2 = Monday
   3 = Tuesday
   4 = Wednesday
   5 = Thursday
   6 = Friday
   7 = Saturday
*/ 

  /*
  if(dayOfWeek == 2 || dayOfWeek == 3 || dayOfWeek == 4 || dayOfWeek == 5 ||dayOfWeek == 6) { //This is days ALLOWED to ring alarm
    if(hour == 18 && minute == 05 ) { //Set Time Here (In 24 hour Time) 
      if(AlarmSet == LOW) { //If alarm pin is on
        Snooze = digitalRead(SnoozePin);
        while(Snooze == HIGH)
        {
          Snooze = digitalRead(SnoozePin);
          AlarmSequence();
        }
      }
    }
  }
  */
    }
}
void loop()
{
unsigned long currentTime = millis(); 
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
   
  //SET_bluetooth();
  show_time();
  
  if(currentTime-previousTime>=eventInterval){
    bluetooth();
    for(i=0;i<10;i++){
         runClock();
        delay(1000);
        dmd.clearScreen( true );
    }
  get_temp();
  delay(5000);
  dmd.clearScreen( true );
  drawMonth();
  drawDate();
  drawYear();
  delay(5000);
  dmd.clearScreen( true );
  previousTime = currentTime;
  
  }
  delay(100);
  CheckAlarm() ; 
  Serial.print("Hour");
  Serial.println(hour);
  Serial.print("setHour");
  Serial.println(Hr);
}


  
  
 
