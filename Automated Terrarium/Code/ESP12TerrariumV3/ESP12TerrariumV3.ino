#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>

#include <ClosedCube_OPT3001.h>

#include <Adafruit_NeoPixel.h>

#include <DS3231.h>

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
  DS3231 Clock;
  
  bool h12 = false;
  bool PM;
  bool Century=false;
  
  struct ts {
    uint8_t sec;         /* seconds */
    uint8_t min;         /* minutes */
    uint8_t hour;        /* hours */
    uint8_t mday;        /* day of the month */
    uint8_t mon;         /* month */
    int16_t year;        /* year */
    uint8_t wday;        /* day of the week */
    uint8_t yday;        /* day in the year */
    uint8_t isdst;       /* daylight saving time */
    uint8_t year_s;      /* year in short notation*/
};

  ts t;

  /*
  Serial.print(Clock.getYear(), DEC);

  Serial.print(Clock.getMonth(Century), DEC);

  Serial.print(Clock.getDate(), DEC);

  Serial.print(Clock.getDoW(), DEC);

  Serial.print(Clock.getHour(h12, PM), DEC);

  Serial.print(Clock.getMinute(), DEC);

  Serial.print(Clock.getSecond(), DEC);

  Clock.getTemperature()
  */
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
 

const char* ssid = "CasaDeJJ";
const char* password = "1104NW32ST";
const char* mqtt_server = "192.168.0.25";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0; //Store flag about last message recieved
char msg[50]; //array to store message to be sent
String Data; 
long lastReconnectAttempt = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

/************************************************
 * General Control
 */

int moisture = A0;
int Motor = D7;
bool motor_state = false;
float temp = 0;
float humidity = 0; 
int RGB_Status = 0;
int watering_hour = 12;

void setup() {
  CCT_RGB[0] = strip.Color(255, 147, 41); //Candle
  CCT_RGB[1] = strip.Color(255, 197, 143); //40W Tungsten
  CCT_RGB[2] = strip.Color(255, 214, 170); //100W Tungsten
  CCT_RGB[3] = strip.Color(255, 241, 224); //Halogen
  CCT_RGB[4] = strip.Color(255, 250, 244); //Carbon Arc
  CCT_RGB[5] = strip.Color(255, 255, 251); //High Noon Sun
  CCT_RGB[6] = strip.Color(255, 255, 255); //Direct Sunlight
  
  Wire.begin(2, 14);
 
  Serial.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  Clock.setClockMode(false); //Set clock to 24H (false) or 12H (true) mode
  //Clock.setHour(byte(15));
  
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
  
}

void loop() {
  client.loop();
  //theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  
  check_light(); //Control lighting and light detection
  check_temp(); //Control watering and temp/humidity detection
  check_Wifi(); //Connect to wifi (or ignore if already connected)
  getTime();
  //Serial.println(t.hour);
  publish_status();
  delay(1000);
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
  client.publish("Automated_Terrarium/light", String(result_light.lux).c_str());
  client.publish("Automated_Terrarium/motor_state", String(motor_state).c_str());
  client.publish("Automated_Terrarium/RGBstatus", String(CCT_RGB[RGB_Status]).c_str());
  client.publish("Automated_Terrarium/humidity", String(humidity).c_str());
  client.publish("Automated_Terrarium/temp", String(temp).c_str());
  client.publish("Automated_Terrarium/time", (String(t.hour) + ":" + String(t.min)).c_str());
 client.publish("Automated_Terrarium/watering_hour", String(watering_hour).c_str());
  
}

void check_light()
{
  result_light = opt3001.readResult(); //To get the lux value, call result.lux 
  //To get the error code (if any) call result.error. Check this before checking
}

void check_temp()
{
  temp = Clock.getTemperature();
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
   t.hour = Clock.getHour(h12, PM);
   t.min = Clock.getMinute();
   t.sec = Clock.getSecond();
   t.year = Clock.getYear();
   t.mon = Clock.getMonth(Century);
   t.mday = Clock.getDate();
   t.wday = Clock.getDoW();
   switch(t.hour){

    case 6:
       RGB_Status = 0; 
       break;
    case 7:
       RGB_Status = 1; 
       break;
    case 8:
       RGB_Status = 2; 
       break;
    case 9:
       RGB_Status = 3; 
       break;
    case 11:
       RGB_Status = 4; 
       break;
    case 12:
       RGB_Status = 5; 
       break;
    case 13:
       RGB_Status = 6; 
       break;   
    case 15:
       RGB_Status = 4; 
       break; 
    case 16:
       RGB_Status = 3; 
       break; 
    case 18:
       RGB_Status = 0; 
       break; 
    
   }

   if(t.hour > 19 || t.hour < 6)
   {
      strip.clear();
      strip.show();
   }
   else
   {
    for(int i = 0; i < 4; i ++)
    {
      strip.setPixelColor(i, CCT_RGB[RGB_Status]);
    }
    strip.show();
    
   }
   //Serial.println("time");
}
