/*
 * This sketch is an introductory sketch for blinking the LEDs on the Wildlogger 
 * board. 
 * 
 * The board has auxiliary LEDs on pins 4, 13, and 27.
 * 
 */

int pinLed = 13;

void setup() 
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);
}

// the loop function runs over and over again forever
void loop() 
{
    digitalWrite(pinLed, HIGH);     // turn LED on by setting pin HIGH
    delay(1000);                    // delay for 1000 milliseconds (1 second)
    digitalWrite(pinLed, LOW);     // turn LED off by setting pin LOW
    delay(1000);                    // delay for 1000 milliseconds (1 second)
}
