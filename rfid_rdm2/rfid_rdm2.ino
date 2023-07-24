#include<Wire.h>
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card
#include <RTClib.h> // for the RTC

#include <SoftwareSerial.h>
SoftwareSerial RFID(5,4); // RX and TX

#define DS1307_ADDRESS 0x68


byte bcd2dec (byte val){
  return ((val/16*10)+(val % 16));  
  
  }

int dec2bcd (byte val){
  return ((val/16*10)+(val % 10)); 
  
  }


String text;
String CardNumber; 

// Create a file to store the data
//File myFile;

// Instance of the class for RTC
RTC_DS1307 rtc;


// Define check in time
const int checkInHour = 9;
const int checkInMinute = 5;

//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;




void printDate(){

Wire.beginTransmission(DS1307_ADDRESS);
byte zero = 0x00;
Wire.write(zero);
Wire.endTransmission();
Wire.requestFrom(DS1307_ADDRESS ,7);
  int second = bcd2dec(Wire.read()  );
  int minute = bcd2dec(Wire.read());
  int hour =   bcd2dec(Wire.read() &0b111111);
  int weekDay=    bcd2dec(Wire.read());
  int monthDay =    bcd2dec(Wire.read());
  int month =   bcd2dec(Wire.read());
  int year =  bcd2dec(Wire.read());

  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  
  }







void setup()
{

  
  Serial.begin(9600);
  RFID.begin(9600);

  
  Serial.println("Bring your RFID Card Closer...");
  CardNumber = "03001AEAD2"; 
}
char c;


 

void loop()
{
  while (RFID.available() > 0) {
     printDate();
    delay(10);
    c = RFID.read();
    text += c;
  }
  if (text.length() > 20)
  check();
  text = "";
}

void check()
{
  text = text.substring(1, 11);
  Serial.println("Card ID : " + text);
  Serial.println("Access ID : " + CardNumber);

  if (CardNumber.indexOf(text) >= 0) {
    Serial.println("Access accepted");
  }
  else {
    Serial.println("Access denied");
  }
  delay(2000);
  Serial.println(" ");
  Serial.println("Bring your RFID card closer …");
}
