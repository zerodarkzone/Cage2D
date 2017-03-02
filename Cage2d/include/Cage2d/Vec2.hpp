#pragma once
#include "Property.hpp"
#include "Point.hpp"

namespace Cage2d
{
	class Vec2
	{
	private:
		double _length;
		double _sqr_length;
		double Length( );
		double Sqr_Length( );
	public:
		double x, y;
		ReadOnlyProperty<double, Vec2> length, square_length;
		Vec2( );
		Vec2( double _x, double _y );
		Vec2( const Vec2& v );
		Vec2( const Point& p1, const Point& p2 );
		Vec2( const Point& p );
		Vec2& operator=( const Vec2& vec );
		bool operator==( const Vec2& rhs ) const;
		bool operator!=( const Vec2& rhs ) const;
		bool operator<( const Vec2& rhs ) const;
		bool operator>( const Vec2& rhs ) const;
		bool operator<=( const Vec2& rhs ) const;
		bool operator>=( const Vec2& rhs ) const;
		Vec2 operator+( const Vec2& rhs );
		Vec2& operator+=( const Vec2& rhs );
		Vec2 operator-( const Vec2& rhs );
		Vec2& operator-=( const Vec2& rhs );
		double angle( );
		Vec2& normalize( );
		friend std::ostream& operator<<( std::ostream& out, Vec2& rhs )
		{
			out << "x: " << rhs.x << ", y: " << rhs.y << ", length: " << rhs.length;
			return out;
		}
		~Vec2( );
	};
}
