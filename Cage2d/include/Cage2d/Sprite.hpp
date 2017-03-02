#pragma once
namespace sp
{
	class Sprite
	{
	protected:
		double _x, _y, _width, _height, _angle, _scaleX, _scaleY;
		alx::Bitmap image;
		double or_x, or_y;
		alx::Rect<double> _rect;
		double width( );
		double height( );
	public:
		ReadOnlyProperty<double> X, Y, Angle, ScaleX, ScaleY;
		ReadOnlyProperty<double, Sprite> Width, Height;

		Sprite( );
		Sprite( const Sprite& sprite );
		explicit Sprite( const alx::Bitmap& image, double x = 0, double y = 0, double angle = 0, double scale = 1 );
		explicit Sprite( const std::string& filename, double x = 0, double y = 0, double angle = 0, double scale = 1 );
		Sprite( double x, double y, double width, double heigth, double angle = 0, double scale = 1 );

		Sprite& operator=( const Sprite& sprite );

		const alx::Bitmap& getBitmap( ) const;
		alx::Point<double> getPosition( ) const;
		virtual alx::Size<double> getSize( ) const;

		void setPosition( double x, double y );
		void setPosition( const alx::Point<double>& position );
		void setPosition( const Cage2d::Point& position );
		void setPosition( const Cage2d::Vec2& position );
		void setSize( double width, double heigth );
		void setSize( const alx::Size<double>& size );
		void setSize( const Cage2d::Point& size );
		void setSize( const Cage2d::Vec2& size );
		void setOrigin( double x, double y );
		void setOrigin( alx::Point<double>& origin );
		void setOrigin( const Cage2d::Point& origin );
		void setOrigin( const Cage2d::Vec2& origin );
		void setBitmap( const alx::Bitmap& image );
		void setBitmap( const std::string& filename );
		void setAngle( double angle );
		void setScale( double scale );
		void setScale( double scaleX, double scaleY );

		void move( double dx, double dy );
		void move( const alx::Point<double>& dist );
		void move( const Cage2d::Point& dist );
		void move( const Cage2d::Vec2& dist );

		virtual void draw( int flags = 0 ) const;
		virtual void update( );

		operator bool( ) const;

		virtual ~Sprite( );
	};

}

#include <exception>
#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif
