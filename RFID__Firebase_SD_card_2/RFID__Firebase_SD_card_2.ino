
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

#define buzzerPin 15

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
//boolean checkIn = true;


int swipeCount =0 ; // Variable to track the number of card swipes

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
  SD_CARD();

  Serial.println("SWIPE YOUR CARD"); 

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}


// Write the sensor readings on the SD card
void logSDCard(String temp) {
    
  dataMessage =  String(timeClient.getFormattedTime()) + "," + String(temp) + "," +
                String(device_id) + "," + String(swipeCount)+ "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/datalog.csv", dataMessage.c_str());
}

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
         
          Serial.println(alertMsg);
          delay(1000);

           Serial.print("ACCESS LEFT: ");
           Serial.println(swipeCount);

          json.add("time", String(timeClient.getFormattedTime()));
          json.add("id", device_id);
          json.add("uid", temp);
          json.add("Access",swipeCount);
          
          if (Firebase.pushJSON(firebaseData, uidPath+ "/attendence", json)) {
            Serial.println(firebaseData.dataPath() + firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }

         

          logSDCard(temp); 
          
          Firebase.setInt(firebaseData, uidPath + "/users/" + temp, swipeCount);
          
      }    

         else{

           Access_denied_sound(7); // Sound the buzzer twice with a 500ms tone duration and 200ms pause between tones
          //delay(2000); // Wait for 2 seconds before repeating the buzzer sound  
            
            alertMsg="ACCESS DENIED";
         
          Serial.println(alertMsg);
          delay(1000);

                  

          Firebase.setInt(firebaseData, uidPath+"/users/"+temp,0); 
          //Serial.println("SWIPE YOUR CARD");

        }
    
 
    }
    else
    {
      Serial.print("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
    }
}


void find_rfid(){

 
 

if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  { 
    Serial.println("CARD FOUND");      
         
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
       
 
      
    } 
  
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually

    Serial.println("SWIPE YOUR CARD");
  }

  
 
  rfid.halt();

  
}
void loop() {


  static unsigned long logTimer = millis();
  static unsigned long lastSyncTimer = millis(); 

  timeClient.update();
  find_rfid();
  

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
    writeFile(SD, "/datalog.csv", "TIME, CARD_NO, DEVICE, ACCESS_LEFT \r\n");
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


