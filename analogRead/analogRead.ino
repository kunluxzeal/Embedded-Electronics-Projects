int Readpin = A3;
int readVal;
int v2 = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(Readpin , INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
readVal = analogRead(Readpin);
v2 = (5./1023.)*readVal;
Serial.println(v2);
if (v2 > 3.0){
digitalWrite(led, HIGH);
  
}
if (v2 < 3.0){
  digitalWrite(led,LOW);
  }

  
}
