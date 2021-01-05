#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;

void setup() 
{
    Serial.begin(57600);
    Serial.println("Module 3, Lab 4a - What Time Is It?");
    
    rtc.setDate(17, 4, 12, 0, 20);
    Serial.println(rtc.formatDate());

    rtc.setTime(22, 37, 0);
    Serial.println(rtc.formatTime());
}

void loop() 
{

}
