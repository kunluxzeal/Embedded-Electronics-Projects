const int s0 = 4;  
const int s1 = 5;  
const int s2 =6;  
const int s3 = 7;  
const int out = 8;   
// values
int red = 0;  
int green = 0;  
int blue = 0;  

void setup()   
{  
  Serial.begin(9600); 
 
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);   
  
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);  
} 
 
void loop() 
{  
  color();
  Serial.println("RED");  
  Serial.println(red, DEC);
  delay(1000);  
  Serial.println(" GREEN ");  
  Serial.println(green, DEC);
  delay(1000);  
  Serial.println(" BLUE ");  
  Serial.println(blue, DEC); 
  delay(1000); 

  if (red < blue && red < green && red < 20)
  {  
      if (red <=10 && green <=10 && blue <=10){ 
     Serial.println("WHILE");   
  } else{
   Serial.println(" - (Red Color)"); 
  }
}  
else if (blue < red && blue < green)   
  {  
      if (red <=10 && green <=10 && blue <= 10){
     Serial.println("WHILE");  
  }else {
   Serial.println(" - (Blue Color)");   }
  }  
  else if (green < red && green < blue)  
  {  
    if (red <= 10 && green <=10 && blue <= 10){
     Serial.println("WHILE");   
  } else{
   Serial.println(" - (Green Color)");  } 
  }  
  else{
  Serial.println();  
  }
}
void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}
 
