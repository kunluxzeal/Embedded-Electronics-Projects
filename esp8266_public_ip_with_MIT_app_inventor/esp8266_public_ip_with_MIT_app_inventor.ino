#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
WiFiClient client;

/* IP address and port number of the server */
const char* serverIP = "";  //public IP address of the server
const int serverPort = 80;  

/* WIFI settings */ 
const char* ssid = "Jimmy";   //WIFI SSID
const char* password = "kunluxzeal9";    //WIFI PASSWORD

void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  /* get public IP address of the ESP8266 */
  HTTPClient http;      //HTTPClient library to make an HTTP GET request to ipify.org 
                         //API to retrieve the public IP address of the ESP8266 board.
  
  http.begin("https://api.ipify.org");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String publicIP = http.getString();
    Serial.print("ESP8266 public IP address: ");
    Serial.println(publicIP);
    serverIP = publicIP.c_str();  //converted the retrieved IP address to a C-style string using the c_str() function and assigned it to the serverIP variable.
  }
  http.end();
}

void loop()
{
  /* If the server available, run the "checkClient" function */  
  client = Server.available();
  if (!client) return; 
  data = checkClient ();
  Serial.print(data);
  
  /* create HTTP client instance */
  HTTPClient http;
  String url = "http://" + String(serverIP) + ":" + String(serverPort) + "/" + data;

  /* send HTTP GET request to the server */
  int httpCode = http.GET(url);

  /* check the HTTP response code */
  if (httpCode == HTTP_CODE_OK)
  {
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
  }

  /* disconnect HTTP client */
  http.end();
  
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
