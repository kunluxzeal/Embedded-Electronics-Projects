
int lane1[] ={13,12,11}; //lane 1 Red,yellow and green
int lane2[] = {10,9,8}; //lane 2 Red,yellow and green
int lane3[] = {7,6,5}; // lane 3 Red,yellow and green


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  for (int i = 0 ; i<3 ;i++)
  {
    pinMode(lane1[i], OUTPUT);
    pinMode(lane2[i],OUTPUT);
    pinMode(lane3[i],OUTPUT);
    }

   for (int i = 0 ; i<3 ;i++)
  {
    pinMode(lane1[i], LOW);
    pinMode(lane2[i],LOW);
    pinMode(lane3[i],LOW);
    }

}

void loop() {
  // put your main code here, to run repeatedly:

   digitalWrite(lane1[2],HIGH);
   digitalWrite(lane2[1],HIGH);
   digitalWrite(lane3[0],HIGH);
   delay(5000);

   digitalWrite(lane1[0],HIGH);
   digitalWrite(lane2[1],LOW);
   digitalWrite(lane1[2],LOW);
   digitalWrite(lane2[2],HIGH);
  digitalWrite(lane3[0],LOW);
    digitalWrite(lane3[1],HIGH);
   delay(5000);
   
    digitalWrite(lane1[0],HIGH);
   digitalWrite(lane2[2],HIGH);
   digitalWrite(lane3[0],HIGH);
   delay(7000);

    digitalWrite(lane1[0],HIGH);
   digitalWrite(lane1[2],HIGH);
   digitalWrite(lane3[1],HIGH);
   delay(3000);

   digitalWrite(lane1[0],HIGH);
   digitalWrite(lane2[0],HIGH);
   digitalWrite(lane3[2],HIGH);
   delay(7000);

    digitalWrite(lane1[1],HIGH);
   digitalWrite(lane2[0],HIGH);
   digitalWrite(lane3[0],HIGH);
   delay(3000);
}
