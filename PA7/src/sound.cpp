#include <sound.h>

Sound::Sound()
{
	SDL_Init(SDL_INIT_AUDIO);
}

Sound::~Sound()
{
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Sound::LoadSound(std::string soundPath)
{
	if( SDL_LoadWAV( soundPath.c_str(), &wavSpec, &wavBuffer, &wavLength ) == NULL )
	{
		std::cout << "Unable to load sound" << std::endl;
		return;
	}

    	deviceId = SDL_OpenAudioDevice( NULL, 0, &wavSpec, NULL, 0 );

        int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	PlaySound();	
}

void Sound::PlaySound()
{
    	SDL_PauseAudioDevice(deviceId, 0);
    	SDL_Delay(3000);
	std::cout << "Playing Audio" << std::endl;
}
