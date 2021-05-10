#include <cmdArduino.h>
#include <LowPower.h>

#define DEBOUNCE_DELAY 200

int intpButton = 2;
volatile int flagButton = false;

void setup() 
{
    cmd.begin(57600);
    Serial.println("Lab 5a - Power Management - How Low Can You Go");

    attachInterrupt(intpButton, isrButton, FALLING);

    cmd.add("sleep", cmdSleep);
}

void loop() 
{
    cmd.poll();

    if (flagButton)
    {
        delay(DEBOUNCE_DELAY);
        flagButton = false;
        Serial.println("The button was pushed.");
    }
}

void cmdSleep(int argCnt, char **args)
{
    Serial.println("Going to sleep now...zzzz");
    delay(100);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);
    Serial.println("Waking up now....");
}

void isrButton()
{
    flagButton = true;
}
