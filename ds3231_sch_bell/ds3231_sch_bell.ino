#include <DS3231.h> //Library for Real Time Clock.
#include <Wire.h>

#define Hr  10
#define Min  4
char val;
bool Stop;
int x = 0;
int buzzer = 10;

int Relay = 8; //Relay declaration pin
DS3231  rtc(SDA, SCL); //RTC declaration
Time t;
int k = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 pinMode(buzzer,OUTPUT);
  rtc.begin();
  pinMode(Relay, OUTPUT);

  /*rtc.setTime(12 , 48 , 50); // Set the time 
  rtc.setDate(11, 11, 2022); // Set the date
  rtc.setDOW(FRIDAY); // Set the Day Of Week (No ring on Saturday and Sunday)
  digitalWrite(Relay, HIGH);
  */
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print(" ");
  Serial.println(rtc.getDOWStr());
  delay(1000); 
  Serial.print("Time:  ");
  Serial.println(rtc.getTimeStr());
  delay(1000);
  Serial.print("Date: ");
  Serial.println(rtc.getDateStr());
  delay(1000);

  t = rtc.getTime();
  if(rtc.getDOWStr()=="Monday"){
      if((t.hour) == Hr && (t.min)== Min){
         digitalWrite(buzzer,HIGH);  
      }
      else if((t.hour) == Hr && (t.min)==( Min+2)){
         digitalWrite(buzzer,HIGH);    
      }
      else if((t.hour) == Hr && (t.min)==( Min+4)){
         digitalWrite(buzzer,HIGH);    
      }
      else if((t.hour) == Hr && (t.min)==( Min+6)){
         digitalWrite(buzzer,HIGH);    
      } 
      else{
        digitalWrite(buzzer ,LOW);
        delay(100);
  }
  
}


}
