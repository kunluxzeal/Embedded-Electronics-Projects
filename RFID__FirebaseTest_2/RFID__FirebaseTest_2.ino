/*SD card pin
 SPI_CS_PIN 5
 SPI_SCK_PIN 18
SPI_MISO_PIN 19
SPI_MOSI_PIN 23
SPI_CLOCK_IN_MHz 16*/

#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h> 
#include <RFID.h>
#include "FirebaseESP32.h"  // Install Firebase ESP8266 library

#define SS_PIN 17 //chip select for RFID
#define RST_PIN 27

#define SD_CS_PIN 5 //chip select for SD card

#define FIREBASE_HOST "esp32-flutter-21c43-default-rtdb.firebaseio.com" //Without http:// or https:// schemes

#define FIREBASE_AUTH "wlowgoZO5qzse2oU67Bp5cY0DaGp6wc7Y8o3d5Hs"
RFID rfid(SS_PIN, RST_PIN);       //D8:pin of tag reader SDA. D0:pin of tag reader RST 
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

const char ssid[] = "Jimmy";
const char pass[] = "kunluxzeal99";

// int user_access = 0;
// String msg = "please enter user acess time";

int swipeCount = 5; // Variable to track the number of card swipes


String uidPath= "/";
//Define FirebaseESP8266 data object
FirebaseData firebaseData;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n connected!");
}

void setup()
{

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  SPI.begin();
  rfid.init();
  
  connect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}
void pushUser (String temp)    //Function to check if an identified tag is registered to allow access
{ 
   
   Serial.println("PUSHING USER ID: "+temp);

    Firebase.setInt(firebaseData, uidPath+"users/"+temp,swipeCount);
}
void loop() {

    
      
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  { 
    Serial.println("Card found"); 
    String temp = "";                             //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read tag serial number 
    { 
      Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                 //Record and display the tag serial number 
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      Serial.println (temp);
      pushUser (temp);     //Check if the identified tag is an allowed to open tag     
    } 
       
      rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();
}



