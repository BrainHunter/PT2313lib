
#include "PT2313.h"


// Registers:
#define PT2313RegVolumeControl      0x00   
#define PT2313RegVolumeControlMask  0x3F 
#define PT2313RegSpeakerAttLR       0xC0
#define PT2313RegSpeakerAttRR       0xD0
#define PT2313RegSpeakerAttLF       0x80
#define PT2313RegSpeakerAttRF       0xA0
#define PT2313RegSpeakerAttMask     0x1F
#define PT2313RegAudioSwich         0x40
#define PT2313RegAudioSwichMask     0x1F
#define PT2313RegBassContol         0x60
#define PT2313RegBassContolMask     0x0F
#define PT2313RegTrebleContol       0x70
#define PT2313RegTrebleContolMask     0x0F

#define VOLUMEMAX		(0x3F)
#define ATTENUATIONMAX	(0x1F)
#define TONEMAX			(0x7)


PT2313::PT2313(TwoWire *bus, uint8_t i2cAddress)
{

  this->bus = bus;
  this->i2cAddress = i2cAddress;

  volume = 0;
  speakerAttenuation[4] = {0}; 

  input = 0;
  loudness = false;
  inputGain = 0;
  bass = 0;
  treble = 0;
  
}

PT2313::~PT2313()
{

}

uint8_t PT2313::sendData(uint8_t data)
{
  bus->beginTransmission(i2cAddress);
  bus->write(data);
  return bus->endTransmission();
}

uint8_t PT2313::setVolume(int8_t volume)
{
  if(volume < 0)
  {
    volume = 0;
  }
  if(volume > VOLUMEMAX)
  {
    volume = VOLUMEMAX;
  }
  this-> volume = volume;

  uint8_t data;
  data = VOLUMEMAX -volume;
  data = data & PT2313RegVolumeControlMask; //make shure no data is where the register is selected
  data = data | PT2313RegVolumeControl;     //set the register bits

  Serial.print("Volume data:");
  Serial.println(data);
  
  return sendData(data);
}
uint8_t PT2313::getVolume()
{
  return volume;
}

uint8_t PT2313::setAttenuator(enum SpeakerSelector s, int8_t att)
{
  if(s < 0 || s > 3) s = LeftFront;
  if(att < 0)
  {
    att = 0;
  }
  if(att > ATTENUATIONMAX)
  {
    att = ATTENUATIONMAX;
  }
  this->speakerAttenuation[s] = att;
  
  uint8_t data;
  data = att;
  data = data & PT2313RegSpeakerAttMask;    		//make shure no data is where the register is selected
  data = data | PT2313RegSpeakerAttLF | s << 5;     //set the register bits
 
  return sendData(data);
}

uint8_t PT2313::getAttenuator(enum SpeakerSelector s){
  if(s < 0 || s > 3) s = LeftFront;
  return speakerAttenuation[s];
}

uint8_t PT2313::setAudioSwitch(uint8_t input, bool loudness, int8_t inputGain)
{
  if(input > 2)
  {
    input = 2;
  }

  if(inputGain < 0)
  {
    inputGain = 0;
  }
  if(inputGain > 3)
  {
    inputGain = 3;
  }

  this->input = input;
  this->loudness = loudness;
  this->inputGain = inputGain;

  uint8_t data = 0;
  data |= input & 0x3;
  data |= loudness ? 0 : (1 << 2);
  data |= (inputGain & 0x3) << 3;

  data = data & PT2313RegAudioSwichMask;    //make shure no data is where the register is selected
  data = data | PT2313RegAudioSwich;      //set the register bits
   
  return sendData(data);
}

uint8_t PT2313::setInput(uint8_t input)
{
    return setAudioSwitch(input, this->loudness, this->inputGain);
}
uint8_t PT2313::getInput()
{
    return this->input;
}


uint8_t PT2313::setLoudness(bool loudness)
{
  return setAudioSwitch(this->input, loudness, this->inputGain);
}
bool PT2313::getLoudness()
{
  return this->loudness;
}


uint8_t PT2313::setInputGain(int8_t gain)
{
  return setAudioSwitch(this->input, this->loudness, gain);
}
uint8_t PT2313::getInputGain()
{
  return this->inputGain;
}


uint8_t PT2313::setBass(int8_t bass)
{
  if(bass < -TONEMAX)
  {
    bass = -TONEMAX;
  }
  if(bass > TONEMAX)
  {
    bass = TONEMAX;
  }
  this->bass = bass;

  bool negative = (bass<0);
  bass = bass & 0x7;

  bass = bass ^ 0x7;

  uint8_t data = 0;
  data |= bass & 0x7;
  data |= negative ? 0 : (1 << 3);
  data = data & PT2313RegBassContolMask;  //make shure no data is where the register is selected
  data = data | PT2313RegBassContol;      //set the register bits
  
  return sendData(data); 
}
int8_t PT2313::getBass()
{
  return this->bass;
}

uint8_t PT2313::setTreble(int8_t treble)
{
  if(treble < -TONEMAX)
  {
    treble = -TONEMAX;
  }
  if(treble > TONEMAX)
  {
    treble = TONEMAX;
  }
  this->treble = treble;

  bool negative = (treble<0);
  treble = treble & 0x7;

  treble = treble ^ 0x7;

  uint8_t data = 0;
  data |= treble & 0x7;
  data |= negative ? 0 : (1 << 3);
  data = data & PT2313RegTrebleContolMask;  //make shure no data is where the register is selected
  data = data | PT2313RegTrebleContol;      //set the register bits
  
  return sendData(data); 
}
int8_t PT2313::getTreble()
{
  return this->treble;
}
