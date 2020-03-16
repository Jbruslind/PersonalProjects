
#include <tinyNeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int pulse_len = 500;
byte color = 0;

void setup() {
  pinMode(2, INPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
}

void loop() {
    pulse_len = map(analogRead(2), 125, 250, 1, 10000);
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(10);
    for(int i = 0; i < 200; i++)
    {
      pixels.setPixelColor(0, pixels.Color(i,0, 0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delayMicroseconds(pulse_len);
    }
    for(int j = 200; j > 0; j--)
    {
      pixels.setPixelColor(0, pixels.Color(j, 0, 0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delayMicroseconds(pulse_len);
    }
    
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
