#pragma once
namespace Cage2d
{
	class Point
	{
	public:
		double x;
		double y;
		double z;
		Point( );
		Point( double x, double y, double z = 0 );
		Point( const Point& point );
		Point& operator=( const Point& rhs );
		bool operator==( const Point& rhs ) const;
		bool operator!=( const Point& rhs ) const;
		bool operator<( const Point& rhs ) const;
		bool operator>( const Point& rhs ) const;
		bool operator<=( const Point& rhs ) const;
		bool operator>=( const Point& rhs ) const;
		Point operator+( const Point& rhs );
		Point& operator+=( const Point& rhs );
		Point operator-( const Point& rhs );
		Point& operator-=( const Point& rhs );
		double distance( const Point& point );
		double square_distance( const Point& point );
		double angle( const Point& point );
		friend std::ostream& operator<<( std::ostream& out, Point& rhs )
		{
			out << "x: " << rhs.x << ", y: " << rhs.y << ", z: " << rhs.z;
			return out;
		}
		~Point( );
	};
}
