#include<EEPROM.h>
#include <DS3231.h> //Library for Real Time Clock.
#include <Wire.h>

//int Hrs =0 ;
//int Mins =0;
bool timeDisplay = false;
int num;

int Interval =0;

#define max_period 9
char period[max_period+1];
char Hrs[13]={8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
char* Mins[2] = {0,50}; // minutes when ring
int Secs[4]= {0,1,2,3} ; // how many secon ring

#define Day 1
#define Month 1
#define Year 22
int val;
int buzzer = 10;
int i;

int Relay = 8; //Relay declaration pin
DS3231  rtc(SDA, SCL); //RTC declaration
Time t;


void setup() {
  // put your setup code here, to run once:

period[0] = '\0';//Initialize string to emtpy.

Serial.begin(9600);

  
   pinMode(buzzer,OUTPUT);
//Serial.print("school bell");
delay(100);
rtc.begin();


 

  /*rtc.setTime(19 , 6 , 0); // Set the time 
  rtc.setDate(12, 11, 2022); // Set the date
  rtc.setDOW(SATURDAY); // Set the Day Of Week (No ring on Saturday and Sunday)
  digitalWrite(Relay, HIGH);
  */
}

void loop() {
  // put your main code here, to run repeatedly:


if (Serial.available()){
  char input = Serial.read();
  static int s_len ;//static variables default to 0
  if (s_len>=max_period) {
    Serial.println("period exceded");
    
      // Have received already the maximum number of characters
      // Ignore all new input until line termination occurs
    } else if (input != '\n' && input != '\r') {
      period[s_len++] = input;
    } else {
      // Have received a LF or CR character

      // INSERT YOUR CODE HERE TO PROCESS THE RECEIVED DATA //
      // YOU COULD COPY TO A NEW VARIABLE WITH strncpy() OR //
      // SET A FLAG TO SAY TO START SOME OTHER TASK         //
      Serial.print("RECEIVED MSG: ");
      Serial.println(period);
      get_time();

      memset(period, 0, sizeof(period));
      s_len = 0;             // Reset input buffer here if you
                             // have already copied the data.
                             // If you don't reset here, then
                             // you can't start receiving more
                             // serial port data. This is your
                             // 'software' serial buffer, contrast
                             // with the hardware serial buffer.
    }
  }



}

void set_time(){
  rtc.setTime(19 , 6 , 0); // Set the time 
  rtc.setDate(12, 11, 2022); // Set the date
  rtc.setDOW(SATURDAY); // Set the Day Of Week (No ring on Saturday and Sunday)
  digitalWrite(Relay, HIGH);
   }

void get_time(){
  t =rtc.getTime();
  for(i =0 ;i<10 ; i++){
  if(rtc.getDOWStr()== "Monday"){

    if((t.hour)==Hrs[i] &&(t.min)==Mins[i]){
      Serial.println(period[i]);
     digitalWrite(buzzer,HIGH);
      }
      
    else{
      digitalWrite(buzzer , LOW);
      }
    
    }
  

  
  }
}
