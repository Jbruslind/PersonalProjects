#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>

#include <ClosedCube_OPT3001.h>

#include <Adafruit_NeoPixel.h>

#include <DS3231.h>

/************************************************
 * Light Sensor Setup
 */
ClosedCube_OPT3001 opt3001;
#define OPT3001_ADDRESS 0x45
/************************************************
 * DS3231 Real Time clock (RTC) setup 
 */
DS3231 Clock;
/************************************************
 * WS2812 RGB LED setup
 */
Adafruit_NeoPixel Array(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#define LED_PIN    6
#define LED_COUNT 4
/************************************************
 * Wifi Control Stuff
 */
 
const char* ssid = "billnyethewifi";
const char* password = "Robot123";

const char* mqtt_server = "192.168.8.101"; //Set ip of MQTT broker 

WiFiClient espClient;  //Initalize Wifi object
PubSubClient client(espClient); //Use wifi object to setup MQTT client
long lastMsg = 0; //Store flag about last message recieved
char msg[50]; //array to store message received
String Data; 
long lastReconnectAttempt = 0;

/************************************************
 * General Control
 */
int moisture = A0;
int Motor = D7

void setup() {
  Wire.begin();
  opt3001.begin(OPT3001_ADDRESS);
  configureSensor();

}

void loop() {
  OPT3001 result = opt3001.readResult(); //To get the lux value, call result.lux 
  //To get the error code (if any) call result.error. Check this before checking
  

}
