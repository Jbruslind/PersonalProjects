
#include <tinyNeoPixel.h>
#include "SparkFun_LIS331.h"
#include <Wire.h>

LIS331 xl;

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

  pinMode(1,INPUT);       // Interrupt pin input
  Wire.begin();
  xl.setI2CAddr(0x19);    // This MUST be called BEFORE .begin() so 
                          //  .begin() can communicate with the chip
  xl.begin(LIS331::USE_I2C); // Selects the bus to be used and sets
                          //  the power up bit on the accelerometer.
                          //  Also zeroes out all accelerometer
                          //  registers that are user writable.
  // This next section configures an interrupt. It will cause pin
  //  INT1 on the accelerometer to go high when the absolute value
  //  of the reading on the Z-axis exceeds a certain level for a
  //  certain number of samples.
  xl.intSrcConfig(LIS331::INT_SRC, 1); // Select the source of the
                          //  signal which appears on pin INT1. In
                          //  this case, we want the corresponding
                          //  interrupt's status to appear. 
}

void loop() {
    //pulse_len = map(analogRead(2), 125, 250, 1, 10000);
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
