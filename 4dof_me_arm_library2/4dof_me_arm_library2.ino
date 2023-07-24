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
//#include "meArm.h"
#include <Servo.h>

int basePin = 11;
int shoulderPin = 10;
int elbowPin = 9;
int gripperPin = 6;

int dx=90,dy=80,dz=0,dg =90;

SoftwareSerial BTSerial(0, 1); // RX, TX
//meArm arm;
Servo base;
Servo shoulder;
Servo elbow;
Servo gripper;

void setup() {
//  arm.begin(basePin, shoulderPin, elbowPin, gripperPin);
  BTSerial.begin(9600);
  base.attach(11);
  shoulder.attach(10);
  elbow.attach(9);
  gripper.attach(6);
}

void loop() {
  if (BTSerial.available()>0) {
    String command = BTSerial.readStringUntil('\n');
      command.toLowerCase();
      Serial.println(command);
     // delay(100);
    if (command == "right") {     //robot move right
     // delay(10); 
      dx += 10;
  }
   else if (command == "left") {    //robot move left
    //  delay(10);
       dx -=10;
  }  
      if (command == "lift") { //shoulder up
    //  delay(10);
      dz += 10;
  }
    else if (command == "drop") {    //shoulder down 
      //delay(10);
       dz -=10;
  } 
        if (command == "foward") { //move forward
      //delay(10);
      dy += 10;
  }
    else if (command == "down") { //bend down a little
      delay(10);
       dy -=10;
  } 
     else if (command == "home") {     //go homw
      dx = 90; dy = 80; dz =0;
    } 
 }
      Serial.print("X = ");Serial.print(dx);
      Serial.print("   Y = ");Serial.print(dy);
      Serial.print("   Z = ");Serial.println(dz);
    base.write(dx);
    shoulder.write(dy);
    elbow.write(dz);
    gripper.write(dg);
    //arm.gotoPoint(dx, y, z);
    delay(15);
  
}
