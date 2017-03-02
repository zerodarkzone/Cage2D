#include "Cage2d/stdafx.h"
#include "Cage2d/AnimatedSprite.hpp"

namespace sp
{
	AnimatedSprite::AnimatedSprite( ) : Sprite( ), _frameSpeed( 1 ), _framesPerRow( 1 ), _totalFrames( 1 ), _currentFrame( 0 ), _frameCounter( 0 ), _animationDirection( 1 )
	{}

	AnimatedSprite::AnimatedSprite( alx::Bitmap image, double width, double height, int frameSpeed, int totalFrames, int animationDirection )
		: AnimatedSprite( )
	{
		this->image = image;
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw std::exception( );
		}
		_width = width;
		_height = height;
		_frameSpeed = frameSpeed;
		_totalFrames = totalFrames;
		_animationDirection = animationDirection;

		_framesPerRow = std::floor( ( image.getWidth( ) / _width ) );
	}

	AnimatedSprite::AnimatedSprite( alx::Bitmap image, double framesPerRow, int frameSpeed, int totalFrames, int animationDirection )
		: AnimatedSprite( )
	{
		this->image = image;
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw std::exception( );
		}
		_framesPerRow = framesPerRow;
		_frameSpeed = frameSpeed;
		_totalFrames = totalFrames;
		_animationDirection = animationDirection;
		_width = this->image.getWidth( ) / _framesPerRow;
		_height = this->image.getHeight( ) / ( _totalFrames / _framesPerRow );
	}

	void AnimatedSprite::update( )
	{
		if( ++_frameCounter >= _frameSpeed )
		{
			_currentFrame += _animationDirection;
			if( _currentFrame >= _totalFrames )
			{
				_currentFrame = 0;
			}
			else if( _currentFrame <= 0 )
			{
				_currentFrame = _totalFrames - 1;
			}
			_frameCounter = 0;
		}
	}

	void AnimatedSprite::draw( int flags ) const
	{
		int fx = ( _currentFrame % _framesPerRow ) * _width;
		int fy = ( _currentFrame / _framesPerRow ) * _height;

		double s_width = _width * _scaleX;
		double s_height = _height * _scaleY;

		draw_image( image.get( ), fx, fy, _width, _height, _x - s_width / 2, _y - s_height / 2, _angle, s_width, s_height, flags );
	}

	AnimatedSprite::~AnimatedSprite( )
	{}
}
