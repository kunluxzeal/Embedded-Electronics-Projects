#include <ESP8266WiFi.h>

const char* ssid = "Jimmy";
const char* password = "kunluxzeal9";
const char* server = "41.242.79.226";


/* define L298N or L293D motor control pins */
int Relay1 = 12;    //D6
int Relay2 = 16;    //D0
int Relay3 = 4;     //D2
int Relay4 = 5;     //D1


void setup() {

/* initialize motor control pins as output */
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT);  
  pinMode(Relay4, OUTPUT);

  digitalWrite(Relay1,LOW);
  digitalWrite(Relay2,LOW);
  digitalWrite(Relay3,LOW);
  digitalWrite(Relay4,LOW);
   
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Set up the connection to the noip server
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(server, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/update?hostname=oaktech.hopto.org&myip=";
  url += WiFi.localIP().toString();

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // Send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the response from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("Closing connection");
}

void loop() {
  WiFiClient client;
  const int httpPort = 80;
  
  // Send a request to the server to get the command to turn on/off the relay
  if (client.connect(server, httpPort)) {
    client.println("GET /get-command HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();
    delay(10);

    // Read the response from the server
    String response = "";
    while (client.available()) {
      String line = client.readStringUntil('\r');
      response += line;
      Serial.print(response);
    }

    // Check if the command is to turn on the relay
    if (response == "/relay1ON") {
      // Turn on the relay
      // Replace "relayPin" with the actual pin number you're using to control the relay
      digitalWrite(Relay1, HIGH);
    }
    else if(response == "relay1/OFF"){
      digitalWrite(Relay1, LOW);
      }

      else if(response == "relay2/ON"){
      digitalWrite(Relay2, HIGH);
      }

      else if(response == "relay2/OFF"){
      digitalWrite(Relay2, LOW);
      }

      else if(response == "relay3/ON"){
      digitalWrite(Relay3, HIGH);
      }

      else if(response == "relay3/OFF"){
      digitalWrite(Relay3, LOW);
      }

      else if(response == "relay4/ON"){
      digitalWrite(Relay4, HIGH);
      }
    else if(response == "relay4/OFF"){
      digitalWrite(Relay4, LOW);
      }
      
  }
  
  // Wait for some time before sending the next request
  delay(1000);
}
