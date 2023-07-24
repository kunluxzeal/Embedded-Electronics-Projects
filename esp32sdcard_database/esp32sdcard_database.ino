#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h> // Library for DHT11 temperature and humidity sensor
#include <SPI.h>
#include <SD.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_WIFI_PASSWORD"

/* 2. Define the Firebase credentials */
#define FIREBASE_HOST "your_project_name.firebaseio.com"
#define FIREBASE_AUTH "your_secret"

/* 3. Define the sensor pin */
#define DHT_PIN 14

// Define Firebase Data object
FirebaseData fbdo;

// Define DHT object
DHT dht(DHT_PIN, DHT11);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Initialize SD card
  if (!SD.begin())
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

void loop()
{
  // Read temperature and humidity from the DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Save temperature to Firebase
  if (Firebase.setFloat(fbdo, "/temperature", temperature))
  {
    Serial.println("Temperature saved to Firebase");
  }
  else
  {
    Serial.println("Failed to save temperature to Firebase");
  }

  // Save humidity to Firebase
  if (Firebase.setFloat(fbdo, "/humidity", humidity))
  {
    Serial.println("Humidity saved to Firebase");
  }
  else
  {
    Serial.println("Failed to save humidity to Firebase");
  }

  // Save temperature and humidity to SD card
  File file = SD.open("/data.txt", FILE_WRITE);
  if (file)
  {
    file.print(temperature);
    file.print(",");
    file.println(humidity);
    file.close();
    Serial.println("Data saved to SD card");
  }
  else
  {
    Serial.println("Failed to save data to SD card");
  }

  delay(5000); // Wait 5 seconds before reading the sensor again
}
