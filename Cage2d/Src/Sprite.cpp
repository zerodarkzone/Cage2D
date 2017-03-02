#include "Cage2d/stdafx.h"
#include "Cage2d/Sprite.hpp"

namespace sp
{
	using namespace Cage2d;

	Sprite::Sprite( ) : _x( 0 ), _y( 0 ), _width( 0 ), _height( 0 ), _angle( 0 ), _scaleX( 1 ), _scaleY( 1 ), image( ), or_x( 0 ), or_y( 0 ),
		X( _x ), Y( _y ), Angle( _angle ), ScaleX( _scaleX ), ScaleY( _scaleY ),
		Width( _width, &Sprite::width, this ), Height( _height, &Sprite::height, this )
	{}

	Sprite::Sprite( const Sprite& sprite ) : Sprite( )
	{
		this->_x = sprite._x;
		this->_y = sprite._y;
		this->_angle = sprite._angle;
		this->_height = sprite._height;
		this->_width = sprite._width;
		this->_scaleX = sprite._scaleX;
		this->_scaleY = sprite._scaleY;
		this->image = sprite.image;
		this->or_x = sprite.or_x;
		this->or_y = sprite.or_y;
		this->_rect = sprite._rect;
	}

	Sprite::Sprite( const alx::Bitmap& image_, double x, double y, double angle, double scale ) : Sprite( )
	{
		this->image = image_;
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw InvalidBitmapException( "Invalid Bitmap" );
		}
		_x = x;
		_y = y;
		_angle = angle;
		_scaleX = scale;
		_scaleY = scale;
		_width = this->image.getWidth( );
		_height = this->image.getHeight( );
		_rect.set( alx::Point<double>( _x - ( _width * _scaleX ) / 2, _y - ( _height * _scaleY ) / 2 ), alx::Size<double>( _width * _scaleX, _height * _scaleY ) );
	}


	Sprite::Sprite( const std::string& filename, double x, double y, double angle, double scale ) : Sprite( )
	{
		this->image = BFC.LoadBitmap( filename );
		_x = x;
		_y = y;
		_angle = angle;
		_scaleX = scale;
		_scaleY = scale;
		_width = this->image.getWidth( );
		_height = this->image.getHeight( );
		_rect.set( alx::Point<double>( _x - ( _width * _scaleX ) / 2, _y - ( _height * _scaleY ) / 2 ), alx::Size<double>( _width * _scaleX, _height * _scaleY ) );
	}

	Sprite::Sprite( double x, double y, double width, double heigth, double angle, double scale ) : _x( x ), _y( y ), _width( width ), _height( heigth ),
		_angle( angle ), _scaleX( scale ), _scaleY( scale ), image( ), or_x( 0 ), or_y( 0 ), X( _x ), Y( _y ),
		Angle( _angle ), ScaleX( _scaleX ), ScaleY( _scaleY ), Width( _width ), Height( _height )
	{
		this->image = al_create_bitmap( static_cast<int>( width * scale ), static_cast<int>( heigth * scale ) );
		_rect.set( alx::Point<double>( _x - ( _width * _scaleX ) / 2, _y - ( _height * _scaleY ) / 2 ), alx::Size<double>( _width * _scaleX, _height * _scaleY ) );
	}

	Sprite& Sprite::operator=( const Sprite& sprite )
	{
		this->_x = sprite._x;
		this->_y = sprite._y;
		this->_angle = sprite._angle;
		this->_height = sprite._height;
		this->_width = sprite._width;
		this->_scaleX = sprite._scaleX;
		this->_scaleY = sprite._scaleY;
		this->image = sprite.image;
		this->or_x = sprite.or_x;
		this->or_y = sprite.or_y;
		this->_rect = sprite._rect;
		return *this;
	}

	const alx::Bitmap& Sprite::getBitmap( ) const
	{
		return image;
	}

	alx::Point<double> Sprite::getPosition( ) const
	{
		return alx::Point<double>( _x, _y );
	}

	alx::Size<double> Sprite::getSize( ) const
	{
		return _rect.getSize( );
	}

	void Sprite::setPosition( double x, double y )
	{
		_x = x;
		_y = y;
		_rect.setCenter( _x, _y );
	}

	void Sprite::setPosition( const alx::Point<double>& position )
	{
		_x = position.getX( );
		_y = position.getY( );
		_rect.setCenter( _x, _y );
	}

	void Sprite::setSize( double width, double height )
	{
		_width = width;
		_height = height;
		_rect.setSize( _width * _scaleX, _height * _scaleY );
	}

	void Sprite::setSize( const alx::Size<double>& size )
	{
		_width = size.getWidth( );
		_height = size.getHeight( );
		_rect.setSize( _width * _scaleX, _height * _scaleY );
	}

	void Sprite::setOrigin( double x, double y )
	{
		or_x = x;
		or_y = y;
	}

	void Sprite::setOrigin( alx::Point<double>& origin )
	{
		or_x = origin.getX( );
		or_y = origin.getY( );
	}

	void Sprite::setBitmap( const alx::Bitmap& image_ )
	{
		this->image = image_;
		if( !this->image )
		{
			std::cerr << "Invalid Bitmap " << std::endl;
			throw InvalidBitmapException( "Invalid Bitmap" );
		}
		_width = this->image.getWidth( );
		_height = this->image.getHeight( );
		_rect.set( alx::Point<double>( _x - ( _width * _scaleX ) / 2, _y - ( _height * _scaleY ) / 2 ), alx::Size<double>( _width * _scaleX, _height * _scaleY ) );
	}

	void Sprite::setBitmap( const std::string& filename )
	{
		this->image = BFC.LoadBitmap( filename );
		_width = this->image.getWidth( );
		_height = this->image.getHeight( );
		_rect.set( alx::Point<double>( _x - ( _width * _scaleX ) / 2, _y - ( _height * _scaleY ) / 2 ), alx::Size<double>( _width * _scaleX, _height * _scaleY ) );
	}

	void Sprite::setAngle( double angle )
	{
		_angle = angle;
	}

	void Sprite::setScale( double scale )
	{
		_scaleX = _scaleY = scale;
		_rect.setSize( _width * _scaleX, _height * _scaleY );
	}

	void Sprite::setScale( double scaleX, double scaleY )
	{
		_scaleX = scaleX;
		_scaleY = scaleY;
		_rect.setSize( _width * _scaleX, _height * _scaleY );
	}

	void Sprite::move( double dx, double dy )
	{
		_x += dx;
		_y += dy;
		_rect.setCenter( _x, _y );
	}

	void Sprite::move( const alx::Point<double>& dist )
	{
		_x += dist.getX( );
		_y += dist.getY( );
		_rect.setCenter( _x, _y );
	}

	void Sprite::draw( int flags ) const
	{
		draw_image( image.get( ), float( or_x ), float( or_y ), float( _width ), float( _height ), float( _rect.getLeft( ) ), float( _rect.getTop( ) ), float( _angle ),
					float( _rect.getWidth( ) ), float( _rect.getHeight( ) ), flags );
	}

	void Sprite::update( )
	{}

	Sprite::operator bool( ) const
	{
		return ( image && true );
	}

	double Sprite::width( )
	{
		return _width * _scaleX;
	}

	double Sprite::height( )
	{
		return _height * _scaleY;
	}

	Sprite::~Sprite( )
	{
		//std::cout << "sprite destroyed";
	}

}
