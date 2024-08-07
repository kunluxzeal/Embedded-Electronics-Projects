#include <WiFi.h>
#include <FirebaseESP32.h>
#include <WiFiClientSecure.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

#define SD_CS_PIN 5

#define WIFI_SSID "Jimmy"
#define WIFI_PASSWORD "kunluxzeal9"

#define API_KEY "AIzaSyBzUuE_AzxE4v6V9VYYnArO-VP4-g2FCIw"
#define DATABASE_URL "https://esp32-flutter-21c43-default-rtdb.firebaseio.com"
#define USER_EMAIL "kunluxzeal23@gmail.com"
#define USER_PASSWORD "adeoluwa"

//#define STORAGE_BUCKET_ID "Your_Storage_Bucket_ID"

#define LOG_INTERVAL 30000 // 30 sec
#define SYNC_INTERVAL 60000 // 1 min

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

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

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed");
    return;
  }
  Serial.println("SD card initialization successful");
}

void loop() {
  static unsigned long logTimer = millis();
  static unsigned long syncTimer = millis();


  byte buffer[256]; // Create a buffer to store the file content
  size_t bufferSize = sizeof(buffer);

  if (millis() - logTimer >= LOG_INTERVAL) {
    logData();
    logTimer = millis();
  }

  if (millis() - syncTimer >= SYNC_INTERVAL) {
    
    // Read the file content
    readFile(SD, "/data.txt", buffer, bufferSize);
    readFile(SD, "/mycsvdata.csv", buffer, bufferSize);

// Convert the buffer to a string
    String data = String((char*)buffer);    
    // Sync the data to Firebase  
    syncData(data);  
    syncTimer = millis();
  }
}

void logData() {
  writeFile(SD, "/data.txt", "Today is Thursday");
 appendCsv(SD, "/mycsvdata.csv", "2024-04-27,12,56");  
}

void syncData(String data) {
  // Convert buffer to string
  
 if (Firebase.setString(firebaseData, "/mydata", data)) {
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

void appendCsv(fs::FS &fs, const char * path, const char * newData) {
  Serial.printf("Appending CSV to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Write a comma-separated line of data, followed by a newline character
  file.printf("%s,\n", newData);

  if (file.print(newData)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }

  file.close();

  Serial.println("CSV appended");
}

void readFile(fs::FS &fs, const char * path, byte* buffer, size_t bufferSize) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Read from file: ");
  size_t bytesRead = 0;
  
  while (file.available() && bytesRead < bufferSize) {
    buffer[bytesRead] = file.read();
    Serial.print((char)buffer[bytesRead]); // Print the current character being read
    bytesRead++;
        
 }
  file.close();
}


