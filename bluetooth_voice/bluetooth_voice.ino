/*
 * USED PINS:
 * BLUETOOTH:   0,1,
 * ULTRASONIC:  4,5,
 * ULTRASONIC1: 6,7,
 * SERVO:       9,
 * BUZZER:      10,
 * MQ135:       A0,
 * MOTOR L:     2,3,
 * MOTOR R:     11,12,
 * ULTRASONIC2: A1,A2,
 * BLENABLE:    A3,
 * 
 * FREE PINS:
 * A4,A5,A6,A7
 */

#include <Ultrasonic.h>
#include <Servo.h>

Servo myservo;
Ultrasonic front(A2,A1);
Ultrasonic outside(4,5);
Ultrasonic inside(6,7);
int distance;
int obstacle;
const int buzzer = 10, blenable = A3;
const int in1 = 2, in2 = 3, in3 = 11, in4 = 12;

String incomingData;
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
  pinMode(blenable, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(buzzer, LOW);
  Serial.begin(9600);
  digitalWrite(blenable, HIGH);
  delay(500);
  digitalWrite(blenable, LOW);
  myservo.attach(9);
  myservo.write(140);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  //wasteLevel();
  //forward();
  //backward();
  //left();
  //right();
  //obstacle = ultrasonic1.read();
  //distance = ultrasonic.read();
  checkPresence();
  checkObstacle();
  odourLevel();
  wasteLevel();
  if (Serial.available()>0) {
    incomingData = Serial.readStringUntil('\n');
    incomingData.toLowerCase();
    if (incomingData == "start"){
      digitalWrite(buzzer, HIGH);
      delay(5000);
      digitalWrite(buzzer, LOW);
    }
    else if (incomingData == "stop"){
      brake();
    }
    else if (incomingData == "open"){
      openLid();
    }
    else if (incomingData == "forward"){
        forward();
    }
    else if (incomingData == "left"){
        left();
    }
    else if (incomingData == "right"){
        right();
    }
    else if (incomingData == "reverse"){
        backward();
    }
  }
  Serial.println(level);
}

void checkPresence(){
  distance = outside.read();
  if (distance < 30){
    openLid();
  }
}

void checkObstacle(){
  obstacle = front.read();
  if (obstacle < 50){
    brake();
    return;
  }
}

void openLid(){
  int pos;
  for(pos=140; pos>=10; pos-=1){
    myservo.write(pos);
    delay(5);
  }
  delay(5000);
  for(pos=10; pos<=140; pos+=1){
    myservo.write(pos);
    delay(5);
  }
}

void wasteLevel(){
  int level=inside.read();
  if(level<10){
    for (int k = 4; k>=0; k--){
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(50);
    }
    openLid();
  } else{
    digitalWrite(buzzer, LOW);
  }
}

void odourLevel(){
  int odour = analogRead(A0);
  if(odour<300){
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(50);
  } else {
    digitalWrite(buzzer, LOW);
  }
}

void forward(){
  while(front.read() < 30){
    brake();
  }
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(50);
} 

void backward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(3000);
  brake();
}

void left(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(1000);
  brake();
}

void right(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);
  brake();
}

void brake(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
