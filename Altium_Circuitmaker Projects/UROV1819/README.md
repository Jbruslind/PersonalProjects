#	UROV19

I have been involved with the OSU Robotics Club Underwater team for around 4 years (as of Dec 2019). I work 
as the electrical team lead and help manage the various electrical components that are necessary to make an Underwater 
robot work. All of the circuit boards are custom designed either by myself or someone in my team and are tested/assembled 
in house. Initially I used Altium Student to design my boards, however I realized that Altium relased a free (lesser) version of 
itself called CircuitMaker that I have been using ever since 2017. One aspect of CircuitMaker is that all projects are 
published on the CircuitMaker cloud and are freely available for anyone to use. All of the projects below 
are no exception and anyone is free to use them as they wish. 

## Power Board (PDB)

This was a project that I was in charge of for most of my time here in the UROV team. This specific design 
uses the LM5117 Synchronous Buck Controller to convert a 48V input to a 12V output for the ROV to operate off of. 
48V is the standard voltage that the competition we compete in gives us to work with. The design of our ROV uses many brushless 
motors for propulsion and so a lot of current must be supplied by this board. We've calculated through some load torque experiments that the motors 
will draw up to 4A at peak load (stall torque) and so we used this value to plan for current consumption. With 6 motors in total this comes to 
around 24 - 25A continous current for just the motors alone. Factoring in the current draw of the main computer and periphery components we can 
estimate that this converter will need to supply up to 30A continously. 

This is quite a bit of current to supply with one board and my designs showed that we could regularly only pull ~10A from each board (as a standalone unit). 
I looked into some load sharing options and I found that most often you would design buck converters using a synchronous or interleaved option so that 
you could combine multiple into a larger current bank. This was not an option at this point however since, at the time, I couldn't find a cheap (<$10) 
buck controller that would allow for interleaved/current sharing operation explicitly from a >48V input and >12V @ 5A output. 

This proved challenging until I found a possible solution by Analog Systems: the ![LTC4370](https://www.analog.com/media/en/technical-documentation/data-sheets/4370f.pdf)
The basic principle is that you can use 2 external MOSFETs to control the voltage drop between 2 independent voltage sources. This would allow for accurate 
current sharing/voltage balancing for sources that have no external control otherwise. This would be perfect for our system and so I implemented 
this design into the bottom right of the board. This allowed us to either A. populate it to be used as the main controller or B. leave it unpopulated 
as a secondary controller (just a buck). 

With this solution I was able to achieve a much higher 15A draw in total. To test this I used a programmable DC load 

![LM5117 Render](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/Underwater%20ROV%20PDB/LM5117_Render.png)

![LM5117 Traces](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/Underwater%20ROV%20PDB/LM5117_Traces.png)

##	LM5085 Buck Converter
Unfortunately, the LM5117 had some weird MOSFET ringing issues in it's design and so the synchronous function 
did not work properly. We needed a new board on short notice (<2 months) and so I used a non-synchronous 
solution found through some online researching.  This project aims to take the LM5085 
Buck controller IC made by Texas Instruments and employ it towards reducing our competition standard 
48V input to our system standard 12V. The same statement in the LM5117 applies to this design in that 
in was designed with power planes and heat sinks in order to dissipate the thermal power gneerated by the MOSFETs. 

![LM5085 Render](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/UROV1819-Backup_LM5085/LM5085_render.png)

![LM5085 Traces](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/UROV1819-Backup_LM5085/LM5085_Traces.png)
 

##	AUX

The AUX board was intended to control all the extra components that the robot must have. 
For example, we need to control 6 different motors, various servos/actuators and read in various sensors. 
The AUX board is used to have a centralized point from which we could route all these connections and they 
could be read in using a Teensy 3.2 microcontroller. 

![AUX Render](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/Underwater_ROV_AuxBoard/AUX_Render.png)

![AUX Traces](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/Underwater_ROV_AuxBoard/AUX_Trace.png)

##	AUX2

We needed more connections than could be supported on the AUX board footprint so another duplicate 
board was created with the extra 2 components on it and broken out connections. This board handled 
the PWM motor control and Analog Signal inputs. 

![AUX2 Render](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/UROV1819_AUX_2/AUX2_Render.png)

![AUX2 Traces](https://github.com/Jbruslind/PersonalProjects/blob/master/Altium_Circuitmaker%20Projects/UROV1819/UROV1819_AUX_2/AUX2_Traces.png)