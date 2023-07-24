/* meArm IK test - York Hackspace May 2014
   Test applying Nick Moriarty's Inverse Kinematics solver
   to Phenoptix' meArm robot arm, to walk a specified path.

   Pins:
   Arduino    Base   Shoulder  Elbow    Gripper
      GND    Brown     Brown   Brown     Brown
       5V      Red       Red     Red       Red
       11    Yellow
       10             Yellow
        9                     Yellow
        6                               Yellow
*/
#include <SoftwareSerial.h>
#include "meArm.h"
#include <Servo.h>

int basePin = 11;
int shoulderPin = 10;
int elbowPin = 9;
int gripperPin = 6;

int x1 ,x2,x3;

SoftwareSerial BTSerial(0, 1); // RX, TX
meArm arm;

void setup() {
  arm.begin(basePin, shoulderPin, elbowPin, gripperPin);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.toLowerCase();

    if (command == "right") {
      delay(10);
      for (x1=-400 ;x1<=-141 ;x1=x1+20){
      arm.gotoPoint(-141, 20, 9);
      }
      while (true){
        if (command == "pic") {
      delay(10);
      arm.gotoPoint(-85, 21, 34);
        }
        break;
    }
  }
    else if (command == "left") {
      delay(10);
      arm.gotoPoint(131, -82, 24);
      while (true){
        if (command == "pic") {
      delay(10);
      arm.gotoPoint(89, -27, 24);
        }
        break;
    }
      
    }

   else if (command == "lift") {
      
      delay(10);
      arm.gotoPoint(131, -82, 24);

    }

    if (command == "down") {
      delay(10);
      arm.gotoPoint(-150, 100, 200);
    }
    if (command == "grab") {
      delay(10);
      arm.gotoPoint(-150, 100, 200);
    }
    if (command == "release") {
      delay(10);
      arm.gotoPoint(-150, 100, 200);
    }
    if (command == "retrieve") {
      delay(10);
      arm.gotoPoint(-150, 100, 200);
    }
    else if (command == "home") {
      arm.gotoPoint(0, 100, 50);
      delay(2000);
    }
    else if (command == "open") {
      arm.openGripper();
    }
  }

}
