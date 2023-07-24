#include<EEPROM.h>
#include <DS3231.h> //Library for Real Time Clock.
#include <Wire.h>

int i,Hr =0,Min=0,Sec,num,Interval,val,x,buzzer = 10,
k = 0,b1=2,b2=4,b3=7,b4=12,b_new1,b_new2,b_new3,
b_new4;
int Relay = 8; //Relay declaration pin
DS3231  rtc(SDA, SCL); //RTC declaration
Time t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(buzzer,OUTPUT);
  pinMode(b1,INPUT);
  pinMode(b2,INPUT);
  pinMode(b3,INPUT);
  pinMode(b4,INPUT);
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
b_new1 = digitalRead(b1);
b_new2 = digitalRead(b2);
b_new3 = digitalRead(b3);
b_new4 = digitalRead(b4);
if (b_new1==LOW){
    Serial.println(rtc.getDOWStr());
    delay(1000); 
    Serial.print("Time:  ");
    Serial.println(rtc.getTimeStr());
    delay(1000); 
  } 
else if(b_new2==LOW){
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
else if(b_new3==LOW){
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
  else if(b_new4==LOW){
    Serial.print("Time:  ");
  Serial.println(rtc.getTimeStr()); // get time from rtc
  Serial.print("settime"); 
  Serial.print(Hr);
  Serial.print(":");
  Serial.println(Min);
  delay(1000); 
  
   Serial.println("Enter period interval");
   delay(1000);
   Interval = Interval+5;
   Serial.print("you set period for ");
   Serial.print(Interval);
   Serial.println(" Minutes ");
   delay(100);
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
if(val==5 && rtc.getDOWStr()=="Wednessday"){
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
if(val==5 && rtc.getDOWStr()=="Thursday"){
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
if(val==5 && rtc.getDOWStr()=="Friday"){
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

if(val==5 && rtc.getDOWStr()=="Saturday"){
    Serial.print("WEEKEND");
  }
  if(val==5 && rtc.getDOWStr()=="Sunday"){
    Serial.print("WEEKEND");    
  }
}
