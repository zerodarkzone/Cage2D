#include "Cage2d/stdafx.h"
#include "Cage2d/BitmapFileCache.hpp"

BitmapFileCache::BitmapFileCache( )
{
	//std::cout << _bitmapChache.max_size() << std::endl;
}

alx::Bitmap BitmapFileCache::LoadBitmap( const std::string& filename )
{
	if( _bitmapChache.empty( ) )
	{
		alx::Bitmap bit( filename );
		if( !bit )
		{
			std::cerr << "Couldn't load the bitmap" << std::endl;
			throw BitmapLoadException( "Couldn't load the bitmap" );
		}
		_bitmapChache.insert( std::pair<std::string, alx::Bitmap>( filename, bit ) );
		return bit;
	}
	else
	{
		auto it = _bitmapChache.find( filename );
		if( it == _bitmapChache.end( ) )
		{
			alx::Bitmap bit( filename );
			if( !bit )
			{
				std::cerr << "Couldn't load the bitmap" << std::endl;
				throw BitmapLoadException( "Couldn't load the bitmap" );
			}
			_bitmapChache.insert( std::pair<std::string, alx::Bitmap>( filename, bit ) );
			return bit;
		}
		else
		{
			return it->second;
		}
	}
}

void BitmapFileCache::ClearCache( )
{
	_bitmapChache.clear( );
}

BitmapFileCache::~BitmapFileCache( )
{
	ClearCache( );
	DEBUG_MSG( "Bitmap Cache Destroyed" );
}
