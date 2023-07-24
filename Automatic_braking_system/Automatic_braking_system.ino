
 #include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
long duration;
int distance;
int speedCar =60;
int speed_coeff =60;
int off_setSpeedCar = 60;
int drive =false;
byte FORWARD = 0;
byte BACKWARD = 0;
byte LEFT = 0;
byte RIGHT = 0;

const int trigP = D0; 
const int buzzer = D1; 
const int echoP = D2;  
const char* ssid = "jimmy";
const char* password = "kunluxzeal";
const int ENA = D5;  
const int ENB = D6;  
const int IN1 = D8;     
const int IN2 = D7;   
const int IN3 = D4;    
const int IN4 = D3;  

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//AsyncWebSocket ws("/ws");


String getDistance() {
 digitalWrite(trigP, LOW);   // Makes trigPin low
delayMicroseconds(2);       // 2 micro second delay 

digitalWrite(trigP, HIGH);  // tigPin high
delayMicroseconds(2);      // trigPin high for 10 micro seconds
digitalWrite(trigP, LOW);   // trigPin low

duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
distance= duration*0.034/2;        //Calculating actual/real distance

Serial.print("Distance = ");       
Serial.println(distance);
delayMicroseconds(1);
  return String(distance);
}
  
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Robo control Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  html {
    font-family: Arial;
    display: inline-block;
    margin: 0px auto;
    text-align: center;
  }
  h1 {
    color: #0F3376;
    padding: 2vh;
  }
  p {
    font-size: 1.5rem;
  }
  .button {
    display: inline-block;
    background-color: #008CBA;
    border: none;
    border-radius: 4px;
    color: white;
    padding: 16px 40px;
    text-decoration: none;
    font-size: 30px;
    margin: 2px;b
    cursor: pointer;
  }
  .button3 {
    background-color: #f44336;
  }

   </style>
</head>
<body>
  <h1>WEB-CONTROLLED ROBOT CAR</hl>
   <p>    <hl>  BY  <h/></p>
       </hl> OLUJIMI OLUKUNLE </h1>
  <p>
     <a href="/forward"><button class="button">FORWARD</button></a>
     <a href="/left"><button class="button button4">LEFT</button></a>
     <a href="/right"><button class="button button5">RIGHT</button></a>
     <a href="/backward"><button class="button button2">BACKWARD</button></a></p>
  <p> <a href="/stop"><button class="button button3">STOP</button></a></p>
  </p>
  <p>
    <span class="sensor-labels">Distance</span>
    <span id="distance">%Distance% cm</span>
  </p>
</body>
<script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("distance").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/distance", true);
    xhttp.send();
  }, 1000 ) ;
</script></html>)rawliteral";

void setup(){
  Serial.begin(115200);

pinMode(trigP, OUTPUT);
pinMode(buzzer , OUTPUT);  
pinMode(echoP, INPUT);   
 pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
   pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
     pinMode(ENA, OUTPUT);
      pinMode(ENB, OUTPUT);
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 // server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
 
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request){
  
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
  analogWrite(ENB, speedCar);
  
    request->send(200, "text/html", index_html);


    
   });
  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request){
     delay(100);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB, speedCar);
   
    request->send(200, "text/html", index_html);
  });
   server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(ENA, speedCar);
   
    request->send(200, "text/html", index_html);
  });
   server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
     delay(100);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB, speedCar);
   
    request->send(200, "text/html", index_html);
  });
 server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
   delay(100);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedCar);
   
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  analogWrite(ENB, speedCar);
     
    request->send(200, "text/html", index_html);
  });
  
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){
     
    request->send_P(200, "text/plain", getDistance().c_str());
  });

  
    
    
    }
  
  // Start server
 // server.begin();
 
void loop(){

if (distance>0 && distance<30 ){

  digitalWrite(buzzer,HIGH);
}

  else{
  digitalWrite(buzzer,LOW);
  }
  
  


if (drive == false ){
  
   if(distance >0 && distance<30 ){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
analogWrite(ENA, off_setSpeedCar);

 digitalWrite(IN3, LOW);
digitalWrite(IN4 ,HIGH);
 analogWrite(ENB,off_setSpeedCar);
 
   }
   
      
    
    

 

}
else {
  
  manualDrive();
  
  
  
  }
}
 

void manualDrive()
{
  if (FORWARD == 1)
    {
      digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
  analogWrite(ENB, speedCar);
      delay(1000);
      
    }
  if (BACKWARD == 1)
    {
     digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB, speedCar);
      delay(1000);
    }

  if (LEFT == 1)
    {
     digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedCar);
   
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  analogWrite(ENB, speed_coeff);
      delay(1000);
    }

  if (RIGHT == 1)
    {
     digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speed_coeff);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB, speedCar);
      delay(1000);
    }

 
  }
   


 

  
    // Disable client connections if it was activated
 // if ( ws.enabled() )
   // ws.enable(false);

  // enable client connections if it was disabled
  //if ( !ws.enabled() )
    //ws.enable(true);


//server.handleClient();
//command = server.arg("State");


    
