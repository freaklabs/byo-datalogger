
int intpNumPIR = 1;
int pinPIR = 3;
int pinLed = 4;

volatile int pirFlag = 0;

void setup() 
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);

    Serial.begin(57600);
    Serial.println("Module 3, Submodule 6, Lab 6B - PIR Sensor Interrupt");

    attachInterrupt(intpNumPIR, isrPIR, RISING);
}

void loop() 
{
    int val = digitalRead(pinPIR);
    digitalWrite(pinLed, val);
    
    if (pirFlag == 1)
    {
        pirFlag = 0;
        Serial.println("Motion detected.");
    }
}

void isrPIR()
{
    pirFlag = 1;
}
