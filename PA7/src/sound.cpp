#include <sound.h>

Sound::Sound()
{
	SDL_Init(SDL_INIT_AUDIO);
	engineIsRunning = true;
}

Sound::~Sound()
{
	engineIsRunning = false;
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
	//PlaySound();	
}

void Sound::PlayMainSound()
{
std::cout << SDL_GetAudioDeviceStatus(deviceId) << std::endl;
    	SDL_PauseAudioDevice(deviceId, 0);
    	SDL_Delay(40000); // 39 second clip in ms
	SDL_CloseAudioDevice(deviceId);
}

void Sound::PlayNGGUP()
{
    	SDL_PauseAudioDevice(deviceId, 0);
    	SDL_Delay(210000); // 3 minute and 30 second clip in ms
	SDL_CloseAudioDevice(deviceId);
}

void Sound::LoopAudio()
{
	LoadSound("../assets/imperial_march.wav");
	PlayMainSound();
	std::cout << "Playing Audio Again" << std::endl;
}

bool Sound::AudioStopped()
{
	return (SDL_GetAudioDeviceStatus(deviceId)  == 0);
}
