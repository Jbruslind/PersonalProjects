#include <LCD5110_Graph.h>
#include <Wire.h>
#include <BH1750.h>

LCD5110 lcd(8,9,10,12,11);
extern unsigned char BigNumbers[];
extern uint8_t splash[];
extern uint8_t ui[];

BH1750 lightSensor;
String light;

void setup() {
 
 lightSensor.begin();
 lcd.InitLCD();
 lcd.setFont(BigNumbers);
 lcd.clrScr();
 lcd.drawBitmap(0, 0, splash, 84, 48);
 lcd.update();  
 delay(3000);
 
}

void loop() {
  
 int stringLength=0;
 
 uint16_t lux = lightSensor.readLightLevel();
 
 light = String(lux);
 stringLength = light.length();
 
 lcd.clrScr();
 lcd.drawBitmap(0, 0, ui, 84, 48);
 printLight(stringLength);
 lcd.update();
 
 delay(150);
}

void printLight(int length)
{
  switch(length)
  {
    case 1:  lcd.print(light,68,22); break;
    case 2:  lcd.print(light,54,22); break;
    case 3:  lcd.print(light,40,22); break;
    case 4:  lcd.print(light,26,22); break;
    case 5:  lcd.print(light,12,22); break;
    default:  lcd.print(light,0,22); break;
  }
}
