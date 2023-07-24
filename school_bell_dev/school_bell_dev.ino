#include <DS3231.h>
#include <Wire.h>
#define Hour 7    //define hour 
#define Minute 0 //define minute
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);   //set up RTC module

int alarm= 7;          //this is connected to the ISD module
bool Stop;            //Bool to stop screaming
Time  t;               //from the Library

void setup()
{

  
  Serial.begin(115200);   //just for debug. not important
  pinMode(alarm,OUTPUT);  //define inputs and outputs
  
  rtc.begin();            //from library
  Stop = false;           //dont scream yet

  Serial.print("welcome");
}

void loop()
{

  
  t = rtc.getTime();     //get the time from RTC module
  if(rtc.getDOWStr()!= "SATURDAY")//unless its the weekend!
  {
    if((t.hour)== Hour && (t.min) == Minute )   //when Time as come...
    {
      while(Stop==false)     //as long as the Bool is false (starting poin is false)
      {
         
        if((t.hour)== Hour && (t.min) == (Minute+40))//if it lastad for more than 20 minutes 
          Stop=true;                              //Stop screaming at me!!
       
      delay(80000);            
      Stop=false;                         //get ready to start again tomorrow!
    }
  
  
  
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  // Send time
  Serial.println(rtc.getTimeStr());

  
  // Wait one second before repeating :)
  delay (1000);
  //   SET this to change time!  do this if u didn't set the RTC time
  //if(digitalRead(Butt))
  //{
     rtc.setDOW(MONDAY);     // Set Day-of-Week to MONDAY
  rtc.setTime(19,35, 0);     // Set the time to 19:35:00 (24hr format)
  rtc.setDate(8, 4, 2019);   // Set the date to april 8, 2019
  //}
  
    }
  }
}
