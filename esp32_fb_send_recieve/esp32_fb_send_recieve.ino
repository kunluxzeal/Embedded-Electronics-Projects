#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>



const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

void setup() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}


void loop() {
  // put your main code here, to run repeatedly:
sendDataToFirebase();  
receiveDataFromFirebase();
}


void sendDataToFirebase() {
  StaticJsonDocument<200> jsonDocument; // Adjust the document size according to your data
  
  // Populate the JSON document
  jsonDocument["sensor"] = "temperature";
  jsonDocument["value"] = 25.5;
  
  // Serialize the JSON document into a string
  String jsonString;
  serializeJson(jsonDocument, jsonString);
  
  // Send the JSON data to Firebase
  HTTPClient http;
  http.begin("https://your-firebase-database-url.firebaseio.com/data.json");
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending data to Firebase: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}


void receiveDataFromFirebase() {
  HTTPClient http;
  http.begin("https://your-firebase-database-url.firebaseio.com/data.json");
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    if (http.getSize() > 0) {
      String jsonResponse = http.getString();
      DynamicJsonDocument jsonDocument(1024); // Adjust the document size according to your data
      deserializeJson(jsonDocument, jsonResponse);
      
      // Extract data from the JSON document
      const char* sensor = jsonDocument["sensor"];
      float value = jsonDocument["value"];
      
      // Process the received data
      Serial.print("Sensor: ");
      Serial.println(sensor);
      Serial.print("Value: ");
      Serial.println(value);
    }
  } else {
    Serial.print("Error receiving data from Firebase: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

