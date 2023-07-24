//Automatic School Bell using Arduino Uno, RTC, and 16x2 LCD 
//Download Libraries
// https://www.electroniclinic.com/arduino-libraries-download-and-projects-they-are-used-in-project-codes/
/*
 * Connections of the DS3231 with Arduino Uno
 * connect SDA of the RTC with A4
 * connect SCL of the RTC with A5
 */

#include <LiquidCrystal.h>
#include "RTClib.h"
#include <Wire.h>
#include<EEPROM.h>

RTC_DS1307 rtc; // you can also use RTC_DS1307
LiquidCrystal lcd(13, 12, 6, 5, 4, 3);// Pins used for RS,E,D4,D5,D6,D7 and the r/w pin is connected with the ground

char* mySubject[]={"MATHS", "ENG","BIO","PHY","CHEM","IT LAB","HIST","GEO"};
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

int subject[7];

int buzzer = 2; // The Buzzer is connected with the Arduino's digital pin 2. 
int push_button1 = A0;     
int push_button2 = A1;                                  
int push_button3 = A2;         
int push_button4 = A3;                     

int push_button1_state = 0;
int push_button2_state = 0;   
int push_button3_state = 0; 
int push_button4_state = 0; 

int newTime,newHour,prevTime=0,prevHour=0,a=0,hoursE=0,hoursH=0, countTime=60;
int i=0,j=0,k=0,var,nextSUB=0,value,pos=0,periodtime=0,starttime=0,endtime=0,totaltime=0,break1=0,break2=0; 
int monA=6,tueA=12,wedA=17,thuA=22,friA=27,satA=32,brk1,brk2,noPeriod=0;
int HOUR,MINUT,SECOND,timeH,timeM,setT=0,breakR=0;



byte Aalpha[8] ={ 0b00000,

                   0b01110,

                   0b10001,

                   0b11111,

                   0b10001,

                   0b10001,

                   0b00000,

                   0b00000

                   };

 byte Balpha[8]={ 0b00000,

                    0b11100,

                    0b10010,

                    0b11100,

                    0b10010,

                    0b11100,

                    0b00000,

                    0b00000};

  byte Calpha[8]={   0b00000,

                     0b01110,

                     0b10000,

                     0b10000,

                     0b10000,

                     0b01110,

                     0b00000,

                     0b00000};                   

 byte Dalpha[8] ={ 0b00000,

                   0b11100,

                   0b10010,

                   0b10010,

                   0b10010,

                   0b11100,

                   0b00000,

                   0b00000};

  byte Ealpha[8] ={0b00000,

                   0b11110,

                   0b10000,

                   0b11100,

                   0b10000,

                   0b11110,

                   0b00000,

                   0b00000}; 

  byte Falpha[8] ={0b00000,

                   0b11110,

                   0b10000,

                   0b11100,

                   0b10000,

                   0b10000,

                   0b00000,

                   0b00000};

    byte Galpha[8]={0b00000,

                    0b01110,

                    0b10000,

                    0b10110,

                    0b10010,

                    0b01110,

                    0b00000,

                    0b00000};

   byte Halpha[8] ={ 0b00000,

                   0b10001,

                   0b10001,

                   0b11111,

                   0b10001,

                   0b10001,

                   0b00000,

                   0b00000};  

   byte Ialpha[8]={0b00000,

                     0b01110,

                     0b00100,

                     0b00100,

                     0b00100,

                     0b01110,

                     0b00000,

                     0b00000}; 

    byte Lalpha[8]={ 0b00000,

                     0b10000,

                     0b10000,

                     0b10000,

                     0b10000,

                     0b11110,

                     0b00000,

                     0b00000}; 

   byte Malpha[8] ={ 0b00000,

                    0b10001,

                   0b11011,

                   0b10101,

                   0b10001,

                   0b10001,

                   0b00000,

                   0b00000};                                                                         

   byte Nalpha[8] ={0b00000,

                    0b10001,

                   0b11001,

                   0b10101,

                   0b10011,

                   0b10001,

                   0b00000,

                   0b00000};     

   byte Oalpha[8]={ 0b00000,

                     0b01110,

                     0b10001,

                     0b10001,

                     0b10001,

                     0b01110,

                     0b00000,

                     0b00000}; 

 byte Palpha[8]={  0b00000,

                      0b11100,

                     0b10010,

                     0b11100,

                     0b10000,

                     0b10000,

                     0b00000,

                     0b00000};      

 byte Ralpha[8]={    0b00000,

                     0b11110,

                     0b10001,

                     0b11110,

                     0b10100,

                     0b10010,

                     0b00000,

                     0b00000};                                    

 byte Salpha[8] ={ 0b00000,

                   0b01110,

                   0b10000,

                   0b01110,

                   0b00001,

                   0b01110,

                   0b00000,

                   0b00000}; 

   byte Talpha[8] ={0b00000,

                    0b11111,

                   0b00100,

                   0b00100,

                   0b00100,

                   0b00100,

                   0b00000,

                   0b00000};  

                                        

  byte Ualpha[8]={   0b00000,

                     0b10001,

                     0b10001,

                     0b10001,

                     0b10001,

                     0b01110,

                     0b00000,

                     0b00000};    

  byte Walpha[8] ={  0b00000,

                     0b10001,

                     0b10001,

                     0b10101,

                     0b11011,

                     0b10001,

                     0b00000,

                     0b00000};

   byte Yalpha[8]={  0b00000,

                     0b10001,

                     0b10001,

                     0b01010,

                     0b00100,

                     0b00100,

                     0b00000,

                     0b00000};   

    

  byte next[8]={0b00000,

                0b10000,

                0b01000,

                0b00100,

                0b01000,

                0b10000,

                0b00000,

                0b00000

              };                               


