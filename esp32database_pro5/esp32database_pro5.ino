/*adjustment section to read the sd card from using sdfat mode by adjusting the
SD card pin
 SPI_CS_PIN 5
 SPI_SCK_PIN 18
SPI_MISO_PIN 19
SPI_MOSI_PIN 23
SPI_CLOCK_IN_MHz 16*/

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

//#include <FirebaseESP32.h>
#include <WiFiClientSecure.h>
//#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define DHTTYPE DHT11 // DHT 11
uint8_t DHTPin = 4; 
DHT dht(DHTPin, DHTTYPE); 

#define SD_CS_PIN 5
String dataMessage;
#define WIFI_SSID "Jimmy"
#define WIFI_PASSWORD "kunluxzeal9"

#define API_KEY "AIzaSyBzUuE_AzxE4v6V9VYYnArO-VP4-g2FCIw"
#define DATABASE_URL "https://esp32-flutter-21c43-default-rtdb.firebaseio.com"
#define USER_EMAIL "kunluxzeal23@gmail.com"
#define USER_PASSWORD "adeoluwa"

//#define STORAGE_BUCKET_ID "Your_Storage_Bucket_ID"

#define LOG_INTERVAL 30000 // 30 sec
#define SYNC_INTERVAL 60000 // 1 min



// Define the size of the buffer that will hold the CSV file content


  // byte buffer[256]; // Create a buffer to store the file content
  // size_t bufferSize = sizeof(buffer);


FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;

size_t BUFFER_SIZE = 1024; // Define buffer size

float Temperature;
float Humidity;
String formattedDate;
String dayStamp;
String timeStamp;
String Time;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(DHTPin, INPUT);
  dht.begin();  

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
// Initialize a NTPClient to get time
timeClient.begin();
timeClient.setTimeOffset(3600);  

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

 Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);
  
  // Wait for connection
  while (!Firebase.ready()) {
    delay(500);
    Serial.println("Connecting to Firebase...");
  }
  
  Serial.println("Connected to Firebase!");


  

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
  
 File file = SD.open("/data.csv");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.csv", "Date, Time, Temperature, Humidity \r\n");
  }

   else {
    Serial.println("File already exists");  
  }
  file.close();
}

  
void loop() {


  static unsigned long logTimer = millis();
  static unsigned long lastSyncTimer = millis();


  if (millis() - logTimer >= LOG_INTERVAL) {
    //logData();
    Read_TempHum();
    getTimeStamp();
    logSDCard();  
    logTimer = millis();
  }

  if (millis() - lastSyncTimer >= SYNC_INTERVAL) {
    syncData();  
    lastSyncTimer = millis();
  }
}

// Function to get temperature
void Read_TempHum()
{
  Temperature = dht.readTemperature(); 
  Humidity = dht.readHumidity(); 
  Serial.print("Temperature = "); 
  Serial.println(Temperature);
  Serial.print("Humidity = ");
  Serial.println(Humidity);
}

//Function to get date and time from NTPClient
void getTimeStamp() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.println(timeStamp);
}

// Write the sensor readings on the SD card
void logSDCard() {
  dataMessage =  String(dayStamp) + "," + String(timeStamp) + "," +
                String(Temperature) + "," + String(Humidity)+ "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.csv", dataMessage.c_str());
}



// void syncData() {
//   // Read the file content
//   File file = SD.open("/data.csv");
//   if (!file) {
//     Serial.println("Failed to open file for reading");
//     return;
//   }

//   // Create a buffer to hold the file content
//   size_t fileSize = file.size();
//   char *buffer = new char[fileSize + 1];
//   if (!buffer) {
//     Serial.println("Failed to allocate memory for buffer");
//     file.close();
//     return;
//   }

//   // Read the file content into the buffer
//   if (file.readBytes(buffer, fileSize) != fileSize) {
//     Serial.println("Error reading file");
//     delete[] buffer;
//     file.close();
//     return;
//   }
//   buffer[fileSize] = '\0'; // Add null terminator

//   // Close the file
//   file.close();

//   // Convert CSV data to JSON format
//   DynamicJsonDocument jsonDoc(1024);
//   JsonArray jsonArray = jsonDoc.to<JsonArray>();

//   char *linePtr = strtok(buffer, "\r\n"); // Split lines by carriage return and newline

//   // Read data lines
//   linePtr = strtok(NULL, "\r\n"); // Move to the next line
  
//   while (linePtr != NULL) {
    
//     char *valuePtr = strtok(linePtr, ",");
//     JsonObject jsonObject = jsonArray.createNestedObject();

//     // Read fields: Date, Time, Temperature, Humidity
//     if (valuePtr != NULL) {
//       jsonObject["Date"] = valuePtr;
//       valuePtr = strtok(NULL, ",");
//     }
//     if (valuePtr != NULL) {
//       jsonObject["Time"] = valuePtr;
//       valuePtr = strtok(NULL, ",");
//     }
//     if (valuePtr != NULL) {
//       jsonObject["Temperature"] = valuePtr;
//       valuePtr = strtok(NULL, ",");
//     }
//     if (valuePtr != NULL) {
//       jsonObject["Humidity"] = valuePtr;
//        valuePtr = strtok(NULL, ",");
//     }

    
    
