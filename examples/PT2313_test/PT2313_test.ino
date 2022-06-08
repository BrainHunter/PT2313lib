/*
 * Example Sketch for testing the PT2313/TDA7313  
 * 
 * This sketch provides a simple interface via the serial port for testing 
 * the PT2313 4-Channel Audio Processor IC.  
 * 
 * USAGE:
 * General logic: lower case chars decrease a value; upper case chars increase a value
 * 
 * v/V : decrease/increase Volume
 * a/A : decrease/increase Front Left Attenuator
 * s/S : decrease/increase Front Right Attenuator
 * q/Q : decrease/increase Rear Left Attenuator
 * w/W : decrease/increase Rear Right Attenuator
 * b/B : decrease/increase bass
 * t/T : decrease/increase treble
 * i/I : decrease/increase input gain
 * l : toggle loudness (lowercase L)
 * 1 : select input 1
 * 2 : select input 2
 * 3 : select input 3
 *
 * 
 * 
 * WIRING:
 * connect the chip to the I2C bus. Don't forget the pullup resistors 
 * on the data and clock line of the bus. 
 * Here some modules as example:
 * ------------------------------------------------------------------
 * Bus        Node MCU    Arduino 
 * Signal     Pin         Nano  
 * ------------------------------------------------------------------
 * SCL        D1          A5
 * SDA        D2          A4
 * 
 * Created 24.04.2022
 * by Nicolas Kaufmann
 * 
 * https://github.com/BrainHunter/PT2313lib
 * 
 * 
 */


// Includes: 
#include "PT2313.h"

void printStatus();


PT2313 pt;   


void setup()
{
  Wire.begin();
  Wire.setClock(100000);    // set the I2C clock to 100kHz. This is the max clock the PT2313 can handle!
 
  Serial.begin(9600);
  while (!Serial);          // Leonardo: wait for serial monitor
  Serial.println("\nPT2313 Test");
}

void loop()
{
  int rxbyte = Serial.read();   // read a character from the serial
  uint8_t ret = 0;              // ret is for the return value of the set commands of the pt

  switch(rxbyte)  // analyze the character and do something if it is valid      
  {
  //volume control:
    case 'V': ret = pt.setVolume(pt.getVolume()+1);     // increase
              break;
    case 'v': ret = pt.setVolume(pt.getVolume()-1);     // decrease
              break;        

  //Attenuation control:
    // LeftFront:
    case 'A': ret = pt.setAttenuator(PT2313::LeftFront, pt.getAttenuator(PT2313::LeftFront)+1);   // increase
              break;
    case 'a': ret = pt.setAttenuator(PT2313::LeftFront, pt.getAttenuator(PT2313::LeftFront)-1);   // decrease
              break;      
    // RightFront:
    case 'S': ret = pt.setAttenuator(PT2313::RightFront, pt.getAttenuator(PT2313::RightFront)+1);  // increase
              break;
    case 's': ret = pt.setAttenuator(PT2313::RightFront, pt.getAttenuator(PT2313::RightFront)-1);  // decrease
              break;          
    // LeftRear:
    case 'Q': ret = pt.setAttenuator(PT2313::LeftRear, pt.getAttenuator(PT2313::LeftRear)+1);    // increase
              break;
    case 'q': ret = pt.setAttenuator(PT2313::LeftRear, pt.getAttenuator(PT2313::LeftRear)-1);    // decrease
              break;      
    // RightRear:
    case 'W': ret = pt.setAttenuator(PT2313::RightRear, pt.getAttenuator(PT2313::RightRear)+1);   // increase
              break;
    case 'w': ret = pt.setAttenuator(PT2313::RightRear, pt.getAttenuator(PT2313::RightRear)-1);   // decrease
              break;          

  // input selector:
    case '1': ret = pt.setInput(0);
              break;
    case '2': ret = pt.setInput(1);
              break;
    case '3': ret = pt.setInput(2);
              break;

  //toggle loudness:
    case 'l': ret = pt.setLoudness(!pt.getLoudness());
              break;

  // input gain:
    case 'I': ret = pt.setInputGain(pt.getInputGain()+1);   // increase
              break;
    case 'i': ret = pt.setInputGain(pt.getInputGain()-1);   // decrease
              break;  

  //Bass:
    case 'B': ret = pt.setBass(pt.getBass()+1);    // increase
              break;
    case 'b': ret = pt.setBass(pt.getBass()-1);    // decrease
              break;  

  //Treble:
    case 'T': ret = pt.setTreble(pt.getTreble()+1);  // increase
              break;
    case 't': ret = pt.setTreble(pt.getTreble()-1);  // decrease
              break; 

    default: 
            rxbyte = -1;
             break;
  }


  // analyze the return value
  if(rxbyte != -1)
  {
    switch(ret)
    {
      case 0: Serial.println("Success");
              break;
      case 1: Serial.println("data too long to fit in transmit buffer");
              break;
      case 2: Serial.println("received NACK on transmit of address");
              break;
      case 3: Serial.println("received NACK on transmit of data");
              break;
      case 4: Serial.println("other error");
              break;
      default: Serial.print("got error code: ");
               Serial.println(ret);
    }
  }
  
  printStatus();
  delay(500);
  
}



void printStatus()  
{
  Serial.print("volume: ");
  Serial.print(pt.getVolume());
  Serial.print(" attenuation: LF ");
  Serial.print(pt.getAttenuator(PT2313::LeftFront));
  Serial.print(" RF ");
  Serial.print(pt.getAttenuator(PT2313::RightFront));
  Serial.print(" LR ");
  Serial.print(pt.getAttenuator(PT2313::LeftRear));
  Serial.print(" RR ");
  Serial.print(pt.getAttenuator(PT2313::RightRear));
  Serial.print(" input: ");
  Serial.print(pt.getInput());
  Serial.print(" loudness: ");
  Serial.print(pt.getLoudness());
  Serial.print(" inputGain: ");
  Serial.print(pt.getInputGain());
  Serial.print(" Bass: ");
  Serial.print(pt.getBass());
  Serial.print(" Treble: ");
  Serial.print(pt.getTreble());
  Serial.print("\n");
}
