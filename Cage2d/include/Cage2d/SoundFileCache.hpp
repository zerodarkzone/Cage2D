#pragma once
#ifndef SOUND_FILE_CACHE_HPP_INCLUDED
#define SOUND_FILE_CACHE_HPP_INCLUDED
class SoundFileCache
{
public:
	INSTANCE( SoundFileCache )
		~SoundFileCache( );

	alx::Sample GetSound( const std::string& soundName ) const;
	alx::SampleInstance GetSong( const std::string& songName ) const;
	void ClearSoundFileCache( );

private:
	SoundFileCache( );
	mutable std::map<std::string, alx::Sample> _sounds;
	mutable std::map<std::string, alx::SampleInstance> _music;
};

class SoundNotFoundException : public std::runtime_error
{
public:
	SoundNotFoundException( const std::string& msg ) : std::runtime_error( msg )
	{}
};

#define SFC SoundFileCache::GetInstance()
#endif // SOUND_FILE_CACHE_HPP_INCLUDED
