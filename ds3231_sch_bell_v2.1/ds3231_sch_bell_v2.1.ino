#include<EEPROM.h>
#include <DS3231.h> //Library for Real Time Clock.
#include <Wire.h>

int i,Hr =0,Min=0,Sec,num,Interval,val,x,buzzer = 10,k = 0;
int Relay = 8; //Relay declaration pin
DS3231  rtc(SDA, SCL); //RTC declaration
Time t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(buzzer,OUTPUT);
  //Serial.print("school bell");
  delay(100);
  rtc.begin();
  /*rtc.setTime(14 , 14 , 15); // Set the time 
  rtc.setDate(12, 11, 2022); // Set the date
  rtc.setDOW(SATURDAY); // Set the Day Of Week (No ring on Saturday and Sunday)
  */
}

void loop() {
  // put your main code here, to run repeatedly:
   //Serial.println(rtc.getTimeStr());
if(Serial.available()>0){
    val = Serial.parseInt();
  if (val == 1){
    Serial.println(rtc.getDOWStr());
    delay(1000); 
    Serial.print("Time:  ");
    Serial.println(rtc.getTimeStr());
    delay(1000); 
    }    
 else if (val == 2){
  Hr++;
  Serial.print(Hr);
  Serial.print(":");
  Serial.println(Min);
  if(Hr>=23){
    Hr = 0;
    Serial.print(Hr);
    Serial.print(":");
    Serial.println(Min);
    }      
 }
  else if (val == 3){
     Min++;
  Serial.print(Hr);
  Serial.print(":");
  Serial.println(Min);
   if(Min>=59){
    Min = 0;
    Serial.print(Hr);
    Serial.print(":");
    Serial.println(Min);
   }      
 }
else if (val == 4){
  Serial.print("Time:  ");
  Serial.println(rtc.getTimeStr()); // get time from rtc
  Serial.print("settime"); 
  Serial.print(Hr);
  Serial.print(":");
  Serial.println(Min);
  delay(1000); 
 }
 else if (val == 5){
   Serial.println("Enter period interval");
   delay(1000);
   Interval = Interval+5;
   Serial.print("you set period for ");
   Serial.print(Interval);
   Serial.println(" Minutes ");
   delay(100);
  }
 }

delay(100);   
val = 5;
   t = rtc.getTime(); //get time from rtc 
if(val==5 && rtc.getDOWStr()=="Monday"){
      if((t.hour) == Hr && (t.min)== Min &&(t.sec>=30)){     
         digitalWrite(buzzer,HIGH);         //period 1
      }
      else if((t.hour) == Hr && (t.min)==(Min+Interval)&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //period 2
      }
      else if((t.hour) == Hr && (t.min)==(Min+(2*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //period 3
      }
      else if((t.hour) == Hr && (t.min)==( Min+(3*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //short break
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(4*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //period 4
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(5*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //period 5
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(6*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //long break
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(7*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);   // period 6
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(8*Interval))&&(t.sec>=30)){
         digitalWrite(buzzer,HIGH);    //period7
      } 
      else{
        digitalWrite(buzzer ,LOW);
        delay(100);
  }
  }

if(val==5 && rtc.getDOWStr()=="Tuesday"){
    if((t.hour) == Hr && (t.min)== Min){     
         digitalWrite(buzzer,HIGH);         //period 1
      }
      else if((t.hour) == Hr && (t.min)==(Min+Interval)){
         digitalWrite(buzzer,HIGH);    //period 2
      }
      else if((t.hour) == Hr && (t.min)==(Min+(2*Interval))){
         digitalWrite(buzzer,HIGH);    //period 3
      }
      else if((t.hour) == Hr && (t.min)==( Min+(3*Interval))){
         digitalWrite(buzzer,HIGH);    //short break
      }
       else if((t.hour) == Hr && (t.min)==( Min+(4*Interval))){
         digitalWrite(buzzer,HIGH);    //period 4
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(5*Interval))){
         digitalWrite(buzzer,HIGH);    //period 5
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(6*Interval))){
         digitalWrite(buzzer,HIGH);    //long break
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(7*Interval))){
         digitalWrite(buzzer,HIGH);   // period 6
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(8*Interval))){
         digitalWrite(buzzer,HIGH);    //period7
      } 
      else{
        digitalWrite(buzzer ,LOW);
        delay(100);
  }
}
if(val==5 && rtc.getDOWStr()=="Wednessday"){
    if((t.hour) == Hr && (t.min)== Min){     
         digitalWrite(buzzer,HIGH);         //period 1
      }
      else if((t.hour) == Hr && (t.min)==(Min+Interval)){
         digitalWrite(buzzer,HIGH);    //period 2
      }
      else if((t.hour) == Hr && (t.min)==(Min+(2*Interval))){
         digitalWrite(buzzer,HIGH);    //period 3
      }
      else if((t.hour) == Hr && (t.min)==( Min+(3*Interval))){
         digitalWrite(buzzer,HIGH);    //short break
      }
       else if((t.hour) == Hr && (t.min)==( Min+(4*Interval))){
         digitalWrite(buzzer,HIGH);    //period 4
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(5*Interval))){
         digitalWrite(buzzer,HIGH);    //period 5
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(6*Interval))){
         digitalWrite(buzzer,HIGH);    //long break
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(7*Interval))){
         digitalWrite(buzzer,HIGH);   // period 6
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(8*Interval))){
         digitalWrite(buzzer,HIGH);    //period7
      } 
      else{
        digitalWrite(buzzer ,LOW);
        delay(100);
  }
}
if(val==5 && rtc.getDOWStr()=="Thursday"){
    if((t.hour) == Hr && (t.min)== Min){     
         digitalWrite(buzzer,HIGH);         //period 1
      }
      else if((t.hour) == Hr && (t.min)==(Min+Interval)){
         digitalWrite(buzzer,HIGH);    //period 2
      }
      else if((t.hour) == Hr && (t.min)==(Min+(2*Interval))){
         digitalWrite(buzzer,HIGH);    //period 3
      }
      else if((t.hour) == Hr && (t.min)==( Min+(3*Interval))){
         digitalWrite(buzzer,HIGH);    //short break
      }
       else if((t.hour) == Hr && (t.min)==( Min+(4*Interval))){
         digitalWrite(buzzer,HIGH);    //period 4
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(5*Interval))){
         digitalWrite(buzzer,HIGH);    //period 5
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(6*Interval))){
         digitalWrite(buzzer,HIGH);    //long break
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(7*Interval))){
         digitalWrite(buzzer,HIGH);   // period 6
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(8*Interval))){
         digitalWrite(buzzer,HIGH);    //period7
      } 
      else{
        digitalWrite(buzzer ,LOW);
        delay(100);
  }
}
if(val==5 && rtc.getDOWStr()=="Friday"){
    if((t.hour) == Hr && (t.min)== Min){     
         digitalWrite(buzzer,HIGH);         //period 1
      }
      else if((t.hour) == Hr && (t.min)==(Min+Interval)){
         digitalWrite(buzzer,HIGH);    //period 2
      }
      else if((t.hour) == Hr && (t.min)==(Min+(2*Interval))){
         digitalWrite(buzzer,HIGH);    //period 3
      }
      else if((t.hour) == Hr && (t.min)==( Min+(3*Interval))){
         digitalWrite(buzzer,HIGH);    //short break
      }
       else if((t.hour) == Hr && (t.min)==( Min+(4*Interval))){
         digitalWrite(buzzer,HIGH);    //period 4
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(5*Interval))){
         digitalWrite(buzzer,HIGH);    //period 5
      } 
       else if((t.hour) == Hr && (t.min)==( Min+(6*Interval))){
         digitalWrite(buzzer,HIGH);    //long break
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(7*Interval))){
         digitalWrite(buzzer,HIGH);   // period 6
      } 
      else if((t.hour) == Hr && (t.min)==( Min+(8*Interval))){
         digitalWrite(buzzer,HIGH);    //period7
      } 
      else{
        digitalWrite(buzzer ,LOW);
        delay(100);
  }
}

if(val==5 && rtc.getDOWStr()=="Saturday"){
    Serial.print("WEEKEND");
  }
  if(val==5 && rtc.getDOWStr()=="Sunday"){
    Serial.print("WEEKEND");    
  }
}
