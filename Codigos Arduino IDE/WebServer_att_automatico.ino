#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

DHT dht;

ESP8266WebServer server(80);

const char* ssid = "Gabriel";
const char* password = "99494728";

String webSite, javaScript, XML, XMLFromSerial;

void handleLed1On() {
  digitalWrite(D5, HIGH);
  handleWebsite();
}

void handleLed1Off() {
  digitalWrite(D5, LOW);
  handleWebsite();
}

void handleLed2On() {
  digitalWrite(D6, HIGH);
  handleWebsite();
}

void handleLed2Off() {
  digitalWrite(D6, LOW);
  handleWebsite();
}

void buildXML() {
  float celsius = dht.getTemperature();// obtem o valor da leitura do sensor
  float humidity = dht.getHumidity();
  int sensorValue = analogRead(A0);   // read the input on analog pin 0
  float luminosidade = map(sensorValue, 0, 1023, 100, 0);
  
  XML = "<?xml version='1.0'?>";
  XML += "<xml>";

  //Cria as label XML atraves das TAG's criadas, podem possuir qualquer nome
  XML += "<humi>";//
  XML += String(humidity);
  XML += "</humi>";

  XML += "<temp>";
  XML += String(celsius);//Exibe a temperatura na pagina
  XML += "</temp>";

  XML += "<lumi>";
  XML += String(luminosidade);//Exibe a temperatura na pagina
  XML += "</lumi>";

 XML += "</xml>";
}

void buildJavascript() {//recupera as informações do XML das TAG's criadas e executa o modo ajax pelo metodo "response"
  javaScript = "<SCRIPT>\n";
  javaScript += "xmlHttp=createXmlHttpObject();\n";

  javaScript += "function createXmlHttpObject(){\n";
  javaScript += "  if(window.XMLHttpRequest){\n";
  javaScript += "    xmlHttp=new XMLHttpRequest();\n";
  javaScript += "  }else{\n";
  javaScript += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript += "  }\n";
  javaScript += "  return xmlHttp;\n";
  javaScript += "}\n";

  javaScript += "function process(){\n";
  javaScript += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "    xmlHttp.onreadystatechange=function(){\n";
  javaScript += "      if(xmlHttp.readyState==4&&xmlHttp.status==200){\n";
  javaScript += "        xmlDoc=xmlHttp.responseXML;\n";

  //recupera os dados das labels xml em buildXML()
  javaScript += "        humimsg=xmlDoc.getElementsByTagName('humi')[0].firstChild.nodeValue;\n";// obtem a humidade
  javaScript += "        document.getElementById('labelHumi').innerHTML=humimsg;\n";// cria a ID:labelHora usada no buildWebsite();

  javaScript += "        tempmsg=xmlDoc.getElementsByTagName('temp')[0].firstChild.nodeValue;\n";//deveria obter a temperatura
  javaScript += "        document.getElementById('labelTemp').innerHTML=tempmsg;\n";// cria a ID:labelTemp usada no buildWebsite();
  
  javaScript += "        lumimsg=xmlDoc.getElementsByTagName('lumi')[0].firstChild.nodeValue;\n";//deveria obter a temperatura
  javaScript += "        document.getElementById('labelLumi').innerHTML=lumimsg;\n";// cria a ID:labelTemp usada no buildWebsite();

  javaScript += "      }\n";
  javaScript += "    }\n";
  javaScript += "    xmlHttp.open('PUT','xml',true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";
  javaScript += "  setTimeout('process()',1000);\n";
  javaScript += "}\n";

  javaScript += "</SCRIPT>\n";
}


void buildWebsite() {// monta o website
  
  buildJavascript();

  webSite = "<!DOCTYPE html>";
  webSite += "<html>";
  webSite += "  <head>";
  webSite += javaScript;
  webSite += "    <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">";
  webSite += "    <meta charset=\"UTF-8\">";
  webSite += "    <title>ESP8266</title>";
  webSite += "  </head>";
  webSite += "  <body onload='process()'>";
  webSite += "    <h1>Você está conectado!</h1>";
  webSite += "    <br></br>";
  webSite += "    Temperatura (Celsius): <p ID='labelTemp'></p>";
  webSite += "    <br></br>";
  webSite += "    Humidade (%): <p ID='labelHumi'></p>";
  webSite += "    <br></br>";
  webSite += "    Luminosidade (%): <p ID='labelLumi'></p>";
  webSite += "    <br></br>";
  webSite += "<table style=\"width:100%\">";
  webSite += "  <tr>";
  webSite += "    <th><h3>LED 1</h3></th>";
  webSite += "    <th><h3>LED 2</h3></th>";
  webSite += "  </tr>";
  webSite += "  <tr>";
  webSite += "    <th><a href=\"LED1On\"><button>ON</button></a></th>";
  webSite += "    <th><a href=\"LED2On\"><button>ON</button></a></th>";
  webSite += "  </tr>";
  webSite += "  <tr>";
  webSite += "    <th><a href=\"LED1Off\"><button>OFF</button></a></th>";
  webSite += "    <th><a href=\"LED2Off\"><button>OFF</button></a></th>";
  webSite += "  </tr>";
  webSite += "</table>";
  webSite += "  </body>";
  webSite += "</html>";  
}


void handleWebsite() {
  buildWebsite();
  server.send(200, "text/html", webSite);
}

void handleXML() {
  buildXML();
  server.send(200, "text/xml", XML);
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.println("\n\nBOOTING ESP8266 ...");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleWebsite);
  server.on("/xml", handleXML);
  server.on("/LED1On", handleLed1On);
  server.on("/LED1Off", handleLed1Off);
  server.on("/LED2On", handleLed2On);
  server.on("/LED2Off", handleLed2Off);

  server.begin();

  dht.setup(4);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
}

void loop() {

  server.handleClient();
  delay(2000);
}
