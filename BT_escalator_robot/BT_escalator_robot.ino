
int no_of_holes , no_of_seeds , spacings;
const int motor=10;
int duration = 300;
//const unsigned long event1;  //spacing distace between holes
const unsigned long event2 = 3000; // wait time to dispense no of seeds


unsigned long previousTime = 0;

String str_val ;



// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B connections   
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup() {

  Serial.begin(9600);
   pinMode (motor, OUTPUT);
   digitalWrite(motor, LOW);
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  
}

void loop() {
  Bluetooth();
  delay(100);
}


void Bluetooth(){  
  if(Serial.available()>0){
    str_val = Serial.readString();
    Serial.println(str_val);
  if (str_val.startsWith("<")){
    str_val.remove(0,1);
    no_of_holes = (str_val.toInt());
    //Serial.println(no_of_holes);
    str_val.remove(0,((str_val.indexOf(","))+1));
   
    no_of_seeds= (str_val.toInt());
    str_val.remove(0,((str_val.indexOf(","))+1));

    spacings = (str_val.toInt());
    str_val.remove(0,((str_val.indexOf(">"))+1));

    directionControl();
    delay(1000);
      }    
  }
   
}

void dispensePill(){
  for (int i = 0; i < no_of_seeds ;i++){
  digitalWrite(motor, HIGH);
  delay(duration);
  digitalWrite(motor, LOW);
  delay(100);
  Serial.println(i);
  }
}

// This function lets you control spinning direction of motors
void directionControl() {  //if (currentTime - previousTime >= eventInterval){
    int event1 =spacings*1000;
  for (int i = 0 ; i < no_of_holes ; i++){
      Serial.print("I");
      Serial.print(i);
    
      Serial.print("no_of_holes");
      Serial.println(no_of_holes);
      
     // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(event1);

  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(event2);
 dispensePill();
    delay(100);
  }    
 //}
 
}
