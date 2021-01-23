#include <SdFat.h>

#define LOGFILE     "test.txt"

SdFat sd;
File myFile;

int pinSD = 10;

void setup() 
{
    Serial.begin(57600);
    Serial.println("Module 3, Lab 5a: Introduction to sdFat");

    sd.begin(pinSD);

    Serial.println("Opening LOGFILE...");
    myFile = sd.open(LOGFILE, O_WRITE | O_CREAT);

    Serial.println("Writing to LOGFILE...");
    myFile.println("Hello file world!");
    myFile.print("Pi is equal to ");
    myFile.println(3.14159);
    myFile.close();

    delay(1000);

    Serial.println("Reading from LOGFILE...");
    myFile = sd.open(LOGFILE, O_READ);
    while(myFile.available() > 0)
    {
        char c = myFile.read();
        Serial.print(c);
    }   
    myFile.close();
}

void loop() 
{

}
