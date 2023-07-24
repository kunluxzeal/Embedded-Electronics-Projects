#include <MD_TCS230.h>
#include <Wire.h>

#define pinOE 8   // Defines pins for module connections.
#define pinS2 6
#define pinS3 7

MD_TCS230  CS(pinS2, pinS3, pinOE);   // Creates TCS230 library instance.

byte rgbArray[256];   // Array we'll use for storing and displaying RGB values.
int red, green, blue; // Red, green and blue integer values for printing.

void setup()
{
  Serial.begin(9600);   // Starts serial connection at 9600 baud rate.
  CS.begin();           // Starts color scanning process.

  Serial.println("Color scanning process initiated.");

  if (CS.begin())       // Informs user about setup status.
  {
    Serial.println("Color sensor setup successful.");
  }
  else
  {
    Serial.println("Color sensor setup failed, check your connections.");
  }


  for (int i = 0; i < 256; i++) {   // Converts data from sensor to RGB values.
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if {
      rgbArray[i] = x;
    }
  }
}


void loop()
{

  CS.getRGB(&rgb);    // Library function that gets RGB values from reading.

  Serial.print("RGB values are: ");   // Prints RGB values to the serial monitor.
  Serial.print("R:");
  Serial.print(rgb.value[TCS230_RGB_R]);
  
  Serial.print("G:");
  Serial.print(rgb.value[TCS230_RGB_G]);
  
  Serial.println("B:");
  Serial.print(rgb.value[TCS230_RGB_B]);

  delay(100);  // Delay command to ensure color scanning process and calculations are done.

}