//     linePtr = strtok(NULL, "\r\n"); // Move to the next line
//   }

  
//   // Print the values for debugging
//   for (int i = 1; i < jsonArray.size(); i++) {
//     JsonObject dataObject = jsonArray[i];
//     Serial.print("Date: ");
//     Serial.println(dataObject["Date"].as<String>());
//     Serial.print("Time: ");
//     Serial.println(dataObject["Time"].as<String>());
//     Serial.print("Temperature: ");
//     Serial.println(dataObject["Temperature"].as<String>());
//     Serial.print("Humidity: ");
//     Serial.println(dataObject["Humidity"].as<String>());
//     Serial.println();
//   }
  

//   // Convert JSON data to a string
//   String jsonData;
//   serializeJson(jsonArray, jsonData); // Serialize the JsonArray instead of JsonDocument

//   // Print JSON data for debugging
//   Serial.println("JSON data:");
//   Serial.println(jsonData);

//   // Generate a unique identifier for the child node
//   String childId = String(millis()); // You can modify the unique identifier generation logic as per your requirements

//   // Create the database path with the child identifier
//   String databasePath = "/data/" + childId;

//   // Create a FirebaseJson object and parse the JSON data
//   FirebaseJson firebaseJson;
//   firebaseJson.setJsonData(jsonData);

// firebaseJson.add("Date", dayStamp);
// firebaseJson.add("Time", timeStamp);
// firebaseJson.add("Temperature", String(Temperature));
// firebaseJson.add("Humidity", String(Humidity));  

//   // Send data to Firebase
//   if (Firebase.updateNode(firebaseData, databasePath.c_str(), firebaseJson)) {
//     Serial.println("Data synced to Firebase");
//     taskCompleted = true;
//   }
//   else {
//     Serial.println("Failed to sync data to Firebase");
//     taskCompleted = false;
//     Serial.println(firebaseData.errorReason());
//   }

//   // Clean up allocated memory
//   delete[] buffer;
// }



void syncData() {
  // Read the file content
  File file = SD.open("/data.csv");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  // Create a buffer to hold the file content
  size_t fileSize = file.size();
  const size_t BUFFER_SIZE = fileSize + 1;
  char *buffer = new char[BUFFER_SIZE];
  if (!buffer) {
    Serial.println("Failed to allocate memory for buffer");
    file.close();
    return;
  }

  // Read the file content into the buffer
  if (file.readBytes(buffer, fileSize) != fileSize) {
    Serial.println("Error reading file");
    delete[] buffer;
    file.close();
    return;
  }
  buffer[fileSize] = '\0'; // Add null terminator

  // Close the file
  file.close();

  // Create a 2D array to hold the rows and columns
  const int MAX_ROWS = 100; // Maximum number of rows in the CSV file
  const int MAX_COLS = 4;  // Number of columns in the CSV file
  String csvData[MAX_ROWS][MAX_COLS];

  // Parse the CSV data and store it in the 2D array
  int rowIndex = 0;
  int colIndex = 0;
  String valuePtr = strtok(buffer, "\r\n"); // Split lines by carriage return and newline

  while (valuePtr != NULL && rowIndex < MAX_ROWS) {
    csvData[rowIndex][colIndex++] = valuePtr;

    if (colIndex >= MAX_COLS) {
      colIndex = 0;
      rowIndex++;
    }

    valuePtr = strtok(NULL, "\r\n"); // Move to the next line
  }
  
  if (valuePtr != NULL && rowIndex >= MAX_ROWS) {
  Serial.println("Max number of rows exceeded");
}
  // Clean up allocated memory
  delete[] buffer;

  // Create a JSON document
  StaticJsonDocument<512> jsonDoc;

  // Convert the CSV data to JSON format
  for (int i = 0; i < rowIndex; i++) {
    JsonObject jsonObject = jsonDoc.createNestedObject();
    jsonObject["Date"] = csvData[i][0];
    jsonObject["Time"] = csvData[i][1];
    jsonObject["Temperature"] = csvData[i][2];
    jsonObject["Humidity"] = csvData[i][3];
  }

  // Convert the JSON data to a string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  // Print the JSON data for debugging
  Serial.println("JSON data:");
  Serial.println(jsonData);

  // Generate a unique identifier for the child node
  String childId = String(millis()); // You can modify the unique identifier generation logic as per your requirements

  // Create the database path with the child identifier
  String databasePath = "/data/" + childId;

  // Create a FirebaseJson object and parse the JSON data
  FirebaseJson firebaseJson;
  firebaseJson.setJsonData(jsonData);

  // Send data to Firebase
  if (Firebase.updateNode(firebaseData, databasePath.c_str(), firebaseJson)) {
    Serial.println("Data synced to Firebase");
  } else {
    Serial.println("Failed to sync data to Firebase");
    Serial.println(firebaseData.errorReason());
  }
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
