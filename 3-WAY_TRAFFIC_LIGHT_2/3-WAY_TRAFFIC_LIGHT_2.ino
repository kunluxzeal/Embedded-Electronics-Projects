#include <avr/power.h>


int signal1[] = {13,12,11};
int signal2[] = {10,9,8};
int signal3[] = {7,6,5};


int redDelay = 5000;
int yellowDelay = 2000;

void setup() {

  clock_prescale_set(clock_div_2);
  // Declaring all the LED's as output
  for (int i = 0; i < 3; i++) {
    pinMode(signal1[i], OUTPUT);
    pinMode(signal2[i], OUTPUT);
    pinMode(signal3[i], OUTPUT);
   
  }
}

void loop() {
  // Making Green  LED at signal 1 and red LED's at other signal HIGH
  digitalWrite(signal1[2], HIGH);
  //digitalWrite(signal1[0], LOW);
  digitalWrite(signal2[0], HIGH);
  digitalWrite(signal3[0], HIGH);
  
  
  delay(redDelay);

  // Making Green LED at signal 1 LOW and making yellow LED at signal 1 HIGH for 2 seconds
  digitalWrite(signal2[1], HIGH);
  digitalWrite(signal1[0], LOW);
  digitalWrite(signal2[0], LOW);
  digitalWrite(signal1[2], HIGH);
  delay(yellowDelay);
  digitalWrite(signal2[1], LOW);
  digitalWrite(signal1[2], LOW);

  // Making Green  LED at signal 2 and red LED's at other signal HIGH
  digitalWrite(signal1[0], HIGH);
  digitalWrite(signal2[2], HIGH);
  digitalWrite(signal2[0], LOW);
  digitalWrite(signal3[0], HIGH);
  
  delay(redDelay);

  // Making Green LED at signal 2 LOW and making yellow LED at signal 2 HIGH for 2 seconds
  digitalWrite(signal2[1], LOW);
  digitalWrite(signal2[2], HIGH);
  digitalWrite(signal3[1], HIGH);
  digitalWrite(signal3[0], LOW);
   
  delay(yellowDelay);
  digitalWrite(signal3[1], LOW);
  
   

  // Making Green  LED at signal 3 and red LED's at other signal HIGH
  digitalWrite(signal3[0], HIGH);
  digitalWrite(signal2[0], HIGH);
  digitalWrite(signal3[2], HIGH);
  digitalWrite(signal3[0], LOW);
  digitalWrite(signal2[2],LOW);
  
  
  delay(redDelay);

  // Making Green LED at signal 3 LOW and making yellow LED at signal 3 HIGH for 2 seconds
  digitalWrite(signal1[1], HIGH);
  digitalWrite(signal1[0], LOW);
  digitalWrite(signal3[0], LOW);
  digitalWrite(signal3[2], HIGH);
  delay(yellowDelay);
  digitalWrite(signal3[2], LOW);
  digitalWrite(signal1[1], LOW);
  
  

  
}
