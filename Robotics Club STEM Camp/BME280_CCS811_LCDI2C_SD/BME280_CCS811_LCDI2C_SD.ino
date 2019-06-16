#include <Wire.h> //<-Need for LCD
#include <LiquidCrystal_I2C.h> //<-Need for LCD
#include <SPI.h> //<-Need for SD card
#include <SD.h> //<-Need for SD card

#include <SparkFunBME280.h>
#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address
//Use I2C scan if for some reason it's not connecting. Use the address it says
//Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;
LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address

const int chipSelect = 4;

void setup()
{
  lcd.begin(16,2);               // initialize the lcd 
  lcd.setCursor(0,0); //go to beginning line
  Serial.begin(9600);
  Serial.println();
  Serial.println("Apply BME280 data to CCS811 for compensation.");

  Wire.begin();//initialize I2C bus

  //This begins the CCS811 sensor and prints error status of .begin()
  CCS811Core::status returnCode = myCCS811.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println("Problem with CCS811");
    Serial.println(returnCode);
  }
  else
  {
  Serial.println("CCS811 online");
  Serial.print("Initializing SD card...");
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
    }
  }

  //Initialize BME280
  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x77;
  myBME280.settings.runMode = 3; //Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  byte id = myBME280.begin(); //Returns ID of 0x60 if successful
  if (id != 0x60)
  {
    Serial.println("Problem with BME280");
  }
  else
  {
    Serial.println("BME280 online");
  }
}

void loop() //Just prints data to LCD or SD card. Comment out any sections if not needed
{
  lcd.clear(); //clear data for new stuff
  lcd.print("CO2");
  lcd.print(myCCS811.getCO2());
  lcd.print("|TVOC");
  lcd.print(myCCS811.getTVOC());
  lcd.setCursor(0,1);
  lcd.print(myBME280.readTempC());
  lcd.print("C|");
  lcd.print(myBME280.readFloatPressure(),4);
  lcd.print("Pa");

  File dataFile = SD.open("PressureData.txt", FILE_WRITE); //Write the same data to an SD card 
  if (dataFile) {
    dataFile.print(myCCS811.getCO2());
    dataFile.print(",");
    dataFile.print(myCCS811.getTVOC());
    dataFile.print(",");
    dataFile.print(myBME280.readTempC());
    dataFile.print(",");
    dataFile.println(myBME280.readFloatPressure());
    dataFile.close();
  }
  delay(500); //wait a small amount for data to be displayed
}
