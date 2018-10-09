#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>

int motor = D0;

const char *ssid     = "CasaDeJJ";
const char *password = "1104NW32ST";

void setup() {
  // put your setup code here, to run once:
pinMode(motor, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
