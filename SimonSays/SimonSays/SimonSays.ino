//---------------------------------------------------------------------------//
int  in[128];
byte NoteV[13]={8,23,40,57,76,96,116,138,162,187,213,241,255}; //data for note detection based on frequency
float f_peaks[5]; // top 5 frequencies peaks in descending order
int Mic_pin = A1;  
//---------------------------------------------------------------------------//

  int Midi_Lookup[] = {16,17,18,19,20,21,23,24,25,27,29,30,32,34,36,38,41,43,46,49,51,55,58,61,65,69,73,77,82,87,92,98,103,110,116,123,130,138,146,155,164,174,185,196,
    207,220,233,246,261,277,293,311,329,349,369,392,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987,1046,1108,1174,1244,1318,1396,1479,1567,1661,1760,
    1864,1975,2093,2217,2349,2489,2637,2793,2959,3135,3322,3520,3729,3951,4186,4434,4698,4978,5274,5587,5919,6271,6644,7040,7458,7902};

byte time_mult = 1;

byte Tetris_Notes[] = {76,76,71,71,72,72,74,74,76,76,72,72,71,71,69,69,69,69,72,72,76,76,74,74,72,72,71,71,71,71,72,72,74,74,76,76,72,72,69,69,69,69};
    
    //,74,74,77,77,81,81,79, 79,77,77,76,76,72,72,76,76,74,74,72,72,71,71,71,71,72,72,74,74,76,76,72,72,69,69,69,69};

    byte Tetris_Play[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
    //,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
    
    int Tetris_Time[] = {326,17,163,9,163,9,163,9,163,9,163,9,163,9,326,17,163,9,163,9,326,17,163,9,163,9,326,17,163,9,163,9,326,17,326,17,326,17,326,17,654,207};
    //,326,17,163,9,326,17,163,9,163,181,326,17,163,9,326,17,163,9,163,9,326,17,163,9,163,9,326,17,326,17,326,17,326,17,1554,35};


    byte ode_Notes[] = {73,73,73,74,74,76,76,76,76,74,74,73,73,71,71,69,69,69,69,71,71,73,73,73,73,71,71,71,71,73};
    //,73,73,73,74,74,76,76,76,76,74,74,73,73,71,71,69,69,69,69,71,71,73,73,71,71,69,69,69,69,71,71,71,71,73,73,69,69,71,71,73,73,74,74,73,73,69,69};
   
    byte ode_Play[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
    //,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
    
    int ode_Time[] = {0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,516,0,171,0,688,0,344};
    //,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0, 344,0,344,0,516,0,171,0,688,0,344,0,344,0,344,0,344,0,344,0,171,0,171,0,344,0,344,0}; 
//
    byte mary_Notes[] ={76,76,74,74,72,72,74,74,76,76,76,76,76,76};
//    //,74,74,74,74,74,74,76,76,79,79,79,79,76,76,74,74,72,72,74,74,76,76,76,76,76,76,76,76,74,74,74,74,76,76,74,74,72,72,76,
//    //76,74,74,72,72,74,74,76,76,76,76,76,76,74,74,74,74,74,74,76,76,79,79,79,79,76,76,74,74,72,72,74,74,76,76,76,76,76,76,76,76,74,74,74,74,76,76,74,74,72,72};
//    
   byte mary_Play[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0};
//    //,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
//    //1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
//    
   int mary_Time[] = {344,0,344,0,344,0,344,0,344,0,344,0,688,0};
//    //,344,0,344,0,688,0,344,0,344,0,688,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,688,
//    //690,344,0,344,0,344,0,344,0,344,0,344,0,688,0,344,0,344,0,688,0,344,0,344,0,688,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,344,0,688,0};


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <SPI.h>

#include "SdFat.h"


#define PIN 6
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

/*
 * F - Red
 * A - Orange
 * C - Yellow
 * E - Green
 * G - Blue
 * B - Pink
 * D - Violet
 */


// Order goes: C, C#, D, D#, E, F, F#, G, G#, A, A#, B
 uint32_t Colors[] = { pixels.Color(255,255,0), pixels.Color(255,255,0), 
 pixels.Color(127,0,255), pixels.Color(127,0,255), 
 pixels.Color(0,255,0), 
 pixels.Color(255,0,0), pixels.Color(255,0,0), 
 pixels.Color(0,0,255), pixels.Color(0,0,255), 
 pixels.Color(255,64,0),  pixels.Color(255,64,0), 
 pixels.Color(255,0,255)};

// Array Order G B D F E C A F
// G = 0, B = 1, D = 2, F = 3, E = 4, C = 5, A = 6, F = 7
 byte pixel_array[] = {5, 5, 2, 2, 4, 7, 7, 0, 0, 6, 6, 1};
 char pixel_notes[] = {'C', 'c', 'D', 'd', 'E', 'F', 'f', 'G', 'g' , 'A', 'a', 'B'};
 byte num_notes[] = {0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6};
//---------------------------------------------------------------------------//

void setup() {
  pixels.begin();
  //Serial.begin(250000);
  pinMode(9, OUTPUT);
  pinMode(A1, INPUT);
  randomSeed(analogRead(A0));
}


void loop() 
{ 
//      noTone(9);
//      pixels.begin();
//      for(int i = 0; i < (sizeof(Tetris_Play) / sizeof(Tetris_Play[0])); i++)
//      {
//        if(Tetris_Play[i])
//        {
//          int to_play = Tetris_Notes[i] - 12;
//          byte note_ = Tetris_Notes[i]%12;
//          tone(9,  Midi_Lookup[to_play]);
//          pixels.setPixelColor(pixel_array[note_], Colors[note_]);
//          pixels.show();
//      }
//      else
//      {
//        noTone(9);
//        pixels.clear();
//        //delay(50);
//      }
//     
//      delay(Tetris_Time[i]*time_mult);
//      
//    }
   bool good_note = 0;

    switch(random(2))
    {

      case 0 :
        for(int i = 0; i < (sizeof(Tetris_Play) / sizeof(Tetris_Play[0])); i++)
        {
          pixels.clear();
          good_note = 0;
          if(Tetris_Play[i])
          {
            int to_play = Tetris_Notes[i] - 12;
            byte note_ = Tetris_Notes[i]%12;
            tone(9,  Midi_Lookup[to_play]);
            pixels.setPixelColor(pixel_array[note_], Colors[note_]);
            pixels.show();
            delay(300);
            
            noTone(9); 
            delay(50);
            pixels.clear();
            delay(1000);
            
            byte temp;

            temp = Tone_det();
            //Serial.println(temp);
            if(num_notes[temp] <= num_notes[note_] + 1 && num_notes[temp] >= num_notes[note_] - 1)
            {
              good_note = 1; 
            }
            delay(10);

            pixels.begin();
                                
            if(good_note != 1)
            {
              for(int y = 0; y < 8; y++)
                {
                  pixels.setPixelColor(y, Colors[5]); 
                  delay(50);
                }
                pixels.show();
                delay(500);
                break;
            }
            else {
              for(int y = 0; y < 8; y++)
                {
                  pixels.setPixelColor(y, Colors[4]);
                  delay(50);
                }
                pixels.show();
                delay(500);
            }
          }
         
        }

      break;

      
      case 1 :

      for(int i = 0; i < (sizeof(mary_Play) / sizeof(mary_Play[0])); i++)
        {
          pixels.clear();
          good_note = 0;
          if(mary_Play[i])
          {
            int to_play = mary_Notes[i] - 12;
            byte note_ = mary_Notes[i]%12;
            tone(9,  Midi_Lookup[to_play]);
            pixels.setPixelColor(pixel_array[note_], Colors[note_]);
            pixels.show();
            delay(300);
            
            noTone(9); 
            delay(50);
            pixels.clear();
            delay(1000);
            
            byte temp;

            temp = Tone_det();
            //Serial.println(temp);
            if(num_notes[temp] <= num_notes[note_] + 1 && num_notes[temp] >= num_notes[note_] - 1)
            {
              good_note = 1; 
            }
            delay(10);

            pixels.begin();
                                
            if(good_note != 1)
            {
              for(int y = 0; y < 8; y++)
                {
                  pixels.setPixelColor(y, Colors[5]); 
                  delay(50);
                }
                pixels.show();
                delay(500);
                break;
            }
            else {
              for(int y = 0; y < 8; y++)
                {
                  pixels.setPixelColor(y, Colors[4]);
                  delay(50);
                }
                pixels.show();
                delay(500);
            }
          }
         
        }

      break;

      case 2 :  
       
      for(int i = 0; i < (sizeof(ode_Play) / sizeof(ode_Play[0])); i++)
        {
          pixels.clear();
          good_note = 0;
          if(ode_Play[i])
          {
            int to_play = ode_Notes[i] - 12;
            byte note_ = ode_Notes[i]%12;
            tone(9,  Midi_Lookup[to_play]);
            pixels.setPixelColor(pixel_array[note_], Colors[note_]);
            pixels.show();
            delay(300);
            
            noTone(9); 
            delay(50);
            pixels.clear();
            delay(1000);
            
            byte temp;

            temp = Tone_det();
            //Serial.println(temp);
            if(num_notes[temp] <= num_notes[note_] + 1 && num_notes[temp] >= num_notes[note_] - 1)
            {
              good_note = 1; 
            }
            delay(10);

            pixels.begin();
                                
            if(good_note != 1)
            {
              for(int y = 0; y < 8; y++)
                {
                  pixels.setPixelColor(y, Colors[5]); 
                  delay(50);
                }
                pixels.show();
                delay(500);
                break;
            }
            else {
              for(int y = 0; y < 8; y++)
                {
                  pixels.setPixelColor(y, Colors[4]);
                  delay(50);
                }
                pixels.show();
                delay(500);
            }
          }
         
        }
     
      break;
      
    }
    while(1){}
}

//-----------------------------Chord Detection Function----------------------------------------------//
// Documentation on Chord_detection:https://www.instructables.com/member/abhilash_patel/instructables/
// Code Written By: Abhilash Patel
// Contact: abhilashpatel121@gmail.com
// this code written for arduino Nano board (should also  work for UNO) or better board
// this code won't work for any board having RAM less than 2kb,
// More accurate detection can be carried out on more powerful borad by increasing sample size



byte Tone_det()
{ long unsigned int a1,b,a2;
  float a;
  float sum1=0,sum2=0;
  float sampling;
  a1=micros();
        for(int i=0;i<128;i++)
          {
            a=analogRead(Mic_pin)-500;     //rough zero shift
            //utilising time between two sample for windowing & amplitude calculation
            sum1=sum1+a;              //to average value
            sum2=sum2+a*a;            // to RMS value
            a=a*(sin(i*3.14/128)*sin(i*3.14/128));   // Hann window
            in[i]=10*a;                // scaling for float to int conversion
            //delayMicroseconds(195);   // based on operation frequency range
          }
b=micros();

sum1=sum1/128;               // Average amplitude
sum2=sqrt(sum2/128);         // RMS amplitude
sampling= 128000000/(b-a1);  // real time sampling frequency

//for very low or no amplitude, this code wont start
//it takes very small aplitude of sound to initiate for value sum2-sum1>3, 
//change sum2-sum1 threshold based on requirement
if(sum2-sum1>1){  
       FFT(128,sampling);        
       //EasyFFT based optimised  FFT code, 
       //this code updates f_peaks array with 5 most dominent frequency in descending order
 
 for(int i=0;i<12;i++){in[i]=0;}  // utilising in[] array for further calculation

int j=0,k=0; //below loop will convert frequency value to note 
       for(int i=0; i<5;i++)
           {
           if(f_peaks[i]>1040){f_peaks[i]=0;}
           if(f_peaks[i]>=65.4   && f_peaks[i]<=130.8) {f_peaks[i]=255*((f_peaks[i]/65.4)-1);}
           if(f_peaks[i]>=130.8  && f_peaks[i]<=261.6) {f_peaks[i]=255*((f_peaks[i]/130.8)-1);}
           if(f_peaks[i]>=261.6  && f_peaks[i]<=523.25){f_peaks[i]=255*((f_peaks[i]/261.6)-1);}
           if(f_peaks[i]>=523.25 && f_peaks[i]<=1046)  {f_peaks[i]=255*((f_peaks[i]/523.25)-1);}
           if(f_peaks[i]>=1046 && f_peaks[i]<=2093)  {f_peaks[i]=255*((f_peaks[i]/1046)-1);}
           if(f_peaks[i]>255){f_peaks[i]=254;}
           j=1;k=0;
         
         while(j==1)
              {
              if(f_peaks[i]<NoteV[k]){f_peaks[i]=k;j=0;}
              k++;  // a note with max peaks (harmonic) with aplitude priority is selected
              if(k>15){j=0;}
              }

              if(f_peaks[i]==12){f_peaks[i]=0;}
              k=f_peaks[i];
              in[k]=in[k]+(5-i);
            }

k=0;j=0;
          for(int i=0;i<12;i++)
             {
              if(k<in[i]){k=in[i];j=i;}  //Max value detection
             }
       // Note print
       // if you need to use note value for some application, use of note number recomendded
       // where, 0=c;1=c#,2=D;3=D#;.. 11=B;      
       //a2=micros(); // time check
        k=j;
        return k;
       }
}

//-----------------------------FFT Function----------------------------------------------//
// Documentation on EasyFFT:https://www.instructables.com/member/abhilash_patel/instructables/
// EasyFFT code optimised for 128 sample size to reduce mamory consumtion

float FFT(byte N,float Frequency)
{
byte data[8]={1,2,4,8,16,32,64,128};
int a,c1,f,o,x;
a=N;  
                                 
      for(int i=0;i<8;i++)                 //calculating the levels
         { if(data[i]<=a){o=i;} }
      o=7;
byte in_ps[data[o]]={};     //input for sequencing
float out_r[data[o]]={};   //real part of transform
float out_im[data[o]]={};  //imaginory part of transform
           
x=0;  
      for(int b=0;b<o;b++)                     // bit reversal
         {
          c1=data[b];
          f=data[o]/(c1+c1);
                for(int j=0;j<c1;j++)
                    { 
                     x=x+1;
                     in_ps[x]=in_ps[j]+f;
                    }
         }
 
      for(int i=0;i<data[o];i++)            // update input array as per bit reverse order
         {
          if(in_ps[i]<a)
          {out_r[i]=in[in_ps[i]];}
          if(in_ps[i]>a)
          {out_r[i]=in[in_ps[i]-a];}      
         }

int i10,i11,n1;
float e,c,s,tr,ti;

    for(int i=0;i<o;i++)                                    //fft
    {
     i10=data[i];              // overall values of sine cosine  
     i11=data[o]/data[i+1];    // loop with similar sine cosine
     e=6.283/data[i+1];
     e=0-e;
     n1=0;

          for(int j=0;j<i10;j++)
          {
          c=cos(e*j); 
          s=sin(e*j); 
          n1=j;
          
                for(int k=0;k<i11;k++)
                 {
                 tr=c*out_r[i10+n1]-s*out_im[i10+n1];
                 ti=s*out_r[i10+n1]+c*out_im[i10+n1];
          
                 out_r[n1+i10]=out_r[n1]-tr;
                 out_r[n1]=out_r[n1]+tr;
          
                 out_im[n1+i10]=out_im[n1]-ti;
                 out_im[n1]=out_im[n1]+ti;          
          
                 n1=n1+i10+i10;
                  }       
             }
     }

//---> here onward out_r contains amplitude and our_in conntains frequency (Hz)
    for(int i=0;i<data[o-1];i++)               // getting amplitude from compex number
        {
         out_r[i]=sqrt((out_r[i]*out_r[i])+(out_im[i]*out_im[i])); // to  increase the speed delete sqrt
         out_im[i]=(i*Frequency)/data[o];
         
//         Serial.print(out_im[i],2); Serial.print("Hz");
//         Serial.print("\t");                            // uncomment to print freuency bin    
//         Serial.println(out_r[i]); 
//         
        }

x=0;       // peak detection
   for(int i=1;i<data[o-1]-1;i++)
      {
      if(out_r[i]>out_r[i-1] && out_r[i]>out_r[i+1]) 
      {in_ps[x]=i;    //in_ps array used for storage of peak number
      x=x+1;}    
      }

s=0;
c=0;
    for(int i=0;i<x;i++)             // re arraange as per magnitude
    {
        for(int j=c;j<x;j++)
        {
            if(out_r[in_ps[i]]<out_r[in_ps[j]]) 
                {s=in_ps[i];
                in_ps[i]=in_ps[j];
                in_ps[j]=s;}
        }
    c=c+1;
    }
    
    for(int i=0;i<5;i++)     // updating f_peak array (global variable)with descending order
     {
     f_peaks[i]=(out_im[in_ps[i]-1]*out_r[in_ps[i]-1]+out_im[in_ps[i]]*out_r[in_ps[i]]+out_im[in_ps[i]+1]*out_r[in_ps[i]+1])
     /(out_r[in_ps[i]-1]+out_r[in_ps[i]]+out_r[in_ps[i]+1]);
     }
}
    
//------------------------------------------------------------------------------------//
