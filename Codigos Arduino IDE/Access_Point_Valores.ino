/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

DHT dht;
/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "12345678";

float humidity;
float temperature;
int sensorValue;
int i = 0;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

void handleLed1On() {
  digitalWrite(D5, HIGH);
  handleRoot();
}

void handleLed1Off() {
  digitalWrite(D5, LOW);
  handleRoot();
}

void handleLed2On() {
  digitalWrite(D6, HIGH);
  handleRoot();
}

void handleLed2Off() {
  digitalWrite(D6, LOW);
  handleRoot();
}

void handleRoot() {
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  int sensorValue = analogRead(A0);   // read the input on analog pin 0
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
