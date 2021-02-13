#include "EmonLib.h"
EnergyMonitor emon1;

#include <SoftwareSerial.h>
#define RX 10
#define TX 11

SoftwareSerial esp8266(RX, TX);

void setup()
{
  Serial.begin(9600);
  esp8266.begin(115200);
  emon1.current(1, 30);
  delay(15000);
}
void loop()
{
  double Irms = emon1.calcIrms(1480);
  if (esp8266.available())
  {
    Serial.println(esp8266.readStringUntil('\n'));
  }
  esp8266.print("cu value=" + String(Irms));
  esp8266.write('\n');
  Serial.write("send\n");
  delay(3000);
}
