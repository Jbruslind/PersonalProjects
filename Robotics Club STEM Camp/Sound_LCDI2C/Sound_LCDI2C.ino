#include <Wire.h> //Libraries needed for LCD
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address

void setup() {
  pinMode(A0, INPUT); //A0 as input for sound meter
  Serial.begin(9600);
  lcd.begin(16,2);               // initialize the lcd 
  lcd.setCursor(0,0); //go to beginning line
}
void loop() {
  int volume = analogRead(A0);
  Serial.println(volume); //print to serial monitor
  lcd.clear();
  lcd.print("Vol:");
  lcd.print(volume);
  delay(100); //If the screen is flashing, change this value to be larger
}
