/*
 * Almost perfect 23Feb2019 
 *https://www.almostperfect.io
 * 
 * Values for Alarm 1
ALM1_EVERY_SECOND -- causes an alarm once per second.
ALM1_MATCH_SECONDS -- causes an alarm when the seconds match (i.e. once per minute).
ALM1_MATCH_MINUTES -- causes an alarm when the minutes and seconds match.
ALM1_MATCH_HOURS -- causes an alarm when the hours and minutes and seconds match.
ALM1_MATCH_DATE -- causes an alarm when the date of the month and hours and minutes and seconds match.
ALM1_MATCH_DAY -- causes an alarm when the day of the week and hours and minutes and seconds match.
Values for Alarm 2
ALM2_EVERY_MINUTE -- causes an alarm once per minute.
ALM2_MATCH_MINUTES -- causes an alarm when the minutes match (i.e. once per hour).
ALM2_MATCH_HOURS -- causes an alarm when the hours and minutes match.
ALM2_MATCH_DATE -- causes an alarm when the date of the month and hours and minutes match.
ALM2_MATCH_DAY -- causes an alarm when the day of the week and hours and minutes match.
*/

#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#define interruptPin 2 //Pin we are going to use to wake up the Arduino
#define buzzer 7
#include <DS3232RTC.h>   // https://github.com/JChristensen/DS3232RTC

DS3232RTC myRTC;
const int arraySize = 12;
const int normal_hours[12]    = {8,  9, 9,  10, 11, 12, 12, 13, 14, 15, 15, 16};
const int normal_minutes[12]  = {55, 0, 55, 50, 5,  0,  25, 40, 35, 30, 40, 30};
const int friday_hours[12] =    {8,  9, 9,  10, 10, 11, 12, 13, 14, 14, 15, 16};
const int friday_minutes[12] =  {55, 0, 50, 40, 45, 35, 20, 55, 00, 50, 40, 30};
const bool bell_longMode[12] = {true, false, false, false, false, false, true, true, false,false,true,true};
bool ringbell_long = false;
int nextBell_hour;
int nextBell_minute;

void setup() {
  Serial.begin(9600);
  pin_init();
  alarm_init();
  timer_init();
  set_alarm(decide_indexOf_nextAlarm());
}

void loop() {
  delay(5000);
  Going_To_Sleep();
}

//Subroutines
void timer_init(){
  myRTC.begin();
  delay(1000);
  if (myRTC.get()==0){ // timer init failed
    Serial.println("Timer init failed");
    Serial.println("Setting time");
    RTC.set(now());   //set the RTC from the system time
    if (myRTC.get()==0){
      Serial.println("Timer init failed even after setting time");
      while(1);
    }
  }
}

void alarm_init(){
  // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);
}
void set_alarm(int timeIndex){
  //RTC.setAlarm(ALM1_MATCH_HOURS,43,15,00);
  Serial.println("***Setting Alarm***");
  Serial.println("***Current Date and Time***");
  display_time();
  time_t t; //create a temporary time variable so we can set the time and read the time from the RTC
  t=RTC.get();//Gets the current time of the RTC
  int dow = weekday(t);
  ringbell_long = bell_longMode[timeIndex];
  if (dow == 6){ //is friday
    Serial.println("***Today is Friday***");
    char buf[40];
    sprintf(buf, "%.2d:%.2d",friday_hours[timeIndex],friday_minutes[timeIndex]);
    Serial.println(buf);
    RTC.setAlarm(ALM1_MATCH_HOURS,friday_minutes[timeIndex],friday_hours[timeIndex],0);
    nextBell_hour = friday_hours[timeIndex];
    nextBell_minute = friday_minutes[timeIndex];
  }else{
    Serial.println("***Today is not friday***");
    char buf[40];
    sprintf(buf, "%.2d:%.2d",normal_hours[timeIndex],normal_minutes[timeIndex]);
    Serial.println(buf);
    RTC.setAlarm(ALM1_MATCH_HOURS,normal_minutes[timeIndex],normal_hours[timeIndex],0);
    nextBell_hour = normal_hours[timeIndex];
    nextBell_minute = normal_minutes[timeIndex];
  }
  //clear the alarm flag
  RTC.alarm(ALARM_1);
  //disable square wave output
  RTC.squareWave(SQWAVE_NONE);
  RTC.alarmInterrupt(ALARM_1, true);
}

