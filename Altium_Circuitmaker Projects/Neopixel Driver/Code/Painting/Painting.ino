#include <Adafruit_NeoPixel.h>
#include <Wire.h>
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS1307 rtc;


#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

struct LED_ {
  int delay_t;
  int fade_t;
  unsigned long prev_t; 
  float brightness; 
  bool delay_on;
};

struct LED_ l_array [NUMPIXELS]; 

unsigned long previousMillis = 0;        // will store last time LED was updated


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
  pixels.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  randomSeed(analogRead(0));

  for(int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(255,255,255));
    pixels.show();
  }
  

  if (! rtc.begin()) {
    //Serial.println("Couldn't find RTC");
    //Serial.flush();
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // red
    pixels.show();
    //abort();
  }
  pixels.setPixelColor(0, pixels.Color(0,255,0)); // Green
  pixels.show();
  if (! rtc.isrunning()) {
    pixels.setPixelColor(0, pixels.Color(255,255,0)); // Green
    pixels.show();
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

}

void loop() {
//
//  unsigned long currentMillis = millis();
//  
//  for(int i = 0; i < NUMPIXELS; i++)
//  {
//    
//    pixels.setPixelColor(i, pixels.Color(l_array[i].brightness, l_array[i].brightness, l_array[i].brightness));
//  }
//  pixels.show();

  DateTime now = rtc.now();

  if(now.hour() == '5')
  {
    pixels.setPixelColor(0, pixels.Color(255,0,255)); // Pink
  }
  
  pixels.show();
  delay(1000);
}

void LED_Done(struct LED_ temp)
{
  temp.brightness = 0; 
  temp.fade_t = random(500, 1000);
  temp.prev_t = millis(); //reset counter for the delay timer
  temp.delay_t = random(500, 4000);
}
