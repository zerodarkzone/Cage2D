#pragma once
#include "IAudioProvider.hpp"
#include "SoundFileCache.hpp"

class ALSoundProvider :
	public IAudioProvider
{
public:
	ALSoundProvider( );
	~ALSoundProvider( );
	void PlaySound( const std::string& filename, float gain = 1.0, float pan = 0.0, float speed = 1.0 ) override;
	void PlaySong( const std::string& filename, bool looping, float gain = 1.0, float pan = 0.0, float speed = 1.0 ) override;
	void SetAudioGain( float gain ) override;
	void PauseSong( ) override;
	void ResumeSong( ) override;

	void StopAllSounds( ) override;
	void StopSong( ) override;
	bool IsSoundPlaying( ) override;
	bool IsSongPlaying( ) override;

private:
	static const int MAX_SOUND_CHANNELS = 24;
	bool _isSongPaused;
	unsigned int _songPosition;

	alx::SampleInstance _currentSounds[MAX_SOUND_CHANNELS];
	std::string _currentSongName;
};

