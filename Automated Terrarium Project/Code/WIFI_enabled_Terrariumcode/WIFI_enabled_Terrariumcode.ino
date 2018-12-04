//#include <idDHT11.h> Uses math functions that the ESP8266 12E board doesn't support
#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#include "DHTesp.h"

const char *ssid     = "CasaDeJJ";
const char *password = "1104NW32ST";
const char* mqtt_server = "192.168.0.17";



WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.0.17", 0 ,60000);

WiFiClient espClient; //Create an instance of the WifiClient Class
PubSubClient client(espClient); //Create an instance of the PUBSUBClient class

long lastMsg = 0; //store the last message number
char msg[50]; //Store the current message
String msg_out;
char* msg_out_char;

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
unsigned long status_timer = 0;
int interval = 5000;
int water_interval = 4000;
//Pin Values/Numbers
int motor = D7;
int B_light = D8;
int R_light = D5;
int G_light = D6;
int light_sens = A0;
// Instantiate the dht11 library/package

//int idDHT11pin = 2; //Digital pin for comunications
//int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
//void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
//idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);
//DHTesp dht;


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
client.setServer(mqtt_server, 1883);
client.setCallback(callback);

//dht.setup(idDHT11pin, DHTesp::DHT11); // Connect DHT sensor to GPIO 2

}

void loop() {
  unsigned long currentMillis = millis();
  if((unsigned long)(millis() - status_timer) >= interval)
  {
    update_status();
    status_timer = currentMillis;
  }
  if(!client.connected())
  {
    reconnect();
  }
  client.loop();
  if(timeClient.update()) //If we can reach the time server, go ahead and go with the rest of the code
  {
      if(day_ != timeClient.getDay()) //Change of day
    {
      daily_water = false; // reset the water flag
    }
    day_ = timeClient.getDay();
    if((day_ == 3 || day_ == 6) && (timeClient.getHours() == 9)) //If the day is Wed or Sat AND it's 9 in the morning OR it's too hot AND not humid, then we'll water the plant
    {
      motor_to_do = true;
      run_motor();  
    }
    light_adjust(); //Change the lights to be reflective of the current light
    status_msg = "ALL NORMAL";
  }
  else
  {
    status_msg = "CANNOT TALK TO TIME SERVER";
  }

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
  if(timeClient.getHours() == 13)
  {
    set_lights(lightval, lightval, lightval);
  }
  if(timeClient.getHours() == 18)
  {
    set_lights(lightval, .5*lightval, lightval);
  }
  if(timeClient.getHours() > 18)
  {
    set_lights(0,0,0);
  }
}

//Set the various light values to specific colors
void set_lights(int R, int G, int B){  
  analogWrite(R_light, R);
  analogWrite(B_light, B);
  analogWrite(G_light, G);
}

/*Send a slew of MQTT messages with various topics (sensors, states) to the broker*/
void update_status()
{
  msg_out = Temp;
  client.publish("home/terrarium/temp", msg_out.c_str());
  msg_out = Humidity;
  client.publish("home/terrarium/humidity", msg_out.c_str());
  msg_out = lightval;
  client.publish("home/terrarium/lightval", msg_out.c_str());
  msg_out = motorstate; 
  client.publish("home/terrarium/motorstate", msg_out.c_str());
  msg_out = daily_water;
  client.publish("home/terrarium/daily_water", msg_out.c_str());
  msg_out = motor_to_do;
  client.publish("home/terrarium/motor_to_do", msg_out.c_str());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP-Terrarium";
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("wakeup", "Terrarium - Online");
      // ... and resubscribe
      //client.subscribe("outTopic/2");
      //client.subscribe("ESP1/R");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
