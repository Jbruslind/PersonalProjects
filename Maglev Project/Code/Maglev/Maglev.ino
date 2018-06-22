int Halleffect = A0; // Hall Effect Sensor Pin
int pwmPin = 3; // Pin being used to switch the transistor circuit
int levitPoint = 585; // Value to have our magnet levitate around, after a couple tuning sessions 590-630 are pretty good ranges for the setpoint
int outputsig = A1;  

#define Filter_setting 4

int Sense_VAL = 0;
int Changeinset = 15;
int Max_levitation, Minimum_levitation;
int filter_input;
int filter_output;
long filter_reg;
int induction = 0; //initially set our PWM duty cycle to be 0 (off) 

void setup() {
  
  //Serial.begin(9600);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20); //PWM direct controller commands for AVR chips (_BV sets the value as a binary value, COM2An and COM2Bn set inverter to TRUE or FALSE, WGM sets the PWM waveform setting to things such as FAST PWM, Phase Correct PWM, etc) 
  //TCCR2B = _BV(CS20); //CS = Clock select, sets the prescaler value for the internal hardware clock for the 3 and 11 digital pins. 20 means SYS_CLK/8 = 16Mhz /8 = ~2 MHz) 

  Max_levitation = levitPoint - Changeinset; //Set the maximum leviation point we will tolerate (so our setpoint - value = lowest point we want our magnet to go to
  Minimum_levitation = levitPoint + Changeinset;

  pinMode(pwmPin, OUTPUT); //Setup our output pin for the switching circuit as an output
  pinMode(outputsig, OUTPUT); 
}

void loop() {
  filter_input = analogRead(Halleffect); //Take measurment of the hall effect sensor 
  //Low pass filter implementation to smooth values coming from Hall effect sensor
  filter_reg = filter_reg - (filter_reg >> Filter_setting) + filter_input; //Update the filter with the current sensor values
  Sense_VAL = filter_reg >> Filter_setting; // Scale the output 

  if (Sense_VAL < levitPoint - 50) { //If our sensor value fall below our setpoint - 50, then activate the electromagnet (send duty cycle 255 which is out of 255 so 100%) 
    induction = 255;
  } else {
    if (Sense_VAL > Max_levitation) induction = 0; //if it turns out our value is greater than what our maximum levitation point is, turn off the electromagnet 
    if (Sense_VAL < Minimum_levitation) induction = 255; //If our value is less than our minimum levitation value, turn on the electromagnet 
    if (Sense_VAL <= Max_levitation and Sense_VAL >= Minimum_levitation) induction = ((Sense_VAL - Max_levitation)/5); //If our value is somewhere between our max/min points, then set the duty cycle of our enable pin to be partially on using our setpoint - max / 5
    // This bascically ensures if we're between our max/min points, we want the eltromagnet to be proactive and try to attract the magnet some of the time and as it gets closer to our max point, we want it to start turning off
    //Effectivly we are controlling the strength of our electromagnet
  }
  //Serial.println(filter_input);
  OCR2B = induction; //Set the PWM pin 3 to be the duty cycle we specified (0 - 255, 0% - 100% on) 
  analogWrite(outputsig, filter_input); //Output value as an analog value for the ADC/FPGA system to measure. 
}
