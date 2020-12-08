#include <cmdArduino.h>

void setup() 
{
    cmd.begin(57600);
    Serial.println("Lab 3-1-1b - Command Line Argument List and RTC");

    cmd.add("list", cmdList);
}

void loop() 
{
    cmd.poll();
}

void cmdList(int argCnt, char **args)
{
    int i;
    for (i=0; i<argCnt; i++)
    {
        Serial.print("Arg ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(args[i]);
    }
}

void cmdSetDate(int argCnt, char **args)
{
    int year = cmd.conv(args[1]);
    int month = cmd.conv(args[2]);
    int day = cmd.conv(args[3]);

    Serial.print("The date is: ");
    Serial.print(year);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/");
    Serial.println(day);
}
