#	PCB Projects

 

##	Robotics Ckub Related 

These projects include those that were made specifically for the OSU Robotics Club 

### Business Cards

I saw a creator on Circuitmaker had made PCBs in the form of business cards and thought the idea would be a great idea to promote the club (as well as my own 
skills). So I created a simple design that would emulate that. The end result is shown below: 


All the design files include the schematics (none are electrically useful) and the gerbers to order them from a manufacturer. 

### Circuits For Kids

This project was actually really fun and I'm glad I decided to do it. While in the OSU Robotics Club I would particiapate/host a variety of kids events 
focused on getting the younger students excited about technology and the possibility of robotics. With that in mind I created a few custom PCBs that would illustrate
certain skills or ideas. 

#### Fruit Piano

The Fruit Piano project is based on the Makey Makey product line (found [here](https://makeymakey.com/))
in which you can use the human body as a resistive element in a circuit to produce sound based on touch. The general idea is that you would connect the 
pads surrounding the circuit board to various fruits or otherwise high water content/conductive materials, have someone hold a grounding wire (placed on 
the arduino wire or external source) and have them complete the circuit between the elements. The result would be the arduino would detect this voltage 
change (based on the voltage pull down) and would send a specific sound frequency to the output headers. This could be amplified by a speaker and you could 
play notes as if using a keyboard. 

![CiruitMaker Render](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Fruit_Piano.png)

The project ended up being a huge hit with kids, who really enjoyed mashing fruit to play notes. 

#### Issues 

I neglected to put an actual dedicated ground connection on the board and so the only way to connect to ground is the place a wire on the arduino ISP header pins. 

The pads were also very difficult to hook up alligator clips (any movement would cause continuity issues) so for the next version it would be good to 
include some kind of dedicated mounting spot for alligator clips to hook on to. 

#### RGB LEDS V2

The RGB LEDS project was made to teach kids how different currents could cause different colors to be shown in an RGB LED. Basically that by using different 
resistors, you cause different currents which in turn cause the specific colors to dim/brighten which would change the overall color. 

The basic idea was to have kids plug jumper wires from the resistor outputs to the LED inputs and cause different colors to appear. Once they figured out 
how the basic elements worked, we would have them create specific colors for a prize (ex: create the color Orange (high red, med green, no blue). 

#### 7 Segment Display

Similar to the 7 Segment display PCB in that you could plug in jumper wires from an output side to the LED inputs to light up specific segments of a 7 segment
display. We would then challenge them to create a specific letter or phrase (such as OSU) to win a prize. 

### UROV 1819

This directory contains all the files for the production PCBS designed for the Underwater ROV 2018 - 2019 project. These included 
1. PDB (Power Distribution Board)
2. Power Conversion (48 - 12V)
3. Aux 
4. Aux2

### UROV 1920

This directory contains all the files for the production PCBS designed for the Underwater ROV 2019 - 2020 project. These included 
1. SODIMM Power Converter
2. SODIMM Power Motherboard
3. Aux 
4. Sensor Board

##	Personal

### IOT Current Sensor

### Robot Arm

### Valentine's Day

This was made for Valentine's day for someone special. 

The basic idea is that you can use a thermistor to determine the temperature of the PCB and use that information to change some characteristics. In this case 
I used an Attiny85 to control a NeoPixel (WS2812) by reading the voltage value of a thermistor. The entire thing is powered by a lithium coin cell battery
which can be recharged through a MCP73831T-2ACI/OT circuit and USB-C connector. The thermistor is placed near the center so that when you place your finger 
or heat near the heart the Attiny will cause the NeoPixel to beat red (an increases the frequency as the temp increases). This mechanism makes it so that 
when you hold the heart with your hand it will slowly increase its "heart beats". 

![CiruitMaker Render Top](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Valentines_top.png)

![CiruitMaker Render Bot](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Valentines_bot.png)


The Attiny was programmed through a set of 6 ISP headers that are located in the center of the board
#### Issues 

Unfortunatly there were 3 components that had mismatching footprints on the PCB. The battery holder footprint and Attiny85 footprints were too small and the
USB-C footprint needed to be offset from the board so that the connector could hang off the side apparently (I was pretty annoyed with that since it didn't 
say anywhere that this was a requirements in the docs). The result was that the USB C connector pins wouldn't reach the actual board and couldn't be soldered. 
In my haste to design this (this was during the Coronavirus outbreak, meaning I needed to order this as soon as I could to avoid delays) I also 
forgot to actually connect the battery voltage to the Attiny/NeoPixel... So I had to bridge a connection to that through a small jumper. 
In addition, I did not label the ISP pins so I had to keep referencing the schematic to make sure I was plugging the programming pins in correctly.  

#### Battery Holder
The battery I was able to create a battery holder model that I held in with some bare wire. What I found out was that the negative portion was 
the pad in the center of the footprint and the positive side was on the "wing" pads on the sides. What I decided to do was to create my own battery holder 
using a 3D printed model and to make electrical connection with the board I would place a small conductive spring in the center (and solder it to the 
pad) so that there was an upward force on the battery. This would be countered by a bare wire above the holder which was soldered to the positive end. The
end result was a battery suspended by some wire and held in place that also made connection to the correct pads. (it looks awful, but the recipient said she didn't mind). 

![Inventor Model of holder](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Valentines_Battery_holder.png)


The Attiny footprint I just had to very carefully solder bridge the pins inward (by bending the small pins on the chip inward towards the pads). This 
process took around an hour and was fairly frustrating, but I ended up getting everything attached. 

## Standard Library 