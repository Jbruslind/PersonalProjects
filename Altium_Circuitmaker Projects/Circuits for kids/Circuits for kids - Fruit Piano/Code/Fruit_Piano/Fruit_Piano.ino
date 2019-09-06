#include <Adafruit_NeoPixel.h>

#define PIN        10 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8 // Popular NeoPixel ring size


float notes[] = {440,494,523,587,659,698,784,880};


int r_arr[] = {255,255,255,0,0,0,127,255};
int g_arr[] = {0,128,255,255,255,0,0,105};
int b_arr[] = {0,0,0,0,255,255,255,180};

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float note_ = 0;
  pixels.clear();

  for(int i = 2; i < 10; i++)
  {
    if(digitalRead(i) == 0)
    {
      note_= note_ + notes[i-2];
      pixels.setPixelColor(i-2, pixels.Color(r_arr[i-2], g_arr[i-2], b_arr[i-2]));
    }
    else
    {
      pixels.setPixelColor(i-2, pixels.Color(0, 0, 0)); 
    }

  }
  Serial.println(note_);
  tone(12, int(note_));
  pixels.show();
  delay(30);
}
