// this sample code provided by www.programmingboss.com
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
const char* ssid = "SenzMate";
const char* password = "fibeR@SenzMate";
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "mqtt.senzmate.com";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
// LED Pin
const int ledPin = 2;
#define RXp2 16
#define TXp2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  setup_wifi();
  Serial.println("point_1");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("point_2");

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "kri/esp32/output") {
    Serial.print("Changing output to ");
    if (messageTemp == "on") {
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if (messageTemp == "off") {
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("kri/esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  Serial.println("Message Received: ");
  String receivedMessage = Serial2.readString();

  if (receivedMessage != "") {
    Serial.println(receivedMessage);
    float x = receivedMessage.toFloat(); // Convert the received string to a float
    Serial.println("Float Value: ");
    Serial.println(x);
      char message[50];
  snprintf(message, sizeof(message), "Alcohol level: %.2f",x);


  delay(1000); // Wait for 1 second
  Serial.println("point_3");

  if (!client.connected()) {
    reconnect();
  }

  Serial.println("point_4");

  client.loop();

  Serial.println("point_5");

  client.publish("kri/esp32/data", message);
  }
}
