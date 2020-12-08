#include "cmdArduino.h"
#include "DHT.h"

DHT dht(A0, DHT11);

void setup() 
{
    dht.begin();
    cmd.begin(57600);
    Serial.println("Lab 3-2-2b - Command Temp and Humidity");

    cmd.add("temp", cmdTemperature);
    cmd.add("humid", cmdHumidity);    
}

void loop() 
{
    cmd.poll();
}

void cmdTemperature(int argc, char **args)
{
    float temperature = dht.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" deg Celsius");
}

void cmdHumidity(int argc, char **args)
{
    float humidity = dht.readHumidity();
    Serial.print("Humdity: ");
    Serial.print(humidity);
    Serial.println("% RH");
}
