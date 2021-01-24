#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;    

int intpNumRtc = 0;
volatile int rtcFlag = 0;

void setup() 
{
    Serial.begin(57600);
    Serial.println("Module 3, Submodule 6, Lab 6c - RTC Interrupt");

    attachInterrupt(intpNumRtc, isrRtc, FALLING);
    rtc.setTimer(1, TMR_1MIN, false);
}

void loop() 
{
    if (rtcFlag == 1)
    {        
        rtcFlag = 0;        
        rtc.resetTimer();
        
        Serial.print("Timer interrupt: ");
        Serial.print(rtc.formatDate());
        Serial.print(", ");
        Serial.println(rtc.formatTime());       
    }
}

void isrRtc()
{
    rtcFlag = 1;
}
