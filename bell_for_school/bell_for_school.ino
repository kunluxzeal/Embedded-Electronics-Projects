#include <Wire.h>
#include<EEPROM.h>

#include<LiquidCrystal_I2C.h>
#include <RTClib.h> // for the RTC
LiquidCrystal_I2C lcd(0x27,16,2);

RTC_DS1307 rtc;
#define DS1307_ADDRESS 0x68

byte bcd2dec (byte val){
  return ((val/16*10)+(val % 16));  
  }

byte dec2bcd (byte val){
  return ((val/16*10)+(val % 10)); 
  
  }




// Instance of the class for RTC



char* school_periods[] = {"period1","period2","period4","period5","preiod6","period7","period8"};

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int periods[7];


int new_time,new_hour,new_minute,prev_time=0 ,prev_hour=0 ,prev_minute=0 ,hoursE=0,hoursH, count_time=60;

int i=0,j=0,k=0,var,value,next_period=0,period_time=0,start_time=0,end_time=0,total_time=0,short_break=0,long_break=0; 

int monA=6,tueA=12,wedA=17,thuA=22,friA=27,satA=32,brk1,brk2,noPeriod=0;

int HOUR,MINUTE,SECOND,timeH,timeM,setT=0,breakR=0;


void setup() {
  // put your setup code here, to run once:

Wire.begin();

   rtc.begin();

   Serial.begin(9600); //Software serial initialization


   if (! rtc.isrunning()) {

    Serial.println("RTC is NOT running!");

    // following line sets the RTC to the date & time this sketch was compiled

     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

   }

  

}

void loop() {
  // put your main code here, to run repeatedly:

//Serial.print("school bell");

start_time = EEPROM.read(0);
end_time = EEPROM.read(1);
short_break = EEPROM.read(2);
long_break = EEPROM.read(3);
hoursE = EEPROM.read(4);//Period time delay

int total_break = short_break+long_break;
total_time=((end_time-start_time)*60- total_break);
period_time=total_time/5;




EEPROM.write(4,period_time);
//Serial.println(EEPROM.read(4));

 DateTime now =rtc.now();
  hoursH=EEPROM.read(0);//start time in hour 
  new_hour = now.hour();
  new_time = now.minute();

 
  if(new_hour >= hoursH && new_time <= 60){
     prev_time=now.minute();
     Serial.println("  CLASS STARTED  ");
     
     while(1){

      matchDAY(); 

  }

}

//else{
 // printDigits2(hoursH);
 // Serial.print("O' Clock");
//  }

  }





void matchDAY(){

   DateTime now =rtc.now();
   //Monday subject display

while(now.dayOfTheWeek()==4){

  currentTIME();
  matchTIM(); 
  
  value = EEPROM.read(thuA);
 // ALLDays(now.dayOfTheWeek());
 // Serial.print("value");
 // Serial.println(value);
  Serial.println(school_periods[value]);
  delay(100);
 
 // school_periods[value]++;
  
  if(noPeriod==2 && breakR==0){
  short_break=EEPROM.read(2);
    Serial.print("***BREAK TIME***");

   while(1){

    currentTIME();

    if(new_time == prev_time + short_break){

       prev_time=prev_time+short_break;
        Serial.print("short break");
        Serial.println(prev_time);

        noPeriod=0;
        breakR=1;
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

    long_break=EEPROM.read(3);//time delay
    lcd.setCursor(0,0);
    lcd.print("***BREAK TIME***");

   while(1){

   currentTIME();
    Serial.print(new_time);
    Serial.print(now.second());

    if(new_time== prev_time + long_break){

      prev_time=prev_time+long_break;
        Serial.print("Break2");
        Serial.println(prev_time);
        noPeriod=0;
        breakR=2;
        break;

        }

   }    

  }
   else{
    next_period = EEPROM.read(thuA+1);
    
    }
}
}

void currentTIME(){
// Serial.print("  ");
 DateTime now = rtc.now();
 
 Serial.print(HOUR=now.hour()); 
 Serial.print(":");
 new_hour=now.hour();
 
 Serial.print(MINUTE=now.minute());
 Serial.print(":");
 new_time = now.minute();
 Serial.println(SECOND=now.second());
 delay(800);
 currentDAY();
 delay(800);

}


void currentDAY(){

 DateTime now = rtc.now();
 
 Serial.print(now.day());
 Serial.print("/");
 Serial.print(now.month());
 Serial.print("/");
 Serial.println(now.year(),DEC);

}


void matchTIM(){

  DateTime now =rtc.now(); 
 int temp_time = prev_time + hoursE;

 if(temp_time >=60){

  temp_time = temp_time - 60;   

 }

 else 

 temp_time = prev_time + hoursE; 

if(new_time == temp_time){

   prev_time=now.minute();
   Serial.print("MatchTime");
   Serial.println(prev_time);
   
  

   if(now.dayOfTheWeek()==4){
   thuA++;
   noPeriod++;
  
   } 

   

}

}  



void endsession(){

   Serial.print("   Session End  ");
  
  delay(3000);

}
