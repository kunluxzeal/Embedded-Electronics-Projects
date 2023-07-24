#include<avr/power.h>
int red_led_pin = 4;
int green_led_pin = 7;
int blue_led_pin =8;
int button = 2;
int buttonState = 0;
int counter =0;
int lastButtonState = LOW;
void setup() {
  // put your setup code here, to run once:
//clock_prescale_set(clock_div_1);
Serial.begin(115200);
pinMode(red_led_pin , OUTPUT);
pinMode(green_led_pin , OUTPUT);
pinMode(blue_led_pin , OUTPUT);
pinMode(button , INPUT);



}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState = digitalRead(button);
if ( buttonState ==HIGH){
  RGB_color (255 , 0, 0);//red
  digitalWrite(red_led_pin , LOW);
delay(1000);
counter++;
} 
  //else{
   //digitalWrite(red_led_pin , HIGH);
   
  // }

  buttonState = digitalRead(button);
if ( buttonState ==HIGH){
RGB_color (0 , 255, 0); //green
    digitalWrite(green_led_pin , LOW);
delay(1000);
   counter++;
}
  //  else{
    //  digitalWrite(green_led_pin , HIGH); 
   
      //}


  buttonState = digitalRead(button);
if (( buttonState!= lastButtonState) &&(buttonState == HIGH)){
   RGB_color (0 , 0, 255); //blue
digitalWrite(blue_led_pin , LOW); 
delay(1000);
counter++;

}
 
}
//else {
//digitalWrite(blue_led_pin , HIGH); 
//delay(1000);
  }



 
 //igitalWrite(blue_led_pin ,LOW);

//B_color(255 ,255 ,128); //raspbery
//delay(1000)
 
//RGB_color (255, 255 ,255); // white
//delay(1000); 



}

void RGB_color(int red_value ,int green_value, int blue_value){
 
analog Write(red_led_pin ,  255-red_value);
   analogWrite(green_led_pin , 255-green_value);
   analogWrite(blue_led_pin , 255-blue_value);
 
  }
