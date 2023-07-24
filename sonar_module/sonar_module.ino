#include <Servo.h>
int trig_pin = 3;
int echo_pin = 2;
int led = 13;
int servo_pos ;

Servo myservo;
void setup() {
  // put your setup code here, to run once:
 myservo.attach(9);
 pinMode(led, OUTPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin , INPUT);
   Serial.begin(9600);


 
}

void loop() {
unsigned long duration;
 float distance;
 
   digitalWrite(trig_pin,LOW);
  delay(10);
  digitalWrite(trig_pin , HIGH);
  delay(10);
    digitalWrite(trig_pin,LOW);
     
  // put your main code here, to run repeatedly:
  duration = pulseIn(echo_pin, HIGH);
  //duration = map(duration,0,1023,0,255);
  constrain(duration,0,1023);
 //Serial.println("cm:");
 //delay(200);

  
float _time = duration/2;
float _speed = 0.03475;
distance = _speed * _time;

if(Serial.available()>0){
  servo_pos = Serial.read();
  //Serial.println(servo_pos);
  myservo.write(servo_pos);
  delay(10);
  }

if((distance<5)&&(distance>2)){
  Serial.println("100");
  //digitalWrite(led,HIGH);
  //Serial.println("FULL");
  delay(1000);
  
  }
else{
  //Serial.println(distance);
  //digitalWrite(led,LOW);
  Serial.println("0");
  //Serial.println("NOT FULL");
  delay(1000);
  }

 //Serial.println("cm:");

 
}
