#include <SoftwareSerial.h>
#include <Servo.h>
#include <SwitchCase.h>

Servo servo1, servo2, servo3, servo4;

SoftwareSerial BTSerial(0, 1); // RX, TX


// Variables to store the current angle of each servo
int servo1Angle = 0;
int servo2Angle = 0;
int servo3Angle = 0;
int servo4Angle = 0;

void setup() {
  BTSerial.begin(9600);
  servo1.attach(11);
  servo2.attach(10);
  servo3.attach(9);
  servo4.attach(6);

  servo1.write(110);
  servo2.write(0);
  servo3.write(160);
  servo4.write(0);
}

void loop() {
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.toLowerCase();
    
    switch(command){
    case "left":
        servo1Angle += 20;
        delay(100);
        if (servo1Angle > 30) {
          servo1Angle = 30;
        }
        servo1.write(servo1Angle);
        break;
      case "right":
        servo1Angle -= 20;
        delay(100);
        if (servo1Angle < 180) {
          servo1Angle = 180;
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
}
