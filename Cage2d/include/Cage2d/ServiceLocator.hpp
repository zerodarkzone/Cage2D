#pragma once
#include "IAudioProvider.hpp"

class ServiceLocator
{
public:
	static IAudioProvider *GetAudio( )
	{
		return _audioProvider;
	}

	static void RegisterServiceLocator( IAudioProvider *provider )
	{
		_audioProvider = provider;
	}

private:
	static IAudioProvider *_audioProvider;
};
