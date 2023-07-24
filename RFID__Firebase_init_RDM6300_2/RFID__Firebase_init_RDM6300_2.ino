


#include <ESP8266WiFi.h>
#include <rdm6300.h>
#include <SPI.h> 
#include "FirebaseESP8266.h"  

#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16


#define FIREBASE_HOST "esp32-flutter-21c43-default-rtdb.firebaseio.com" //Without http:// or https:// schemes

#define FIREBASE_AUTH "wlowgoZO5qzse2oU67Bp5cY0DaGp6wc7Y8o3d5Hs"

Rdm6300 rdm6300;

//unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 
long tag ;
const char ssid[] = "Jimmy";
const char pass[] = "kunluxzeal99";

int accessCount = 5;

String uidPath= "/";
//Define FirebaseESP8266 data object
FirebaseData firebaseData;

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

  Serial1.begin(115200);
  WiFi.begin(ssid, pass);
  rdm6300.begin(RDM6300_RX_PIN);
  connect();

  Serial1.println("\nPlace RFID tag near the rdm6300 for registration...");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
void pushUser (long tag)    //Function to check if an identified tag is registered to allow access
{ 
   Serial.println("PUSHING USER ID: "+tag);

    Firebase.setInt(firebaseData, uidPath+"users/"+tag,accessCount);
}
void loop() {
  
find_rfid();

}



void find_rfid(){

 /* get_new_tag_id returns the tag_id of a "new" near tag,
	following calls will return 0 as long as the same tag is kept near. */
	if (rdm6300.get_new_tag_id()){
    tag = rdm6300.get_tag_id();
    Serial1.println(tag);
    pushUser (tag);     //Check if the identified tag is an allowed to open tag
    //checkAccess (String temp);
    
  }
		

	/* get_tag_id returns the tag_id as long as it is near, 0 otherwise. */
	digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());

	delay(10);
 

  
}

