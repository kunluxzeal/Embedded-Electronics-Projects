#include <avr/power.h>
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

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () 
{
  clock_prescale_set(clock_div_2);
  Serial.begin(9600);
  RFID.begin(9600);
  SPI.begin();
  RFID.write(0x02);
  delay(100);
 

   
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
/*
  if (!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    while (1);
  
  } 
  */
  Serial.println("initialization done.");
  Serial.println("Bring your RFID Card Closer...");
   delay(1000);

    lcd.init();
    lcd.backlight();
    lcd.clear();
     lcd.setCursor(2,0);
   lcd.print("Welcome!");
   delay(3000);
   lcd.clear();
   
   lcd.setCursor(3,1);
   lcd.print("place card");
   delay(1000);
   lcd.flush();
   

  CardNumber = "03001AEAD2";
  //CardNumber[1] = "0300175962";
}

char c;

void loop()
{
 

DateTime now = rtc.now();

// Save check in time;
    userCheckInHour = now.hour();
    userCheckInMinute = now.minute();


  while (RFID.available()>0) {
    delay(10);
    c = RFID.read();
    Serial.print(c);  
    text += c;  
  }
  
 delay(1000);
 
  if (text.length() > 20){
    check();
  delay(1000);
  text = ""; 
  
  }
}



void check()
{
int i ;
for (i=0 ;i <1 ;++i){

 
  text = text.substring(1, 11);
  Serial.println("Card ID : " + text);
  delay(500);
  Serial.println("Access ID : " + CardNumber);
  delay(1000);

 


  if (CardNumber.indexOf(text) >= 0) {
       lcd.setCursor(0,0);
     lcd.print("processing");
     delay(2000);
    Serial.println("Access accepted");
    delay(2000);
    
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Access granted");
  delay(2000);
  
verifyCheckIn();


logCard();

//return;
 
  }
  else {
    Serial.println("Access denied");
     delay(2000);
  lcd.backlight();
  lcd.setCursor(0,0);
    lcd.print("Acess denied");
    delay(2000);
  lcd.flush();
    
  }
 
}
delay(2000);
  Serial.println(" ");
  Serial.println("Bring your RFID card closer …");
  delay(2000);

  lcd.init();
    lcd.backlight();
    lcd.setCursor(3,1);
   lcd.print("place card");
   delay(2000);
    lcd.flush();
   
  
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
  digitalWrite(CS_SD,LOW);
  
 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  int i ;
for (i=0 ;i<1 ;++i){
  
  //check();
  if (myFile) {
    Serial.print("File opened ok ");
    myFile.print(CardNumber[i]);
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

    return;
  }
  delay(1000);
  }
    // Save check in time;
   // userCheckInHour = now.hour();
  //  userCheckInMinute = now.minute();
  

    Serial.println("error opening data.txt");  
  
   // Disables SD card chip select pin  
  digitalWrite(CS_SD,HIGH);
}
