#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <String.h>
#include "DHT.h"

DHT dht;

#define wifi_ssid "gdgcuiaba"
#define wifi_password "gdgcuiaba"

#define mqtt_server "iot.eclipse.org"
#define mqtt_user ""
#define mqtt_password ""

#define humidity_topic "alvaro/sensor/humidity"              // mudar aqui  
#define temperature_topic "alvaro/sensor/temperature"        // mudar aqui  
#define led1_topic "alvaro/led1"                            // mudar aqui  
#define luminosidade_topic "alvaro/sensor/luminosidade"     // mudar aqui  

WiFiClient espClient;
PubSubClient client(espClient);
void setup_wifi();
ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("Tópico => " + String(topic) + " | Valor => " + String(message));

  if(String(topic) == led1_topic)
  {
    if(String(message) == "Liga")
      digitalWrite(D5, HIGH);
    else
      digitalWrite(D5, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  dht.setup(4); // data pin D2
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);
  pinMode(D5, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("alvaroviebrantz")) {                // mudar aqui  
      Serial.println("connected");
      client.subscribe(led1_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    float newTemp = dht.getTemperature();
    float newHum = dht.getHumidity();

    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("New temperature:");
      Serial.println(String(temp).c_str());
      client.publish(temperature_topic, String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
      client.publish(humidity_topic, String(hum).c_str(), true);
    }
    int sensorValue = analogRead(A0);   // read the input on analog pin 0
    float luminosidade = map(sensorValue, 0, 1023, 100, 0);
    client.publish(luminosidade_topic, String(luminosidade).c_str(), true);
  }
}

