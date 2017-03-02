#pragma once

namespace sp
{

	class SpriteSheet :
		public Sprite
	{
		int _framesPerRow, _totalFrames, _currentFrame, _xOffset, _yOffset;
		Sprite *_spriteList;
		bool _checkIndex( int row, int col ) const;
		bool _checkIndex( int index ) const;
		friend class Animation;
	public:
		SpriteSheet( );
		SpriteSheet( const alx::Bitmap& image, double width, double height, int totalFrames, int xOffset = 0, int yOffset = 0 );
		SpriteSheet( const alx::Bitmap& image, int framesPerRow, int totalFrames, int xOffset = 0, int yOffset = 0 );

		SpriteSheet( const std::string& filename, double width, double height, int totalFrames, int xOffset = 0, int yOffset = 0 );
		SpriteSheet( const std::string& filename, int framesPerRow, int totalFrames, int xOffset = 0, int yOffset = 0 );

		SpriteSheet( const SpriteSheet& rhs );
		SpriteSheet& operator=( const SpriteSheet& rhs );

		const Sprite& GetSpriteAt( int row, int col ) const;
		const Sprite& GetSpriteAt( int index ) const;

		void draw( int flags = 0 ) const override;
		void drawAt( int row, int col, double x, double y, int flags = 0 ) const;
		void drawAt( int row, int col, int flags = 0 ) const;

		alx::Size<double> getSize( ) const override;
		alx::Size<double> getSpriteSize( ) const;

		operator bool( ) const;

		~SpriteSheet( );
	};

}

class IndexOutOfRange : public std::exception
{
public:
	explicit IndexOutOfRange( const char *_arg ) _NOEXCEPT
	{
		ex_arg = _arg;
	}
	virtual const char *what( ) const _NOEXCEPT
	{
		return ex_arg;
	}
	virtual ~IndexOutOfRange( ) _NOEXCEPT {}
protected:
	const char *ex_arg;
};
