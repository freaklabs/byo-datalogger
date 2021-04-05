#include <cmdArduino.h>
#include <SdFat.h>
#include <Rtc_Pcf8563.h>
#include <DHT.h>

// logfile
#define LOGFILE "LOGFILE.TXT"

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

// create objects
SdFat sd;
File myFile;
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
    Serial.println("Module 4-0: Tying It All Together");

    // for testing only
    cmd.add("fread", cmdFileRead);
    cmd.add("ls", cmdFileList);
    cmd.add("rm", cmdFileDelete);

    // initialize peripherals that need it
    sd.begin(pinSD);
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
        logData(TIMER);
        rtc.resetTimer();
        Serial.println("Timer interrupt received.");
    }

    // handle PIR interrupt
    if (flagPIR == 1)
    {
        flagPIR = 0;
        logData(MOTION);
        Serial.println("Motion interrupt received.");
    }

    // handle button interrupt
    if (flagButton == 1)
    {
        // debounce button
        delay(DEBOUNCE_TIME);
        flagButton = 0;
        logData(BUTTON);
        Serial.println("Button interrupt received.");
    }
}

// handle logging data
void logData(int type)
{   
    myFile = sd.open(LOGFILE, O_WRITE | O_CREAT | O_APPEND);
    myFile.print(rtc.formatDate());
    myFile.print(",");
    myFile.print(rtc.formatTime());
    myFile.print(",");
    myFile.print(dht.readTemperature());
    myFile.print(",");
    myFile.print(dht.readHumidity());
    myFile.print(",");
    myFile.print(getBatteryVoltage());

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
    Serial.println("Data logged to SD Card.");
}

float getBatteryVoltage()
{
    int battAdc = analogRead(pinBatt);
    float voltsPerAdcUnit = ADC_REF_VOLTAGE / ADC_LEVELS;   // this is the voltage per ADC unit
    float pinVoltage = battAdc * voltsPerAdcUnit;           // this is the voltage at the pin
    float battVoltage = pinVoltage * ADC_SCALE_FACTOR;      // this is the voltage at the battery
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
    while(myFile.available() > 0)
    {
        char c = myFile.read();
        Serial.write(c);
    }       
    myFile.close();
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
