#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card
#include <RTClib.h> // for the RTC
#include <avr/power.h> // uncomment when using Arduino nano clone
#define buzzer_pin 6

#include <SoftwareSerial.h>
SoftwareSerial RFID(5,4); // RX and TX

LiquidCrystal_I2C lcd(0x27,16,2);


#define DS1307_ADDRESS 0x68
#define CS_SD 10 
#define RX_PIN  5

byte bcd2dec (byte val){
  return ((val/16*10)+(val % 16));  
  }

byte dec2bcd (byte val){
  return ((val/16*10)+(val % 10)); 
  
  }


String text;
String CardNo; 

// Create a file to store the data
File myFile;

// Instance of the class for RTC
RTC_DS1307 rtc;

// Define check in time
const int checkInHour = 8;
const int checkInMinute = 5;

//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () 
{
  pinMode(buzzer_pin , OUTPUT);
//clock_prescale_set(clock_div_2); // uncomment set clock when using arduino clone

 
  Serial.begin(9600);
  RFID.begin(9600);
  SPI.begin();
 
 // delay(100);
while (!Serial) { 
  ;  // wait for serial port to connect. Needed for native USB port only
  }   
  if (!rtc.begin()) {
    Serial.println("No RTC");
    while (1);
  }
  else {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC off!");
  }
  Serial.print("SD  setup...");
  delay(100); 
/*
   if (!SD.begin(CS_SD)) {
    Serial.println("failed!");
    while (1);
  
  } 
  */
  //Serial.println("done.");
  Serial.println("place Card...");
   delay(1000);

   lcd.init();
   lcd.backlight();
   
  CardNo = "03001AEAD2";
}

char c;

void loop()
{
 

DateTime now = rtc.now();

// Save check in time;
    userCheckInHour = now.hour();
    userCheckInMinute = now.minute();

while (!RFID.available()) {
    lcd.setCursor(0,0);
   lcd.print("SWIPE YOUR CARD");
   delay(10);
   lcd.clear();
 
  }
  while (RFID.available()>0) {
    RFID.begin(9600);
    delay(10); 
    c = RFID.read();
    Serial.print(c);
      
    text += c;
     lcd.setCursor(3,0);
   lcd.print("Checking...");
  delay(50);
  lcd.clear(); 
  }
   
  if (text.length() > 20){
    
    check();
     RFID.end();
  delay(10);
   
  text = ""; 
    
 
  }
  RFID.begin(9600);
}



void check()
{

  text = text.substring(1, 11);
  Serial.println("Card ID : " + text);
 Serial.println("Access ID : " + CardNo);
  delay(100);
  
  if (CardNo.indexOf(text) >= 0) { 
    Serial.println("Accepted");
    
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Acess Granted");
  digitalWrite(buzzer_pin , HIGH);
  delay(1000);
    digitalWrite(buzzer_pin , LOW);
    
  
  verifyCheckIn();
  logCard();
  }
  else {
    Serial.println("denied");
     
   
  delay(100);
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Access Denied");
    for (int i =0 ;i<8 ;i++){
     digitalWrite(buzzer_pin , HIGH);
  delay(200);
   digitalWrite(buzzer_pin , LOW);
  delay(200);
     }  
  }
  Serial.println(" ");
  Serial.println("place card …");
  delay(2000);
}

void verifyCheckIn(){
  if((userCheckInHour < checkInHour)||((userCheckInHour == checkInHour) && (userCheckInMinute <= checkInMinute))){
    
    Serial.println("welcome!");
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("You're welcome!");
    //delay(5000);
    
   
  }
  else{
    
    Serial.println("late...");
    delay(500);
      lcd.setCursor(1,0);
    lcd.print("You're Late!");
    //delay(5000);
    
   
  }
}


void logCard(){
  digitalWrite(CS_SD,LOW);
  
 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
 
  //check();
  if (myFile) {
    Serial.print("File opened ok ");
    myFile.print(CardNo);
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
    Serial.println("written on SD card ");
    delay(1000);
    myFile.close();
    
  }
 
    else{
    Serial.println("error");  
 
    }
     // Disables SD card chip select pin  
  digitalWrite(CS_SD,HIGH);
}
