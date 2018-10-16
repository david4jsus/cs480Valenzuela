#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

class Sound
{
  public:
   Sound();
   ~Sound();
   void LoadSound(std::string soundPath);
   void PlaySound();

  private:
   SDL_AudioSpec wavSpec;
   Uint32 wavLength;
   Uint8 *wavBuffer;
   SDL_AudioDeviceID deviceId;
};

#endif


