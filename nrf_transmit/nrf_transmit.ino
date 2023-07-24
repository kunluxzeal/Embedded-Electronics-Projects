

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DHT.h"
const uint64_t pipeOut = 0xE8E8F0F0E1LL; 
#define DHTPIN 3
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
RF24 radio(9, 10); //  CN and CSN  pins of nrf
bool result;
struct MyData {
  float h;
  float t;
};
MyData data;
void setup()
{
  Serial.begin(9600); 
  Serial.println("start");
  dht.begin();
  radio.begin();
  radio.setAutoAck(false);
 // radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
Serial.println(radio.isChipConnected()); 
radio.printDetails(); 
Serial.println();
delay(1000);
}
void loop()
{
  data.h = dht.readHumidity();
  data.t = dht.readTemperature();
  if (isnan(data.h) || isnan(data.t)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: ");
  Serial.println(data.h);
  delay(2000);
  Serial.print("Temperature: ");
  Serial.println(data.t);
  delay(2000);
  
  result = radio.write(&data, sizeof(MyData));
     if (result){
       Serial.println("sent");
       delay(2000);
     }
    else{
      
  Serial.println("not sent");
  delay(2000);
      }
}

 