void setup()  {

   Wire.begin();

   rtc.begin();

   Serial.begin(9600); //Software serial initialization

   pinMode(push_button1, INPUT);
   pinMode(push_button2, INPUT);
   pinMode(push_button3, INPUT);
   pinMode(push_button4, INPUT);
   pinMode( buzzer, OUTPUT ); 
   digitalWrite( buzzer, LOW ); 

   lcd.begin(16,2);
   lcd.setCursor(0,0);
   lcd.print("E Clinic");  
   lcd.setCursor(0,1);
   lcd.print("   TIME TABLE   ");
   delay(3000);

   if (! rtc.isrunning()) {

    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

   }

   delay(3000);

   lcd.clear();
lcd.setCursor(0,0);
lcd.print(" ENTER ALL INFO ");
Serial.print(" ENTER ALL INFO ");
lcd.setCursor(0,1);
lcd.print(" YES*       NO# ");
Serial.print(" YES*       NO# ");
}


void loop() { 

push_button1_state = digitalRead(push_button1);
push_button2_state = digitalRead(push_button2);
push_button3_state = digitalRead(push_button3);
push_button4_state = digitalRead(push_button4); 


if(push_button1_state==LOW){

      pos=20;

      DateTime now =rtc.now();
      prevTime=now.minute();
      prevHour=now.hour();
      Serial.print("previous hour");
      Serial.println(prevHour);
      Serial.print("previous time");
      Serial.println(prevTime);
      Serial.print("eeprom");
      Serial.println(EEPROM.read(4));
      lcd.clear();}

if(push_button4_state==LOW){

     pos=30; 

     DateTime now =rtc.now();
     prevTime=now.minute();
     prevHour=now.hour();
     hoursE = EEPROM.read(4);//Period time delay
     Serial.println(prevHour);
     Serial.println(prevTime);
     Serial.println(EEPROM.read(4));
     lcd.clear();}

while(pos==20){

    pos=0;
    delay(1000);
    lcd.clear();

  while(pos==0){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);
    lcd.setCursor(0,0);
    lcd.print("ENTER START TIME");  
    Serial.println("ENTER START TIME");  

   if(push_button1_state==LOW && pos==0){

    lcd.setCursor(5,1);
    starttime++;
    printDigits2(starttime);
    lcd.setCursor(8,1);
    lcd.print("O' clock");
    Serial.print("O' clock");
    EEPROM.write(5,starttime);
    delay(200);

    if(starttime==24)

    starttime=0;

   }

   if(push_button4_state==LOW && pos==0){

     prevHour=starttime;
     lcd.setCursor(0,0);
    lcd.print(" *****SAVED**** ");
    Serial.println(" *****SAVED**** ");
     EEPROM.write(0,starttime);
     Serial.println(starttime);
     delay(1000);
     pos=1;
     lcd.clear();
     break;   

    }

  }

 while(pos==1){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);

    lcd.setCursor(0,0);
    lcd.print(" ENTER END TIME ");  
    Serial.println(" ENTER END TIME ");  

   if(push_button1_state==LOW && pos==1){
   lcd.setCursor(5,1);
    endtime++;
    printDigits2(endtime);
    Serial.println("O' clock");
    lcd.setCursor(7,1);
    lcd.print("O' clock");
    delay(200);

    if(endtime==24)
    endtime=0;

   }

   if(push_button4_state==LOW && pos==1){
     lcd.setCursor(0,0);
    lcd.print(" *****SAVED**** ");
    Serial.println(" *****SAVED**** ");
     EEPROM.write(1,endtime);
     Serial.println(endtime);
     delay(1000);
     pos=2;
     lcd.clear();
     break;   

    }

  }

  while(pos==2){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);

    lcd.setCursor(0,0);
    lcd.print(" FIRST BRK TIME "); 
    Serial.println(" FIRST BRK TIME ");  
   if(push_button1_state==LOW && pos==2){
    lcd.setCursor(5,1);
    break1++;
    printDigits2(break1);
    lcd.setCursor(8,1);
    lcd.print("minutes");
    delay(200);
    
    if(break1==40)
    break1=0;

   }

   if(push_button4_state==LOW && pos==2){
     lcd.setCursor(0,0);
    lcd.print(" *****SAVED**** ");
     EEPROM.write(2,break1);
     Serial.println(break1);
     delay(1000);
     pos=3;
     lcd.clear();
     break;   

    }

      }

 while(pos==3){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);

    lcd.setCursor(0,0);
    lcd.print("SECOND BRK TIME ");  
   if(push_button1_state==LOW && pos==3){
    lcd.setCursor(5,1);
    break2++;
    printDigits2(break2);
    lcd.setCursor(8,1);
    lcd.print("minutes");
    delay(200);
    if(break2==40)
    break2=0;

   }

   if(push_button4_state==LOW && pos==3){

     lcd.setCursor(0,0);
    lcd.print(" *****SAVED**** ");
     EEPROM.write(3,break2);
     Serial.println(break2);
     delay(1000);
     pos=4;
     lcd.clear();
     break;   

    }

  }

