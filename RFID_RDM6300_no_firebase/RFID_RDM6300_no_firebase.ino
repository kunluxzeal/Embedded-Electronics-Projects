#include <Wire.h>
#include <ESP8266WiFi.h>
#include <rdm6300.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16
#define buzzerPin 12

Rdm6300 rdm6300;

int tokenCountCard1 = 5;
int tokenCountCard2 = 3;


void setup()
{
  Serial1.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  delay(2000);
  home_display();
  delay(3000);
  swipe_your_card();
  delay(2000);

  pinMode(READ_LED_PIN, OUTPUT);
  digitalWrite(READ_LED_PIN, LOW);
  pinMode(buzzerPin, OUTPUT);

  rdm6300.begin(RDM6300_RX_PIN);
  Serial1.println("\nPlace RFID tag near the rdm6300...");
  swipe_your_card();
}

void home_display()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25, 15); //(col, row)
  display.println("JOURNEY");
  display.setCursor(30, 42);
  display.println("ACCESS");
  display.display();
}

void swipe_your_card()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 7);
  display.println("Swipe your");
  display.setCursor(10, 30);
  display.println("Card here");
  display.setCursor(50, 48);
  display.println(">>>");
  display.display();
}

void access_granted(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("Access");
  display.setCursor(25,38);
  display.println("granted");
  display.display(); 
}

void invalid_card(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("Invalid");
  display.setCursor(45,38);
  display.println("Card");
  display.display(); 
}

void access_left1(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 7);
  display.println("Access");
  display.setCursor(33,30);
  display.println("Left:");
  display.setCursor(55,48);
  display.println(tokenCountCard1);
  display.display(); 
}


void access_left2(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 7);
  display.println("Access");
  display.setCursor(33,30);
  display.println("Left:");
  display.setCursor(55,48);
  display.println(tokenCountCard2);
  display.display(); 
}

void no_access_left(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(57, 7);
  display.println("No");
  display.setCursor(33,30);
  display.println("Access");
  display.setCursor(50,48);
  display.println("Left");
  display.display(); 
}


void access_denied(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("Access");
  display.setCursor(25,38);
  display.println("Denied");
  display.display(); 
}


void checkAccess(long tag)
{
  bool isValidCard = false;

  if (tag == 1824741) // Example tag ID for card1
  {
    isValidCard = true;
    if (tokenCountCard1 > 0)
    {
      access_granted();
      Access_buzzer_sound(1); 
      tokenCountCard1--;     
      Serial1.println("CHECKING IN");
      delay(1000);
      Serial1.print("ACCESS LEFT: ");
      Serial1.println(tokenCountCard1);
      access_left1();
       delay(2000);
      home_display();
      delay(2000);
      swipe_your_card();
      
      
      
      
    }
    else
    {
      Serial1.println("ACCESS DENIED");
      delay(1000);
      access_denied();
      delay(1000);
      no_access_left();
    Access_denied_sound(4);
     delay(2000);
    home_display();
    delay(2000);
    swipe_your_card() ;   
    }
  }
  else if (tag == 6714035) // Example tag ID for card2
  {
    isValidCard = true;
    if (tokenCountCard2 > 0)
    {
      access_granted();
      Access_buzzer_sound(1);     
      tokenCountCard2--;
      Serial1.println("CHECKING IN");
      delay(1000);
      Serial1.print("ACCESS LEFT: ");
      Serial1.println(tokenCountCard2);
      access_left2();
      delay(2000);
      home_display();
      delay(2000);
      swipe_your_card(); 
           
      
    }
    else
    {
      Serial1.println("ACCESS DENIED");
      delay(1000);
      access_denied();
      delay(1000);
      no_access_left();
      Access_denied_sound(4);
       delay(2000);
      home_display();
      delay(2000);
      swipe_your_card();
    }
  }
  

  if (!isValidCard)
  {
    invalid_card();
    Serial1.println("Invalid card");
    Access_denied_sound(4);
     delay(2000);
    home_display();
    delay(2000);
    swipe_your_card();
  }
}

void loop()
{
  find_rfid();
}

void find_rfid()
{
  if (rdm6300.get_new_tag_id())
  {
    long tag = rdm6300.get_tag_id();
    Serial1.println(tag);
    checkAccess(tag);
  }

  digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
  delay(10);
}



void Access_buzzer_sound(int numTimes)
{
  for (int i = 0; i < numTimes; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(300);
  }
}

void Access_denied_sound(int numTimes)
{
  for (int i = 0; i < numTimes; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(500);
  }
}
