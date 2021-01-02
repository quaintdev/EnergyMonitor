
#include "EmonLib.h"
EnergyMonitor emon1;

#include <SoftwareSerial.h>
#define RX 10
#define TX 11

String AP = "SSID";
String PASS = "PASSWORD";
String HOST = "192.168.0.255";
String PORT = "7007";

double valSensor = 0.0;

SoftwareSerial esp8266(RX, TX);

double getSensorData()
{
  double Irms = emon1.calcIrms(1480);
  return Irms;
}

bool sendCommand(String command, char readReplay[])
{
  esp8266.println(command);
  esp8266.readStringUntil(*readReplay);
}

void setup()
{
  esp8266.begin(115200);
  Serial.begin(9600);
  sendCommand("AT", "OK");
  //sendCommand("AT+CWMODE=1", 5, "OK");
  //sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
  sendCommand("AT+CIPMUX=1", "OK");
  emon1.current(1, 33);
}

void loop()
{
  sendCommand("AT+CIPSTART=0,\"UDP\",\"" + HOST + "\"," + PORT, "OK");
  valSensor = getSensorData();
  String getData = "> I = " + String(valSensor) + ", P = " + String(valSensor * 230);
  int dataLength = getData.length() + 2;
  sendCommand("AT+CIPSEND=0," + String(dataLength), ">");
  esp8266.println(getData);
  sendCommand("AT+CIPCLOSE=0", "OK");
  delay(3000);
}