/*Slection of subject DAY wise*/ 

while(pos==4){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);

    lcd.setCursor(0,0);
    lcd.print("SLCT MONDAY SUB ");  
   if(push_button1_state==LOW && pos==4){
    if(i>0){
      i--;
      delay(400);
    lcd.setCursor(6,1);
    lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button2_state==LOW && pos==4){

    if(i<7){
         i++;
        delay(400);
        lcd.setCursor(6,1);
        lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

    if(push_button4_state==LOW && pos==4){

      EEPROM.write(j+6,i);
      lcd.setCursor(0,0);
      lcd.print(" *****SAVED**** ");
      Serial.print(j+6);
      Serial.println(i);
      delay(1000);
      lcd.clear();
      j++;

   }

if(push_button3_state==LOW && pos==4){

      lcd.setCursor(0,0);
      lcd.print("*SUB SAVED MON*");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("   THANK YOU   ");
      pos=5;
       delay(3000);
       lcd.clear();
       break;

}    

  }

  /*Subject selection for TUESDAY*/

  while(pos==5){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);

    lcd.setCursor(0,0);
    lcd.print("SLCT TUESDAY SUB");  
  if(push_button1_state==LOW && pos==5){

    if(i>0){
      i--;
      delay(400);
    lcd.setCursor(6,1);
    lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button2_state==LOW && pos==5){

    if(i<7){

         i++;
        delay(400);
        lcd.setCursor(6,1);
        lcd.print("      ");

    }

    lcd.setCursor(6,1);

    lcd.print(mySubject[i]);

   }

    if(push_button4_state==LOW && pos==5){

      EEPROM.write(j+7,i);
      lcd.setCursor(0,0);
      lcd.print(" *****SAVED**** ");
      Serial.print(j+7);
      Serial.println(i);
      delay(1000);
      lcd.clear();
      j++;

   }

if(push_button3_state==LOW && pos==5){

      lcd.setCursor(0,0);
      lcd.print(" *SUB SAVED TUE* ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("    THANK YOU    ");
      pos=6;
      delay(3000);
      lcd.clear();
      break;

}    

  }

/*Subject selection for WEDNESDAY*/

while(pos==6){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);

    lcd.setCursor(0,0);
    lcd.print(" WEDNESDAY SUB ");  
   if(push_button1_state==LOW && pos==6){

    if(i>0){

      i--;
      delay(400);
    lcd.setCursor(6,1);
    lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button2_state==LOW && pos==6){

    if(i<7){

         i++;
        delay(400);
        lcd.setCursor(6,1);
        lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

    if(push_button4_state==LOW && pos==6){

      EEPROM.write(j+7,i);
      lcd.setCursor(0,0);
      lcd.print(" *****SAVED**** ");
      Serial.print(j+7);
      Serial.println(i);
      delay(1000);
      lcd.clear();
      j++;

   }

if(push_button3_state==LOW && pos==6){

      lcd.setCursor(0,0);
      lcd.print(" *SUB SAVED WED* ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("    THANK YOU    ");
      pos=7;
      delay(3000);
      lcd.clear();
      break;

}    

  }

 /*Subject selection for THUSDAY*/

while(pos==7){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);
    
    lcd.setCursor(0,0);
    lcd.print(" SLT THUSDAY SUB ");  

   if(push_button1_state==LOW && pos==7){

    if(i>0){

      i--;
      delay(400);
    lcd.setCursor(6,1);
    lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button2_state==LOW && pos==7){

    if(i<7){

         i++;
        delay(400);
        lcd.setCursor(6,1);
        lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button4_state==LOW && pos==7){

      EEPROM.write(j+7,i);
      lcd.setCursor(0,0);
      lcd.print(" *****SAVED**** ");
      Serial.print(j+7);
      Serial.println(i);
      delay(1000);
      lcd.clear();
      j++;

   }

if(push_button3_state==LOW && pos==7){

      lcd.setCursor(0,0);
      lcd.print(" *SUB SAVED THU* ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("    THANK YOU    ");
      pos=8;
      delay(3000);
      lcd.clear();
      break;

}    

  }


 /*Subject selection for FRIDAY*/

while(pos==8){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);
    lcd.setCursor(0,0);
    lcd.print(" SLT FRIDAY SUB ");  
   if(push_button1_state==LOW && pos==8){

    if(i>0){

      i--;
      delay(400);
    lcd.setCursor(6,1);
    lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button2_state==LOW && pos==8){

    if(i<7){

         i++;
        delay(400);
        lcd.setCursor(6,1);
        lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

    if(push_button4_state==LOW && pos==8){

      EEPROM.write(j+7,i);
      lcd.setCursor(0,0);
      lcd.print(" *****SAVED**** ");
      Serial.print(j+7);
      Serial.println(i);
      delay(1000);
      lcd.clear();
      j++;

   }

if(push_button3_state==LOW && pos==8){

      lcd.setCursor(0,0);
      lcd.print(" *SUB SAVED FRI* ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("    THANK YOU    ");
      pos=9;
      delay(3000);
      lcd.clear();
      break;

}    

  } 

 /*Subject selection for SATURDAY*/

while(pos==9){

    push_button1_state = digitalRead(push_button1);
    push_button2_state = digitalRead(push_button2);
    push_button3_state = digitalRead(push_button3);
    push_button4_state = digitalRead(push_button4);
    lcd.setCursor(0,0);
    lcd.print("SLT SATURDAY SUB ");  
   if(push_button1_state==LOW && pos==9){
    if(i>0){

      i--;

      delay(400);
    lcd.setCursor(6,1);
    lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

   if(push_button2_state==LOW && pos==9){

    if(i<7){

         i++;
        delay(400);
        lcd.setCursor(6,1);
        lcd.print("      ");

    }

    lcd.setCursor(6,1);
    lcd.print(mySubject[i]);

   }

    if(push_button4_state==LOW && pos==9){

      EEPROM.write(j+7,i);
      lcd.setCursor(0,0);
      lcd.print(" *****SAVED**** ");
      Serial.print(j+7);
      Serial.println(i);
      delay(1000);
      lcd.clear();
      j++;

   }

if(push_button3_state==LOW && pos==9){

      lcd.setCursor(0,0);
      lcd.print(" *SUB SAVED SAT* ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("    THANK YOU    ");
      pos=30;
      delay(3000);
      lcd.clear();
      break;

}    

  }

   

 /*READING ALL THE DATA FROM THE INTERNAL EEPROM*/

  starttime = EEPROM.read(0);     
  endtime = EEPROM.read(1);
  break1 = EEPROM.read(2);
  break2 = EEPROM.read(3);
  hoursE = EEPROM.read(4);//Period time delay
  int totalbreak=break1+ break2;
  //totaltime=((endtime-starttime)*10 - totalbreak);
  totaltime=((endtime-starttime)*60-totalbreak);
  periodtime=totaltime/5;
  EEPROM.write(4,periodtime);
  Serial.println(EEPROM.read(4));
  pos=30; 

}

  while(pos==30){

  DateTime now =rtc.now();
  hoursH=EEPROM.read(0);//start time in hour 
  newHour = now.hour();
  newTime = now.minute();
//if(newHour == hoursH && newTime == 0){  
  if(newHour == hoursH && newTime <= 60){
     prevTime=now.minute();
     lcd.setCursor(0,0);
     lcd.print("  CLASS STARTED  ");
     lcd.setCursor(0,1);
     lcd.print("                 "); 
     delay(3000);
     lcd.clear();

     while(1){

      matchDAY(); 

  }

}

else{

  lcd.setCursor(0,0);
  lcd.print("  Waiting for...  ");
  lcd.setCursor(5,1);
  printDigits2(hoursH);
  lcd.setCursor(8,1);
  lcd.print("O' Clock");}

  }

}



void matchDAY(){

   DateTime now =rtc.now();
   //Monday subject display

while(now.dayOfTheWeek()== 1){

  currentTIME();
  matchTIM(); 
  
  value = EEPROM.read(monA);
  ALLDays(now.dayOfTheWeek());
  lcd.setCursor(0,0);
  lcd.print(mySubject[value]);
  if(noPeriod==2 && breakR==0){
  brk1=EEPROM.read(2);
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime == prevTime + brk1){

       prevTime=prevTime+brk1;
        Serial.print("Break1");
        Serial.println(prevTime);

        noPeriod=0;
        breakR=1;
        lcd.clear();
        break;

                  }

            }

      }

  if(noPeriod==1 &&  breakR==2){

     breakR=0;
     lcd.clear();
     endsession(); 

     while(1){

        //End of the session

     }

  }

  //hoursE

  if(noPeriod==2 && breakR==1){

    brk2=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();
    lcd.setCursor(11,0);
    printDigits2(newTime);
    printDigits2(now.second());

    if(newTime== prevTime + brk2){

      prevTime=prevTime+brk2;
        Serial.print("Break2");
        Serial.println(prevTime);
        noPeriod=0;
        breakR=2;
        lcd.clear();
        break;

                       }

            }

  }

  else{

    

     nextSUB = EEPROM.read(monA+1);

      ALLSubjects(9);

        if(noPeriod == 0 && breakR == 2)

          ALLSubjects(8);

       else

        ALLSubjects(nextSUB);

      }

  }   

  // Tuesday subject display

while(now.dayOfTheWeek()== 2){

  currentTIME();

  matchTIM();

  
  value = EEPROM.read(tueA);
  ALLDays(now.dayOfTheWeek());
  lcd.setCursor(0,0);
  lcd.print(mySubject[value]);
  
  if(noPeriod==2 && breakR==0){
  brk1=EEPROM.read(2);
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();
    if(newTime== prevTime + brk1){

       prevTime=prevTime+brk1;
        Serial.print("Break1");
        Serial.println(prevTime);
        noPeriod=0;
        breakR=1;
        lcd.clear();
        break;

                   }

            }

      }

  if(noPeriod==1 &&  breakR==2){

     breakR=0;

     endsession(); 

     while(1){

        //End of the session

     }

  }

  if(noPeriod==2 && breakR==1){

    brk2=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();
    if(newTime== prevTime + brk2){

      prevTime=prevTime+brk2;
        Serial.print("Break2");
        Serial.println(prevTime);
       noPeriod=0;
        breakR=2;
        lcd.clear();
        break;

                       }

            }

  }

  else{

     nextSUB = EEPROM.read(tueA+1);

      ALLSubjects(9);

        if(noPeriod == 0 && breakR == 2)

          ALLSubjects(8);

       else

        ALLSubjects(nextSUB);

      }

  }

  

//Wednesday subject display

while(now.dayOfTheWeek()== 3){

  currentTIME();
   matchTIM(); 
  value = EEPROM.read(wedA);
  ALLDays(now.dayOfTheWeek());
  lcd.setCursor(0,0);
  lcd.print(mySubject[value]);
  
  if(noPeriod==2 && breakR==0){
  brk1=EEPROM.read(2);
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk1){

       prevTime=prevTime+brk1;
        Serial.print("Break1");
        Serial.println(prevTime);
        noPeriod=0;
        breakR=1;
        lcd.clear();
        break;

                        }

            }

      }

  if(noPeriod==1 &&  breakR==2){

     breakR=0;
     endsession(); 

     while(1){

        //End of the session

     }

  }

  if(noPeriod==2 && breakR==1){

    brk2=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk2){

      prevTime=prevTime+brk2;
        Serial.print("Break2");
        Serial.println(prevTime);
       noPeriod=0;
        breakR=2;
        lcd.clear();
        break;

                       }

            }

  }

  else{

     nextSUB = EEPROM.read(wedA+1);

      ALLSubjects(9);

        if(noPeriod == 0 && breakR == 2)

          ALLSubjects(8);

       else

        ALLSubjects(nextSUB);

      }

  }


// Thusday subject display   

while(now.dayOfTheWeek()== 4){

  currentTIME();
  matchTIM(); 
  value = EEPROM.read(thuA);
  ALLDays(now.dayOfTheWeek());
  lcd.setCursor(0,0);
  lcd.print(mySubject[value]);

  if(noPeriod==2 && breakR==0){

  brk1=EEPROM.read(2);
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk1){
       prevTime=prevTime+brk1;
        Serial.print("Break1");
        Serial.println(prevTime);
        noPeriod=0;
        breakR=1;
        lcd.clear();
        break;

                        }

            }

      }

  if(noPeriod==1 &&  breakR==2){

     breakR=0;

     endsession(); 

     while(1){

        //End of the session

     }

  }

  if(noPeriod==2 && breakR==1){

    brk2=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk2){

      prevTime=prevTime+brk2;
        Serial.print("Break2");
        Serial.println(prevTime);
       noPeriod=0;
        breakR=2;
        lcd.clear();
        break;

                       }

            }

  }

  else{

     nextSUB = EEPROM.read(thuA+1);
      ALLSubjects(9);
        if(noPeriod == 0 && breakR == 2)

          ALLSubjects(8);

       else

        ALLSubjects(nextSUB);

      }

  }


  

  //Friday subject display

