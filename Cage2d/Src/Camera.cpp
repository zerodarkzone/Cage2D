#include "Cage2d/stdafx.h"
#include "Cage2d/Camera.hpp"

namespace Cage2d
{
#define ADD_CONST_POINT(__p1, __p2) (*(const_cast<decltype(__p1)*>(&__p1)) + *(const_cast<decltype(__p2)*>(&__p2)))

	Camera::Camera( double x, double y, double width, double height ) : _position( x, y ), _resX1( INT_MIN ), _resY1( INT_MIN ), _resX2( INT_MAX ), _resY2( INT_MAX ), _scale( 1 ),
		_buffer( width == INT_MAX ? (double)Game::SCREEN_WIDTH : width, height == INT_MAX ? (double)Game::SCREEN_HEIGHT : height )
	{}

	Camera::Camera( const alx::Point<double>& pos, alx::Size<double> size ) : Camera( pos.getX( ), pos.getY( ), size.getWidth( ), size.getHeight( ) )
	{}

	alx::Point<double> Camera::GetPosition( ) const
	{
		return  _position;
	}

	double Camera::GetPositionX( ) const
	{
		return Camera::GetPosition( ).getX( );
	}

	double Camera::GetPositionY( ) const
	{
		return Camera::GetPosition( ).getY( );
	}

	double Camera::GetScale( ) const
	{
		return _scale;
	}

	void Camera::SetPosition( double x, double y )
	{
		_position.set( x, y );
		if( _position.getX( ) < _resX1 )
		{
			_position.setX( _resX1 );
		}

		if( _position.getX( ) > _resX2 )
		{
			_position.setX( _resX2 );
		}

		if( _position.getY( ) < _resY1 )
		{
			_position.setY( _resY1 );
		}

		if( _position.getY( ) > _resY2 )
		{
			_position.setY( _resY2 );
		}
	}

	void Camera::SetPosition( const alx::Point<double>& position )
	{
		Camera::SetPosition( position.getX( ), position.getY( ) );
	}

	void Camera::SetPositionX( double x )
	{
		Camera::SetPosition( x, Camera::GetPosition( ).getY( ) );
	}

	void Camera::SetPositionY( double y )
	{
		Camera::SetPosition( Camera::GetPosition( ).getX( ), y );
	}

	void Camera::SetRestrictions( double x1, double y1, double x2, double y2 )
	{
		_resX1 = x1;
		_resX2 = x2;
		_resY1 = y1;
		_resY2 = y2;
	}

	void Camera::SetSize( double width, double height )
	{
		_buffer = alx::Bitmap( width, height );
	}

	void Camera::SetSize( const alx::Size<double>& size )
	{
		Camera::SetSize( size.getWidth( ), size.getHeight( ) );
	}

	void Camera::SetWidth( double width )
	{
		Camera::SetSize( width, _buffer.getHeight( ) );
	}

	void Camera::SetHeight( double height )
	{
		Camera::SetSize( _buffer.getWidth( ), height );
	}

	void Camera::SetScale( double scale )
	{
		_scale = scale;
	}

	void Camera::ClearFunctions( )
	{
		_drawFuncs.clear( );
	}

	Camera& Camera::operator << ( const std::function<void( )>& func )
	{
		_drawFuncs.push_back( func );
		return *this;
	}

	void Camera::AddDrawFunction( const std::function<void( )>& func )
	{
		_drawFuncs.push_back( func );
	}

	void Camera::AddBackDrawFunction( const std::function<void( )>& func )
	{
		_drawFuncs.insert( _drawFuncs.begin( ), func );
	}

	void Camera::Draw( alx::Display& window )
	{
		if( _buffer )
		{
			_buffer.setTarget( );
			al_clear_to_color( al_map_rgba( 0, 0, 0, 0 ) );
			double width = _buffer.getWidth( );
			double height = _buffer.getHeight( );
			double s_width = width * _scale;
			double s_height = height * _scale;
			alx::Transform transform;
			transform.setIdentity( );
			transform.translate( ( width / 2 ) - _position.getX( ),
				( height / 2 ) - _position.getY( ) );
			transform.scale( _scale, _scale );
			transform.translate( ( width - s_width ) / 2, ( height - s_height ) / 2 );
			transform.setCurrent( );
			for( auto fun : _drawFuncs )
			{
				fun( );
			}
			transform.setIdentity( );
			transform.setCurrent( );

			alx::Display::setTarget( window );

			_buffer.draw( 0, 0 );
		}
	}

	Camera::~Camera( )
	{
		DEBUG_MSG( "Camera destroyed" );
	}

}
