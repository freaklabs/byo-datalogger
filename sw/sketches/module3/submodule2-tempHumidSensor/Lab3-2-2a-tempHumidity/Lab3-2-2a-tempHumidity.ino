#include "DHT.h"

DHT dht(A0, DHT11);

void setup() 
{
    dht.begin();
    Serial.begin(57600);
    Serial.println("Lab 3-2-2a - Reading Temperature and Humidity");
}

void loop() 
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" degrees Celsius, Humdidity: ");
    Serial.print(humidity);
    Serial.println("% RH.");
    delay(2000);                // sensor updates every 2 seconds
}
