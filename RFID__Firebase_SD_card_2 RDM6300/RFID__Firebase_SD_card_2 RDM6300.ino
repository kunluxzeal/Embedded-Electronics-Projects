
/*
* SD CARD PIN*
* CS GPIO 5
* MOSI  GPIO 23
* CLK GPIO 18
* MISO GPIO 19
 // RFID 
 * SS_PIN 17
*  RST_PIN 27
 
*/ 


#include <ESP8266WiFi.h>
#include <SPI.h> 
#include <rdm6300.h>
#include "FirebaseESP8266.h"  //

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SD.h>


#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16

//#define SD_CS_PIN 5 //chip select for SD card

#define buzzerPin 15

#define LOG_INTERVAL 30000 // 30 sec
#define SYNC_INTERVAL 60000 // 1 min
String dataMessage = "";

#define FIREBASE_HOST "esp32-flutter-21c43-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "wlowgoZO5qzse2oU67Bp5cY0DaGp6wc7Y8o3d5Hs"

Rdm6300 rdm6300;
        
//unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

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
long tag;


int swipeCount =0 ; // Variable to track the number of card swipes

void connect() {
  Serial1.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial1.print(".");
    delay(1000);
  }

  Serial1.println("\n connected!");
}


void setup()
{

/* Serial1 is the debug! remember to bridge GPIO-01 to GPIO-02 */
	Serial1.begin(115200);

	pinMode(READ_LED_PIN, OUTPUT);
	digitalWrite(READ_LED_PIN, LOW);

	rdm6300.begin(RDM6300_RX_PIN);

     
  pinMode(buzzerPin , OUTPUT);
    
  WiFi.begin(ssid, pass);

  SPI.begin();
  
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  connect();

	Serial1.println("\nPlace RFID tag near the rdm6300...");   

  //SD_CARD();

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}


// Write the sensor readings on the SD card
// void logSDCard(String temp) {
    
//   dataMessage =  String(timeClient.getFormattedTime()) + "," + String(temp) + "," +
//                 String(device_id) + "," + String(swipeCount)+ "\r\n";
//   Serial.print("Save data: ");
//   Serial.println(dataMessage);
//   appendFile(SD, "/datalog.csv", dataMessage.c_str());
// }

void checkAccess (String temp)    //Function to check if an identified tag is registered to allow access
{
      
   

    if(Firebase.getInt(firebaseData, uidPath+"/users/"+temp)){

      swipeCount = firebaseData.intData();
      
      if (swipeCount > 0)         
      {  
          swipeCount--;
        Access_buzzer_sound(1); // Sound the buzzer twice with a 500ms tone duration and 200ms pause between tones
          //delay(2000); // Wait for 2 seconds before repeating the buzzer sound
        

          alertMsg="CHECKING IN";
         
          Serial1.println(alertMsg);
          delay(1000);

           Serial1.print("ACCESS LEFT: ");
           Serial1.println(swipeCount);

          json.add("time", String(timeClient.getFormattedTime()));
          json.add("id", device_id);
          json.add("uid", temp);
          json.add("Access",swipeCount);
          
          if (Firebase.pushJSON(firebaseData, uidPath+ "/attendence", json)) {
            Serial1.println(firebaseData.dataPath() + firebaseData.pushName()); 
          } else {
            Serial1.println(firebaseData.errorReason());
          }

         

          //logSDCard(temp); 
          
          Firebase.setInt(firebaseData, uidPath + "/users/" + temp, swipeCount);
          
      }    

         else{

           Access_denied_sound(7); // Sound the buzzer twice with a 500ms tone duration and 200ms pause between tones
          //delay(2000); // Wait for 2 seconds before repeating the buzzer sound  
            
            alertMsg="ACCESS DENIED";
         
          Serial1.println(alertMsg);
          delay(1000);

                  

          Firebase.setInt(firebaseData, uidPath+"/users/"+temp,0); 
          //Serial.println("SWIPE YOUR CARD");

        }
    
 
    }
    else
    {
      Serial1.print("FAILED");
      Serial1.println("REASON: " + firebaseData.errorReason());
    }
}


void find_rfid(){

 /* get_new_tag_id returns the tag_id of a "new" near tag,
	following calls will return 0 as long as the same tag is kept near. */
	if (rdm6300.get_new_tag_id()){
    tag = rdm6300.get_tag_id();
    Serial1.println(tag);

    //checkAccess (String temp);
    
  }
		

	/* get_tag_id returns the tag_id as long as it is near, 0 otherwise. */
	digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());

	delay(10);
 

  
}
void loop() {


  static unsigned long logTimer = millis();
  static unsigned long lastSyncTimer = millis(); 

  //timeClient.update();
  find_rfid();
  

  // if(millis()- lastSyncTimer >= SYNC_INTERVAL){
    
  //   lastSyncTimer = millis()               
  // }
}


// void SD_CARD(){
  
//  // Initialize SD card
//   SD.begin(SD_CS_PIN);  
//   if(!SD.begin(SD_CS_PIN)) {
//     Serial.println("Card Mount Failed");
//     return;
//   }
//   uint8_t cardType = SD.cardType();
//   if(cardType == CARD_NONE) {
//     Serial.println("No SD card attached");
//     return;
//   }

//  Serial.println("Initializing SD card...");
//   if (!SD.begin(SD_CS_PIN)) {
//     Serial.println("ERROR - SD card initialization failed!");
//     return;    // init failed
//   }
//   Serial.println("SD card initialization successful");
  
//  File file = SD.open("/datalog.csv");
//   if(!file) {
//     Serial.println("File doens't exist");
//     Serial.println("Creating file...");
//     writeFile(SD, "/datalog.csv", "TIME, CARD_NO, DEVICE, ACCESS_LEFT \r\n");
//   }

//    else {
//     Serial.println("File already exists");  
//   }
//   file.close();


// }


// void writeFile(fs::FS &fs, const char * path, const char * message) {
//   Serial.printf("Writing file: %s\n", path);

//   File file = fs.open(path, FILE_APPEND);
//   if (!file) {
//     Serial.println("Failed to open file for writing");
//     return;
//   }
//   if (file.print(message)) {
//     Serial.println("File written");
//   } else {
//     Serial.println("Write failed");
//   }
//   file.close();
// }





// void appendFile(fs::FS &fs, const char * path, const char * message) {
//   Serial.printf("Appending to file: %s\n", path);
//   File file = fs.open(path, FILE_APPEND);
//   if(!file) {
//     Serial.println("Failed to open file for appending");
//     return;
//   }
//   if(file.print(message)) {
//     Serial.println("Message appended");
//   } else {
//     Serial.println("Append failed");
//   }
//   file.close();
// }





void Access_buzzer_sound(int numTimes) {
  for(int i = 0 ; i <numTimes; i++){
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  delay(1000);    
  }
}


void Access_denied_sound(int numTimes) {
  for(int i = 0 ; i <numTimes; i++){
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);
  delay(500);    
  }
}


