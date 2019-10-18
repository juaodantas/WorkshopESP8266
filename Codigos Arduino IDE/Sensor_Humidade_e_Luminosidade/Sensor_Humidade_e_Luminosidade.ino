#include "DHT.h"

DHT dht;

int estado_LED1 = 0;
int estado_LED2 = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F) \tLuminosidade (%)");

  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);

  dht.setup(14); // gpio 14(D5), data from DHT
}

void loop()
{
  if(Serial.available() > 0)
  {
    int val = Serial.parseInt();
    if(val == 1)  // liga OU desliga o led 1
    {
      if (estado_LED1)
      {
        digitalWrite(D4, LOW);
        estado_LED1 = 0;    
      }
      else
      {
        digitalWrite(D4, HIGH);
        estado_LED1 = 1;
      }
    }
    else if(val == 2) // liga OU desliga o led 2
    {
      if (estado_LED2)
      {
        digitalWrite(D2, LOW);
        estado_LED2 = 0;    
      }
      else
      {
        digitalWrite(D2, HIGH);
        estado_LED2 = 1;
      }
    }
    else
      Serial.println("Entrada Inválida");
  }

  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
 //int sensorValue = analogRead(A0);   // read the input on analog pin 0
 // int luminosidade = map(sensorValue, 0, 1023, 100, 0);

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.println("\t\t");
  //Serial.println(luminosidade);   // print out the value you read

}
