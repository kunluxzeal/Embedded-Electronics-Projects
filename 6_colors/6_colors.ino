/*********
  
*********/

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;


void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Begins serial communication
  Serial.begin(9600);
}

void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  
    redColor = map(redFrequency, 80, 147, 255,0);
  
  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redColor);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  
    greenColor = map(greenFrequency, 126, 180, 255, 0);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
 
  blueColor = map(blueFrequency, 53, 118, 255, 0);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);





  

  // Checks the current detected color and prints
  // a message in the serial monitor
  if(redColor>0 &&redColor>greenColor &&(redColor>blueColor || redColor<blueColor)&& blueColor>greenColor && (greenColor<0 ||greenColor>50)){
      Serial.println(" - RED detected!");
  }
  else if((redColor<0||redColor>0)&&greenColor > redColor &&(greenColor>blueColor||greenColor<blueColor)&&blueColor>redColor &&blueColor>0 &&greenColor>0 ){
    Serial.println(" - GREEN detected!");
  }
  else if((redColor<0 || redColor > 0) &&blueColor >redColor && blueColor>greenColor && (greenColor<0||greenColor>50) &&(blueColor>22 ||blueColor>100)){
    Serial.println(" - BLUE detected!");
  }

   else if(redColor>130 && redColor<380 && greenColor <0 &&greenColor >600 && blueColor < 0 && blueColor >290 && redColor> greenColor|| redColor < greenColor && redColor>blueColor  ){
    Serial.println(" - YELLOW detected!");
  }

   else if(redColor < 0 && greenColor <0  &&  blueColor>0 && blueColor >redColor){
    Serial.println(" - BLACK detected!");
  }
  else if(redColor >=425 && greenColor >1400 && blueColor >800 ){
    Serial.println(" - WHITE detected!");
  }
  else{
    Serial.println(" - NO COLOR FOUND!");
    }

   
}