while(now.dayOfTheWeek()== 5){

  currentTIME();
   matchTIM(); 
  value = EEPROM.read(friA);
  ALLDays(now.dayOfTheWeek());
  lcd.setCursor(0,0);
  lcd.print(mySubject[value]);

  if(noPeriod==2 && breakR==0){

  brk1=EEPROM.read(2);
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk1){

       prevTime=prevTime+brk1;
        Serial.print("Break1");
        Serial.println(prevTime);
        noPeriod=0;
        breakR=1;
        lcd.clear();
        break;

                        }

            }

      }

  if(noPeriod==1 &&  breakR==2){

     breakR=0;
     endsession(); 

     while(1){

        //End of the session

     }

  }

  if(noPeriod==2 && breakR==1){

    brk2=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk2){

      prevTime=prevTime+brk2;
        Serial.print("Break2");
        Serial.println(prevTime);
       noPeriod=0;
        breakR=2;
        lcd.clear();
        break;

                       }

            }

  }

  else{

     nextSUB = EEPROM.read(friA+1);

      ALLSubjects(9);

        if(noPeriod == 0 && breakR == 2)

          ALLSubjects(8);

       else

        ALLSubjects(nextSUB);

      }

  }


  

  //Saturday subject display

while(now.dayOfTheWeek()== 6){

  currentTIME();
   matchTIM(); 

  value = EEPROM.read(satA);
  ALLDays(now.dayOfTheWeek());
  lcd.setCursor(0,0);
  lcd.print(mySubject[value]);

  if(noPeriod==2 && breakR==0){

  brk1=EEPROM.read(2);
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk1){

       prevTime=prevTime+brk1;
        Serial.print("Break1");
        Serial.println(prevTime);
        noPeriod=0;
        breakR=1;
        lcd.clear();
        break;

                        }

            }

      }

  if(noPeriod==1 &&  breakR==2){

     breakR=0;
     endsession(); 

     while(1){

        //End of the session

     }

  }

  if(noPeriod==2 && breakR==1){

    brk2=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(newTime== prevTime + brk2){

      prevTime=prevTime+brk2;
        Serial.print("Break2");
        Serial.println(prevTime);
       noPeriod=0;
        breakR=2;
        lcd.clear();
        break;

                       }

            }

  }

  else{

     nextSUB = EEPROM.read(satA+1);
      ALLSubjects(9);

        if(noPeriod == 0 && breakR == 2)

          ALLSubjects(8);

       else

        ALLSubjects(nextSUB);

      }

  }

}




