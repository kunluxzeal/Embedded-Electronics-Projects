
#include <WiFi.h>
#include <SPI.h> 
#include <RFID.h>
#include "FirebaseESP32.h"  //
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SD.h>

#define SS_PIN 17 //GPIO27  for RFID 
#define RST_PIN 27

#define SD_CS_PIN 5 //chip select for SD card

#define buzzerPin 15

#define LOG_INTERVAL 30000 // 30 sec
#define SYNC_INTERVAL 60000 // 1 min
String dataMessage = "";
String token ;
String scannedToken = "488964025";

#define FIREBASE_HOST "esp32-flutter-21c43-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "wlowgoZO5qzse2oU67Bp5cY0DaGp6wc7Y8o3d5Hs"
RFID rfid(SS_PIN, RST_PIN);        
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 3600; //(UTC+1)
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char ssid[] = "Jimmy";
const char pass[] = "kunluxzeal99";

String uidPath= "/";



FirebaseJson json;
//Define FirebaseESP32 data object
FirebaseData firebaseData;

unsigned long lastMillis = 0;

String alertMsg;
String device_id="device1";
//boolean checkIn = true;


int tokenCount ; // Variable to track the number of card swipes

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
     
  pinMode(buzzerPin , OUTPUT);
  Serial.begin(115200);
    
  WiFi.begin(ssid, pass);

  SPI.begin();
  rfid.init();
  
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  connect();
  //SD_CARD();

  Serial.println("SWIPE YOUR CARD"); 

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}


void checkAccess ()    //Function to check if an identified tag is registered to allow access
{
      
    if(Firebase.getInt(firebaseData, uidPath+"/Tokens/"+scannedToken)){

      tokenCount = firebaseData.intData();
      
      if (tokenCount > 0)         
      {  
          tokenCount--;
        
          alertMsg="CHECKING IN";
         
          Serial.println(alertMsg);
          delay(1000);

           Serial.print("ACCESS LEFT: ");
           Serial.println(tokenCount);

          Firebase.setInt(firebaseData, uidPath + "/Tokens/" + scannedToken ,tokenCount);
          
      }    

         else{
          
            alertMsg="ACCESS DENIED";
         
          Serial.println(alertMsg);
          delay(1000);
          //Firebase.setInt(firebaseData, uidPath+"/Tokens/"+token,0); 
          //Serial.println("SWIPE YOUR CARD");

        }
   
    }
    else
    {
      Serial.print("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
    }
}


void loop() {


  static unsigned long logTimer = millis();
  static unsigned long lastSyncTimer = millis(); 

  timeClient.update();
 
   if(millis()- lastSyncTimer >= SYNC_INTERVAL){

    checkAccess () ;
    
    lastSyncTimer = millis() ;              
  }
}
