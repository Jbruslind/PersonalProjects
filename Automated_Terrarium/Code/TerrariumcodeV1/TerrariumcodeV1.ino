

#include <idDHT11.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <TimeLib.h> 
 //SCL = 5 SDA = 4
int watertime = 0;
bool threeweeks = true;
int idDHT11pin = 2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)
float Humidvalue = 0; 
float Tempvalue = 0;
int lightvalue = 0;
int motorcontrol = 4;
int lightpin_B = A1; 
int lightpin_G = 6; 
int lightpin_R = 3;  
int lightsensor = A0; 
int high = A7;
int motorstate = 0;
//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);
void setup() {
  // put your setup code here, to run once:
  pinMode(high, OUTPUT);
  digitalWrite(high, HIGH);
  //setTime(9, 30, 00, 6, 4, 2017);   //set the system time to 23h31m30s on 13Feb2009
  //RTC.set(now());
  pinMode(motorcontrol , OUTPUT);
  pinMode(lightpin_B , OUTPUT);
  pinMode(lightpin_R , OUTPUT);
  pinMode(lightpin_G , OUTPUT);
  pinMode(lightsensor , INPUT);
  digitalWrite(7, HIGH);
  digitalWrite(5, LOW);
Serial.begin(9600);
setSyncProvider(RTC.get);
 if(timeStatus() != timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");
  Serial.println("idDHT11 Example program");
  Serial.print("LIB version: ");
  Serial.println(IDDHT11LIB_VERSION);
  Serial.println("---------------");
  Lightadapt();
}

void dht11_wrapper() {
  DHT11.isrCallback();
}
void loop() 
{
  fixtemp();
  waterschedule();
 if(motorstate == 1) {
  for(int i = 0; i < 3; i++){
    digitalWrite(motorcontrol, HIGH);
     maincode();   
     Serial.println("water");
  }
  motorstate = 0;
  digitalWrite(motorcontrol, LOW);
  watertime = 1;
  
 }
 digitalWrite(motorcontrol, LOW);
 maincode();
 
}
 void maincode(){    
    time_t t = RTC.get();
Serial.print(hour(t));
Serial.print(":"); 
Serial.print(minute(t));
Serial.print(":");
Serial.print(second(t));
Serial.println(); 


if(hour(t) >= 20 || hour(t) < 6)
{
  Lightoff();
}
else
  {
    if(minute(t)%10 == 0)
    {
      Lightadapt();
    }
 
} 

  switch(hour(t)) 
  {
  
    case(6):
    watertime = 0;
    break;
  }

DHT11.acquire();
  while (DHT11.acquiring())
    ;
  int result = DHT11.getStatus();
  switch (result)
  {
  case IDDHTLIB_OK: 
    Serial.println("OK"); 
    break;
  case IDDHTLIB_ERROR_CHECKSUM: 
    Serial.println("Error\n\r\tChecksum error"); 
    break;
  case IDDHTLIB_ERROR_ISR_TIMEOUT: 
    Serial.println("Error\n\r\tISR Time out error"); 
    break;
  case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
    Serial.println("Error\n\r\tResponse time out error"); 
    break;
  case IDDHTLIB_ERROR_DATA_TIMEOUT: 
    Serial.println("Error\n\r\tData time out error"); 
    break;
  case IDDHTLIB_ERROR_ACQUIRING: 
    Serial.println("Error\n\r\tAcquiring"); 
    break;
  case IDDHTLIB_ERROR_DELTA: 
    Serial.println("Error\n\r\tDelta time to small"); 
    break;
  case IDDHTLIB_ERROR_NOTSTARTED: 
    Serial.println("Error\n\r\tNot started"); 
    break;
  default: 
    Serial.println("Unknown error"); 
    break;
  } 
  
  Serial.print("Humidity (%): ");
  Serial.println(DHT11.getHumidity(), 2);
  Serial.println(DHT11.getHumidity());
 Serial.print("Temperature (oF): ");
  Serial.println(DHT11.getFahrenheit(), 2);
Serial.println("Lightvalue :");
Serial.print(analogRead(lightsensor));
Serial.println();
Serial.println("Watertime ");
Serial.println(watertime);
Serial.println("Weekday");
Serial.println(weekday(t));
Serial.println("Motorstate");
Serial.println(motorstate);


  delay(1000);
}


void Lightadapt() 
{
  int light = analogRead(lightsensor);
  Serial.println(light);
  int blue = map(light, 0, 1023, 255, 0);
  int green = blue;
  int red = blue;
  red = constrain(red, 0, 255);
  green = constrain(green, 0, 255);
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  analogWrite(lightpin_B, blue);
  analogWrite(lightpin_G , green);
  analogWrite(lightpin_R, red);
  
}
void Lightoff() 
{
    analogWrite(lightpin_B, 0);
    analogWrite(lightpin_G, 0);
    analogWrite(lightpin_R, 0);
}

void fixtemp() 
{  
  if(Tempvalue > 90 && Humidvalue < 50 && watertime < 1)
  {
    digitalWrite(motorcontrol, HIGH);
    motorstate = 1;
  }
  
}
void waterschedule() 
{
  time_t t = RTC.get();
  if(((weekday(t) == 1 || weekday(t) == 3 || weekday(t) == 5) && hour(t) == 15 ) && watertime < 1) 
  {
   digitalWrite(7, HIGH);
   digitalWrite(5, LOW);
   digitalWrite(motorcontrol, HIGH);
       motorstate = 1;
       //Serial.println("REGULARCODE");
  }
  else{}
}

