
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 


const uint64_t pipeIn = 0xE8E8F0F0E1LL; 
RF24 radio(4, 5);
struct MyData {
  float h; 
  float t;
};
MyData data;
void setup()
{

  Serial.begin(115200); 
  Serial.print("initializing");
  radio.begin();
Serial.print("checking for communication");  // lcd.begin();
  // lcd.home();
  // lcd.backlight();
  // lcd.clear();
  radio.setAutoAck(false);
  //radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
  //lcd.println("Receiver ");
Serial.println(radio.isChipConnected());
}
void recvData()
{
  if ( radio.available() ) {
    radio.read(&data, sizeof(MyData));
    }
}
void loop()
{
  Serial.println("Waiting for Data");
  recvData();
  Serial.print("Humidity: ");
  Serial.println(data.h);
  delay(1000);
  // lcd.setCursor(0,0);
  // lcd.print("Humidity:"); 
  // lcd.print(data.h);
  // lcd.print("%");
  // lcd.setCursor(0,1);
  Serial.print("Temperature: ");
  Serial.println(data.t);
  delay(1000);
  // lcd.print("Temperature:");
  // lcd.print(data.t);
  // lcd.print(" C");  
  //Serial.print("\n");
}