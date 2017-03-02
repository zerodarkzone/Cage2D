#pragma once
#include "SpriteSheet.hpp"

namespace sp
{
	class Animation
	{
		int _currentFrame, _startFrame, _endFrame, _frameSpeed, _frameCounter;
		bool _canAnimate;
		int _animationDirection;
		bool _paused;
		int _length;
		bool _copy;
		SpriteSheet _default;
		const SpriteSheet *_spriteSheet;
		std::vector<int> _animationSeqeuence;
	public:
		Animation( );
		Animation( const SpriteSheet& spriteSheet, int frameSpeed, int startFrame, int endFrame, int animationDirection = 1, bool copy = false );
		void update( );
		void draw( double x, double y, int flags = 0 ) const;
		void draw( const alx::Point<double>& point, int flags = 0 ) const;
		void pauseAnimation( int frame = -1 );
		void resumeAnimation( );
		bool isAnimationPaused( );
		const SpriteSheet& getSpriteSheet( ) const;

		~Animation( );
	};
}
