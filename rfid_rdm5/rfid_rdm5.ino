#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card
#include <RTClib.h> // for the RTC



#include <SoftwareSerial.h>
SoftwareSerial RFID(5,4); // RX and TX

LiquidCrystal_I2C lcd(0x27,16,2);


#define DS1307_ADDRESS 0x68

#define CS_SD 7 


byte bcd2dec (byte val){
  return ((val/16*10)+(val % 16));  
  
  }

byte dec2bcd (byte val){
  return ((val/16*10)+(val % 10)); 
  
  }


String text;
String CardNumber; 

// Create a file to store the data
File myFile;

// Instance of the class for RTC
RTC_DS1307 rtc;


// Define check in time
const int checkInHour = 9;
const int checkInMinute = 5;

//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;



// variables to keep state
int readVal = 0; // individual character read from serial
unsigned int readData[10]; // data read from serial
int counter = -1; // counter to keep position in the buffer
char tagId[11]; // final tag ID converted to a string

char* authorizedTags[4]; // array to hold the list of authorized tags

// CardNumber = "03001AEAD2"; 

// fills the list of authorzied tags
void initAuthorizedTags() {
  // add your own tag IDs here
  authorizedTags[0] = "03001AEAD2";
 // authorizedTags[1] = "0400063EB9";
 // authorizedTags[2] = "040004F3F5";
 // authorizedTags[3] = "04006813AB";
}




char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () 
{
  Serial.begin(9600);
  RFID.begin(9600);
  
  //RFID.write(0x02);
  delay(100);
 initAuthorizedTags() ;

   
while (!Serial) { 
  ;  // wait for serial port to connect. Needed for native USB port only
  }
  


 



 
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  else {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  }
  Serial.print("Initializing SD card...");
  delay(100); 

  if (!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    while (1);
  
  } 
  Serial.println("initialization done.");
  Serial.println("Bring your RFID Card Closer...");
   delay(1000);
   

}
// check if the tag ID we just read is any of the authorized tags
int checkTag() {
  int i;

  for (i = 0; i < 1; ++i) {
    if (strcmp(authorizedTags[i], tagId) == 0) {
      return 1;
    }
  }
  return 0;
}

// convert the int values read from serial to ASCII chars
void parseTag() {
  int i;
  for (i = 0; i < 10; ++i) {
    tagId[i] = readData[i];
  }
  tagId[10] = 0;
}

// once a whole tag is read, process it
void processTag() {
  // convert id to a string
  parseTag();

  // print it
  printTag();

  // check if the tag is authorized
  if (checkTag() == 1) {
    tagSuccess(); // if so, perform an action (blink a led, open a door, etc...)
  } else {
    tagFailed(); // otherwise, inform user of failure
  }
}

void printTag() {
  Serial.print("Tag value: ");
  Serial.println(tagId);
}

// perform an action when an authorized tag was read
void tagSuccess() {
  Serial.println("Tag authorized.");

  // here, we simply turn on the success LED for 2s
  // digitalWrite(SUCCESS, HIGH);
  //digitalWrite(ERROR, LOW);
  // delay(2000);
}

// inform the user that the tag is not authorized
void tagFailed() {
  Serial.println("Unauthorized access!");

  //digitalWrite(SUCCESS, LOW);
  // digitalWrite(ERROR, HIGH);
  // delay(2000);
}

// this function clears the rest of data on the serial, to prevent multiple scans
void clearSerial() {
  while (Serial.read() >= 0) {
    ; // do nothing
  }
 
}

char c;

void loop(){
 

DateTime now = rtc.now();

// Save check in time;
    userCheckInHour = now.hour();
    userCheckInMinute = now.minute();


  while (RFID.available()>0) {
    delay(10);
    c = RFID.read();
    Serial.print(c);
     //delay(10);
    text += c;
   
   
  }
  
  if (text.length() > 20){
   for (int i = 0 ; i<1; i++){
    //checkTag()
    text = ""; 
    }
  delay(1000);
  
  

}
  
 Serial.println(" ");
  Serial.println("Bring your RFID card closer …");
  delay(2000);
  
}



void verifyCheckIn(){
  if((userCheckInHour < checkInHour)||((userCheckInHour == checkInHour) && (userCheckInMinute <= checkInMinute))){
    
    Serial.println("You're welcome!");
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("You're welcome!");
    delay(1000);
    lcd.flush();
   
  }
  else{
    
    Serial.println("You are late...");
    delay(500);
      lcd.setCursor(0,0);
    lcd.print("You're late! ");
    delay(500);
    lcd.flush();
   
  }
}


void logCard(){
 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("File opened ok ");
    myFile.print(CardNumber);
    myFile.print(" , ");  

    // Save time on SD card
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.println(now.minute(), DEC);
    delay(1000);
    myFile.close();
    
    // Print time on Serial monitor
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.println(now.minute(), DEC);
    Serial.println("sucessfully written on SD card ");
    delay(1000);

    
   

    // Save check in time;
   // userCheckInHour = now.hour();
  //  userCheckInMinute = now.minute();
  }
  else {
    Serial.println("error opening data.txt");  
  }
     
}
