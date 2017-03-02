#pragma once

namespace sp
{
	class AnimatedSprite :
		public Sprite
	{
		int _frameSpeed, _framesPerRow, _totalFrames, _currentFrame, _frameCounter, _animationDirection;
	public:
		AnimatedSprite( );
		AnimatedSprite( alx::Bitmap image, double width, double height, int frameSpeed, int totalFrames, int animationDirection = 1 );
		AnimatedSprite( alx::Bitmap image, double framesPerRow, int frameSpeed, int totalFrames, int animationDirection = 1 );

		void update( ) override;
		void draw( int flags = 0 ) const override;

		~AnimatedSprite( );
	};
}
