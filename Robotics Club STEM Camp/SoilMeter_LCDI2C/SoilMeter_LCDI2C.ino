#include <Wire.h> //Libraries needed for LCD
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address

const int sensor_pin = A1;  /* Soil moisture sensor O/P pin */

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Soil Moisture");
}

void loop() {
  // put your main code here, to run repeatedly:
  float moisture_percentage;
  int sensor_analog;
  lcd.clear();
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  lcd.print("Soil Moisture");
  lcd.setCursor(0, 1);
  Serial.print("Moisture Percentage = ");
  lcd.print(moisture_percentage);
  delay(1000);
}
