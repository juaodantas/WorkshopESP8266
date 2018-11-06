#include <Arduino.h>

#include "DHT.h"

DHT dht;

int estado_LED1 = 0;
int estado_LED2 = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F) \tLuminosidade (%)");

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

  dht.setup(4); // data pin 22
}

void loop()
{
  if(Serial.available() > 0)
  {
    int val = Serial.parseInt();
    if(val == 1)
    {
      if (estado_LED1)
      {
        digitalWrite(D5, LOW);
        estado_LED1 = 0;    
      }
      else
      {
        digitalWrite(D5, HIGH);
        estado_LED1 = 1;
      }
    }
    else if(val == 2)
    {
      if (estado_LED2)
      {
        digitalWrite(D6, LOW);
        estado_LED2 = 0;    
      }
      else
      {
        digitalWrite(D6, HIGH);
        estado_LED2 = 1;
      }
    }
    else
      Serial.println("Entrada Inválida");
  }

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  int sensorValue = analogRead(A0);   // read the input on analog pin 0
  int luminosidade = map(sensorValue, 0, 1023, 100, 0);

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.println(luminosidade);   // print out the value you read

  delay(2000);
}