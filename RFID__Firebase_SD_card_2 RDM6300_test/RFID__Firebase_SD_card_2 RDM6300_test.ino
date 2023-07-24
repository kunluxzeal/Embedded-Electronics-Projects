
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


#include <SPI.h> 
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <rdm6300.h>
#include "FirebaseESP8266.h"  //
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16

//#define SD_CS_PIN 5 //chip select for SD card
#define buzzerPin 12

#define LOG_INTERVAL 30000 // 30 sec
#define SYNC_INTERVAL 60000 // 1 min
String dataMessage = "";

#define FIREBASE_HOST "esp32-flutter-21c43-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "wlowgoZO5qzse2oU67Bp5cY0DaGp6wc7Y8o3d5Hs"

Rdm6300 rdm6300;
      
const char ssid[] = "Jimmy";
const char pass[] = "kunluxzeal99";

String uidPath= "/";
FirebaseJson json;
//Define FirebaseESP32 data object
FirebaseData firebaseData;

unsigned long lastMillis = 0;
String alertMsg;
String device_id="device1";
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

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  //base_display();
  home_display();
  delay(3000);
  //display_data_head();
  swipe_your_card();
  //access_granted();
  delay(2000);
  //access_left();
	pinMode(READ_LED_PIN, OUTPUT);
	digitalWrite(READ_LED_PIN, LOW);   
  pinMode(buzzerPin , OUTPUT);
  WiFi.begin(ssid, pass);
  //SPI.begin();
  connect();
  rdm6300.begin(RDM6300_RX_PIN);
	Serial1.println("\nPlace RFID tag near the rdm6300...");
  swipe_your_card();  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true); 
}



void home_display(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25, 15); //(col, row)
  display.println("JOURNEY");
  display.setCursor(30, 42);
  display.println("ACCESS");
  display.display(); 
}
// (col, row)
void display_data_head(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 15);
  display.println("D1:");
  display.setCursor(40,15);
  display.println("Locked");
  display.setCursor(0,48);
  display.println("Access:");
  display.setCursor(100,48);
  display.println("5");
  display.display(); 

}
void swipe_your_card(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 7);
  display.println("Swipe your");
  display.setCursor(10,30);
  display.println("Card here");
  display.setCursor(50,48);
  display.println(">>>");
  display.display(); 
}

void access_granted(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("Access");
  display.setCursor(25,38);
  display.println("granted");
  display.display(); 
}

void access_left(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 7);
  display.println("Access");
  display.setCursor(33,30);
  display.println("Left:");
  display.setCursor(55,48);
  display.println(swipeCount);
  display.display(); 
}

void checkAccess (long tag)    //Function to check if an identified tag is registered to allow access
{
    
    if(Firebase.getInt(firebaseData, uidPath+"/users/"+tag)){
     swipeCount = firebaseData.intData();
           
      if (swipeCount > 0)         
      {    
        Access_buzzer_sound(1); 
        alertMsg="CHECKING IN";
        Serial1.println(alertMsg);
        delay(1000);
        access_granted();
        swipeCount--;
        Serial1.print("ACCESS LEFT: ");
        Serial1.println(swipeCount);
        access_left();        
        Firebase.setInt(firebaseData, uidPath + "/users/" + tag, swipeCount);   
      }   

      else if(!(Firebase.getInt(firebaseData, uidPath+"/users/"+tag))){
        Serial1.print("Card Not registered");
    
  }  
      else{
        Access_denied_sound(7); // Sound the buzzer twice with a 500ms tone duration and 200ms pause between tones
        //delay(2000); // Wait for 2 seconds before repeating the buzzer sound  
        alertMsg="ACCESS DENIED";
        Serial1.println(alertMsg);
        delay(1000);
        
        Firebase.setInt(firebaseData, uidPath+"/users/"+tag,0); 
        Serial1.println("SWIPE YOUR CARD");
        swipe_your_card();      

        }
    
    }
    else
    {
      Serial1.print("FAILED");
      Serial1.println("REASON: " + firebaseData.errorReason());
    }
}




void loop() {
 //Serial1.println("welcome");
  find_rfid();
  static unsigned long logTimer = millis();
  static unsigned long lastSyncTimer = millis(); 
}

void find_rfid(){

 /* get_new_tag_id returns the tag_id of a "new" near tag,
	following calls will return 0 as long as the same tag is kept near. */
	if (rdm6300.get_new_tag_id()){
    tag = rdm6300.get_tag_id();
       
    Serial1.println(tag);
     
    checkAccess (tag);       
  }
		
	/* get_tag_id returns the tag_id as long as it is near, 0 otherwise. */
	digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
	delay(10);  
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

// void fb_error_reason(){
//     String error_status = firebaseData.errorReason()
        
//     if (errror_status == "")  {
//       Serial1.println(error_status);      
//     }
// }