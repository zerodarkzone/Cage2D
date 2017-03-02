#include "Cage2d/stdafx.h"
#include "Cage2d/Vec2.hpp"

namespace Cage2d
{

	Vec2::Vec2( ) : _length( 0 ), _sqr_length( 0 ), x( 0 ), y( 0 ), length( _length, &Vec2::Length, this ), square_length( _sqr_length, &Vec2::Sqr_Length, this )
	{}

	Vec2::Vec2( double x_, double y_ ) : _length( 0 ), _sqr_length( 0 ),
		x( x_ ), y( y_ ), length( _length, &Vec2::Length, this ), square_length( _sqr_length, &Vec2::Sqr_Length, this )
	{
		Length( );
	}

	Vec2::Vec2( const Vec2& v ) : Vec2( v.x, v.y )
	{}

	Vec2::Vec2( const Point& p1, const Point& p2 ) : Vec2( )
	{
		x = p2.x - p1.y;
		y = p2.y - p1.y;
		Length( );
	}

	Vec2::Vec2( const Point& p ) : Vec2( p.x, p.y )
	{}

	Vec2& Vec2::operator=( const Vec2& vec )
	{
		x = vec.x;
		y = vec.y;
		Length( );
		return *this;
	}

	bool Vec2::operator==( const Vec2& rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool Vec2::operator!=( const Vec2& rhs ) const
	{
		return !operator==( rhs );
	}

	bool Vec2::operator<( const Vec2& rhs ) const
	{
		return x < rhs.x && y < rhs.y;
	}

	bool Vec2::operator>( const Vec2& rhs ) const
	{
		return x > rhs.x && y > rhs.y;
	}

	bool Vec2::operator<=( const Vec2& rhs ) const
	{
		return !operator>( rhs );
	}

	bool Vec2::operator>=( const Vec2& rhs ) const
	{
		return !operator<( rhs );
	}

	Vec2 Vec2::operator+( const Vec2& rhs )
	{
		double _dx = x + rhs.x;
		double _dy = y + rhs.y;
		return Vec2( _dx, _dy );
	}

	Vec2& Vec2::operator+=( const Vec2& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		Length( );
		return *this;
	}

	Vec2 Vec2::operator-( const Vec2& rhs )
	{
		double _dx = x - rhs.x;
		double _dy = y - rhs.y;
		return Vec2( _dx, _dy );
	}

	Vec2& Vec2::operator-=( const Vec2& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		Length( );
		return *this;
	}

	double Vec2::angle( )
	{
		return std::atan2( y, x );
	}
	Vec2& Vec2::normalize( )
	{
		double norm = length;
		y /= norm;
		x /= norm;
		Length( );
		return *this;
	}

	double Vec2::Length( )
	{
		return _length = std::sqrt( ( x ) * (x) +( y ) * ( y ) );
	}

	double Vec2::Sqr_Length( )
	{
		return _sqr_length = ( ( x ) * (x) +( y ) * ( y ) );
	}

	Vec2::~Vec2( )
	{}
}
