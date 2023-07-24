#include <RTClib.h>     // Include the RTClib library for real-time clock functionality
#include <EEPROM.h> // Include the EEPROM library to store user routines

RTC_DS1307 rtc;          // Create an instance of the RTC_DS1307 class

const int relayPin1 = 7; 
const int relayPin2 = 8;  

const int buttonAPin = 2;  
const int buttonBPin = 4;  
const int buttonCPin = 12;
const int buttonDPin = 13;

bool clockSetMode = false;
bool saveTime = false;
bool progTime = false;
bool progSetMode = false;
//bool programActive = false;
bool setStartTime = false;
int hours = 0;
int Min = 0;


int startHour[3] = {0}; // Start times for each routine (routine 1, routine 2, routine 3)
int startMin[3] = {0};
int stopHour[3] = {0};  // Stop times for each routine (routine 1, routine 2, routine 3)
int stopMin[3] = {0};
 

unsigned long debounceDelay = 50;
bool lastButtonStateA = HIGH;
bool lastButtonStateB = HIGH;
bool lastButtonStateC = HIGH;
bool lastButtonStateD = HIGH;

unsigned long buttonPressStartTime = 0;
unsigned long longPressDuration = 1000; // Define the duration for a long press (in milliseconds)

int currentRoutine = 0; // Keeps track of the current routine being set


enum ProgramState {
  IDLE,
  SET_START_TIME,
  SET_STOP_TIME
};
ProgramState progState = SET_START_TIME;

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
  // Load user routines from EEPROM on startup
  //loadUserRoutines();
  
}

