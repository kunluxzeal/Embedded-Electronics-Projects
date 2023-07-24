#include <Wire.h> 
// make sure esp8266 is installed (in boards)

#include "SH1106Wire.h"

SH1106Wire display(0x3c, D2, D1);
// (d2-- sda) (d1--scl)

void setup() { 
  Serial.begin(9600);

  display.init(); // initalise the display 
  display.flipScreenVertically(); // flip display vertically, i like it vertically so...
  display.setFont(ArialMT_Plain_16); // set the font and size you can change 16 to what ever size you want
  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  // sets the alignment to left( you can use center, right also)

}

void loop() {
  
  /*As there is no function to display Integer
  we convert interger to string for display */

  // logic to convert int to string
  
  char str[20];
  //create an empty string to store number
  
  int number = 11220910; 
  
  sprintf(str, "%d", number);
  // here value of integer is converted to string and saved in str

  display.clear(); // clear display 
  
  display.drawString(0,0, "Hello! : "); 
  // there are 128*64 (1.3") pixels, (0,0) means Hello! will display from 0,0 position
  
  display.drawString(0,20, str); // display integer value
  // here 0,20 shows that this word will be 20p below first word
  
  display.display(); 
  // display the above strings
}