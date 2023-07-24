#include<Wire.h>

#define DS1307_ADDRESS 0x68


 
byte bcd2dec (byte val){
  return ((val/16*10)+(val % 16));  
  
  }

int dec2bcd (byte val){
  return ((val/16*10)+(val % 10)); 
  
  }


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







  
void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
printDate();
delay(1000);
}
