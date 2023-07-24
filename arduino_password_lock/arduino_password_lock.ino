*
   Password Lock with TM1637

   You can set a number of predefined codes that can be used for further action.
   using the keypad. the input is displayed on a 4-digit LED display.
   If the user inputs matches one of the known codes, a relay module is turned on
   or off accordingly.

   Demonstrates:
   - Keypad input
   - TM1637 4-digitdeb LED display
   - Relay module
*/

// Libraries used
// 4-digit 7-segment LED Display library, download from
// https://github.com/avishorp/TM1637
#include "TM1637Display.h"
// Keypad library
#include "Keypad.h"

// CONSTANTS
// Define the characters on the keypad layout
const char keys[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Row pins
const byte keypadRowPins[4] = {7, 8, 9, 10};
// Column pins
const byte keypadColPins[3] = {11, 12, 13};
// Pins which the relay modules are connected to
const byte relayPins[] = {3};
// Clock pin for the display
const byte displayClockPin = A0;
// Data pin for the display
const byte displayDataPin = A1;

// GLOBALS
// Create a keypad input class from definitions above
Keypad keypad = Keypad( makeKeymap(keys), keypadRowPins, keypadColPins, 4, 3 );
// Create a display object, specifying pin parameters (Clock pin, Data pin)
TM1637Display display(displayClockPin, displayDataPin);
// Record the code which the user has entered
char data[] = "    ";
// What position through the code is being entered?
int sequenceNum = 0;


// Updates the display to show the current code entered by the user
void updateDisplay() {
  // Initialise a new blank display array
  uint8_t displayData[] = { 0, 0, 0, 0 };
  // Loop over each character in the code
  for (int i = 0; i < (sizeof(data) / sizeof(data[0])); i++) { 
  // Digits can be displayed directly using the encodeDigit() function
  if (data[i] >= '0' && data[i] <= '9') {
      displayData[i] = display.encodeDigit((int)data[i] - 48);
    }
    // For the * character, display a single line in the middle 
    // of the digit display
    else if (data[i] == '*') {
      displayData[i] = 0b01000000;
    }
    // For the # character, display parallel lines 
    //at the top and bottom of the digit
    else if (data[i] == '#') {
      displayData[i] = 0b00001001;
    }
  }
  // Pass the data array to the display
  display.setSegments(displayData);
}

// Flash the current value of the display on and off
void flashDisplay() {
  // Define an "empty" array (i.e. all segments off)
  uint8_t OFF[] = { 0, 0, 0, 0 };
  // Toggle between the current value and the empty value
  for (int i = 0; i < 4; i++) {
    delay(250);
    display.setSegments(OFF);
    delay(250);
    updateDisplay();
  }
}

// Initial setup
void setup() {
  // Initialise serial communication

  Serial.begin(9600);

  // Set brightness
  display.setBrightness(4);

  // Initialise relay pins
  for (int i = 0; i < 1; i++) {
    digitalWrite(relayPins[i], LOW);
    pinMode(relayPins[i], OUTPUT);
  }
}

// Main Program Loop
void loop() {

  // Get the keypad input this frame
  char key = keypad.getKey();

  // Has a key been pressed?
  if (key) {


    // Log it to the serial output
    Serial.println(key);


    // Set the current position of the code sequence to the key pressed
    data[sequenceNum] = key;
    // Increment the counter
    sequenceNum++;

    // Update the display to reflect the current sequence
    updateDisplay();

    // If the player has entered all 4 digits of a code
    if (sequenceNum == 4) {


      Serial.print(F("Code entered: "));
      // Log the whole code to the serial output
      Serial.println(data);

      // Take action based on the code entered
      if (strcmp(data, "1000") == 0) {
        digitalWrite(relayPins[0], HIGH);
        delay(2000);
      }
      else if (strcmp(data, "1001") == 0) {
        digitalWrite(relayPins[0], LOW);
        delay(2000);
      }

      // If none of the conditions above have matched, it's an unknown code
      else {
        // FLash the display
        flashDisplay();
      }

      // Clear the data array
      memset(data, 0, sizeof(data));
      sequenceNum = 0;

      // Update the display
      updateDisplay();
    }
  }
}
