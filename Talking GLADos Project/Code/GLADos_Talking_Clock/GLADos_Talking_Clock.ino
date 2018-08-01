

#include <SdFat.h>
SdFat sd;

#define SD_ChipSelectPin 4  //use digital pin 4 on arduino Uno, nano etc, or can use other pins
#include <TMRpcm.h>          //  also need to include this library...
#include <SPI.h> //General Serial_ comms
#include <DS3231.h>  //RTC
#include <LiquidCrystal_I2C.h> //LCD I2C library

TMRpcm audio;   // create an object for use in this sketch
DS3231  rtc(SDA, SCL); //Init rtc object with SDA (4) and SCL (5) pins
LiquidCrystal_I2C lcd(0x27, 16, 2); 

int analogpin = A0;
int ledR = 3;
int ledG = 5;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

Time t;
int value = 0;
String Song;
int trackselect;
char buff[5];
void setup(){

  randomSeed(analogRead(A2));
  //________________________________________________________

  pinMode(ledR, OUTPUT);  
  pinMode(ledG, OUTPUT);  
  pinMode(analogpin, INPUT);
  //________________________________________________________
  lcd.begin();
  lcd.backlight();
  
  //________________________________________________________
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time

  //________________________________________________________
  audio.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(12,OUTPUT); //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45  
  digitalWrite(12, HIGH);
  
  Serial.begin(115200);
  
  if (!sd.begin(4, SPI_FULL_SPEED)) {
    Serial.println("BAD");
    return; 
  }
  else{ Serial.println("SD OK"); }
    
}

void loop(){  
t = rtc.getTime();
lcd.print("Now ");
lcd.print(t.hour); //Some string
lcd.print(":"); //Some string
lcd.print(t.min); //Some string
lcd.print(" ");
lcd.print(rtc.getTemp());
lcd.print("F");
lcd.setCursor(0,1);
lcd.print("On ");
lcd.print(rtc.getMonthStr(FORMAT_SHORT));
lcd.print(" ");
lcd.print(t.date);
lcd.print(",");
lcd.print(t.year);

//Serial.println(rtc.getTimeStr());
//Serial.print(rtc.getDateStr());
//Serial.print(rtc.getDOWStr());
if(audio.isPlaying() == 1){
  ledresponse();
}
else{
  if(value != 0)
  {
    value = 0; 
    analogWrite(ledR, 0);
    analogWrite(ledG, 0);
    Serial.println("What");
    
  }
  if(t.sec == 0)
  { 
    trackselect = random(1,12); //Pick a random number
    Song = String(trackselect) + ".wav";  //Add that number to a string with .wav
      Song.toCharArray(buff, Song.length()+1); //Make that string a char array 
      //this function ^ does NOT account for the null terminator, hence the +1
      audio.play(buff); //Play that song (1-12).wav as a char array
  }
  else
  {
    delay(100);
  }
}

lcd.clear();
}

void ledresponse(){
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(analogpin);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  value = (432*volts) - 90;
  if(value < 0){
    value = 0;
  }
  analogWrite(ledR, value);
  analogWrite(ledG, value);
  Serial.println(volts);
}
