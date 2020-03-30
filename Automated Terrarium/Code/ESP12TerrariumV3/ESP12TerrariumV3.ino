#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>

#include <ClosedCube_OPT3001.h>

#include <Adafruit_NeoPixel.h>

#include <ds3231.h>

#include "DHTesp.h"

/************************************************
 * Light Sensor Setup
 */
  ClosedCube_OPT3001 opt3001;
  #define OPT3001_ADDRESS 0x44
  bool light_error = 0;
  OPT3001 result_light;
/************************************************
 * DS3231 Real Time clock (RTC) setup 
 */
  //I2C Comms are on SDA: GPIO2, SCL: GPIO14
  //CHANGE ON ALL I2C LIBRARIES
 
  ts t; //ts is a struct findable in ds3231.h

  //struct ts {
    //uint8_t sec;         /* seconds */
    //uint8_t min;         /* minutes */
    //uint8_t hour;        /* hours */
    //uint8_t mday;        /* day of the month */
    //uint8_t mon;         /* month */
    //int16_t year;        /* year */
    //uint8_t wday;        /* day of the week */
    //uint8_t yday;        /* day in the year */
    //uint8_t isdst;       /* daylight saving time */
    //uint8_t year_s;      /* year in short notation*/
/************************************************
 * WS2812 RGB LED setup
 */
  #define LED_PIN    4
  #define LED_COUNT 4
  Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

  uint32_t CCT_RGB [7] = {0,0,0,0,0,0,0};
  
/************************************************
 * Wifi Control Stuff
 */
 
char* ssid = "CasaDeJJ";
char* password = "1104NW32ST";

const char* mqtt_server = "192.168.0.25"; //Set ip of MQTT broker 

WiFiClient espClient;  //Initalize Wifi object
PubSubClient client(espClient); //Use wifi object to setup MQTT client
long lastMsg = 0; //Store flag about last message recieved
char msg[50]; //array to store message to be sent
String Data; 
long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

/************************************************
 * General Control
 */
DHTesp dht;
int moisture = A0;
int Motor = D7;
bool motor_state;
float temp;
float humidity; 

void setup() {
  CCT_RGB[0] = strip.Color(255, 147, 41); //Candle
  CCT_RGB[1] = strip.Color(255, 197, 143); //40W Tungsten
  CCT_RGB[2] = strip.Color(255, 214, 170); //100W Tungsten
  CCT_RGB[3] = strip.Color(255, 241, 224); //Halogen
  CCT_RGB[4] = strip.Color(255, 250, 244); //Carbon Arc
  CCT_RGB[5] = strip.Color(255, 255, 251); //High Noon Sun
  CCT_RGB[6] = strip.Color(255, 255, 255); //Direct Sunlight
  
  Wire.begin(14, 2);
 
  Serial.begin(9600);
 
  DS3231_init(DS3231_INTCN); //register the ds3231 (DS3231_INTCN is the default address of ds3231, this is set by macro for no performance loss)
  
  opt3001.begin(OPT3001_ADDRESS);
  Serial.print("OPT3001 Manufacturer ID");
  Serial.println(opt3001.readManufacturerID());
  Serial.print("OPT3001 Device ID");
  Serial.println(opt3001.readDeviceID());
  configureSensor();
  printResult("High-Limit", opt3001.readHighLimit());
  printResult("Low-Limit", opt3001.readLowLimit());
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  dht.setup(17, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  
  WiFi.begin(ssid, password);
}

void loop() {

  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  
  check_light(); //Control lighting and light detection
  check_temp(); //Control watering and temp/humidity detection
  check_Wifi(); //Connect to wifi (or ignore if already connected)

  publish_status();
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

void printResult(String text, OPT3001 result) {
  if (result.error == NO_ERROR) {
    Serial.print(text);
    Serial.print(": ");
    Serial.print(result.lux);
    Serial.println(" lux");
  }
  else {
    printError(text,result.error);
  }
}

void printError(String text, OPT3001_ErrorCode error) {
  Serial.print(text);
  Serial.print(": [ERROR] Code #");
  Serial.println(error);
}

void publish_status()
{
  client.publish("Automated_Terrarium/light", result_light.lux);
  client.publish("Automated_Terrarium/motor_state", result_light.lux);
  client.publish("Automated_Terrarium/RGBstatus", result_light.lux);
  //client.publish("Automated_Terrarium/humidity", result_light.lux);
  //client.publish("Automated_Terrarium/temp", result_light.lux);
  client.publish("Automated_Terrarium/time", ts.hour + ":" + ts.min);
  client.publish("Automated_Terrarium/watering_hour", result_light.lux);
}

void check_light()
{
  light_result = opt3001.readResult(); //To get the lux value, call result.lux 
  //To get the error code (if any) call result.error. Check this before checking
}

void check_temp()
{
  
}

void check_Wifi()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    WiFi.begin(ssid, password);
  }

  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Automated_Terrarium";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Automated_Terrarium/status", "Connected");
      // ... and resubscribe
      client.subscribe("Automated_Terrarium/input");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void getTime()
{
   DS3231_get(&t); //get the value and pass to the function the pointer to t, that make an lower memory fingerprint and faster execution than use return
}
