#define DCwater_pump 8
void setup()
{
  pinMode(DCwater_pump, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
 digitalWrite(DCwater_pump,HIGH);
 Serial.println("DC Pump is ON Now!!");
 delay(500);
 digitalWrite(DCwater_pump,LOW);
 Serial.println("DC Pump is OFF Now!!");
 delay(500);
}
