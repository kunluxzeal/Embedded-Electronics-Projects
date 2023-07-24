#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
WiFiServer server(80);

WiFiClient client;

/* data received from application */
String  data =""; 

// IP address and port number of the server 
const char* serverIP = "api.ipify.org";
const int serverPort = 80;

// WIFI settings 
const char* ssid = "Jimmy";
const char* password = "kunluxzeal9";

/* Relay pins */
const int Relay1 = 12;
const int Relay2 = 16;
const int Relay3 = 4;
const int Relay4 = 5;

void connectWiFi()
{
Serial.println("Connecting to Wi-Fi...");
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to WiFi...");
}
Serial.println("Connected to Wi-Fi");

String globalIP = getGlobalIP();
Serial.println("Global IP Address: " + globalIP);
delay(10000); // wait for 10 seconds

}

String getGlobalIP() {
// Use HTTP GET request to get the global IP address from the API
HTTPClient http;

String url = "http://" + String(serverIP) + ":" + String(serverPort) + "/";
Serial.print(url);
http.begin(client, url);
int httpCode = http.GET();
Serial.print("http code");
Serial.println(httpCode);
String response = http.getString();
Serial.print("response");
Serial.println(response);
response.remove(0, 5);
  response.remove(response.length()-9,9);
  Serial.print("response2");
Serial.println(response);

//Serial.print(response);
http.end();
return response;

}

void setup() {
Serial.begin(115200);
pinMode(Relay1, OUTPUT);
pinMode(Relay2, OUTPUT);
pinMode(Relay3, OUTPUT);
pinMode(Relay4, OUTPUT);

digitalWrite(Relay1,LOW);
digitalWrite(Relay2,LOW);
digitalWrite(Relay3,LOW);
digitalWrite(Relay4,LOW);
connectWiFi();
}

void loop() {

 /* If the server available, run the "checkClient" function */  
    client = server.available();
    if (!client) return; 
    data = getGlobalIP();
Serial.print(data);
/************************ Run function according to incoming data from application *************************/

    
    if (data == "relay1ON")
    { 
      digitalWrite(Relay1,HIGH);
      }
    
    else if (data == "relay1OFF")
    {
      digitalWrite(Relay1,LOW);
      }

    else if (data == "relay2ON")
    {
      digitalWrite(Relay2,HIGH);
      }
      
    else if (data == "relay2OFF")
    {
      digitalWrite(Relay2,LOW);
      }
      
    else if (data == "relay3ON")
    {
      digitalWrite(Relay3,HIGH);
      }
      
    else if (data == "relay3OFF")
    {
      digitalWrite(Relay3,LOW);
      }
      
    else if (data == "relay4ON")
    {
      digitalWrite(Relay4,HIGH);
      }
      
    else if (data == "relay4OFF")
    {
      digitalWrite(Relay4,LOW);
      }
}


String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
