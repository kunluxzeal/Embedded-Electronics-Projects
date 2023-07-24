
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

//#include <Firebase_ESP_Client.h>
#define SD_CS_PIN 5

//* 1. Define the WiFi credentials */
#define WIFI_SSID "Jimmy"
#define WIFI_PASSWORD "kunluxzeal9"



/* 2. Define the API Key */
#define API_KEY "AIzaSyBzUuE_AzxE4v6V9VYYnArO-VP4-g2FCIw"

/* 3. Define the RTDB URL */
#define DATABASE_URL "esp32-flutter-21c43-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "kunluxzeal23@gmail.com"
#define USER_PASSWORD "adeoluwa"

// Enter Firebase storage bucket ID
#define STORAGE_BUCKET_ID "WRITE_YOUR_BUCKET_ID"

//set data logging interval in milliseconds
#define LOG_INTERVAL 60000 // 1 min

//set firebase synchronization in milliseconds
#define SYNC_INTERVAL 600000 // 10 min

//define firebase data objects
FirebaseData firebaseData;


FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

//define SD card objects
File dataFile;

//define data logging timer and synchronization timer

unsigned long logTimer = 0;
unsigned long syncTimer = 0;



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

   Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
 // config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);
   // Set maximum size of the Firebase HTTP response buffer
  //Firebase.setMaxResponseSize(1000);
  // Initialize SD card
  if (!SD.begin(SD_CS_PIN))
  {
    Serial.println("SD card initialization failed");
    return;
  }
  Serial.println("SD card initialization succesful");  

//initialize data logging  timer and synchronization timer
  logTimer = millis();
  syncTimer = millis();
  
}


void loop() {
  // check if its time to log data
  if(millis()-logTimer >=LOG_INTERVAL){
    logData();
    logTimer = millis();   
  }

  // check if its time to synchronize with firebase
    if(millis()-syncTimer >= SYNC_INTERVAL){
      syncData();
      syncTimer = millis();
    }    
      
}


void logData(){
  writeFile(SD, "/data.txt", "Today is wedenessday ");
 
  }

void syncData(){
// Read data from the file on the SD card
  String data = ""; 
  readFile(SD, "/data.txt");
 // data += String(file.read());
  
  if(Firebase.setString(firebaseData, "/mydata", data)) {
        Serial.println("Data uploaded to Firebase");
      }
       else {
        Serial.println("Error uploading data to Firebase");
      }
   
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
       
    while(file.available()){
       Serial.write(file.read());
       
    }
    file.close();
}


