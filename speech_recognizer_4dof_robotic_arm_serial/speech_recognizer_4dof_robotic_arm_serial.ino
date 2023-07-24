#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo1, servo2, servo3, servo4;

SoftwareSerial BTSerial(0, 1); // RX, TX


// Variables to store the current angle of each servo
int servo1Angle = 0;
int servo2Angle = 0;
int servo3Angle = 0;
int servo4Angle = 0;

void setup() {
  Serial.begin(9600);
  delay(100);
  
  servo1.attach(11);
  servo2.attach(10);
  servo3.attach(9);
  servo4.attach(6);

  
}

void loop() {
  if (Serial.available()>0) {
    String command = Serial.readString();
    //command.toLowerCase();
    
    //delay(100);
    if (command == "left") {
      servo1.write(90);
      delay(1000);
      servo1.write(0);
      Serial.println(command);
    }
    else if (command == "right") {
      servo1.write(180);
      delay(1000);
      servo1.write(90);
    }
    else if (command == "up") {
      servo2.write(180);
      delay(1000);
      servo2.write(90);
    }
    else if (command == "down") {
      servo2.write(0);
      delay(1000);
      servo2.write(90);
    }
    else if (command == "pick") {
      servo3.write(180);
      delay(1000);
      servo3.write(90);
    }
    else if (command == "lift") {
      servo3.write(0);
      delay(1000);
      servo3.write(90);
    }
    else if (command == "grip") {
      servo4.write(180);
      delay(1000);
      servo4.write(90);
    }
    else if (command == "release") {
      servo4.write(0);
      delay(1000);
      servo4.write(90);
    }
  }
}
