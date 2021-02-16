#include <cmdArduino.h>
#include <SdFat.h>
#include <Rtc_Pcf8563.h>
#include <DHT.h>

// logfile
#define LOGFILE "LOGFILE.TXT"
#define ERRORFILE "ERRFILE.TXT"

// ADC
#define ADC_REF_VOLTAGE 3.3     
#define ADC_LEVELS 1024        
#define ADC_SCALE_FACTOR 2      

// pushbutton
#define DEBOUNCE_TIME 200

// sensor
#define DHTTYPE     DHT11   
#define DHTPIN      A0

// timer intervals
#define TIMER_MINUTE 1
#define TIMER_HOUR  60

// interrupt sources
#define TIMER   0
#define MOTION  1
#define BUTTON  2

// error codes
#define ERROR_SD_INIT       2
#define ERROR_LOW_BATTERY   3
#define ERROR_FILE_OPEN     4
#define ERROR_SENSOR        5

// error code intervals
#define LED_FLASH_TIME      200
#define LED_OFF_TIME        1000

// for three alkaline batteries
#define THRESH_LOW_BATTERY  3.6

// create objects
SdFat sd;
File myFile;
File myErrorFile;
DHT dht(DHTPIN, DHTTYPE);
Rtc_Pcf8563 rtc;

// create and set pins
int pinSD = 10;
int pinLED = 4;
int pinBatt = A6;

// create and set interrupts
int intpNumRtc = 0;
int intpNumPIR = 1;
int intpButton = 2;

// create flags
volatile int flagTimer = 0;
volatile int flagPIR = 0;
volatile int flagButton = 0;

void setup() 
{
    // init LED
    pinMode(pinLED, OUTPUT);
    digitalWrite(pinLED, LOW);

    cmd.begin(57600);
    Serial.println("Module 4-1: Error Checking and Diagnostics");

    // for testing only
    cmd.add("fread", cmdFileRead);
    cmd.add("ls", cmdFileList);
    cmd.add("rm", cmdFileDelete);

    // init SD card
    if (sd.begin(pinSD) != true)
    {
        while(1)
        {
            // SD card not present or malfunctioned
            errorHandler(ERROR_SD_INIT);
        }
    }

    // init DHT sensor
    dht.begin();

    // initialize interrupts
    attachInterrupt(intpNumRtc, isrRtc, FALLING);
    attachInterrupt(intpNumPIR, isrPIR, RISING);
    attachInterrupt(intpButton, isrButton, FALLING);

   // misc
    myFile.dateTimeCallback(sdDateTime);
    rtc.setTimer(TIMER_MINUTE, TMR_1MIN, false);
}

void loop() 
{
    cmd.poll();
    
    // handle timer interrupt
    if (flagTimer == 1)
    {
        flagTimer = 0;
        Serial.println("Timer interrupt received.");
        logData(TIMER);
    }

    // handle PIR interrupt
    if (flagPIR == 1)
    {
        flagPIR = 0;
        Serial.println("Motion interrupt received.");
        logData(MOTION);
    }

    // handle button interrupt
    if (flagButton == 1)
    {
        // debounce button
        delay(DEBOUNCE_TIME);
        flagButton = 0;
        Serial.println("Button interrupt received.");
        logData(BUTTON);
    }
}

// handle logging data
void logData(int type)
{   
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();
    float voltage = getBatteryVoltage();

    if (isnan(temp))
    {
        errorHandler(ERROR_SENSOR);               
    }
    else if (isnan(humid))
    {
        errorHandler(ERROR_SENSOR);
    }
    
    // log data
    myFile = sd.open(LOGFILE, O_WRITE | O_CREAT | O_APPEND);
    if (myFile == true)
    {        
        myFile.print(rtc.formatDate());
        myFile.print(",");
        myFile.print(rtc.formatTime());
        myFile.print(",");
        myFile.print(temp);
        myFile.print(",");
        myFile.print(humid);
        myFile.print(",");
        myFile.print(voltage);
    
        if (type == TIMER)
        {
            myFile.println(",RTC");
        }
        else if (type == MOTION)
        {
            myFile.println(",PIR"); 
        }
        else if (type == BUTTON)
        {
            myFile.println(",BTN");
        }    
        myFile.close();    
    }
    else
    {
        errorHandler(ERROR_FILE_OPEN);
    }
}

// flash led error codes and log errors
void errorHandler(int error)
{
    // display error
    if (error == ERROR_SD_INIT)
    {
        // don't do anything. infinite loop
    }
    else if (error == ERROR_LOW_BATTERY)
    {
        Serial.println("ERROR - Low Battery.");
    }
    else if (error == ERROR_FILE_OPEN)
    {
        Serial.println("ERROR - File cannot be opened.");
    }
    else if (error == ERROR_SENSOR)
    {
        Serial.println("ERROR - Sensor malfunction.");
    }

    // flash error code
    for (int i=0; i<error; i++)
    {
        digitalWrite(pinLED, HIGH);
        delay(LED_FLASH_TIME);
        digitalWrite(pinLED, LOW);
        delay(LED_FLASH_TIME);
    }
    delay(LED_OFF_TIME);

    // write error to logfile
    if (error == ERROR_SD_INIT)
    {
        return;
    }
    else
    {
        myErrorFile = sd.open(ERRORFILE, O_WRITE | O_CREAT | O_APPEND);
        if (myErrorFile == true)
        {
            myErrorFile.print(rtc.formatDate());
            myErrorFile.print(",");
            myErrorFile.print(rtc.formatTime());
            myErrorFile.print(",");
            if (error == ERROR_LOW_BATTERY)
            {
                myErrorFile.println("Error - Low battery event.");
            }
            else if (error == ERROR_FILE_OPEN)
            {
               myErrorFile.println("Error - Logfile cannot be opened."); 
            }
            else if (error == ERROR_SENSOR)
            {
                myErrorFile.println("Error - Sensor malfunction."); 
            }
            myErrorFile.close();
        }
    }
}

float getBatteryVoltage()
{
    int battAdc = analogRead(pinBatt);
    float voltsPerAdcUnit = ADC_REF_VOLTAGE / ADC_LEVELS;   // this is the voltage per ADC unit
    float pinVoltage = battAdc * voltsPerAdcUnit;           // this is the voltage at the pin
    float battVoltage = pinVoltage * ADC_SCALE_FACTOR;      // this is the voltage at the battery

    if (battVoltage < THRESH_LOW_BATTERY)
    {
        errorHandler(ERROR_LOW_BATTERY);
    }

    return battVoltage;
}

// ISR for RTC
void isrRtc()
{
    flagTimer = 1;
}

// ISR for PIR motion sensor
void isrPIR()
{
    flagPIR = 1;
}

// ISR for pushbutton
void isrButton()
{
    flagButton = 1;
}

/**************************************************************************/
// Command functions
/**************************************************************************/
void cmdFileRead(int argCnt,char **args)
{
    myFile = sd.open(args[1], O_READ);
    if (myFile == true)
    {
        while(myFile.available() > 0)
        {
            char c = myFile.read();
            Serial.write(c);
        }       
        myFile.close();
    }
    else
    {
        errorHandler(ERROR_FILE_OPEN);
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
