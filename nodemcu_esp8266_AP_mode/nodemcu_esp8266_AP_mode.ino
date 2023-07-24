/*
 * This Code will configure ESP8266 in SoftAP mode and allow different devices (Laptop, Mobile, PCs) connect to it
 */
#include <ESP8266WiFi.h> 
 
const char* ap_ssid = "ESP8266"; //Access Point SSID
const char* ap_password= "embedded-robotics"; //Access Point Password
uint8_t max_connections=8;//Maximum Connection Limit for AP
int current_stations=0, new_stations=0;


 
/* data received from application */
String  data =""; 

/* define L298N or L293D motor control pins */
int Relay1 = 12;    //D6
int Relay2 = 16;    //D0
int Relay3 = 4;     //D2
int Relay4 = 5;     //D1

void setup()
{
  /* initialize motor control pins as output */
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT);  
  pinMode(Relay4, OUTPUT);

digitalWrite(Relay1,LOW);
  digitalWrite(Relay2,LOW);
  digitalWrite(Relay3,LOW);
  digitalWrite(Relay4,LOW);
  
  //Start the serial communication channel
  Serial.begin(115200);
  Serial.println();
   
  //Setting the AP Mode with SSID, Password, and Max Connection Limit
  if(WiFi.softAP(ap_ssid,ap_password,1,false,max_connections)==true)
  {
    Serial.print("Access Point is Creadted with SSID: ");
    Serial.println(ap_ssid);
    Serial.print("Max Connections Allowed: ");
    Serial.println(max_connections);
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Unable to Create Access Point");
  }
}
 
void loop()
{
  //Continuously check how many stations are connected to Soft AP and notify whenever a new station is connected or disconnected
 
  new_stations=WiFi.softAPgetStationNum();
   
  if(current_stations<new_stations)//Device is Connected
  {
    current_stations=new_stations;
    Serial.print("New Device Connected to SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
   
  if(current_stations>new_stations)//Device is Disconnected
  {
    current_stations=new_stations;
    Serial.print("Device disconnected from SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
   
}
