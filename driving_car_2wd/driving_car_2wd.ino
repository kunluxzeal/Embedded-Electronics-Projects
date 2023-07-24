////////////////////////////////////////////////////////
//          Arduino Obstacle Avoiding Robot v2.0      //
//            By Aarav Garg                           //
////////////////////////////////////////////////////////

//including the libraries
#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

//defining pins and variables
#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 220  // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

//defining motors,servo,sensor
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
AF_DCMotor motor1(2, MOTOR12_64KHZ);
AF_DCMotor motor2(3, MOTOR12_64KHZ);
Servo myservo;

//defining global variables
boolean goesForward = false;
int distance ;
int speedSet = 0;
int FrontDistance;
int LeftDistance;
int RightDistance;
int  Time;
int CollisionCounter;
    
void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(90);//rotate the servo to face front
  delay(1000);
  FrontDistance = readPing();
  delay(1000);
  Serial.println("front distance =");
  Serial.print(distance);
  FrontDistance = readPing();
  delay(1000);
  Serial.println("front distance =");
  Serial.print(distance);
   distance = readPing();
  //delay(1000);
  //distance = readPing();
  //delay(1000);
}

void loop() {
  int distanceR = 0;
  int distanceL =  0;
  delay(500);

  if (FrontDistance > 60 || FrontDistance ==0)
{
  //
  moveForward();
  else{
    CollisionCounter =CollisionCounter+1;
    moveStop();
    navigate();
  }
    }
  

    if (distanceR >= distanceL)
    {
      turnRight();
      moveStop();
    }
    else
    {
      turnLeft();
      moveStop();
    }
  }
  else
  {
    moveForward();
  }

  //reseting the variable after the operations
  distance = readPing();
}

int lookRight()
{
  myservo.write(90);
  delay(500);
  int distance = readPing();
  delay(500);
  myservo.write(90);
  return distance;
}

int lookLeft()
{
  myservo.write(0);
  delay(500);
  int distance = readPing();
  delay(500);
  myservo.write(0);
  return distance;
  // delay(1000);
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void moveForward() {

  if (!goesForward)
  {
    goesForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
    {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet + MAX_SPEED_OFFSET);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet + MAX_SPEED_OFFSET);
    delay(5);
  }
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(300);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(300);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}


//
