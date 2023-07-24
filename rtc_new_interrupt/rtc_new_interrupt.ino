#include <RTClib.h>     // Include the RTClib library for real-time clock functionality

RTC_DS1307 rtc;          // Create an instance of the RTC_DS1307 class

const int relayPin1 = 7; 
const int relayPin2 = 8;  

const int buttonAPin = 2;  
const int buttonBPin = 4;  
const int buttonCPin = 12;
const int buttonDPin = 13;

bool clockSetMode = false;
bool progSetMode = false;
int hours = 0;
int Min = 0;
 

unsigned long debounceDelay = 50;
bool lastButtonStateA = HIGH;
bool lastButtonStateB = HIGH;
bool lastButtonStateC = HIGH;
bool lastButtonStateD = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT);
  pinMode(buttonDPin , INPUT) ; // for time setting 
 
  pinMode(relayPin1, OUTPUT);   
  pinMode(relayPin2, OUTPUT);
  digitalWrite(buttonAPin, HIGH); 
  digitalWrite(buttonBPin, HIGH);
  digitalWrite(buttonCPin, HIGH);
  digitalWrite(buttonDPin , HIGH);
  
   Serial.println("welcome");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  }
  
}

void loop() {
   //DateTime now = rtc.now();
  
  
  // Check button A press
  if (digitalRead(buttonAPin) == LOW && lastButtonStateA == HIGH) {
    
    clockSetMode = !clockSetMode; 
    Serial.println(clockSetMode ? "Clock Set Mode enabled" : "Clock Set Mode disabled");
    
    lastButtonStateA = LOW;
    delay(debounceDelay);
    
  } else if (digitalRead(buttonAPin) == HIGH) {
    lastButtonStateA = HIGH;
  }


  if (clockSetMode) {
    if (digitalRead(buttonBPin) == LOW && lastButtonStateB == HIGH) {
      hourIncrease();
      lastButtonStateB = LOW;
      delay(debounceDelay);
    }

    if (digitalRead(buttonCPin) == LOW && lastButtonStateC == HIGH) {
      MinIncrease();
      lastButtonStateC = LOW;
      delay(debounceDelay);
    }

    if (digitalRead(buttonDPin) == LOW && lastButtonStateD == HIGH) { 
      adjustTimeOnRTC();      
      delay(100);
      check_time();
      delay(100);
  
      lastButtonStateA = LOW;
      delay(debounceDelay);
    
  }     
    // Reset button states
    if (digitalRead(buttonBPin) == HIGH) {
      lastButtonStateB = HIGH;
    }
    if (digitalRead(buttonCPin) == HIGH) {
      lastButtonStateC = HIGH;
    }

    if (digitalRead(buttonDPin) == HIGH) {
      lastButtonStateD = HIGH;
  }    
  }
  
  
}

void hourIncrease() {
    hours ++;
    Serial.print("Hours:") ;  
    Serial.println(hours);
    if (hours > 23) {
      hours = 0;
    }
}

void MinIncrease() {
    Min++;
    Serial.print("Minute:") ;
    Serial.println(Min);
    if (Min > 59) {
      Min = 0;
    }
  
}
void adjustTimeOnRTC() {
  DateTime currentTime = rtc.now();
  DateTime newTime = DateTime(currentTime.year(), currentTime.month(), currentTime.day(), hours, Min, currentTime.second());

  
  Serial.print("Current Time: ");
  Serial.print(currentTime.year());
  Serial.print("/");
  Serial.print(currentTime.month());
  Serial.print("/");
  Serial.print(currentTime.day());
  Serial.print(" ");
  Serial.print(currentTime.hour());
  Serial.print(":");
  Serial.print(currentTime.minute());
  Serial.print(":");
  Serial.println(currentTime.second());

   rtc.adjust(currentTime);

  Serial.print("New Time: ");
  Serial.print(newTime.year());
  Serial.print("/");
  Serial.print(newTime.month());
  Serial.print("/");
  Serial.print(newTime.day());
  Serial.print(" ");
  Serial.print(newTime.hour());
  Serial.print(":");
  Serial.print(newTime.minute());
  Serial.print(":");
  Serial.println(newTime.second());
  
  //rtc.adjust(newTime);

  
 
}

  
void activateRelay() {
  digitalWrite(relayPin1, HIGH);
  
  digitalWrite(relayPin2, HIGH);

}

void deactivateRelay() {
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
}


bool isMorning(DateTime newTime) {
  bool morning = (newTime.hour() >= 6 && newTime.hour() < 12);
  if (morning) {
    Serial.println("Good Morning");
  }
  return morning;
}

bool isNoon(DateTime newTime) {
  bool noon = (newTime.hour() >= 12 && newTime.hour() < 18);
  if (noon) {
    Serial.println("Good Afternoon");
  }
  return noon;
}

bool isEvening(DateTime newTime) {
  bool evening = (newTime.hour() >= 18 && newTime.hour() < 22);
  if (evening) {
    Serial.println("Good Evening");
  }
  return evening;
}


void check_time(){
    DateTime currentTime = rtc.now();
    DateTime newTime = DateTime(currentTime.year(), currentTime.month(), currentTime.day(), hours, Min, currentTime.second());
     
  bool morning = isMorning(newTime);
  bool noon = isNoon(newTime);  
  bool evening = isEvening(newTime);

  if (morning) {
    activateRelay();
  } 
  else if (morning) {
    activateRelay();
  }   
  else if (evening) {
    deactivateRelay();
  }
}


