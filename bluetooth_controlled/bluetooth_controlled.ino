#include<AFMotor.h>

AF_DCMotor motor1(2, MOTOR12_1KHZ);
AF_DCMotor motor2(3 ,MOTOR12_1KHZ);

char command ;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}
 
void loop() {
  // put your main code here, to run repeatedly:

if (Serial.available()>0){
  command = Serial.read();
  stop();
  
  Serial.println(command);

  switch(command){

    case 'F' :
      forward();
      break;
    case 'B' :
    back();
     break;
     case 'L' :
     left();
     break;
     case 'R' :
     right();
     break;
    
    
    
    
    }
  }
}

void forward(){
  
  motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  
}

void back(){
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
  
  }

  void left(){
    motor1.setSpeed(255);
    motor1.run(FORWARD);
    motor2.setSpeed(255);
    motor2.run(BACKWARD);
    
    
    
    }


   void right(){
motor1.setSpeed(255);
motor1.run(BACKWARD);
motor2.setSpeed(255);
motor2.run(FORWARD);


    
   }

   void stop(){
    
    motor1.setSpeed(0);
    motor1.run(RELEASE);
    motor2.setSpeed(0);
    motor2.run(RELEASE);
    
    
    
    }
   
