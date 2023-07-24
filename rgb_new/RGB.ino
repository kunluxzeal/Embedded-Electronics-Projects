 
#include<avr/power.h>

int redPin =4;
int greenPin = 7;
int bluePin =8;
int button =2;
int buttonState;
int counter =0;
void setup() {
  // put your setup code here, to run once:
clock_prescale_set(clock_div_1);

pinMode(redPin ,  OUTPUT);
pinMode(greenPin , OUTPUT);
pinMode(bluePin , OUTPUT);
pinMode(button , INPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
 buttonState = digitalRead(button);
 
 Serial.println(counter);
if(buttonState ==HIGH){
  counter++;
  delay(500);
  }
  else if(counter ==1){
  digitalWrite(redPin , HIGH);
  digitalWrite(greenPin , LOW);
 digitalWrite(bluePin , LOW);
}
   else if(counter ==2) {
 digitalWrite(redPin , HIGH);
 digitalWrite(greenPin , HIGH);
 digitalWrite(bluePin , HIGH);
}
  else if (counter ==3){
digitalWrite(redPin , HIGH);
digitalWrite(greenPin , LOW);
digitalWrite(bluePin , HIGH);
}
   else if (counter ==4){
digitalWrite(redPin , LOW);
digitalWrite(greenPin , LOW);
digitalWrite(bluePin , HIGH);
   }
else if(counter ==5)
{
digitalWrite(redPin , LOW);
digitalWrite(greenPin , HIGH);
digitalWrite(bluePin , LOW);
}
 else if (counter ==6){

digitalWrite(redPin , LOW);
digitalWrite(greenPin , HIGH);
digitalWrite(bluePin , HIGH);

}
else if(counter ==7){

digitalWrite(redPin , LOW);
digitalWrite(greenPin ,LOW );
digitalWrite(bluePin , LOW);

  
    }
    else{
      
      counter =0;
      }
}








  
