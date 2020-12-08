#include "DHT.h"

DHT dht(A0, DHT11);

int threshold = 26;
int pinLed = 4;

void setup() 
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);
    
    dht.begin();
    Serial.begin(57600);
    Serial.println("Lab 3-2-2c: Temperature Warning Light");
}

void loop() 
{
    float temperature = dht.readTemperature();
    if (temperature > threshold)
    {
        Serial.println("Temperature over threshold!");
        digitalWrite(pinLed, HIGH);
    }
    else
    {
        digitalWrite(pinLed, LOW);
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" deg C");
    delay(1000);
}
