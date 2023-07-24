
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

#define Buzzer 15

#define LOG_INTERVAL 30000 // 30 sec
#define SYNC_INTERVAL 60000 // 1 min
String dataMessage = "";

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
boolean checkIn = true;

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

  pinMode(Buzzer , OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  SPI.begin();
  rfid.init();
  
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  connect();
  SD_CARD();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}


// Write the sensor readings on the SD card
void logSDCard(String temp) {
    
  dataMessage =  String(timeClient.getFormattedTime()) + "," + String(temp) + "," +
                String(device_id) + "," + String("0")+ "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/datalog.csv", dataMessage.c_str());
}

void checkAccess (String temp)    //Function to check if an identified tag is registered to allow access
{
      
    Serial.println("SCAN YOUR RFID");

    if(Firebase.getInt(firebaseData, uidPath+"/users/"+temp)){
      
      if (firebaseData.intData() == 0)         //If firebaseData.intData() == checkIn
      {  
          alertMsg="CHECKING IN";
         
          Serial.print(alertMsg);
          delay(1000);

          json.add("time", String(timeClient.getFormattedTime()));
          json.add("id", device_id);
          json.add("uid", temp);
          json.add("status",1);

          Firebase.setInt(firebaseData, uidPath+"/users/"+temp,1);
          
          if (Firebase.pushJSON(firebaseData, uidPath+ "/attendence", json)) {
            Serial.println(firebaseData.dataPath() + firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }
      }
      else if (firebaseData.intData() == 1)   //If the lock is open then close it
      { 
          alertMsg="CHECKING OUT";
         
          Serial.print(alertMsg);
          delay(1000);

          Firebase.setInt(firebaseData, uidPath+"/users/"+temp,0);
          
          json.add("time", String(timeClient.getFormattedTime()));
          json.add("id", device_id);
          json.add("uid", temp);
          json.add("status",0);
          if (Firebase.pushJSON(firebaseData, uidPath+ "/attendence", json)) {
            Serial.println(firebaseData.dataPath() + firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }
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
      checkAccess (temp);     //Check if the identified tag is an allowed to open tag
      logSDCard(temp);       
      
    } 

    
  // if (millis() - logTimer >= LOG_INTERVAL) {
  
  //   logSDCard(temp);  
  //   logTimer = millis();
    
  // }    
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();

  

   
    


  // if(millis()- lastSyncTimer >= SYNC_INTERVAL){
    
  //   lastSyncTimer = millis()               
  // }
}


void SD_CARD(){
  
 // Initialize SD card
  SD.begin(SD_CS_PIN);  
  if(!SD.begin(SD_CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

 Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  Serial.println("SD card initialization successful");
  
 File file = SD.open("/datalog.csv");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/datalog.csv", "ID, Status, Time, UID \r\n");
  }

   else {
    Serial.println("File already exists");  
  }
  file.close();


}


void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}





void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
