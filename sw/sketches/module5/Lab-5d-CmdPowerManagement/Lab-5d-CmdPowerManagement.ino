/* 
This sketch is a bonus sketch that demonstrates how the command line and deployment code can be
integrated into the same application. In this way, you can debug the actual code you're deploying
in a safe way.
*/

#include <cmdArduino.h>
#include <LowPower.h>
#include <limits.h>

#define DEBOUNCE_DELAY 200

int intpButton = 2;
volatile int flagButton = false;

// these are for switching between normal mode and command mode
#define START_WAIT_TIME             5000         
#define CMD_MODE_TIMEOUT_LIMIT      5
#define ONE_MINUTE                  60000

bool normalMode = true;
uint32_t cmdModeTimeCnt;
uint32_t cmdModeTimeLimit = CMD_MODE_TIMEOUT_LIMIT * ONE_MINUTE;

/****************************************************************************/
// Setup
/****************************************************************************/
void setup() 
{
    cmd.begin(57600);
    Serial.println("Lab 5a - Power Management - How Low Can You Go");

    attachInterrupt(intpButton, isrButton, FALLING);

    cmd.add("sleep", cmdSleep);
    cmd.add("normal", cmdNormal);

    // wait for input to see if we go into command mode
    selectMode();
}


/****************************************************************************/
// Loop
/****************************************************************************/
void loop() 
{
    // handle interrupt notifications here
    if (flagButton)
    {
        delay(DEBOUNCE_DELAY);
        flagButton = false;
        Serial.println("The button was pushed.");
        Serial.flush(); // finish printing before sleep
    }

    if (normalMode)
    {
        // handle normal mode here
        // can add sleep code and power management code in here also.         
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);           
        Serial.println("Interrupt event detected. Waking now...");     
    }
    else
    {
        // it's important cmd.poll() is here. we don't want to be checking the command line
        // interface if nothing is plugged into serial/USB since we could get random noise data and have the
        // serial buffer overflow & crash system
        cmd.poll();
                
        // implement timeout in command line mode. That way, if someone forgets to go to normal mode, we won't stay
        // here forever
        if (elapsedTime(cmdModeTimeCnt) > cmdModeTimeLimit)
        {
            // after timeout, we give the user the option to re-enter command mode. If no input
            // we will fall into normal mode
            selectMode();
        }
    }
}

/****************************************************************************/
// ISRs
/****************************************************************************/
void isrButton()
{
    flagButton = true;
}

/****************************************************************************/
// Helper Functions
/****************************************************************************/
/********************************************************************/
//  selectMode - allows system to go into developer mode rather than normal operating mode
/********************************************************************/
void selectMode()
{   
    uint32_t now = millis();;
     
    Serial.println("Press 'c' to go into command mode");
    while (elapsedTime(now) < START_WAIT_TIME)
    {
      if (Serial.available() > 0)
      {
        if (Serial.read() == 'c')
        {
            normalMode = false;
            Serial.println("Type 'normal' to go back to normal mode");
            Serial.print("Command line mode times out in ");
            Serial.print(CMD_MODE_TIMEOUT_LIMIT);
            Serial.println(" minutes.");
            cmdModeTimeCnt = millis();
            return;
        }
      }
    }
    normalMode = true;
    Serial.println("Going into normal operation mode.");
    Serial.flush();     // finish printing before sleep
}

/************************************************************************/
// elapsedTime - calculates time elapsed from startTime
// startTime : time to start calculating
/************************************************************************/
uint32_t elapsedTime(uint32_t startTime)
{
  uint32_t stopTime = millis();
  
  if (stopTime >= startTime)
  {
    return stopTime - startTime;
  }
  else
  {
    return (ULONG_MAX - (startTime - stopTime));
  }
}


/****************************************************************************/
// Commands
/****************************************************************************/
void cmdSleep(int argCnt, char **args)
{
    Serial.println("Going to sleep now...zzzz");
    delay(100);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);
    Serial.println("Waking up now....");
}

void cmdNormal(int argCnt, char **args)
{
    normalMode = true;
    Serial.flush(); 
}