void currentTIME(){

 lcd.setCursor(6,1);
 lcd.print("  ");
 DateTime now = rtc.now();
 lcd.setCursor(8,1);
 printDigits2(HOUR=now.hour()); 
 lcd.print(":");
 newHour=now.hour();
 printDigits2(MINUT=now.minute());
 lcd.print(":");
 newTime = now.minute();
 printDigits2(SECOND=now.second());
 delay(800);
 currentDAY();
 delay(800);

}


void currentDAY(){

 DateTime now = rtc.now();
 lcd.setCursor(6,1);
 printDigits2(now.day());
 lcd.print("/");
 printDigits2(now.month());
 lcd.print("/");
 lcd.print(now.year(),DEC);

}



void matchTIM(){

  DateTime now =rtc.now(); 
 int tempTime = prevTime + hoursE;

 if(tempTime >=60){

  tempTime = tempTime - 60;   

 }

 else 

 tempTime = prevTime + hoursE; 

if(newTime == tempTime){

   prevTime=now.minute();
   Serial.print("MatchTime");
   Serial.println(prevTime);
   
   if(now.dayOfTheWeek()==1){

   digitalWrite( buzzer, HIGH );
   delay(3000);
   digitalWrite( buzzer, LOW );
   monA++;
   noPeriod++;
   lcd.setCursor(0,0);
   lcd.print("                 "); 

   }

   if(now.dayOfTheWeek()==2){

    digitalWrite( buzzer, HIGH );
   delay(3000);
   digitalWrite( buzzer, LOW );
   tueA++;
   noPeriod++;
   lcd.setCursor(0,0);
   lcd.print("                 "); 

   }

  if(now.dayOfTheWeek()==3){

    digitalWrite( buzzer, HIGH );
   delay(3000);
   digitalWrite( buzzer, LOW );
   wedA++;
   noPeriod++;
  lcd.setCursor(0,0);
   lcd.print("                 "); 

   }

   if(now.dayOfTheWeek()==4){

    digitalWrite( buzzer, HIGH );
   delay(3000);
   digitalWrite( buzzer, LOW );
   thuA++;
   noPeriod++;
   lcd.setCursor(0,0);
   lcd.print("                 "); 

   } 

   if(now.dayOfTheWeek()==5){

    digitalWrite( buzzer, HIGH );
   delay(3000);
   digitalWrite( buzzer, LOW );
   friA++;
   noPeriod++;
   lcd.setCursor(0,0);
   lcd.print("                 "); 

   }

   if(now.dayOfTheWeek()==6){

    digitalWrite( buzzer, HIGH );
   delay(3000);
   digitalWrite( buzzer, LOW );
   satA++;
   noPeriod++;
   lcd.setCursor(0,0);
   lcd.print("                 "); 

   }

}

}


