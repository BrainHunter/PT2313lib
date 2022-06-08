#ifndef __PT2313_H__
#define __PT2313_H__

#include <Arduino.h>
#include <Wire.h>


// Default i2C address
#define PT2313DefaultAddress 0x44


class PT2313 
{
  public:

    enum SpeakerSelector
    {
      LeftFront   = 0,
      RightFront  = 1,
      LeftRear    = 2,
      RightRear   = 3     
    };

    PT2313(TwoWire *bus = &Wire, uint8_t i2cAddress = PT2313DefaultAddress);
    ~PT2313();

    void setDefaults();

    uint8_t setVolume(int8_t volume);
    uint8_t getVolume();
    uint8_t setAttenuator(enum SpeakerSelector s, int8_t att);
    uint8_t getAttenuator(enum SpeakerSelector s);
    uint8_t setAudioSwitch(uint8_t input, bool loudness, int8_t inputGain);
    uint8_t setInput(uint8_t input);
    uint8_t getInput();
    uint8_t setLoudness(bool loudness);
    bool getLoudness();
    uint8_t setInputGain(int8_t gain);
    uint8_t getInputGain();

    uint8_t setBass(int8_t bass);
    int8_t getBass();
    uint8_t setTreble(int8_t treble);
    int8_t getTreble();

  protected:
    TwoWire *bus;
    uint8_t i2cAddress;

    
    uint8_t sendData(uint8_t data);

  private:
    uint8_t volume;
    uint8_t speakerAttenuation[4]; 

    uint8_t input;
    bool loudness;
    uint8_t inputGain;
    int8_t bass;
    int8_t treble;
    
    
};

#endif
