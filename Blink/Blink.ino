 /*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/ 
#include<avr/power.h>
// the setup function runs once when you press reset or power the board
  int i;
int n;
    int led_pin[3] ={11,12,13};
    
void setup() {
clock_prescale_set(clock_div_1);
    Serial.begin(9600);
    
 pinMode(led_pin[i] , OUTPUT);
  // initialize digital pin LED_BUILTIN as an output.

}
// the loop function runs over and over again forever
void loop() {
  
  for ( i = 0 ; i<3 ;i++){
  
   
  digitalWrite(led_pin[i], HIGH);// turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(led_pin[i], LOW);// turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  }
for ( i=2 ; i>0; i--){
   digitalWrite(led_pin[i-1], HIGH);// turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(led_pin[i], LOW);// turn the LED off by making the voltage LOW
  delay(1000);
Serial.println("hello world !");
  }
}
  
  
  
  

  //led_pin[i] = led_pin[j];
//  }
  

     
     
    
    
 // digitalWrite(led_pin [1] , HIGH);
 // delay(1000);
 // digitalWrite(led_pin[1] , LOW);
  //delay(1000); 
  
 //digitalWrite(led_pin[2] ,HIGH);
 //delay(1000);
//   digitalWrite(led_pin[2] , LOW);
  // delay(1000);}
// Serial.println("hello);
  

  