//this void function is really useful; it adds a "0" to the beginning of the number, 

//so that 5 minutes is displayed as "05", rather than "5 "

void printDigits2(int digits)  

{

  if(digits < 10) 

  {

    lcd.print("0");
    lcd.print(digits);

  }

  else

  {

    lcd.print(digits);

  }

}



//char* mySubject[]={"MATHS", "ENG","BIO","PHY","CHEM","IT LAB","HIST","GEO"};
void ALLSubjects(int sub)

{

  switch(sub)

  {

    case 0:

          lcd.createChar(1,Malpha);
          lcd.createChar(2,Aalpha);
          lcd.createChar(3,Talpha);
          lcd.createChar(4,Halpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          lcd.setCursor(3,1);
          lcd.write(4);
          break;

    case 1: 

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Ealpha);
          lcd.createChar(2,Nalpha);
          lcd.createChar(3,Galpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          break;

 //char* mySubject[]={"MATHS", "ENG","BIO","PHY","CHEM","IT LAB","HIST","GEO"};         

    case 2: 

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Balpha);
          lcd.createChar(2,Ialpha);
          lcd.createChar(3,Oalpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          break;

    case 3:

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Palpha);
          lcd.createChar(2,Halpha);
          lcd.createChar(3,Yalpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);break;

 //char* mySubject[]={"MATHS", "ENG","BIO","PHY","CHEM","IT LAB","HIST","GEO"};

    case 4: 

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Calpha);
          lcd.createChar(2,Halpha);
          lcd.createChar(3,Ealpha);
          lcd.createChar(4,Malpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          lcd.setCursor(3,1);
          lcd.write(4);
          break;

          

    case 5: 

          lcd.createChar(1,Ialpha);
          lcd.createChar(2,Talpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);break;

          
    case 6: 

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Halpha);
          lcd.createChar(2,Ialpha);
          lcd.createChar(3,Salpha);
          lcd.createChar(4,Talpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          lcd.setCursor(3,1);
          lcd.write(4);
          break;

 //char* mySubject[]={"MATHS", "ENG","BIO","PHY","CHEM","IT LAB","HIST","GEO"};

    case 7: 

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Galpha);
          lcd.createChar(2,Ealpha);
          lcd.createChar(3,Oalpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          break;

   case 8: 

          lcd.setCursor(0,1);
          lcd.write("     ");
          delay(50);
          lcd.createChar(1,Ealpha);
          lcd.createChar(2,Nalpha);
          lcd.createChar(3,Dalpha);
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.setCursor(1,1);
          lcd.write(2);
          lcd.setCursor(2,1);
          lcd.write(3);
          break;       

   case 9: 

          lcd.createChar(8,next);
          lcd.setCursor(5,1);
          lcd.write(8);
          delay(1000);
          lcd.setCursor(5,1);
          lcd.write(" ");
          delay(1000);break;
  default:break;

  } 

}


