#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>

int motor = D0;

const char *ssid     = "CasaDeJJ";
const char *password = "1104NW32ST";

int day_;
int hour_;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.0.17", 0 ,60000);

String apiKey= SECRET_APIKEY;
int status = WL_IDLE_STATUS;
char server[] = "api.openweathermap.org";
String location= "torino,IT";

void setup() {
  // put your setup code here, to run once:
pinMode(motor, OUTPUT);
WiFi.begin(ssid, password);
timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(timeClient.update())
  {
    day_ = timeClient.getDay();
  }
}
