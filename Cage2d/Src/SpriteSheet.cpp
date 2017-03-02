#include "Cage2d/stdafx.h"
#include "Cage2d/SpriteSheet.hpp"

namespace sp
{

	SpriteSheet::SpriteSheet( ) : Sprite( ), _framesPerRow( 0 ), _totalFrames( 0 ), _currentFrame( 0 ), _xOffset( 0 ), _yOffset( 0 )
	{
		_spriteList = nullptr;
	}

	SpriteSheet::SpriteSheet( const alx::Bitmap& image, double width, double height, int totalFrames, int xOffset, int yOffset ) :
		SpriteSheet( )
	{
		this->image = image;
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw InvalidBitmapException( "Invalid Bitmap" );
		}
		_totalFrames = totalFrames;
		_xOffset = xOffset;
		_yOffset = yOffset;
		_width = width + _xOffset;
		_height = height + _yOffset;

		_framesPerRow = image.getWidth( ) / _width;
		DEBUG_MSG(_framesPerRow);
		_spriteList = new Sprite[_totalFrames];
		for( int i = 0; i < _totalFrames; ++i )
		{
			int fx = ( i % _framesPerRow ) * _width;
			int fy = ( i / _framesPerRow ) * _height;
			if( _xOffset >= 0 && _yOffset >= 0 )
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx + _xOffset, fy + _yOffset, _width - _xOffset, _height - _yOffset ) );
			}
			else
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx, fy, _width + _xOffset, _height + _yOffset ) );
			}
		}
	}

	SpriteSheet::SpriteSheet( const alx::Bitmap& image, int framesPerRow, int totalFrames, int xOffset, int yOffset )
	{
		this->image = image;
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw InvalidBitmapException( "Invalid Bitmap" );
		}
		_framesPerRow = framesPerRow;
		_totalFrames = totalFrames;
		_xOffset = xOffset;
		_yOffset = yOffset;
		_width = (int) ( this->image.getWidth( ) / _framesPerRow );
		_height = (int) ( this->image.getHeight( ) / ( _totalFrames / _framesPerRow ) );
		_spriteList = new Sprite[_totalFrames];
		for( int i = 0; i < _totalFrames; ++i )
		{
			int fx = ( i % _framesPerRow ) * _width;
			int fy = ( i / _framesPerRow ) * _height;
			if( _xOffset >= 0 && _yOffset >= 0 )
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx + _xOffset, fy + _yOffset, _width - _xOffset, _height - _yOffset ) );
			}
			else
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx, fy, _width + _xOffset, _height + _yOffset ) );
			}
		}
	}

	SpriteSheet::SpriteSheet( const std::string& filename, double width, double height, int totalFrames, int xOffset, int yOffset ) :
		SpriteSheet( )
	{
		this->image = BFC.LoadBitmap( filename );
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw InvalidBitmapException( "Invalid Bitmap" );
		}
		_totalFrames = totalFrames;
		_xOffset = xOffset;
		_yOffset = yOffset;
		_width = width + _xOffset;
		_height = height + _yOffset;

		_framesPerRow = image.getWidth( ) / _width;
		_spriteList = new Sprite[_totalFrames];
		for( int i = 0; i < _totalFrames; ++i )
		{
			int fx = ( i % _framesPerRow ) * _width;
			int fy = ( i / _framesPerRow ) * _height;
			if( _xOffset >= 0 && _yOffset >= 0 )
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx + _xOffset, fy + _yOffset, _width - _xOffset, _height - _yOffset ) );
			}
			else
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx, fy, _width + _xOffset, _height + _yOffset ) );
			}
		}
	}

	SpriteSheet::SpriteSheet( const std::string& filename, int framesPerRow, int totalFrames, int xOffset, int yOffset )
	{
		this->image = BFC.LoadBitmap( filename );
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw InvalidBitmapException( "Invalid Bitmap" );
		}
		_framesPerRow = framesPerRow;
		_totalFrames = totalFrames;
		_xOffset = xOffset;
		_yOffset = yOffset;
		_width = (int) ( this->image.getWidth( ) / _framesPerRow );
		_height = (int) ( this->image.getHeight( ) / ( _totalFrames / _framesPerRow ) );
		_spriteList = new Sprite[_totalFrames];
		for( int i = 0; i < _totalFrames; ++i )
		{
			int fx = ( i % _framesPerRow ) * _width;
			int fy = ( i / _framesPerRow ) * _height;
			if( _xOffset >= 0 && _yOffset >= 0 )
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx + _xOffset, fy + _yOffset, _width - _xOffset, _height - _yOffset ) );
			}
			else
			{
				_spriteList[i].setBitmap( alx::Bitmap( this->image, fx, fy, _width + _xOffset, _height + _yOffset ) );
			}
		}
	}

	SpriteSheet::SpriteSheet( const SpriteSheet& rhs ) : SpriteSheet( )
	{
		Sprite::operator=( rhs );
		_framesPerRow = rhs._framesPerRow;
		_totalFrames = rhs._totalFrames;
		_xOffset = rhs._xOffset;
		_yOffset = rhs._yOffset;
		_spriteList = new Sprite[_totalFrames];
#ifdef __MINGW32__
		std::copy( rhs._spriteList, rhs._spriteList + _totalFrames, _spriteList );
#else
		std::copy( rhs._spriteList, rhs._spriteList + _totalFrames, stdext::checked_array_iterator<Sprite *>( _spriteList, _totalFrames ) );
#endif // __MINGW32__
	}

	SpriteSheet& SpriteSheet::operator = ( const SpriteSheet& rhs )
	{
		Sprite::operator=( rhs );
		_framesPerRow = rhs._framesPerRow;
		_totalFrames = rhs._totalFrames;
		_xOffset = rhs._xOffset;
		_yOffset = rhs._yOffset;
		if( _spriteList )
		{
			delete[] _spriteList;
		}
		_spriteList = new Sprite[_totalFrames];
#ifdef __MINGW32__
		std::copy( rhs._spriteList, rhs._spriteList + _totalFrames, _spriteList );
#else
		std::copy( rhs._spriteList, rhs._spriteList + _totalFrames, stdext::checked_array_iterator<Sprite *>( _spriteList, _totalFrames ) );
#endif // __MINGW32__
		return *this;
	}

	const Sprite& SpriteSheet::GetSpriteAt( int row, int col ) const
	{
		if( _spriteList )
		{
			if( !_checkIndex( row, col ) )
			{
				std::cerr << "Invalid index \n";
				throw IndexOutOfRange( "Invalid index \n" );
			}
			auto& sp = _spriteList[row * _framesPerRow + col];
			sp.setAngle( _angle );
			sp.setOrigin( or_x, or_y );
			sp.setScale( _scaleX, _scaleY );
			return sp;
		}
		throw std::invalid_argument( "The spriteSheet is empty \n" );
	}

	const Sprite& SpriteSheet::GetSpriteAt( int index ) const
	{
		if( _spriteList )
		{
			if( !_checkIndex( index ) )
			{
				std::cerr << "Invalid index \n";
				throw IndexOutOfRange( "Invalid index \n" );
			}
			auto& sp = _spriteList[index];
			sp.setAngle( _angle );
			sp.setOrigin( or_x, or_y );
			sp.setScale( _scaleX, _scaleY );
			return sp;
		}
		throw std::invalid_argument( "The spriteSheet is empty \n" );
	}

	void SpriteSheet::draw( int flags ) const
	{
		int w = _width * _framesPerRow;
		int h = _height * ( _totalFrames / _framesPerRow );
		draw_image( image.get( ), or_x, or_y, w, h, _x - ( w * _scaleX ) / 2, _y - ( h * _scaleY ) / 2, _angle, w * _scaleX, h * _scaleY, flags );
	}

	void SpriteSheet::drawAt( int row, int col, double x, double y, int flags ) const
	{
		const auto& _sp = GetSpriteAt( row, col );
		const_cast<sp::Sprite *>( &_sp )->setPosition( x, y );
		_sp.draw( flags );
	}

	void SpriteSheet::drawAt( int row, int col, int flags ) const
	{
		drawAt( row, col, _x, _y, flags );
	}

	inline bool SpriteSheet::_checkIndex( int row, int col ) const
	{
		if( row >= 0 && row < ( _totalFrames / _framesPerRow ) &&
			col >= 0 && col < _framesPerRow )
		{
			return true;
		}
		return false;
	}

	inline bool SpriteSheet::_checkIndex( int index ) const
	{
		if( index >= 0 && index < _totalFrames )
		{
			return true;
		}
		return false;
	}

	alx::Size<double> SpriteSheet::getSize( ) const
	{
		return alx::Size<double>( _width * _scaleX * _framesPerRow, _height * _scaleY * (double) ( _totalFrames / _framesPerRow ) );
	}

	alx::Size<double> SpriteSheet::getSpriteSize( ) const
	{
		DEBUG_MSG( ScaleX );
		return alx::Size<double>( ( _width - ( _xOffset >= 0 ? _xOffset : -_xOffset ) ) * _scaleX, ( _height - ( _yOffset >= 0 ? _yOffset : -_yOffset ) ) * _scaleY );
	}

	SpriteSheet::operator bool( ) const
	{
		return ( image && _spriteList != nullptr );
	}

	SpriteSheet::~SpriteSheet( )
	{
		if( _spriteList )
		{
			delete[] _spriteList;
		}
	}

}
