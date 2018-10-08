//#include <idDHT11.h> Uses math functions that the ESP8266 12E board doesn't support
#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>

#include "DHTesp.h"

const char *ssid     = "CasaDeJJ";
const char *password = "1104NW32ST";


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.0.17", 0 ,60000);

//SCL = 5, SDA = 4
//Variables to store data
float Humidity = 0; 
float Temp = 0;
int lightval = 0;
bool motorstate = false;
bool daily_water = false;
bool motor_to_do = false;
int day_ = 0; //Sunday starts at 0
unsigned long previousMillis = 0;
int water_interval = 4000;
//Pin Values/Numbers
int motor = D7;
int B_light = D8;
int R_light = D5;
int G_light = D6;
int light_sens = A0;
// Instantiate the dht11 library/package

int idDHT11pin = D2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
//void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
//idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);
DHTesp dht;


String status_msg = "";



void setup() {
//Light pins are outputs
pinMode(R_light, OUTPUT);
pinMode(G_light, OUTPUT);
pinMode(B_light, OUTPUT);
//motor is an output
pinMode(motor, OUTPUT);
//Light sensor (photo resistor) is an input
pinMode(light_sens, INPUT);

WiFi.begin(ssid, password);
timeClient.begin();

dht.setup(idDHT11pin, DHTesp::DHT22); // Connect DHT sensor to GPIO 2

}

void loop() {
  if(timeClient.update()) //If we can reach the time server, go ahead and go with the rest of the code
  {
      if(day_ != timeClient.getDay()) //Change of day
    {
      daily_water = false; // reset the water flag
    }
    delay(dht.getMinimumSamplingPeriod());
    Humidity = dht.getHumidity();
    Temp = dht.getTemperature();
    day_ = timeClient.getDay();
    if((day_ == 3 || day_ == 6) && (timeClient.getHours() == 9) || (dht.toFahrenheit(Temp) >= 90 && Humidity <= 90)) //If the day is Wed or Sat AND it's 9 in the morning or it's too hot or not humid, then we'll water the plant
    {
      run_motor();  
    }
    light_adjust(); //Change the lights to be reflective of the current light
    status_msg = "ALL NORMAL";
    update_status();
  }
status_msg = "CANNOT TALK TO TIME SERVER";
update_status();

}


//Functions to help visibility

/*We don't want to water whatever it is more than one time per day,
 * so after this get's called (for whatever reason, intentional or auto) we
 * set a flag to say the motor was called and keep a timer that will reset said  
 * flag until 24 hours have passed. 
 */
void run_motor(){
  if(motorstate == true) //check if motor is running
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= water_interval) {  //check if the motor has exceeded or met it's timer
        digitalWrite(motor, LOW);     
    }
  }
  else
  {
    if(motor_to_do && !daily_water) //check if the motor is supposed to run AND it hasn't been watered today 
    {
      motorstate = true; //set motor flag
      daily_water = true; //set daily water flag 
      previousMillis = millis(); //start timer
      digitalWrite(motor, HIGH); //start motor
    }
  }
  
}

void light_adjust()
{
  lightval = analogRead(light_sens) + 100; //This should probably be changed to something a little more adaptive (read: smarter), but for right now it works
  if(timeClient.getHours() == 7)
  {
    set_lights(.6*lightval, lightval, .6*lightval);
  }
  if(timeClient.getHours() == 7)
  {
    set_lights(lightval, lightval, lightval);
  }
  if(timeClient.getHours() == 7)
  {
    set_lights(lightval, .5*lightval, lightval);
  }
}

//Set the various light values to specific colors
void set_lights(int R, int G, int B){  
  analogWrite(R_light, R);
  analogWrite(B_light, B);
  analogWrite(G_light, G);
}

/*Send a slew of MQTT messages with various topics (sensors, states) to the broker*/
void update_status(){
  
}

