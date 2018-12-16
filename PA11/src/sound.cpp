#include <sound.h>

Sound::Sound()
{
	SDL_Init(SDL_INIT_AUDIO);
	engineIsRunning = true;
}

Sound::~Sound()
{
   if (engineIsRunning)
   {
	   engineIsRunning = false;
   }
   SDL_CloseAudioDevice(deviceId);
}

void Sound::LoadSound(std::string soundPath)
{
	if( SDL_LoadWAV( soundPath.c_str(), &wavSpec, &wavBuffer, &wavLength ) == NULL )
	{
		std::cout << "Unable to load sound" << std::endl;
		engineIsRunning = false;
		return;
	}
	
	 deviceId = SDL_OpenAudioDevice( NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE );
	 int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	 SDL_FreeWAV(wavBuffer);
	//PlaySound();	
}

void Sound::PlaySound()
{
	if (engineIsRunning)
	{
    	SDL_PauseAudioDevice(deviceId, 0);
    	SDL_Delay(200);
	}
}

void Sound::PlayNGGUP()
{
	if (engineIsRunning)
	{
    	SDL_PauseAudioDevice(deviceId, 0);
    	SDL_Delay(200);
	}
}

void Sound::PlaySoundEffect()
{
	if (engineIsRunning)
	{
    	SDL_PauseAudioDevice(deviceId, 0);
    	SDL_Delay(200);
	}
}

void Sound::LoopAudio()
{
	/*if (engineIsRunning)
	{
		LoadSound("../assets/imperial_march.wav");
		PlaySound();
		std::cout << "Playing Audio Again" << std::endl;
	}*/
}

bool Sound::AudioStopped()
{
	return (SDL_GetAudioDeviceStatus(deviceId)  == 0);
}
