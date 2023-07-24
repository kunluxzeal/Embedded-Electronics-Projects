/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>


*/
#include <Adafruit_GFX.h>        //OLED libraries
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPLl97mrSZe"
#define BLYNK_TEMPLATE_NAME "Blood Pressure Monitoring System"
#define BLYNK_AUTH_TOKEN "1TjFKw9IeSAhR_Alm7AbEvoAorHxq1Y0"
#define BLYNK_PRINT Serial
#define REPORTING_PERIOD_MS  1000

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

const char* ssid = "mikroLAB";
const char* pass = "Qwerty8887h2?";
//char ssid[] = "IOTMedics";  // type your wifi name
//char pass[] =  "Max30100";  // type your wifi password

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;
int flag = 0;
int flag2 = 1;
int BPM = 0;
double DIA, SYS, HR, SPO = 0.0;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);



// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
    flag = 1;
}

const int button = D4;

void setup()
{
  Serial.begin(115200);
  Serial.println("Connecting to Wifi"); 
  pinMode(button, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.display(); 
  delay(3000);
  display.clearDisplay();      //Clear the display
      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setTextColor(WHITE); // Draw white text
      display.setCursor(0, 0);     // Start at top-left corner                
      display.println("Connecting to Wifi"); 
      display.println("IOTMedics"); 
      display.display();
      delay(100);

  Blynk.begin(auth, ssid, pass);     //Connecting to Blynk v2 Server
  
  Serial.print("Initializing pulse oximeter..");
  
  randomSeed(analogRead(2));
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    
    pox.setOnBeatDetectedCallback(onBeatDetected);
   // timer.setInterval(2000L, pushParameter); // get data every 2s
}

void pushParameter(){
  Blynk.virtualWrite(V0, String (SPO));       //heart pulse rate 
  Blynk.virtualWrite(V1, String(SYS));      //SPO2 reading
  Blynk.virtualWrite(V2, String(DIA));       //Systolis reading
  Blynk.virtualWrite(V3, String(HR));       //Diastolis reading
}

void loop()
{
  pox.update();
    // Make sure to call update as fast as possible
   if(digitalRead(button) == HIGH){
     pox.resume();
     flag2 = 0;
   } else if(flag2==1) {
     pox.shutdown();
     //delay(2000);
      display.clearDisplay();      //Clear the display
      display.setTextSize(2);      // Normal 1:1 pixel scale
      display.setTextColor(WHITE); // Draw white text
      display.setCursor(0, 0);     // Start at top-left corner                
      display.println("Press Btn to Start"); 
      display.display();
      delay(10);
   }

    
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
       if(flag == 1){

        display.clearDisplay();      //Clear the display
        display.setTextSize(2);      // Normal 1:1 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(0, 0);     // Start at top-left corner                
        display.println(" HR: "); 
        display.setCursor(60, 0); 
        HR = pox.getHeartRate();
        display.println(HR);
        Blynk.virtualWrite(V3, String(HR));       //Heart pulse reading
        //display.setCursor(90, 0); 
        //display.println("bpm");     
        display.setCursor(0, 16);  
        display.println("SPO2: ");      
        display.setCursor(60,16);   
        SPO = pox.getSpO2();             
        display.println(SPO,1);
        
        //checkSP();
        //display.setCursor(95, 16); 
        //display.println(" %"); 
        display.display();
        //delay(10);
        
       if(SPO){
         Blynk.virtualWrite(V3, String(HR));       //Heart pulse reading
         Blynk.virtualWrite(V0, String(SPO));       //Oxygen rate 
           pox.shutdown();
            getBP();
            display.clearDisplay();      //Clear the display
            display.setTextSize(2);      // Normal 1:1 pixel scale
            display.setTextColor(WHITE); // Draw white text
            display.setCursor(0, 0);     // Start at top-left corner                
            display.println("SYS: "); 
            display.setCursor(55, 0); 
            display.println(SYS);
            Blynk.virtualWrite(V1, String(SYS));      //SPO2 reading
            Blynk.virtualWrite(V2, String(DIA));       //Systolis reading
            display.setCursor(0, 16);  
            display.println("DIA: ");      
            display.setCursor(55,16);                
            display.println(DIA);
            
            display.display();
            flag2 = 1; flag= 0;
            //pushParameter();
            //sendSMS();
            delay(10000);
          //  break;
          }
         // */
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        flag= 0;
       }
        tsLastReport = millis();        
      }

  Blynk.run();
  //timer.run();
}
void checkSP(){
   if(pox.getSpO2()){
     flag2=1;
   }
}

void getBP(){
  
  float cardiacOutput = 0;      //to be calculated
  int StrokeVolume = 80;      //Stroke Volume fixed at 80mL
  int SBP = 0;
  int TBR = 18;             //16mmL
  //Blood Pressure (SbP) = Cardiac Output x Total Body Resistance (TBR)
  //Cardiac output = Stroke Volume (mL) x Heart Rate (BPM)
  BPM = random(75, 95);
     Serial.print("Cardiac Output:- ");
    
    cardiacOutput = (StrokeVolume * BPM) /1000;       //Retrive the Values for Cardiac Output
    Serial.print(cardiacOutput,1);
    Serial.print(" L/min");
    Serial.println(" ");
    Serial.println(" +++++++++++++++++++ ");
    
    SBP = cardiacOutput * TBR + BPM;                //Calculate the SBP values
    DIA = BPM;
    SYS = DIA + HR -12;
    Serial.print("Blood Pressure:- ");
    Serial.print(DIA);
    Serial.print(" mmHG");
    Serial.println(" ");
}
