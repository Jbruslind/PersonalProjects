#include <idDHT11.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>

//SCL = 5, SDA = 4
//Variables to store data
int idDHTT11pin = 2;
int idDHTT11intNumber = 0; 
float Humidity = 0; 
float Temp = 0;
int lightval = 0;
int motorstate = 0;
//Pin Values/Numbers
int motor = 7;
int B_light = 8;
int R_light = 5;
int G_light = 6;
int light_sens = A0;
// Instatiate the dht11 library/package
void dht11_wrapper();
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);
void setup() {
//Light pins are outputs
pinMode(R_light, OUTPUT);
pinMode(G_light, OUTPUT);
pinMode(B_light, OUTPUT);
//motor is an output
pinMode(motor, OUTPUT);
//Light sensor (photo resistor) is an input
pinMode(light_sens, INPUT);

}

void loop() {

  

}


//Functions to help visibility

/*We don't want to water whatever it is more than one time per day,
 * so after this get's called (for whatever reason, intentional or auto) we
 * set a flag to say the motor was called and keep a timer that will reset said  
 * flag until 24 hours have passed. 
 */
void run_motor(){
  
}

//Set the various light values to specific colors
void set_lights(int R, int G, int B){

}

/*Send a slew of MQTT messages with various topics (sensors, states) to the broker*/
void update_status(){
  
}

