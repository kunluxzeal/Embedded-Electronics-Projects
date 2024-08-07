// orignal file is from https://roboindia.com/tutorials/i2c-address-scanner-nodemcu/
// included in the project to provide flexiblity of having all tools in one place
// all credits go to orignal author

// Robo India Tutorial 
// Digital Input and Output on LED 
// Hardware: NodeMCU
// Ref- Nick Gammon  http://www.gammon.com.au/forum/?id=10896

// I2C Scanner

// for oled display connect (SCL, SDA) (D1, D2) respictively 
// and check if device is found

#include <Wire.h>

void setup() {
  Serial.begin (9600); 
  while (!Serial) // Waiting for serial connection
    {
    }

  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}  // end of setup

void loop() {}