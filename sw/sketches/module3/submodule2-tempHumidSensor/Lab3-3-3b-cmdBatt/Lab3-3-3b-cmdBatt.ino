#include <cmdArduino.h>

#define ADC_REF_VOLTAGE 3.3     // this is the reference voltage or voltage range of the ADC
#define ADC_LEVELS 1024         // this is number of levels for 10-bit ADC (2^10 = 1024)
#define ADC_SCALE_FACTOR 2      // this is needed because we divided the batt voltage by 2

int pinBatt = A6;

void setup() 
{
    pinMode(pinBatt, INPUT);
    
    cmd.begin(57600);
    Serial.println("Lab 3-3-3b - Command Line Battery Monitoring");

    cmd.add("batt", cmdBatt);
}

void loop() 
{
    cmd.poll();
}

void cmdBatt(int argCnt, char **args)
{
    int battAdc = analogRead(pinBatt);
    float voltsPerAdcUnit = ADC_REF_VOLTAGE / ADC_LEVELS;   // this is the voltage per ADC unit
    float pinVoltage = battAdc * voltsPerAdcUnit;           // this is the voltage at the pin
    float battVoltage = pinVoltage * ADC_SCALE_FACTOR;      // this is the voltage at the battery

    Serial.print("Raw ADC value: ");
    Serial.print(battAdc);
    Serial.print(", Battery Voltage: ");
    Serial.println(battVoltage);
}
