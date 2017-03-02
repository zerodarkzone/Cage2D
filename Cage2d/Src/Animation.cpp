#include "Cage2d/stdafx.h"
#include "Cage2d/Animation.hpp"

namespace sp
{
	Animation::Animation( ) :
		_currentFrame( 0 ), _startFrame( 0 ), _endFrame( 0 ), _frameSpeed( 0 ), _frameCounter( 0 ), _canAnimate( false ), _animationDirection( 0 ),
		_paused( false ), _length( 0 ), _copy( false ), _default( ), _spriteSheet( &_default )
	{}

	Animation::Animation( const SpriteSheet& spriteSheet, int frameSpeed, int startFrame, int endFrame, int animationDirection, bool copy ) :
		_currentFrame( 0 ), _startFrame( startFrame ), _endFrame( endFrame ), _frameSpeed( frameSpeed ), _frameCounter( 0 ), _canAnimate( false ),
		_animationDirection( animationDirection ), _paused( false ), _length( 0 ), _copy( copy ), _default( _copy ? spriteSheet : SpriteSheet( ) ),
		_spriteSheet( _copy ? &_default : &spriteSheet )
	{
		_currentFrame = _startFrame;
		if( !_spriteSheet || !( *_spriteSheet ) )
		{
			std::cerr << "Couldn't create animation" << std::endl;
			throw std::invalid_argument( "Couldn't create animation" );
		}
		_canAnimate = true;
		for( int frameNumber = _startFrame; frameNumber <= _endFrame; ++frameNumber )
		{
			_animationSeqeuence.push_back( frameNumber );
		}
		_length = static_cast<int>( _animationSeqeuence.size( ) );
	}

	void Animation::update( )
	{
		if( _canAnimate && !_paused )
		{
			if( ++_frameCounter >= _frameSpeed )
			{
				_currentFrame += _animationDirection;
				if( _currentFrame >= _length )
				{
					_currentFrame = 0;
				}
				else if( _currentFrame <= 0 )
				{
					_currentFrame = _length - 1;
				}
				_frameCounter = 0;
			}
		}
	}

	void Animation::draw( double x, double y, int flags ) const
	{
		if( _spriteSheet && *_spriteSheet )
		{
			int row = ( _animationSeqeuence[_currentFrame] / _spriteSheet->_framesPerRow );
			int col = ( _animationSeqeuence[_currentFrame] % _spriteSheet->_framesPerRow );
			_spriteSheet->drawAt( row, col, x, y, flags );
		}
	}

	void Animation::draw( const alx::Point<double>& point, int flags ) const
	{
		draw( point.getX( ), point.getY( ), flags );
	}

	void Animation::pauseAnimation( int frame )
	{
		_paused = true;
		if( frame >= 0 )
		{
			if( frame > _endFrame )
			{
				throw IndexOutOfRange( "Invalid frame parameter" );
			}
			_currentFrame = frame;
		}
	}

	void Animation::resumeAnimation( )
	{
		_paused = false;
	}

	bool Animation::isAnimationPaused( )
	{
		return _paused;
	}

	const SpriteSheet& Animation::getSpriteSheet( ) const
	{
		if( _spriteSheet && *_spriteSheet )
		{
			return *_spriteSheet;
		}
		throw InvalidBitmapException( "SpriteSheet was not created properly" );
	}

	Animation::~Animation( )
	{}
}