void pin_init(){
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(interruptPin,INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  digitalWrite(buzzer,LOW);
}

void display_time(){
    char buf[40];
    time_t t = myRTC.get();
    sprintf(buf, "%.2d:%.2d:%.2d %.2d%s%d ",
        hour(t), minute(t), second(t), day(t), monthShortStr(month(t)), year(t));
    Serial.println(buf);
}

void Going_To_Sleep(){
    sleep_enable();//Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN,LOW);//turning LED off
    time_t t;// creates temp time variable
    t=RTC.get(); //gets current time from rtc
    Serial.println("Sleep  Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//prints time stamp on serial monitor
    delay(1000); //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();//activating sleep mode
    Serial.println("just woke up!");//next line of code executed after the interrupt 
    digitalWrite(LED_BUILTIN,HIGH);//turning LED on
    t=RTC.get();
    Serial.println("WakeUp Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//Prints time stamp 
    //Ring bell
    routineToDoWhenWakeUp();
    //Set New Alarm
    set_alarm(decide_indexOf_nextAlarm());
    // clear the alarm flag
    RTC.alarm(ALARM_1);
  }

  void wakeUp(){
  Serial.println("Interrrupt Fired");//Print message to serial monitor
   sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
}

void routineToDoWhenWakeUp(){
  //check current time
  time_t t = myRTC.get();
  int dow = weekday(t);
  //check sunday and saturday
  if ((dow == 7) || (dow == 1)){
    Serial.println("****No Alarm today. Happy weekend****");
    return;
  }
  //check if correct time or not
  if ((nextBell_hour == hour(t)) && 
  ((nextBell_minute > minute(t) - 2) || 
  (nextBell_minute < minute(t)+2))){
    if (ringbell_long == true){
      long_bell();
    }else{
      short_bell();
    }
  }
}

int decide_indexOf_nextAlarm(){
  time_t t = myRTC.get();
  int current_hour = hour(t);
  int current_minute = minute(t);
  int dow = weekday(t);
  int bufferIndex = -1;
  if (dow == 6){ //day is friday
    bufferIndex = get_indexOf(current_hour,friday_hours);
    if (bufferIndex >= 0 && bufferIndex < 12){ //its a valid index
      for (int i=bufferIndex; i< arraySize; i++){
        if (((current_hour == friday_hours[i])&&
              (current_minute < friday_minutes[i]))||
              (friday_hours[i] == current_hour+1)){ // same hour and upcoming minute or upcoming hour
            return i;
        }
      }
    }
    else{ // current hour is not in hour array
      return 0;
    }
  }else{
    bufferIndex = get_indexOf(current_hour,normal_hours);
    if (bufferIndex >= 0 && bufferIndex < 12){ //its a valid index
    for (int i=bufferIndex; i< arraySize; i++){
      if (((current_hour == normal_hours[i])&&
      (current_minute < normal_minutes[i]))||
      (normal_hours[i] == current_hour+1)){ // same hour and upcoming minute or upcoming hour
         return i;
      }
    }
  }
  else{ // current hour is not in hour array
    return 0;
  }
  }
  Serial.println("***Wrong index Selected***");
  return 0;
}

int get_indexOf(int element,int array_input[]){
  Serial.print("***Find index of***  ");
  Serial.println(element);
  for (int i=0; i<arraySize; i++){
    if (element == array_input[i]){
      return i;
    }
  }
  return -1;
}

void error_beeping(){
  while(1){
    digitalWrite(buzzer, HIGH);
    delay(1500);
    digitalWrite(buzzer,LOW);
    delay(1500);
  }
}

void long_bell(){
  digitalWrite(buzzer, HIGH);
  delay(3000);
  digitalWrite(buzzer, LOW);
}

void short_bell(){
  digitalWrite(buzzer, HIGH);
    delay(1500);
    digitalWrite(buzzer,LOW);
    delay(1000);

    digitalWrite(buzzer, HIGH);
    delay(1500);
    digitalWrite(buzzer,LOW);
}
