/*
 * code modified by olujimi kunle 20/1/2023
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
#include <Servo.h>
#include <NewPing.h>

Servo myservo;

#define front_trigger A2
#define front_echo A1

#define outside_trigger 4
#define outside_echo 5

#define inside_trigger 6
#define inside_echo 7



NewPing sonar1(front_trigger,front_echo,200);
NewPing sonar2(outside_trigger,outside_echo,200);
NewPing sonar3(inside_trigger,inside_echo,200);

int distance;
int obstacle;
const int buzzer = 10, blenable = A3;
const int in1 = 2, in2 = 3, in3 = 11, in4 = 12;
int level;
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
  checkPresence();
  checkObstacle();
//odourLevel();
  wasteLevel();
  if (Serial.available()>0) {
    Serial.println(level);
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
  
}

void checkPresence(){
  distance = sonar2.ping_cm();
  if (distance > 0 && distance < 40){
    openLid(); 
  } 
}

void checkObstacle(){
  obstacle = sonar1.ping_cm();
  if (obstacle<25){
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
   level = sonar3.ping_cm();
   Serial.println(level);
   delay(1000);
  
  
  if(level > 0 && level<10){
    for (int k = 4; k>=0; k--){
      digitalWrite(buzzer, HIGH);
      delay(200);  delay(1000);

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
  while(sonar1.ping_cm() <=40){
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
  delay(2500);
  brake();
}

void right(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(2500);
  brake();
}

void brake(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
