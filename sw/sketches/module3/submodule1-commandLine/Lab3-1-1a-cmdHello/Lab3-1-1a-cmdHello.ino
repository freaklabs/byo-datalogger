#include <cmdArduino.h>

void setup() 
{
    cmd.begin(57600);
    Serial.println("Lab 3-1-1a - cmdHello");

    cmd.add("hello", cmdHello);
}

void loop() 
{
    cmd.poll();
}

void cmdHello(int argCnt, char **args)
{
    Serial.println("Hello Command Line");
}
