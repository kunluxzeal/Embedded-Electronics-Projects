
#include "RTClib.h"     

RTC_DS1307 rtc;          

  
const int relayPin1 = 7; 
const int relayPin2 = 8;  

bool clockedIn = false;   

void setup() {
   

Serial.begin(115200);   
  pinMode(relayPin1, OUTPUT);   
   pinMode(relayPin2, OUTPUT); 
   Serial.println("welcome");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}          
                        


void loop() {
 
  // Check if it's morning or evening to control the relay
  DateTime now = rtc.now();
  if (isMorning(now)) {
    clockIn();
    delay(500);
    activateRelay();
    delay(500);

  } else if (isEvening(now)) {
    deactivateRelay();
    delay(500);
    clockOut();
    delay(500);
    Serial.println("Relay off");
  }
}

void clockIn() {
  DateTime now = rtc.now();  // Get the current time from the RTC



  // Display clock in information
  Serial.print("Clocked in at: ");
  printTime(now);
  Serial.println();

  // Update clockedIn flag
  clockedIn = true;
}

void clockOut() {
  DateTime now = rtc.now();  // Get the current time from the RTC

  // Display clock out information
  Serial.print("Clocked out at: ");
  printTime(now);
  Serial.println();

  // Update clockedIn flag
  clockedIn = false;
}

void activateRelay() {
  digitalWrite(relayPin1, HIGH);
}

void deactivateRelay() {
  digitalWrite(relayPin2, HIGH);
}

bool isMorning(DateTime time) {
  
  return (time.hour() >= 6 && time.hour() < 12);
}

bool isEvening(DateTime time) {
  
  return (time.hour() >= 18 && time.hour() < 22);
}

void printTime(DateTime time) {
  Serial.print(time.hour());
  Serial.print(':');
  if (time.minute() < 10) {
    Serial.print('0');
  }
  Serial.print(time.minute());
  Serial.print(':');
  if (time.second() < 10) {
    Serial.print('0');
  }
  Serial.print(time.second());
}
