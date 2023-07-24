#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo1, servo2, servo3, servo4;

SoftwareSerial BTSerial(0, 1); // RX, TX

// Variables to store the current angle of each servo
int servo1Angle = 90;
int servo2Angle = 120;
int servo3Angle = 0;
int servo4Angle = 0;

void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600);
  
  servo1.attach(11);
  servo2.attach(10);
  servo3.attach(9);
  servo4.attach(6);

  servo1.write(90);
  servo2.write(120);
  servo3.write(45);
  servo4.write(90);
}

void loop() {
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.toLowerCase();
    Serial.println(command);
    delay(100);
   
    if (command == "left") {
      servo1Angle -=20;
      servo1.write(servo1Angle);
      Serial.print("servoAngler:");
      Serial.println(servo1Angle);
      delay(100);
    }
    else if (command == "right") {
      servo1Angle +=20;
      servo1.write(servo1Angle);
      Serial.print("servoAngler:");
      Serial.println(servo1Angle);
      delay(100);  
    }
    else if (command == "down") {
       servo2Angle -=20;
      servo2.write(servo2Angle);
      Serial.print("servoAngle2:");
      Serial.println(servo2Angle);
      delay(100);
    }
    else if (command == "up") {
    servo2Angle +=20;
      servo2.write(servo2Angle);
      Serial.print("servoAngle2:");
      Serial.println(servo2Angle);
      delay(100);
    }
    else if (command == "grab") {
      servo3Angle -=10;
      servo3.write(servo3Angle);
      //Serial.print("servoAngle3:");
      //Serial.println(servo3Angle);
      delay(100);
  
      servo4Angle +=90;
      servo4.write(servo4Angle);
      //Serial.print("servoAngle4:");
      //Serial.println(servo4Angle);
      
    }
      else if (command == "open"){
      servo4Angle -=90;
      servo4.write(servo4Angle);
      //Serial.print("servoAngle4:");
      //Serial.println(servo4Angle);
      
    }
    else if (command == "raise") {
      servo3.write(0);
      delay(1000);
      servo3.write(90);
    }
    else if (command == "release") {
      servo4.write(0);
      delay(1000);
      servo4.write(90);
    }
   /* else if (command == "home") {
      servo1.write(90);
      servo2.write(120);
      servo3.write(45);
      servo4.write(90);
      }
  */
  }
  
}
