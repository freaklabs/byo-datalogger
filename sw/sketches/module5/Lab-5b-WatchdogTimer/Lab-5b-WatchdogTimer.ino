#include <cmdArduino.h>
#include <avr/wdt.h>

int pinLed = 4;

void setup() 
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);
    
    cmd.begin(57600);
    Serial.println("Lab 5b - Who Let the Watchdog Out?");
    Serial.println("Reset occurred.");

    cmd.add("wdtest", cmdWatchdogTest);

    wdt_enable(WDTO_8S);
}

void loop() 
{
    cmd.poll();
    wdt_reset();
}

void cmdWatchdogTest(int argCnt, char **args)
{
    digitalWrite(pinLed, HIGH);
    Serial.println("Testing Watchdog. About to simulate hang situation.");
    
    // hang here    
    while (1)
    {
        ;
    }
}
