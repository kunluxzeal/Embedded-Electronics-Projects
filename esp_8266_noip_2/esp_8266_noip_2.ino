#include <ESP8266WiFi.h>

const char* ssid = "Jimmy";
const char* password = "kunluxzeal9";
const char* server = "41.242.79.226";

// Set up the GPIO pin connected to the relay
const int relayPin = 5;  // Change this to the GPIO pin you're using
bool relayState = false;

void setup() {
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
  String url = "/update?hostname=your_noip_host_name&myip=";
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

  // Set up the relay pin as an output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {
  // Check if the relay needs to be turned on or off
  if (relayState) {
    digitalWrite(relayPin, HIGH);  // Turn on the relay
  } else {
    digitalWrite(relayPin, LOW);  // Turn off the relay
  }

  // Nothing to do here
}

// Function to handle incoming messages from the No-IP server
void callback(char* topic, byte* payload, unsigned int length) {
  // Convert the payload to a string
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Check if the message is "on" or "off" and update the relay state accordingly
  if (message == "on") {
    relayState = true;
  } else if (message == "off") {
    relayState = false;
  }
}
