#include <Arduino.h>
​
#include "DHT.h"
​
DHT dht;
const int botao = D5;
​
int estado_LED1 = 0;
int estado_LED2 = 0;
int estadoBotao = 0;
​
int statusBotao;
​
void setup()
{
  Serial.begin(9600);
  Serial.println();
​
  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT);
​
  dht.setup(D3);
​
  delay(3000);
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F) \tEstado do Botao\n");
}
​
void loop()
{
  if(Serial.available() > 0)
  {
    int val = Serial.parseInt();
    if(val == 1)
    {
      if (estado_LED1)
      {
        digitalWrite(D4, LOW);
        estado_LED1 = 0;
        Serial.println("LED1 Desligado");
      }
      else
      {
        digitalWrite(D4, HIGH);
        estado_LED1 = 1;
        Serial.println("LED1 Ligado");
      }
    }
    else if(val == 2)
    {
      if (estado_LED2)
      {
        digitalWrite(D2, LOW);
        estado_LED2 = 0;
        Serial.println("LED2 Desligado");    
      }
      else
      {
        digitalWrite(D2, HIGH);
        estado_LED2 = 1;
        Serial.println("LED2 Ligado");
      }
      
    }
    else
      Serial.println("Entrada Inválida");
  }
​
  estadoBotao = digitalRead(botao); 
​
​
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
​
​
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print( String(humidity, 1) + " %");
  Serial.print("\t\t");
  Serial.print( String(temperature, 1) + " ºC");
  Serial.print("\t\t");
  Serial.print( String(dht.toFahrenheit(temperature) , 1) + " ºF");
  Serial.print("\t\t");
  //Serial.println(luminosidade);   // print out the value you read
  Serial.println(estadoBotao);
​
​
  delay(1000);
}#include <Arduino.h>
​
#include "DHT.h"
​
DHT dht;
const int botao = D5;
​
int estado_LED1 = 0;
int estado_LED2 = 0;
int estadoBotao = 0;
​
int statusBotao;
​
void setup()
{
  Serial.begin(9600);
  Serial.println();
​
  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT);
​
  dht.setup(D3);
​
  delay(3000);
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F) \tEstado do Botao\n");
}
​
void loop()
{
  if(Serial.available() > 0)
  {
    int val = Serial.parseInt();
    if(val == 1)
    {
      if (estado_LED1)
      {
        digitalWrite(D4, LOW);
        estado_LED1 = 0;
        Serial.println("LED1 Desligado");
      }
      else
      {
        digitalWrite(D4, HIGH);
        estado_LED1 = 1;
        Serial.println("LED1 Ligado");
      }
    }
    else if(val == 2)
    {
      if (estado_LED2)
      {
        digitalWrite(D2, LOW);
        estado_LED2 = 0;
        Serial.println("LED2 Desligado");    
      }
      else
      {
        digitalWrite(D2, HIGH);
        estado_LED2 = 1;
        Serial.println("LED2 Ligado");
      }
      
    }
    else
      Serial.println("Entrada Inválida");
  }
​
  estadoBotao = digitalRead(botao); 
​
​
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
​
​
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print( String(humidity, 1) + " %");
  Serial.print("\t\t");
  Serial.print( String(temperature, 1) + " ºC");
  Serial.print("\t\t");
  Serial.print( String(dht.toFahrenheit(temperature) , 1) + " ºF");
  Serial.print("\t\t");
  //Serial.println(luminosidade);   // print out the value you read
  Serial.println(estadoBotao);
​
​
  delay(1000);
}