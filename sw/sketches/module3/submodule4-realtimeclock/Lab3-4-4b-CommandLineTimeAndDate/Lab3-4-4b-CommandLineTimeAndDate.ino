#include <Rtc_Pcf8563.h>
#include <cmdArduino.h>

Rtc_Pcf8563 rtc;

void setup() 
{
    cmd.begin(57600);
    Serial.println("Module 3, Lab 4b - Command Line Time and Date");

    cmd.add("settime", cmdSetTime);
    cmd.add("setdate", cmdSetDate);
    cmd.add("gettime", cmdGetTime);
    cmd.add("getdate", cmdGetDate);
}

void loop() 
{
    cmd.poll();
}

void cmdSetTime(int argCnt, char **args)
{
    int hour = cmd.conv(args[1]);
    int minute = cmd.conv(args[2]);
    int second = cmd.conv(args[3]);
    
    rtc.setTime(hour, minute, second);
    Serial.println(rtc.formatTime());    
}

void cmdSetDate(int argCnt, char **args)
{
    int year = cmd.conv(args[1]);
    int month = cmd.conv(args[2]);
    int day = cmd.conv(args[3]);
    
    rtc.setDate(day, 0, month, 0, year);
    Serial.println(rtc.formatDate());
}

void cmdGetTime(int argCnt, char **args)
{
    Serial.println(rtc.formatTime()); 
}

void cmdGetDate(int argCnt, char **args)
{
    Serial.println(rtc.formatDate());
}
