#define DEBOUNCE_TIME 50

int intpButton = 2;
int pinLed = 4;
volatile int flagButton;

void setup() 
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);
    
    Serial.begin(57600);
    Serial.println("Module 3, Submodule 6, Lab 6a - Pushbutton Interrupt");

    attachInterrupt(intpButton, isrButton, FALLING);
}

void loop() 
{
    if (flagButton == 1)
    {     
        int ledVal = digitalRead(pinLed); 

        delay(DEBOUNCE_TIME);
        flagButton = 0; 
        
        if (ledVal == 0)
        {
            Serial.println("Button was pushed. LED is turned on.");
            digitalWrite(pinLed, HIGH);
        }
        else
        {
            Serial.println("Button was pushed. LED is turned off.");
            digitalWrite(pinLed, LOW);
        }
    }
}

void isrButton()
{
    flagButton = 1;
}
