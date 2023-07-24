#include <TinyWireM.h>  // Include the TinyWireM library for I2C communication
#include <RTClib.h>     // Include the RTClib library for real-time clock functionality

RTC_DS1307 rtc;          // Create an instance of the RTC_DS1307 class

const int buttonAPin = 0;  // Button A pin connected to ATtiny85 pin PB0
const int buttonBPin = 1;  // Button B pin connected to ATtiny85 pin PB1
const int buttonCPin = 2;  // Button C pin connected to ATtiny85 pin PB2
const int buttonDPin = 3;  // Button D pin connected to ATtiny85 pin PB3

volatile bool clockSetMode = false;   // Flag to track clock set mode
volatile bool programSetMode = false; // Flag to track program set mode
volatile int hours = 0;               // Variable to store the hour value

void setup() {
  pinMode(buttonAPin, INPUT_PULLUP);  // Set button A pin as input with internal pull-up resistor
  pinMode(buttonBPin, INPUT_PULLUP);  // Set button B pin as input with internal pull-up resistor
  pinMode(buttonCPin, INPUT_PULLUP);  // Set button C pin as input with internal pull-up resistor
  pinMode(buttonDPin, INPUT_PULLUP);  // Set button D pin as input with internal pull-up resistor
  TinyWireM.begin();                   // Initialize I2C communication
  rtc.begin();                         // Initialize the RTC
  attachInterrupt(digitalPinToInterrupt(buttonAPin), clockSetInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonBPin), programSetInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonCPin), hourIncreaseInterrupt, FALLING);
}

void loop() {
  // Check if it's morning or evening to control the relay
  DateTime now = rtc.now();
  if (isMorning(now)) {
    activateRelay();
  } else if (isEvening(now)) {
    deactivateRelay();
  }

  // Handle other tasks in the loop
  // ...
}

void clockSetInterrupt() {
  // Toggle clock set mode on button A press
  clockSetMode = !clockSetMode;
}

void programSetInterrupt() {
  // Toggle program set mode on button B press
  programSetMode = !programSetMode;
}

void hourIncreaseInterrupt() {
  // Increase the hour value on button C press
  if (programSetMode) {
    hours++;
    if (hours > 23) {
      hours = 0;
    }
  }
}

void activateRelay() {
  // Relay activation code
  // ...
}

void deactivateRelay() {
  // Relay deactivation code
  // ...
}

bool isMorning(DateTime time) {
  // Check if it's morning based on predefined time range
  return (time.hour() >= 6 && time.hour() < 12);
}

bool isEvening(DateTime time) {
  // Check if it's evening based on predefined time range
  return (time.hour() >= 18 && time.hour() < 22);
}
