#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

DHT dht;
/* Set these to your desired credentials. */
const char *ssid = "Rede do ESP";
const char *password = "12345678";

float humidity;
float temperature;
int sensorValue;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  sensorValue = analogRead(A0);   // read the input on analog pin 0
  int luminosidade = map(sensorValue, 0, 1023, 100, 0);
  
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);

  String esp = "<!DOCTYPE html>";
  esp += "<html>";
  esp += "  <head>";
  esp += "    <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">";
  esp += "    <meta charset=\"UTF-8\">";
  esp += "    <title>Access Point ESP8266</title>";
  esp += "  </head>";
  esp += "  <body>";
  esp += "    <h1>Você está conectado!</h1>";
  esp += "    <br></br>";
  esp += "    Temperatura: " + String(temperature) + " Celsius";
  esp += "    <br></br>";
  esp += "    Humidade: " + String(humidity);
  esp += "    <br></br>";
  esp += "    Luminosidade: " + String(luminosidade);
  esp += "    <br></br>";
  esp += "<table style=\"width:50%\">";
  esp += "  <tr>";
  esp += "    <th><h3>LED 1</h3></th>";
  esp += "    <th><h3>LED 2</h3></th>";
  esp += "  </tr>";
  esp += "  <tr>";
  esp += "    <th><a href=\"LED1On\"><button>ON</button></a></th>";
  esp += "    <th><a href=\"LED2On\"><button>ON</button></a></th>";
  esp += "  </tr>";
  esp += "  <tr>";
  esp += "    <th><a href=\"LED1Off\"><button>OFF</button></a></th>";
  esp += "    <th><a href=\"LED2Off\"><button>OFF</button></a></th>";
  esp += "  </tr>";
  esp += "</table>";
  esp += "  </body>";
  esp += "</html>";
  
  server.send(200, "text/html", esp);
}

void handleLed1On() {
  handleRoot();
  digitalWrite(D5, HIGH); 
}

void handleLed1Off() {
  handleRoot();
  digitalWrite(D5, LOW);
}

void handleLed2On() {
  handleRoot();
  digitalWrite(D6, HIGH);  
}

void handleLed2Off() {
  handleRoot();
  digitalWrite(D6, LOW);
}



void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/LED1On", handleLed1On);
  server.on("/LED1Off", handleLed1Off);
  server.on("/LED2On", handleLed2On);
  server.on("/LED2Off", handleLed2Off);
  server.begin();
  Serial.println("HTTP server started");
  
  dht.setup(4); // data pin D2

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
}

void loop() {
  server.handleClient();
}