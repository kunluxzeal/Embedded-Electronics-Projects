#include<avr/power.h>
int i;
int n =1;

int leds[3] ={11,12,13};
void setup() {
  // put your setup code here, to run once:
clock_prescale_set(clock_div_1);


pinMode(leds[i] , OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

 i = 0;
while(i<3){
   digitalWrite(leds[i], HIGH);// turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(leds[i], LOW);// turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
   i++;

   }
  

 i = 2;
  while (i>0){
     digitalWrite(leds[i-n], HIGH);// turn the LED on (HIGH is the voltage level)
  delay(100 );
  digitalWrite(leds[i], LOW);// turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
   i--;
  }
}
