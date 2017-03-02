#pragma once
#include <string>

class IAudioProvider
{
public:

	virtual void PlaySound( const std::string& filename, float gain = 1.0, float pan = 0.0, float speed = 1.0 ) = 0;
	virtual void PlaySong( const std::string& filename, bool looping, float gain = 1.0, float pan = 0.0, float speed = 1.0 ) = 0;
	virtual void SetAudioGain( float gain ) = 0;
	virtual void PauseSong( ) = 0;
	virtual void ResumeSong( ) = 0;

	virtual void StopAllSounds( ) = 0;
	virtual void StopSong( ) = 0;
	virtual bool IsSoundPlaying( ) = 0;
	virtual bool IsSongPlaying( ) = 0;

	virtual ~IAudioProvider( ) = 0;
};
inline IAudioProvider::~IAudioProvider( ) {}
