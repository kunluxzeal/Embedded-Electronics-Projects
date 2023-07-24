#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BTserial(2, 3); // RX, TX

// Servo objects for each DOF of the robotic arm
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Pin numbers for each servo
const int servo1Pin = 11;
const int servo2Pin = 10;
const int servo3Pin = 9;
const int servo4Pin = 6;

// Variables to store the current angle of each servo
int servo1Angle = 0;
int servo2Angle = 0;
int servo3Angle = 0;
int servo4Angle = 0;

void setup() {
  // Start the Bluetooth serial communication
  BTserial.begin(9600);
  Serial.begin(9600);
  // Attach the servos to their corresponding pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
}

void loop() {
  // Check if there is data available on the Bluetooth serial port
  if (Serial.available() > 0) {
    // Read the incoming data
    char data = Serial.read();
    Serial.print(data);
    // Check the command and update the corresponding servo angle
    switch (data) {
      case '1':
        servo1Angle += 20;
        delay(100);
        if (servo1Angle > 180) {
          servo1Angle = 180;
        }
        servo1.write(servo1Angle);
        break;
      case '2':
        servo1Angle -= 20;
        delay(100);
        if (servo1Angle < 0) {
          servo1Angle = 0;
        }
        servo1.write(servo1Angle);
        break;
      case '3':
        servo2Angle += 20;
        delay(100);
        if (servo2Angle > 180) {
          servo2Angle = 180;
        }
        servo2.write(servo2Angle);
        break;
      case '4':
        servo2Angle -= 20;
        delay(100);
        if (servo2Angle < 0) {
          servo2Angle = 0;
        }
        servo2.write(servo2Angle);
        break;
      case '5':
        servo3Angle += 20;
        delay(100);
        if (servo3Angle > 180) {
          servo3Angle = 180;
        }
        servo3.write(servo3Angle);
        break;
      case '6':
        servo3Angle -= 20;
        delay(100);
        if (servo3Angle < 0) {
          servo3Angle = 0;
        }
        servo3.write(servo3Angle);
        break;
      case '7':
        servo4Angle += 20;
        delay(100);
        if (servo4Angle > 180) {
          servo4Angle = 180;
        }
       servo4.write(servo4Angle);
        break;

        case '8':
        servo4Angle -= 20;
        delay(100);
        if (servo4Angle < 0) {
          servo4Angle = 0;
        }
       servo4.write(servo4Angle);
        break;

    }
  }
}