void ALLDays(int dey)

{

  switch(dey)

  {

    case 0://sun

          lcd.createChar(5,Salpha);
          lcd.createChar(6,Ualpha);
          lcd.createChar(7,Nalpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=1;break;   

       case 1://mon

          lcd.createChar(5,Malpha);
          lcd.createChar(6,Oalpha);
          lcd.createChar(7,Nalpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=2;break;

      case 2://tue

          lcd.createChar(5,Talpha);
          lcd.createChar(6,Ualpha);
          lcd.createChar(7,Ealpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=3;break;

    

    case 3://wed

          lcd.createChar(5,Walpha);
          lcd.createChar(6,Ealpha);
          lcd.createChar(7,Dalpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=4;break;

    case 4://thu

          lcd.createChar(5,Talpha);
          lcd.createChar(6,Halpha);
          lcd.createChar(7,Ualpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=5;break;

     case 5://fri

          lcd.createChar(5,Falpha);
          lcd.createChar(6,Ralpha);
          lcd.createChar(7,Ialpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=6;break;

      case 6://sat

          lcd.createChar(5,Salpha);
          lcd.createChar(6,Aalpha);
          lcd.createChar(7,Talpha);
          lcd.setCursor(7,0);
          lcd.write(5);
          lcd.setCursor(8,0);
          lcd.write(6);
          lcd.setCursor(9,0);
          lcd.write(7);var=7;break;    

      default:break;

  } 

}


void endsession(){

   lcd.setCursor(0,0);
   lcd.print("   Session End  ");
   lcd.setCursor(0,1);
   lcd.print("   Good Luck :) ");

  delay(3000);

}
