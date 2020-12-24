# Valentine's Day

This was made for Valentine's day for someone special. 

The basic idea is that you can use a thermistor to determine the temperature of the PCB and use that information to change some characteristics. In this case 
I used an Attiny85 to control a NeoPixel (WS2812) by reading the voltage value of a thermistor. The entire thing is powered by a lithium coin cell battery
which can be recharged through a MCP73831T-2ACI/OT circuit and USB-C connector. The thermistor is placed near the center so that when you place your finger 
or heat near the heart the Attiny will cause the NeoPixel to beat red (an increases the frequency as the temp increases). This mechanism makes it so that 
when you hold the heart with your hand it will slowly increase its "heart beats". 

![CiruitMaker Render Top](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Valentines_top.png)

![CiruitMaker Render Bot](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Valentines_bot.png)


The Attiny was programmed through a set of 6 ISP headers that are located in the center of the board
## Issues 

Unfortunatly there were 3 components that had mismatching footprints on the PCB. The battery holder footprint and Attiny85 footprints were too small and the
USB-C footprint needed to be offset from the board so that the connector could hang off the side apparently (I was pretty annoyed with that since it didn't 
say anywhere that this was a requirements in the docs). The result was that the USB C connector pins wouldn't reach the actual board and couldn't be soldered. 
In my haste to design this (this was during the Coronavirus outbreak, meaning I needed to order this as soon as I could to avoid delays) I also 
forgot to actually connect the battery voltage to the Attiny/NeoPixel... So I had to bridge a connection to that through a small jumper. 
In addition, I did not label the ISP pins so I had to keep referencing the schematic to make sure I was plugging the programming pins in correctly.  

## Battery Holder
The battery I was able to create a battery holder model that I held in with some bare wire. What I found out was that the negative portion was 
the pad in the center of the footprint and the positive side was on the "wing" pads on the sides. What I decided to do was to create my own battery holder 
using a 3D printed model and to make electrical connection with the board I would place a small conductive spring in the center (and solder it to the 
pad) so that there was an upward force on the battery. This would be countered by a bare wire above the holder which was soldered to the positive end. The
end result was a battery suspended by some wire and held in place that also made connection to the correct pads. (it looks awful, but the recipient said she didn't mind). 

![Inventor Model of holder](https://github.com/Jbruslind/PersonalProjects/blob/master/Reference_Pictures/Valentines_Battery_holder.png)


The Attiny footprint I just had to very carefully solder bridge the pins inward (by bending the small pins on the chip inward towards the pads). This 
process took around an hour and was fairly frustrating, but I ended up getting everything attached. 

## Standard Library 

## Notes 

### Programming the Attiny85

Something I realized while playing around with this design is that you need to set up the Attiny 85 before actually running your code. This 
means you need to flash the bootloader to the chip before any programming can be done. As far as I understand, this inital flashing will set the fuses
in the microcontroller which will enable/disable features such as clock frequency, pin usages, etc. 

Overall it's pretty easy to use, I found a simple 