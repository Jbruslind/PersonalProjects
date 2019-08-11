#	Inventor Project

I use Autodesk Inventor for all my 3D modeling needs and this directory holds all the projects that were either solely based in Inventor or had large 
parts done in Inventor. 

##	Infinity 
Infinity Stone pendants I made for some friends. Each one was designed by me with some model help from various sources (linked in the actual directory). 

The design revolves around the use of [Tritium](https://nuclearsafety.gc.ca/eng/pdfs/Fact_Sheets/January-2013-Fact-Sheet-Tritium_e.pdf) 
which is a radioactive isotope of hydrogen. It's not radioactive in the sense that it'll cause damage, 
just enough to cause some cool glowing effects. The radiation is low levels of Beta-particles and is not
damaging unless ingested (so just don't eat the vials). With it as a core I created geometric designs 
corresponding to each stone and their back story/ability. 

Here's a picture of the full assembly in light and dark. 

![Metal casings by themselves](https://github.com/Jbruslind/PersonalProjects/blob/master/Inventor%20Projects/Infinity/Pics/Infinity_light_none.jpg)

![In the dark after ~1s long exposure](https://github.com/Jbruslind/PersonalProjects/blob/master/Inventor%20Projects/Infinity/Pics/Infinity_dark_full.jpg)

![Inventor Models](https://github.com/Jbruslind/PersonalProjects/blob/master/Inventor%20Projects/Infinity/Infinity_Assembly.png)
 

##	Bevel Clock
I used to work for Maxim Integrated up in their Beaverton location and during that time there was some scrap metal parts that were going to be thrown away. I found them and thought 
it would be cool to do an art project with them. This is currently work-in-progress and will be updated later (updated 8/19)

## RPI_APC_UPS

When I worked for the City of Corvallis automation department one of my projects was to make a network of connected Uninterruptible Power Supplies (UPSs). Using a Raspberry Pi
and some clever software called APCUPSD [Link](http://www.apcupsd.org/) I was able to connect quite a few. The UPSs in question usually had a Network Interface Card that you 
could insert into a slot on the back. These NICs cost ~$300 a piece and weren't terribly feature-friendly. So instead I made use of that slot by using a Raspberry Pi connected 
to the UPS through a usb connection and the APCUPSD software constantly mointoring it and sending that info back to a central server. This directory contains the slot mounting hardware
that I created to mount the RPI inside the UPS. 

## ROV_Models

This directory holds one of the models for the 2018-2019 UROV project I did with the OSU Robotics Club. It was a PCB model for a 48 - 12 V buck converter I designed with the 
LM5085 TI chip. Its goal was to supply ~10A for the ROV and we would have 2 load shared for a total supply of 20A. 

##Useful Models
This contains some useful models I found such as the RPI model 2 B+, some metric screws, etc