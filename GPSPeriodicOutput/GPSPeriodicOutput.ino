#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
/*
   This sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pin 13(TX from the GPS module to RX on the Arduino).
*/
static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gpsEngine;

// The serial connection to the GPS device (TXPin is available on the Sparkfun GP1818MK)
SoftwareSerial gpsPort(RXPin, TXPin);

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds

void setup()
{
  Serial.begin(115200);
  Serial.println("\nReady...");
  gpsPort.begin(GPSBaud);

  Serial.println(F("GPSPeriodicOutput.ino"));
  Serial.println(F("Outputs the location and date and time from the GPS module to the SerialMonitor"));
  Serial.println();
  startMillis = millis();  //initial start time
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsPort.available() > 0)
//    if (gpsEngine.encode(gpsPort.read()))
    gpsEngine.encode(gpsPort.read());

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    if (gpsEngine.location.isUpdated() || gpsEngine.time.isUpdated())
    {
      displayInfo();
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
  
  if (millis() > 8000 && gpsEngine.charsProcessed() < 100)
  {
    Serial.println(F("No GPS detected: check wiring and restart."));
    while(true);
  }
}

void displayInfo()
{
  bool  somethingDisplayed = false;
  Serial.println("in displayInfo");

  if(gpsEngine.location.isUpdated())
  {
    Serial.print(F("Location: ")); 
    if (gpsEngine.location.isValid()) 
    {
      Serial.print(gpsEngine.location.lat(), 6);
      Serial.print(F(","));
      Serial.print(gpsEngine.location.lng(), 6);
    }
    else
    {
      Serial.print(F("INVALID"));
    }
    Serial.print(F(" "));
    somethingDisplayed = true;
  }

  if (gpsEngine.time.isUpdated())
  {
    if (gpsEngine.time.isValid())
    {
      Serial.print(gpsEngine.date.month());
      Serial.print(F("/"));
      Serial.print(gpsEngine.date.day());
      Serial.print(F("/"));
      Serial.print(gpsEngine.date.year());

      Serial.print(F(" "));

      if (gpsEngine.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gpsEngine.time.hour());
      Serial.print(F(":"));
      if (gpsEngine.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gpsEngine.time.minute());
      Serial.print(F(":"));
      if (gpsEngine.time.second() < 10) Serial.print(F("0"));
      Serial.print(gpsEngine.time.second());
    }
    else
    {
      Serial.print(F("INVALID"));
    }
    somethingDisplayed = true;
  }
  if (somethingDisplayed)
    Serial.println();
}
