#include "LowPower.h"
#include "EmonLib.h"
#include <SoftwareSerial.h>

EnergyMonitor emon1;
#define RX 10
#define TX 11

SoftwareSerial esp8266(RX, TX);
int interval = 3000;

void writeToESP(double reading)
{
  esp8266.print("cu value=" + String(reading));
  esp8266.write('\n');
  Serial.write("send\n");
}

void setup()
{
  Serial.begin(9600);
  esp8266.begin(115200);
  emon1.current(1, 30);
  delay(15000);
}
int invalidCount = 1;
int validCount = 1;
void loop()
{
  double I = emon1.calcIrms(1480);
  if (I < 1)
  {
    invalidCount++;
    validCount = 1;
    if (invalidCount >= 2)
    {
      if (invalidCount > 15)
      {
        writeToESP(0.0);
        invalidCount = 2;
      }
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      return;
    }
  }
  else
  {
    validCount++;
    invalidCount = 1;
  }
  if (validCount > 3 && invalidCount == 1)
  {
    if (esp8266.available())
    {
      Serial.println(esp8266.readStringUntil('\n'));
    }
    writeToESP(I);
  }
  else
  {
    writeToESP(0.0);
  }
  delay(interval);
}
