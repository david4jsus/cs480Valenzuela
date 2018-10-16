#include <sound.h>

Sound::Sound()
{
}

Sound::~Sound()
{
    SDL_PauseAudioDevice(deviceId, 1);
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

    deviceId = SDL_OpenAudioDevice( NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE );

	if( deviceId == 1)
	{
		std::cout << "Couldn't open audio: " << SDL_GetError( ) << std::endl;

	}

	SDL_PauseAudioDevice( deviceId, 0 );
	
}

void Sound::PlaySound()
{
    SDL_PauseAudioDevice(deviceId, 1);
    SDL_Delay(50);
    SDL_PauseAudioDevice(deviceId, 0);
}
