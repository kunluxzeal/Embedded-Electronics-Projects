#define volt_pin A1
#define buzzer_pin 0
#define led_pin 1
int read_val;
float v2;
void setup() {
  // put your setup code here, to run once:
//Serial.begin(9600);
pinMode(volt_pin , INPUT);
pinMode(buzzer_pin, OUTPUT);
pinMode(led_pin , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_val = analogRead(A1);
  v2 = (5./1023.)*read_val;
 // Serial.println(read_val);
 // Serial.println(v2);

  if (v2 < 2.51){
    digitalWrite(buzzer_pin ,LOW);
    digitalWrite(led_pin ,LOW);
    }

    if(v2 > 2.51){
      digitalWrite(buzzer_pin , HIGH);
      digitalWrite(led_pin ,HIGH);
        delay(100);
      }
  delay(500);

}
