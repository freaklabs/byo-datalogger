#include <SdFat.h>
#include <Rtc_Pcf8563.h>
#include <DHT.h>
#include <cmdArduino.h>

#define DHTTYPE     DHT11   
#define DHTPIN      A0

SdFat sd;
File myFile;
DHT dht(DHTPIN, DHTTYPE);
Rtc_Pcf8563 rtc;

int pinSD = 10;
int pinLED = 4;

void setup() 
{
    pinMode(pinLED, OUTPUT);
    digitalWrite(pinLED, LOW);
    
    cmd.begin(57600);
    Serial.println("Module 3, Lab 5c: Manual Datalogger");

    if (sd.begin(pinSD) == false)
    {
        while(1)
        {
            digitalWrite(pinLED, HIGH);
            delay(1000);
            digitalWrite(pinLED, LOW);
            delay(1000);   
        }
    }
    
    dht.begin();

    cmd.add("fwrite", cmdFileWrite);
    cmd.add("fread", cmdFileRead);
    cmd.add("ls", cmdFileList);
    cmd.add("rm", cmdFileDelete);

    myFile.dateTimeCallback(sdDateTime);     
}

void loop() 
{
    cmd.poll();
}

void cmdFileRead(int argCnt, char **args)
{
    myFile = sd.open(args[1], O_READ);
    if (myFile == true)
    {
        while(myFile.available())
        {
            Serial.write(myFile.read());
        }       
        myFile.close();
    }
    else
    {
        Serial.println("Error opening file for reading.");
    }
}

void cmdFileWrite(int argCnt, char **args)
{
    myFile = sd.open(args[1], O_WRITE | O_CREAT | O_APPEND);
    if (myFile == true)
    {
        Serial.println("Writing to LOGFILE...");
        myFile.print(rtc.formatDate());
        myFile.print(",");
        myFile.print(rtc.formatTime());
        myFile.print(",");
        myFile.print(dht.readTemperature());
        myFile.print(",");
        myFile.println(dht.readHumidity());
        myFile.close();
    }
    else
    {
        Serial.println("Error opening file for writing.");
    }
}

void cmdFileList(int argCnt, char **args)
{
    sd.ls(LS_DATE);
}

void cmdFileDelete(int argCnt, char **args)
{
    sd.remove(args[1]);
    Serial.print(args[1]);
    Serial.println(" deleted.");
}

/**************************************************************************/
// 
/**************************************************************************/
void sdDateTime(unsigned int *date, unsigned int *time)
{
    rtc.getDateTime();

    // return date using FAT_DATE macro to format fields
    *date = FAT_DATE(2000+rtc.getYear(), rtc.getMonth(), rtc.getDay());

    // return time using FAT_TIME macro to format fields
    *time = FAT_TIME(rtc.getHour(), rtc.getMinute(), rtc.getSecond());
}
