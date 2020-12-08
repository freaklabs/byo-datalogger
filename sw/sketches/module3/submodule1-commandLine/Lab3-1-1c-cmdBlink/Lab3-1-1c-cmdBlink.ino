#include <cmdArduino.h>

int pinLed = 13;
int delayTime = 1000;

void setup() 
{
    cmd.begin(57600);
    Serial.println("Lab 3-1-1c - Command Blink");

    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);

    cmd.add("blink", cmdBlink);
}

void loop() 
{
    cmd.poll();

    digitalWrite(pinLed, HIGH);
    delay(delayTime);
    digitalWrite(pinLed, LOW);
    delay(delayTime);        
}

void cmdBlink(int argCnt, char **args)
{
    delayTime = cmd.conv(args[1]);    
}
