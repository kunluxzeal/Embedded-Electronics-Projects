



#include<ESP8266WiFi.h>



 
const char* ssid = "jimmy";
const char* password = "kunluxzeal";
 
//int ledPin = 13; // GPIO13---D7 of NodeMCU
WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(10);
 
//  pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
// Serial.println(ssid,password);
 
  WiFi.begin(ssid,password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}

void loop() {
  // put your main code here, to run repeatedly:

}
