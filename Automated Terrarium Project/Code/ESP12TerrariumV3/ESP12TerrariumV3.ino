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
#define OPT3001_ADDRESS 0x45 //Address changed to 0x45 from 0x44 (ADDR tied to GND rather than VCC)
/************************************************
 * DS3231 Real Time clock (RTC) setup 
 */
DS3231 Clock;
/************************************************
 * WS2812 RGB LED setup
 */
#define LED_PIN 4
#define LED_COUNT 4
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
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
int Motor = D7;

void setup() {
  Wire.begin();
  opt3001.begin(OPT3001_ADDRESS);
  Serial.print("OPT3001 Manufacturer ID");
  Serial.println(opt3001.readManufacturerID());
  Serial.print("OPT3001 Device ID");
  Serial.println(opt3001.readDeviceID());
  configureSensor();
  printResult("High-Limit", opt3001.readHighLimit());
  printResult("Low-Limit", opt3001.readLowLimit());

  pinMode(Motor, OUTPUT); 

}

void loop() {
  OPT3001 result = opt3001.readResult(); //To get the lux value, call result.lux 
  //To get the error code (if any) call result.error. Check this before checking
  
  

}

void configureSensor() {
  OPT3001_Config newConfig;
  
  newConfig.RangeNumber = B1100;  
  newConfig.ConvertionTime = B0;
  newConfig.Latch = B1;
  newConfig.ModeOfConversionOperation = B11;

  OPT3001_ErrorCode errorConfig = opt3001.writeConfig(newConfig);
  if (errorConfig != NO_ERROR)
    printError("OPT3001 configuration", errorConfig);
  else {
    OPT3001_Config sensorConfig = opt3001.readConfig();
    Serial.println("OPT3001 Current Config:");
    Serial.println("------------------------------");
    
    Serial.print("Conversion ready (R):");
    Serial.println(sensorConfig.ConversionReady,HEX);

    Serial.print("Conversion time (R/W):");
    Serial.println(sensorConfig.ConvertionTime, HEX);

    Serial.print("Fault count field (R/W):");
    Serial.println(sensorConfig.FaultCount, HEX);

    Serial.print("Flag high field (R-only):");
    Serial.println(sensorConfig.FlagHigh, HEX);

    Serial.print("Flag low field (R-only):");
    Serial.println(sensorConfig.FlagLow, HEX);

    Serial.print("Latch field (R/W):");
    Serial.println(sensorConfig.Latch, HEX);

    Serial.print("Mask exponent field (R/W):");
    Serial.println(sensorConfig.MaskExponent, HEX);

    Serial.print("Mode of conversion operation (R/W):");
    Serial.println(sensorConfig.ModeOfConversionOperation, HEX);

    Serial.print("Polarity field (R/W):");
    Serial.println(sensorConfig.Polarity, HEX);

    Serial.print("Overflow flag (R-only):");
    Serial.println(sensorConfig.OverflowFlag, HEX);

    Serial.print("Range number (R/W):");
    Serial.println(sensorConfig.RangeNumber, HEX);

    Serial.println("------------------------------");
  }
  
}

void change_light(int val[]) //Expects a 4 value array of hex color values
{
  pixels.clear()
  for (int i = 0; i < 4; i++)
  {
    pixels.setPixelColor(i , pixels.Color(val[i] & 0x0000FF, val[i] & 0x00FF00, val[i] & 0xFF0000));
  }
}

void activate_pump()
{
  int Ts = millis(); 
  while(analogRead(moisture) > 550 && millis() - Ts =< 6000)
  {
    digitalWrite(motor, HIGH);
  }
  digitalWrite(motor, LOW);
}
