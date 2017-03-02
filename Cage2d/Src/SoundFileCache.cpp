#include "Cage2d/stdafx.h"
#include "Cage2d/SoundFileCache.hpp"

SoundFileCache::SoundFileCache( )
{}

alx::Sample SoundFileCache::GetSound( const std::string& soundName ) const
{
	if( _sounds.empty( ) )
	{
		alx::Sample sample( soundName.c_str( ) );
		if( !sample )
		{
			throw SoundNotFoundException( soundName + " was not found \n" );
		}
		_sounds.insert( std::pair<std::string, alx::Sample>( soundName, sample ) );
		return sample;
	}
	auto itr = _sounds.find( soundName );
	if( itr == _sounds.end( ) )
	{
		alx::Sample sample( soundName.c_str( ) );
		if( !sample )
		{
			throw SoundNotFoundException( soundName + " was not found \n" );
		}
		_sounds.insert( std::pair<std::string, alx::Sample>( soundName, sample ) );
		return sample;
	}
	else
	{
		return itr->second;
	}
}

alx::SampleInstance SoundFileCache::GetSong( const std::string& songName ) const
{
	if( _music.empty( ) )
	{
		alx::Sample sample( GetSound( songName ) );
		if( !sample )
		{
			throw SoundNotFoundException( songName + " was not found \n" );
		}
		alx::SampleInstance sampleInstance( sample );
		if( !sampleInstance )
		{
			throw SoundNotFoundException( songName + " instance was not found \n" );
		}
		if( !alx::Mixer::getDefault( ).attach( sampleInstance ) )
		{
			throw std::runtime_error( "Couldn't attach " + songName + " to the default mixer \n" );
		}
		_music.insert( std::pair<std::string, alx::SampleInstance>( songName, sampleInstance ) );
		return sampleInstance;
	}
	auto itr = _music.find( songName );
	if( itr == _music.end( ) )
	{
		alx::Sample sample( GetSound( songName ) );
		if( !sample )
		{
			throw SoundNotFoundException( songName + " was not found \n" );
		}
		alx::SampleInstance sampleInstance( sample );
		if( !sampleInstance )
		{
			throw SoundNotFoundException( songName + " instance was not found \n" );
		}
		if( !alx::Mixer::getDefault( ).attach( sampleInstance ) )
		{
			throw std::runtime_error( "Couldn't attach " + songName + " to the default mixer \n" );
		}
		_music.insert( std::pair<std::string, alx::SampleInstance>( songName, sampleInstance ) );
		return sampleInstance;
	}
	else
	{
		alx::SampleInstance sampleInstance( itr->second );
		if( !sampleInstance )
		{
			throw SoundNotFoundException( songName + " was not found \n" );
		}
		if( !sampleInstance.isAttached( ) && !alx::Mixer::getDefault( ).attach( sampleInstance ) )
		{
			throw std::runtime_error( "Couldn't attach " + songName + " to the default mixer \n" );
		}
		return sampleInstance;
	}
}

void SoundFileCache::ClearSoundFileCache( )
{
	_sounds.clear( );
	_music.clear( );
}

SoundFileCache::~SoundFileCache( )
{
	ClearSoundFileCache( );
	DEBUG_MSG( "Sound Cache Destroyed" );
}
