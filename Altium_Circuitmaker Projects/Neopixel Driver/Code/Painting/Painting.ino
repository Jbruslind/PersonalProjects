#include <tinyNeoPixel.h>

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

struct LED_ l_array [10]; 

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
  randomSeed(analogRead(0));

  pixels.setPixelColor(0, pixels.Color(255,255,255));
  pixels.setPixelColor(1, pixels.Color(255,255,255));
  pixels.setPixelColor(2, pixels.Color(255,255,255));
  pixels.setPixelColor(3, pixels.Color(50,0, 255));
  pixels.setPixelColor(4, pixels.Color(255,255,255));
  pixels.setPixelColor(5, pixels.Color(255,255,255));
  pixels.setPixelColor(6, pixels.Color(255,255,255));
  pixels.setPixelColor(7, pixels.Color(255,255,255));
  pixels.setPixelColor(8, pixels.Color(255,255,255));
  pixels.setPixelColor(9, pixels.Color(255,255,255));
  
  for(int i = 0; i < 10; i++)
  {
    l_array[i].delay_t = random(500, 4000);
    l_array[i].fade_t = random(500, 1000);
    l_array[i].prev_t = millis();
    l_array[i].brightness = 0;
    l_array[i].delay_on = true;
  }
}

void loop() {

  unsigned long currentMillis = millis();
  
  for(int i = 0; i < 10; i++)
  {
    if(currentMillis - l_array[i].prev_t <= l_array[i].delay_t && l_array[i].delay_on == true)
    {
      l_array[i].brightness = 0; 
      l_array[i].delay_on = false;
      l_array[i].prev_t = millis(); //reset counter for the fade timer
    }
    else if(currentMillis - l_array[i].prev_t >= l_array[i].fade_t && l_array[i].delay_on == false)
    {
      LED_Done(l_array[i]);
    }
    else if(l_array[i].delay_on == false)
    {
      unsigned long temp = currentMillis - l_array[i].prev_t; 
      float val = temp / l_array[i].fade_t;

      if(val >= 0.5)
      {
        l_array[i].brightness -= 255 / l_array[i].fade_t; 
      }
      else
      {
        l_array[i].brightness -= 255 / l_array[i].fade_t; 
      }
    }
    pixels.setPixelColor(i, pixels.Color(l_array[i].brightness, l_array[i].brightness, l_array[i].brightness));
  }
  pixels.show();
}

void LED_Done(struct LED_ temp)
{
  temp.brightness = 0; 
  temp.fade_t = random(500, 1000);
  temp.prev_t = millis(); //reset counter for the delay timer
  temp.delay_t = random(500, 4000);
}
