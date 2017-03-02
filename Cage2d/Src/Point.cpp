#include "Cage2d/stdafx.h"
#include "Cage2d/Point.hpp"

namespace Cage2d
{
	Point::Point( ) : x( 0 ), y( 0 ), z( 0 )
	{}

	Point::Point( double x, double y, double z ) : x( x ), y( y ), z( z )
	{}

	Point::Point( const Point& point )
	{
		x = point.x;
		y = point.y;
		z = point.z;
	}

	Point& Point::operator=( const Point& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	bool Point::operator==( const Point& rhs ) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool Point::operator!=( const Point& rhs ) const
	{
		return !operator==( rhs );
	}

	bool Point::operator<( const Point& rhs ) const
	{
		return x < rhs.x && y < rhs.y && z < rhs.z;
	}

	bool Point::operator>( const Point& rhs ) const
	{
		return x > rhs.x && y > rhs.y && z > rhs.z;
	}

	bool Point::operator<=( const Point& rhs ) const
	{
		return !operator>( rhs );
	}

	bool Point::operator>=( const Point& rhs ) const
	{
		return !operator<( rhs );
	}

	Point Point::operator+( const Point& rhs )
	{
		return Point( x + rhs.x, y + rhs.y, z + rhs.z );
	}

	Point& Point::operator+=( const Point& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Point Point::operator-( const Point& rhs )
	{
		return Point( x - rhs.x, y - rhs.y, z - rhs.z );
	}

	Point& Point::operator-=( const Point& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	double Point::distance( const Point& point )
	{
		double dx = point.x - x;
		double dy = point.y - y;
		double dz = point.z - z;

		return std::sqrt( dx * dx + dy * dy + dz * dz );
	}

	double Point::square_distance( const Point& point )
	{
		double dx = point.x - x;
		double dy = point.y - y;
		double dz = point.z - z;

		return ( dx * dx + dy * dy + dz * dz );
	}

	double Point::angle( const Point& point )
	{
		double dx = point.x - x;
		double dy = point.y - y;
		return std::atan2( dy, dx );
	}

	Point::~Point( )
	{}
}
