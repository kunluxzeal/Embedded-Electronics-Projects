int button_pin = 12;
int button_read ;
int led_pin = 7;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(button_pin , INPUT);
pinMode(led_pin , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
button_read = digitalRead(button_pin);
Serial.println(button_read); 
if (button_read == 0){
  digitalWrite(led_pin , HIGH);
  }
if (button_read == 1){
  digitalWrite(led_pin , LOW);
  
  }
}
