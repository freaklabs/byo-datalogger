#include <SdFat.h>
#include <DHT.h>
#include <cmdArduino.h>

#define DHTTYPE     DHT11   
#define DHTPIN      A0

SdFat sd;
File myFile;
DHT dht(DHTPIN, DHTTYPE);

int pinSD = 10;

void setup() 
{
    cmd.begin(57600);
    Serial.println("Module 3, Lab 5b: Command FAT");

    // make sure your microSD card is plugged in!
    sd.begin(pinSD);
    dht.begin();

    cmd.add("fwrite", cmdFileWrite);
    cmd.add("fread", cmdFileRead);
    cmd.add("ls", cmdFileList);
    cmd.add("rm", cmdFileDelete); 
}

void loop() 
{
    cmd.poll();
}

void cmdFileRead(int argCnt, char **args)
{
    myFile = sd.open(args[1], O_READ);
    while(myFile.available() > 0)
    {
        char c = myFile.read();
        Serial.write(c);
    }       
    myFile.close();
}

void cmdFileWrite(int argCnt, char **args)
{
    myFile = sd.open(args[1], O_WRITE | O_CREAT | O_APPEND);

    myFile.print(dht.readTemperature());
    myFile.print(",");
    myFile.println(dht.readHumidity());
    myFile.close();
}

void cmdFileList(int argCnt, char **args)
{
    sd.ls();
}

void cmdFileDelete(int argCnt, char **args)
{
    sd.remove(args[1]);
    Serial.print(args[1]);
    Serial.println(" deleted.");
}