void loop() {
   //DateTime now = rtc.now();
  // Check button A press
  if (digitalRead(buttonAPin) == LOW && lastButtonStateA == HIGH) {
    //Serial.print(digitalRead(buttonAPin));
    buttonPressStartTime = millis();
    lastButtonStateA = LOW;
    delay(debounceDelay);
    
  }
  
   else if (digitalRead(buttonAPin) == HIGH && lastButtonStateA == LOW) {
     //Serial.print(digitalRead(buttonAPin));
    if (millis() - buttonPressStartTime >= longPressDuration) {
      // Long press detected
      if (clockSetMode) {
        saveTime = true;
        Serial.println("Setting Time....");
        delay(1000);
      adjustTimeOnRTC();       
        
      } 
    
    } 
    else {
      // Short press detected
      clockSetMode = !clockSetMode;
      if (clockSetMode) {
        progSetMode = false; // Disable program set mode when entering clock set mode
      }      
      displayTime();
      Serial.println(clockSetMode ? "Clock Set Mode enabled" : "Clock Set Mode disabled");
    }
    lastButtonStateA = HIGH;
  }

  if (clockSetMode) {
    // Code to adjust hour and minute using buttons B and C (same as before)
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
      
  }

  if (saveTime) {
    // Code to adjust time on RTC (same as before)
    adjustTimeOnRTC();
    delay(100);
    saveTime = false;
    clockSetMode = false;
    Serial.println("Time Set Succesfully");
    delay(1000);
    Serial.println("Clock Set Mode disabled");
  }
else{
  
  
  if (digitalRead(buttonDPin) == LOW && lastButtonStateD == HIGH) {
    buttonPressStartTime = millis();
    lastButtonStateD = LOW;
    delay(debounceDelay);
  } 
  else if (digitalRead(buttonDPin) == HIGH && lastButtonStateD == LOW) {
    
    if (millis() - buttonPressStartTime >= longPressDuration) {
      // Long press detected
      progSetMode = !progSetMode;      
      if (progSetMode) {
        // Exit program set mode if already in it
        //progSetMode = false;
        progTime = false;
        progState = SET_START_TIME;
        currentRoutine = 0;
        
        Serial.println("Program Set Mode enabled");
      }
      else {
        Serial.println("Program Set Mode disabled");
        // Save user routines to EEPROM on exiting program set mode
        //saveUserRoutines();
      }
    } 
    else {
      // Short press detected
      if (progSetMode) {

        // Code to adjust hour and minute using buttons B and C (same as before)
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
        if (progState == SET_START_TIME) {
          progTime = true;
         setStartTime = true; // Set start time
          // Serial.print("Setting Routine ");
          // Serial.print(currentRoutine + 1);
          Serial.println(" Start Time....");
          delay(1000);
        } else if (progState == SET_STOP_TIME) {
          progTime = true;
          //progState = SET_START_TIME;
          setStartTime = false; // Set stop time

          
            // Serial.print("Setting Routine ");
            // Serial.print(currentRoutine + 1);
            Serial.println(" Stop Time....");
            delay(1000);
        } 
      }    

      else {
          // Exit program set mode and start the program
          progSetMode = !progSetMode;
          if (progSetMode) {
            progSetMode = false;
          }
          displayTime();
          Serial.println(progSetMode ? "program Set Mode enabled" : "program Set Mode disabled");
        }
         
         
        }
        lastButtonStateD = HIGH;        
      }
    
    
  if (progTime) {
    // adjustTimeOnRTC();
    //adjustTimeOnRTC();
    
    routineTIme();
    delay(100);
    progTime = true;    
     //adjustProgramTime();
    // After setting the time, update the program state and current routine accordingly
    if (progState == SET_START_TIME) {
      startHour[currentRoutine] = hours;
      startMin[currentRoutine] = Min;
      Serial.print("Routine ");
      Serial.print(currentRoutine + 1);
      Serial.print(" Start Time Set: ");
      Serial.print(startHour[currentRoutine]);
      Serial.print(":");
      Serial.println(startMin[currentRoutine]);
      progState = SET_STOP_TIME;
      if (setStartTime) {
        progState = SET_STOP_TIME; // If we were setting the start time, move to setting the stop time
      } 
      else {
        progState = SET_START_TIME; // If we were setting the stop time, move to the next routine
        currentRoutine++;
        if (currentRoutine >= 3) {
          currentRoutine = 0;
          progSetMode = false;
          Serial.println("Program Set Mode disabled");
          // Save user routines to EEPROM on exiting program set mode
          //saveUserRoutines();
        } 
        else {
          Serial.print("Setting Routine ");
          Serial.print(currentRoutine + 1);
          Serial.println(" Start Time....");
          delay(1000);
        }
      }
    } 
    else if (progState == SET_STOP_TIME) {
      stopHour[currentRoutine] = hours;
      stopMin[currentRoutine] = Min;
      Serial.print("Routine ");
      Serial.print(currentRoutine + 1);
      Serial.print(" Stop Time Set: ");
      Serial.print(stopHour[currentRoutine]);
      Serial.print(":");
      Serial.println(stopMin[currentRoutine]);
      progState = SET_START_TIME;
      currentRoutine++;
      if (currentRoutine >= 3) {
        currentRoutine = 0;
        progSetMode = false;
        Serial.println("Program Set Mode disabled");
        // Save user routines to EEPROM on exiting program set mode
        //saveUserRoutines();
      } 
      else {
        Serial.print("Setting Routine ");
        Serial.print(currentRoutine + 1);
        Serial.println(" Start Time....");
        delay(1000);
      }
    }
    progTime = false;
  }
  //}
   
 


   
    
 } //end of else loop 
  

 
      
    // Reset button states
    if (digitalRead(buttonBPin) == HIGH) {
      lastButtonStateB = HIGH;
    }
    if (digitalRead(buttonCPin) == HIGH) {
      lastButtonStateC = HIGH;
    }

  //   if (digitalRead(buttonDPin) == HIGH) {
  //     lastButtonStateD = HIGH;
  // } 
     
   // Check if it's time to activate/deactivate the relay
  if (!progSetMode) {
     DateTime currentTime = rtc.now();
    for (int i = 0; i < 3; i++) {
      if (currentTime.hour() == startHour[i] && currentTime.minute() == startMin[i]) {
        activateRelay();
        Serial.print("Relay for Routine ");
        Serial.print(i + 1);
        Serial.println(" Activated");
      } else if (currentTime.hour() == stopHour[i] && currentTime.minute() == stopMin[i]) {
        deactivateRelay();
        Serial.print("Relay for Routine ");
        Serial.print(i + 1);
        Serial.println(" Deactivated");
      }
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


void setStartHour() {
  startHour[currentRoutine]++;
  if (startHour[currentRoutine] > 23) {
    startHour[currentRoutine] = 0;
  }
}


void setStartMin() {
  startMin[currentRoutine]++;
  if (startMin[currentRoutine] > 59) {
    startMin[currentRoutine] = 0;
  }
}

void setStopHour() {
  stopHour[currentRoutine]++;
  if (stopHour[currentRoutine] > 23) {
    stopHour[currentRoutine] = 0;
  }
}

void setStopMin() {
  stopMin[currentRoutine]++;
  if (stopMin[currentRoutine] > 59) {
    stopMin[currentRoutine] = 0;
  }
}

 

void displayTime(){
  DateTime currentTime = rtc.now();
  
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
}
 

 
void adjustProgramTime() {
  DateTime currentTime = rtc.now();
  if (progState == SET_START_TIME) {
    if (setStartTime) {
      startHour[currentRoutine] = hours;
      startMin[currentRoutine] = Min;
      Serial.print("Routine ");
      Serial.print(currentRoutine + 1);
      Serial.print(" Start Time Set: ");
      Serial.print(startHour[currentRoutine]);
      Serial.print(":");
      Serial.println(startMin[currentRoutine]);
      progState = SET_STOP_TIME;
      setStartTime = false;
    } else {
      stopHour[currentRoutine] = hours;
      stopMin[currentRoutine] = Min;
      Serial.print("Routine ");
      Serial.print(currentRoutine + 1);
      Serial.print(" Stop Time Set: ");
      Serial.print(stopHour[currentRoutine]);
      Serial.print(":");
      Serial.println(stopMin[currentRoutine]);
      progState = SET_START_TIME;
      currentRoutine++;
      if (currentRoutine >= 3) {
        currentRoutine = 0;
        progSetMode = false;
        Serial.println("Program Set Mode disabled");
      } else {
        Serial.print("Setting Routine ");
        Serial.print(currentRoutine + 1);
        Serial.println(" Start Time....");
        delay(1000);
      }
    }
  }
  progTime = false;
}

void routineTIme(){
   DateTime realTime = rtc.now();
   DateTime newStartTime = DateTime(realTime.year(), realTime.month(), realTime.day(), startHour, startMin, realTime.second());
  DateTime newStopTime = DateTime(realTime.year(), realTime.month(), realTime.day(), stopHour, stopMin, realTime.second());

  Serial.print("Start Time: ");
  Serial.print(newStartTime.year());
  Serial.print("/");
  Serial.print(newStartTime.month());
  Serial.print("/");
  Serial.print(newStartTime.day());
  Serial.print(" ");
  Serial.print(newStartTime.hour());
  Serial.print(":");
  Serial.print(newStartTime.minute());
  Serial.print(":");
  Serial.println(newStartTime.second());

  Serial.print("Stop Time: ");
  Serial.print(newStopTime.year());
  Serial.print("/");
  Serial.print(newStopTime.month());
  Serial.print("/");
  Serial.print(newStopTime.day());
  Serial.print(" ");
  Serial.print(newStopTime.hour());
  Serial.print(":");
  Serial.print(newStopTime.minute());
  Serial.print(":");
  Serial.println(newStopTime.second());

}
void adjustTimeOnRTC() {
  DateTime updatedTime = rtc.now();
  DateTime setTime =  DateTime(updatedTime.year(), updatedTime.month(), updatedTime.day(), hours, Min, updatedTime.second());  
 
  Serial.print("set Time: ");
  Serial.print(setTime.year());
  Serial.print("/");
  Serial.print(setTime.month());
  Serial.print("/");
  Serial.print(setTime.day());
  Serial.print(" ");
  Serial.print(setTime.hour());
  Serial.print(":");
  Serial.print(setTime.minute());
  Serial.print(":");
  Serial.println(setTime.second());

  updatedTime = setTime;

  rtc.adjust(updatedTime);  

//rtc.adjust(currentTime);
  // rtc.adjust(newStartTime);
  // rtc.adjust(newStopTime);

  
}


  
void activateRelay() {
  digitalWrite(relayPin1, HIGH);
  
  digitalWrite(relayPin2, HIGH);

}

void deactivateRelay() {
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
}




void modifyProgramSettings() {
  Serial.println("Program Settings:");
  
  
}


// void saveUserRoutines() {
//   for (int i = 0; i < 3; i++) {
//     EEPROM.write(i * 4, startHour[i]);
//     EEPROM.write(i * 4 + 1, startMin[i]);
//     EEPROM.write(i * 4 + 2, stopHour[i]);
//     EEPROM.write(i * 4 + 3, stopMin[i]);
//   }

// }

// void loadUserRoutines() {
//   for (int i = 0; i < 3; i++) {
//     startHour[i] = EEPROM.read(i * 4);
//     startMin[i] = EEPROM.read(i * 4 + 1);
//     stopHour[i] = EEPROM.read(i * 4 + 2);
//     stopMin[i] = EEPROM.read(i * 4 + 3);
//   }
// }
