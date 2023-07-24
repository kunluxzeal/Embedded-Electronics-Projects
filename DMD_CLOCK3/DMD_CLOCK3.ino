
#include <SPI.h>
#include  <DMD.h>
#include <TimerOne.h>
#include<Arial_black_16.h>  
#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68
#include "RTClib.h"
int AlarmSetPin = 2;
int SnoozePin = 3;
int Alarm = 4;
int AlarmSet = 0;
int Snooze = 0;
int a,b;

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
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
//Fire up the DMD library as dmd
DMD dmd(1,1);
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}






void setup()
{
  Wire.begin(); 
   Serial.begin(9600);
if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
     //rtc.adjust(DateTime(2022, 9, 11, 2, 51, 0));
  }
}
  Timer1.initialize( 1000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()  //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
  dmd.selectFont(Arial_Black_16);
  
    
    //pinMode(AlarmSetPin, INPUT_PULLUP);
  pinMode(SnoozePin, INPUT_PULLUP);
  pinMode(Alarm, OUTPUT);
  digitalWrite(Alarm, LOW);
}


void drawDay()
// shows day of week
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(dayOfWeek)
  {
  case 1:
    dmd.drawString( 1,0, "Sun,", 3, GRAPHICS_NORMAL );    
    break;  case 2:
    dmd.drawString( 1,0, "Mon,", 3, GRAPHICS_NORMAL );        
    break;  case 3:
    dmd.drawString( 1,0, "Tue,", 3, GRAPHICS_NORMAL );        
    break;  case 4:
    dmd.drawString( 1,0, "Wed,", 3, GRAPHICS_NORMAL );        
    break;  case 5:
    dmd.drawString( 1,0, "Thu,", 3, GRAPHICS_NORMAL );        
    break;  case 6:
    dmd.drawString( 1,0, "Fri,", 3, GRAPHICS_NORMAL );        
    break;  case 7:
    dmd.drawString( 1,0, "Sat,", 3, GRAPHICS_NORMAL );        
    break;
  }
}void drawDate()
// shows numerical date
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(dayOfMonth)
  {
  case 1:
    dmd.drawString( 20,0, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 20,0, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 20,0, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 20,0, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 20,0, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 20,0, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 20,0, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 20,0, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 20,0, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 20,0, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 20,0, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 20,0, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 20,0, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 20,0, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 20,0, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 20,0, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 20,0, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 20,0, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 20,0, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 20,0, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 20,0, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 20,0, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 20,0, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 20,0, "24", 2, GRAPHICS_NORMAL );
    break;
  case 25:
    dmd.drawString( 20,0, "25", 2, GRAPHICS_NORMAL );
    break;
  case 26:
    dmd.drawString( 20,0, "26", 2, GRAPHICS_NORMAL );
    break;
  case 27:
    dmd.drawString( 20,0, "27", 2, GRAPHICS_NORMAL );
    break;
  case 28:
    dmd.drawString( 20,0, "28", 2, GRAPHICS_NORMAL );
    break;
  case 29:
    dmd.drawString( 20,0, "29", 2, GRAPHICS_NORMAL );
    break;
  case 30:
    dmd.drawString( 20,0, "30", 2, GRAPHICS_NORMAL );
    break;
  case 31:
    dmd.drawString( 20,0, "31", 2, GRAPHICS_NORMAL );
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
  case 1:
    dmd.drawString( 2,0, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 2,0, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 2,0, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 2,0, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 2,0, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 2,0, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 2,0, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 2,0, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 2,0, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 2,0, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 2,0, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 2,0, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 2,0, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 2,0, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 2,0, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 2,0, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 2,0, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 2,0, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 2,0, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 2,0, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 2,0, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 2,0, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 2,0, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 2,0, "24", 2, GRAPHICS_NORMAL );
    break;
  }
}void drawMinute()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(minute)
  {
  case 0:
    dmd.drawString( 12,0, "00", 2, GRAPHICS_NORMAL );
    break;
  case 1:
    dmd.drawString( 12,0, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 12,0, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 12,0, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 12,0, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 12,0, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 12,0, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 12,0, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 12,0, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 12,0, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 12,0, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 12,0, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 12,0, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 12,0, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 12,0, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 12,0, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 12,0, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 12,0, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 12,0, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 12,0, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 12,0, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 12,0, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 12,0, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 12,0, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 12,0, "24", 2, GRAPHICS_NORMAL );
    break;
  case 25:
    dmd.drawString( 12,0, "25", 2, GRAPHICS_NORMAL );
    break;
  case 26:
    dmd.drawString( 12,0, "26", 2, GRAPHICS_NORMAL );
    break;
  case 27:
    dmd.drawString( 12,0, "27", 2, GRAPHICS_NORMAL );
    break;
  case 28:
    dmd.drawString( 12,0, "28", 2, GRAPHICS_NORMAL );
    break;
  case 29:
    dmd.drawString( 12,0, "29", 2, GRAPHICS_NORMAL );
    break;
  case 30:
    dmd.drawString( 12,0, "30", 2, GRAPHICS_NORMAL );
    break;
  case 31:
    dmd.drawString( 12,0, "31", 2, GRAPHICS_NORMAL );
    break;
  case 32:
    dmd.drawString( 12,0, "32", 2, GRAPHICS_NORMAL );
    break;
  case 33:
    dmd.drawString( 12,0, "33", 2, GRAPHICS_NORMAL );
    break;
  case 34:
    dmd.drawString( 12,0, "34", 2, GRAPHICS_NORMAL );
    break;
  case 35:
    dmd.drawString( 12,0, "35", 2, GRAPHICS_NORMAL );
    break;
  case 36:
    dmd.drawString( 12,0, "36", 2, GRAPHICS_NORMAL );
    break;
  case 37:
    dmd.drawString( 12,0, "37", 2, GRAPHICS_NORMAL );
    break;
  case 38:
    dmd.drawString( 12,0, "38", 2, GRAPHICS_NORMAL );
    break;
  case 39:
    dmd.drawString( 12,0, "39", 2, GRAPHICS_NORMAL );
    break;
  case 40:
    dmd.drawString( 12,0, "40", 2, GRAPHICS_NORMAL );
    break;
  case 41:
    dmd.drawString( 12,0, "41", 2, GRAPHICS_NORMAL );
    break;
  case 42:
    dmd.drawString( 12,0, "42", 2, GRAPHICS_NORMAL );
    break;
  case 43:
    dmd.drawString( 12,0, "43", 2, GRAPHICS_NORMAL );
    break;
  case 44:
    dmd.drawString( 12,0, "44", 2, GRAPHICS_NORMAL );
    break;
  case 45:
    dmd.drawString( 12,0, "45", 2, GRAPHICS_NORMAL );
    break;
  case 46:
    dmd.drawString( 12,0, "46", 2, GRAPHICS_NORMAL );
    break;
  case 47:
    dmd.drawString( 12,0, "47", 2, GRAPHICS_NORMAL );
    break;
  case 48:
    dmd.drawString( 12,0, "48", 2, GRAPHICS_NORMAL );
    break;
  case 49:
    dmd.drawString( 12,0, "49", 2, GRAPHICS_NORMAL );
    break;
  case 50:
    dmd.drawString( 12,0, "50", 2, GRAPHICS_NORMAL );
    break;
  case 51:
    dmd.drawString( 12,0, "51", 2, GRAPHICS_NORMAL );
    break;
  case 52:
    dmd.drawString( 12,0, "52", 2, GRAPHICS_NORMAL );
    break;
  case 53:
    dmd.drawString( 12,0, "53", 2, GRAPHICS_NORMAL );
    break;
  case 54:
    dmd.drawString( 12,0, "54", 2, GRAPHICS_NORMAL );
    break;
  case 55:
    dmd.drawString( 12,0, "55", 2, GRAPHICS_NORMAL );
    break;
  case 56:
    dmd.drawString( 12,0, "56", 2, GRAPHICS_NORMAL );
    break;
  case 57:
    dmd.drawString( 12,0, "57", 2, GRAPHICS_NORMAL );
    break;
  case 58:
    dmd.drawString( 12,0, "58", 2, GRAPHICS_NORMAL );
    break;
  case 59:
    dmd.drawString( 12,0, "59", 2, GRAPHICS_NORMAL );
    break;
  }
}void drawSec()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(second)
  {
  case 0:
    dmd.drawString( 24,0, "00", 2, GRAPHICS_NORMAL );
    break;
  case 1:
    dmd.drawString( 24,0, "01", 2, GRAPHICS_NORMAL );
    break;
  case 2:
    dmd.drawString( 24,0, "02", 2, GRAPHICS_NORMAL );
    break;
  case 3:
    dmd.drawString( 24,0, "03", 2, GRAPHICS_NORMAL );
    break;
  case 4:
    dmd.drawString( 24,0, "04", 2, GRAPHICS_NORMAL );
    break;
  case 5:
    dmd.drawString( 24,0, "05", 2, GRAPHICS_NORMAL );
    break;
  case 6:
    dmd.drawString( 24,0, "06", 2, GRAPHICS_NORMAL );
    break;
  case 7:
    dmd.drawString( 24,0, "07", 2, GRAPHICS_NORMAL );
    break;
  case 8:
    dmd.drawString( 24,0, "08", 2, GRAPHICS_NORMAL );
    break;
  case 9:
    dmd.drawString( 24,0, "09", 2, GRAPHICS_NORMAL );
    break;
  case 10:
    dmd.drawString( 24,0, "10", 2, GRAPHICS_NORMAL );
    break;
  case 11:
    dmd.drawString( 24,0, "11", 2, GRAPHICS_NORMAL );
    break;
  case 12:
    dmd.drawString( 24,0, "12", 2, GRAPHICS_NORMAL );
    break;
  case 13:
    dmd.drawString( 24,0, "13", 2, GRAPHICS_NORMAL );
    break;
  case 14:
    dmd.drawString( 24,0, "14", 2, GRAPHICS_NORMAL );
    break;
  case 15:
    dmd.drawString( 24,0, "15", 2, GRAPHICS_NORMAL );
    break;
  case 16:
    dmd.drawString( 24,0, "16", 2, GRAPHICS_NORMAL );
    break;
  case 17:
    dmd.drawString( 24,0, "17", 2, GRAPHICS_NORMAL );
    break;
  case 18:
    dmd.drawString( 24,0, "18", 2, GRAPHICS_NORMAL );
    break;
  case 19:
    dmd.drawString( 24,0, "19", 2, GRAPHICS_NORMAL );
    break;
  case 20:
    dmd.drawString( 24,0, "20", 2, GRAPHICS_NORMAL );
    break;
  case 21:
    dmd.drawString( 24,0, "21", 2, GRAPHICS_NORMAL );
    break;
  case 22:
    dmd.drawString( 24,0, "22", 2, GRAPHICS_NORMAL );
    break;
  case 23:
    dmd.drawString( 24,0, "23", 2, GRAPHICS_NORMAL );
    break;
  case 24:
    dmd.drawString( 24,0, "24", 2, GRAPHICS_NORMAL );
    break;
  case 25:
    dmd.drawString( 24,0, "25", 2, GRAPHICS_NORMAL );
    break;
  case 26:
    dmd.drawString( 24,0, "26", 2, GRAPHICS_NORMAL );
    break;
  case 27:
    dmd.drawString( 24,0, "27", 2, GRAPHICS_NORMAL );
    break;
  case 28:
    dmd.drawString( 24,0, "28", 2, GRAPHICS_NORMAL );
    break;
  case 29:
    dmd.drawString( 24,0, "29", 2, GRAPHICS_NORMAL );
    break;
  case 30:
    dmd.drawString( 24,0, "30", 2, GRAPHICS_NORMAL );
    break;
  case 31:
    dmd.drawString( 24,0, "31", 2, GRAPHICS_NORMAL );
    break;
  case 32:
    dmd.drawString( 24,0, "32", 2, GRAPHICS_NORMAL );
    break;
  case 33:
    dmd.drawString( 24,0, "33", 2, GRAPHICS_NORMAL );
    break;
  case 34:
    dmd.drawString( 24,0, "34", 2, GRAPHICS_NORMAL );
    break;
  case 35:
    dmd.drawString( 24,0, "35", 2, GRAPHICS_NORMAL );
    break;
  case 36:
    dmd.drawString( 24,0, "36", 2, GRAPHICS_NORMAL );
    break;
  case 37:
    dmd.drawString( 24,0, "37", 2, GRAPHICS_NORMAL );
    break;
  case 38:
    dmd.drawString( 24,0, "38", 2, GRAPHICS_NORMAL );
    break;
  case 39:
    dmd.drawString( 24,0, "39", 2, GRAPHICS_NORMAL );
    break;
  case 40:
    dmd.drawString( 24,0, "40", 2, GRAPHICS_NORMAL );
    break;
  case 41:
    dmd.drawString( 24,0, "41", 2, GRAPHICS_NORMAL );
    break;
  case 42:
    dmd.drawString( 24,0, "42", 2, GRAPHICS_NORMAL );
    break;
  case 43:
    dmd.drawString( 24,0, "43", 2, GRAPHICS_NORMAL );
    break;
  case 44:
    dmd.drawString( 24,0, "44", 2, GRAPHICS_NORMAL );
    break;
  case 45:
    dmd.drawString( 24,0, "45", 2, GRAPHICS_NORMAL );
    break;
  case 46:
    dmd.drawString( 24,0, "46", 2, GRAPHICS_NORMAL );
    break;
  case 47:
    dmd.drawString( 24,0, "47", 2, GRAPHICS_NORMAL );
    break;
  case 48:
    dmd.drawString( 24,0, "48", 2, GRAPHICS_NORMAL );
    break;
  case 49:
    dmd.drawString( 24,0, "49", 2, GRAPHICS_NORMAL );
    break;
  case 50:
    dmd.drawString( 24,0, "50", 2, GRAPHICS_NORMAL );
    break;
  case 51:
    dmd.drawString( 24,0, "51", 2, GRAPHICS_NORMAL );
    break;
  case 52:
    dmd.drawString( 24,0, "52", 2, GRAPHICS_NORMAL );
    break;
  case 53:
    dmd.drawString( 24,0, "53", 2, GRAPHICS_NORMAL );
    break;
  case 54:
    dmd.drawString( 24,0, "54", 2, GRAPHICS_NORMAL );
    break;
  case 55:
    dmd.drawString( 24,0, "55", 2, GRAPHICS_NORMAL );
    break;
  case 56:
    dmd.drawString( 24,0, "56", 2, GRAPHICS_NORMAL );
    break;
  case 57:
    dmd.drawString( 24,0, "57", 2, GRAPHICS_NORMAL );
    break;
  case 58:
    dmd.drawString( 24,0, "58", 2, GRAPHICS_NORMAL );
    break;
  case 59:
    dmd.drawString( 24,0, "59", 2, GRAPHICS_NORMAL );
    break;
  }
}
void drawCol1()
{
  dmd.drawString( 8,0, ":", 1, GRAPHICS_NORMAL );
}
void drawCol2()
{
  dmd.drawString( 20,0, ":", 1, GRAPHICS_NORMAL );
}
void runClock()
{
 // drawDay();  
 // drawDate();
  drawHour();
  drawCol1();
  drawMinute();
  //drawCol2();
  //drawSec();
 // CheckAlarm();}/*void AlarmSequence() {
  //drawDay();  
  //drawDate();
  //drawHour();
 // drawCol();
  //drawMinute();
  digitalWrite(Alarm, HIGH);
  delay(1000);
  dmd.clearScreen( true );
  digitalWrite(Alarm, LOW);
  delay(1000);
}

/*void CheckAlarm() {
  AlarmSet = digitalRead(AlarmSetPin);
  Snooze = digitalRead(SnoozePin);
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);*/
  /* Only Trigger Alarm If it is a day listed below. This can be changed by changing the numbers to any of the following:
   1 = Sunday
   2 = Monday
   3 = Tuesday
   4 = Wednesday
   5 = Thursday
   6 = Friday
   7 = Saturday
   */
/*  if(dayOfWeek == 2 || dayOfWeek == 3 || dayOfWeek == 4 || dayOfWeek == 5 ||dayOfWeek == 6) { //This is days ALLOWED to ring alarm
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
}
*/
void loop()
{
  runClock();  
}
