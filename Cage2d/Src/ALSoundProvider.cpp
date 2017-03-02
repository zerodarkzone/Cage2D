#include "Cage2d/stdafx.h"
#include "Cage2d/ALSoundProvider.hpp"
#include "Cage2d/SoundFileCache.hpp"

ALSoundProvider::ALSoundProvider( )
{
	for( int i = 0; i < MAX_SOUND_CHANNELS; i++ )
	{
		_currentSounds[i] = al_create_sample_instance( nullptr );
		alx::Mixer::getDefault( ).attach( _currentSounds[i] );
	}
	_currentSongName = "";
	_isSongPaused = false;
	_songPosition = 0;
}

ALSoundProvider::~ALSoundProvider( )
{
	StopAllSounds( );
	SFC.ClearSoundFileCache( );
}

void ALSoundProvider::PlaySound( const std::string& filename, float gain, float pan, float speed )
{
	int availChannel = -1;
	for( int i = 0; i < MAX_SOUND_CHANNELS; i++ )
	{
		if( !_currentSounds[i].isPlaying( ) )
		{
			availChannel = i;
			break;
		}
	}
	if( availChannel != -1 )
	{
		try
		{
			alx::Sample sample = SFC.GetSound( filename );
			_currentSounds[availChannel].setSample( sample );
			_currentSounds[availChannel].setGain( gain );
			_currentSounds[availChannel].setPan( pan );
			_currentSounds[availChannel].setSpeed( speed );
			_currentSounds[availChannel].setPlayMode( ALLEGRO_PLAYMODE_ONCE );
			_currentSounds[availChannel].play( );
		}
		catch( SoundNotFoundException& snfe )
		{
			std::cerr << snfe.what( ) << std::endl;
		}
	}
}

void ALSoundProvider::PlaySong( const std::string& filename, bool looping, float gain, float pan, float speed )
{
	alx::SampleInstance currentSong;
	try
	{
		currentSong = SFC.GetSong( filename );
		if( !currentSong )
		{
			std::cerr << "bad instance" << std::endl;
		}
	}
	catch( SoundNotFoundException& snfe )
	{
		std::cerr << snfe.what( ) << std::endl;
		return;
	}
	if( _currentSongName != "" )
	{
		try
		{
			alx::SampleInstance priorSong = SFC.GetSong( _currentSongName );
			if( priorSong.isPlaying( ) )
			{
				priorSong.stop( );
			}
		}
		catch( SoundNotFoundException& snfe )
		{
			std::cerr << snfe.what( ) << std::endl;
		}
	}
	_currentSongName = filename;
	currentSong.setGain( gain );
	currentSong.setPan( pan );
	currentSong.setSpeed( speed );
	if( looping )
	{
		currentSong.setPlayMode( ALLEGRO_PLAYMODE_LOOP );
	}
	else
	{
		currentSong.setPlayMode( ALLEGRO_PLAYMODE_ONCE );
	}
	currentSong.play( );
	_isSongPaused = false;
	_songPosition = 0;
}

void ALSoundProvider::SetAudioGain( float gain )
{
	alx::Mixer::getDefault( ).setGain( gain );
}

void ALSoundProvider::PauseSong( )
{
	if( IsSongPlaying( ) )
	{
		_isSongPaused = true;
		alx::SampleInstance currentSong = SFC.GetSong( _currentSongName );
		_songPosition = currentSong.getPosition( );
		StopSong( );
	}
}

void ALSoundProvider::ResumeSong( )
{
	if( _isSongPaused && _songPosition != 0 )
	{
		alx::SampleInstance currentSong = SFC.GetSong( _currentSongName );
		currentSong.setPosition( _songPosition );
		currentSong.play( );
	}
}

void ALSoundProvider::StopAllSounds( )
{
	for( int i = 0; i < MAX_SOUND_CHANNELS; i++ )
	{
		if( _currentSounds[i].isPlaying( ) )
		{
			_currentSounds[i].stop( );
		}
	}
	if( _currentSongName != "" )
	{
		alx::SampleInstance currentSong = SFC.GetSong( _currentSongName );
		if( currentSong.isPlaying( ) )
		{
			currentSong.stop( );
		}
	}
	_isSongPaused = false;
	_songPosition = 0;
}

void ALSoundProvider::StopSong( )
{
	if( _currentSongName != "" )
	{
		alx::SampleInstance currentSong = SFC.GetSong( _currentSongName );
		if( currentSong.isPlaying( ) )
		{
			currentSong.stop( );
		}
	}
	_isSongPaused = false;
	_songPosition = 0;
}

bool ALSoundProvider::IsSoundPlaying( )
{
	for( int i = 0; i < MAX_SOUND_CHANNELS; i++ )
	{
		if( _currentSounds[i].isPlaying( ) )
		{
			return true;
		}
	}
	return false;
}

bool ALSoundProvider::IsSongPlaying( )
{
	if( _currentSongName != "" )
	{
		alx::SampleInstance currentSong = SFC.GetSong( _currentSongName );
		return currentSong.isPlaying( );
	}
	return false;
}
