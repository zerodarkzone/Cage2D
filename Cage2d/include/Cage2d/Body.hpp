#pragma once
#ifndef BODY_HPP_INCLUDED
#define BODY_HPP_INCLUDED

namespace Cage2d
{
	class Entity;
	//class Body;
	//inline bool intersect( const Body &a, const Body &b );
	//inline bool intersect( const alx::Rect<double> &a, const alx::Rect<double> &b );

	class Body
	{
	public:
		enum class Type { STATIC, DYNAMIC, KINEMATIC, Type_MAX };
		enum Dir { RIGHT, LEFT, DOWN, UP, Dir_MAX };
	private:
		template <typename T> friend class JList::DoubleChainedList;
		friend bool intersect( const Body& a, const Body& b );
		friend class Entity;

		Body( bool solid = true );
		Body( const alx::Rect<double>& rect, bool solid = true );
		Body( const alx::Point<double>& pos, const alx::Size<double>& size, bool solid = true );
		Body( double x, double y, double w, double h, bool solid = true );

		static double _gravity;
		bool _coll[4];

		alx::Size<double> _vel, _accel, _iVel;
		alx::Rect<double> _rect, _oRect;
		bool _alive, _solid, _grav, _collision;
		std::vector<alx::Size<double>> _forces;
		std::vector<alx::Size<double>> _impulses;
		double _mass;
		Entity *_entity;
		uint16_t _groupMask;
		uint16_t _collisionMask;
		bool _draw;
		int8_t _dirX, _dirY;

		Type _type;

		Body( const Body& b );

	public:
		virtual ~Body( );
		/**
		*  \brief Variadic template function, the arguments must be the same as the constructors:
		*  Body( const alx::Rect<double> &rect, bool solid = true );
		*  Body( const alx::Point<double> &pos, const alx::Size<double> &size, bool solid = true );
		*  Body( double x, double y, double w, double h, bool solid = true );
		**/
		CREATE_FUNC( Cage2d::Body )
			Body& operator=( const Body& b );
		bool Init( )
		{
			return true;
		}
		virtual void Update( );
		virtual void Draw( );
		void Draw( bool draw );
		void Solid( bool solid );
		bool Solid( );
		void Alive( bool alive );
		bool Alive( ) const;
		void Gravity( bool gravity );
		static void Gravity( double gravity );
		static double Gravity( );
		void ApplyForce( double forceX, double forceY );
		void ApplyForce( const alx::Size<double>& force );
		void ApplyImpulse( double impulseX, double impulseY );
		void ApplyImpulse( const alx::Size<double>& impulse );
		void SetVelocity( const alx::Size<double>& velocity );
		void SetVelocity( double x, double y );
		void SetVelocityX( double x );
		void SetVelocityY( double y );
		alx::Size<double> GetVelocity( ) const;
		double GetVelotityX( ) const;
		double GetVelocityY( ) const;
		void SetMass( double mass );
		double GetMass( ) const;
		void SetGroupMask( uint16_t mask );
		void SetCollisionMask( uint16_t mask );
		void SetType( Type type );
		Type GetType( ) const;

		uint16_t GetGroupMask( ) const;
		uint16_t GetCollisionMask( ) const;

		alx::Point<double> GetPosition( ) const;
		alx::Size<double> GetSize( ) const;
		alx::Rect<double> GetRect( ) const;

		void SetEntity( Entity *entity );

		bool Collision( const Body& other );
		const bool( &Collision( ) const )[4];
		void ResetCollisions( );

	private:
		bool RightCollision( const Body& other );
		bool LeftCollision( const Body& other );
		bool DownCollision( const Body& other );
		bool UpCollision( const Body& other );
	};

	inline
		bool Body::RightCollision( const Body& other )
	{
		return _oRect.getRight( ) < other._rect.getLeft( ) &&
			_rect.getRight( ) >= other._rect.getLeft( );
	}

	inline
		bool Body::LeftCollision( const Body& other )
	{
		return _oRect.getLeft( ) >= other._rect.getRight( ) &&
			_rect.getLeft( ) < other._rect.getRight( );
	}

	inline
		bool Body::DownCollision( const Body& other )
	{
		return _oRect.getBottom( ) < other._rect.getTop( ) &&
			_rect.getBottom( ) >= other._rect.getTop( );
	}

	inline
		bool Body::UpCollision( const Body& other )
	{
		return _oRect.getTop( ) >= other._rect.getBottom( ) &&
			_rect.getTop( ) < other._rect.getBottom( );
	}

	inline
		bool intersect( const Body& a, const Body& b )
	{
		return ( a._rect.getRight( ) > b._rect.getLeft( ) &&
				 a._rect.getLeft( ) < b._rect.getRight( ) &&
				 a._rect.getBottom( ) > b._rect.getTop( ) &&
				 a._rect.getTop( ) < b._rect.getBottom( ) );
	}

	inline
		bool intersect( const alx::Rect<double>& a, const alx::Rect<double>& b )
	{
		return ( a.getRight( ) >= b.getLeft( ) &&
				 a.getLeft( ) <= b.getRight( ) &&
				 a.getBottom( ) >= b.getTop( ) &&
				 a.getTop( ) <= b.getBottom( ) );
	}
}

#endif // BODY_HPP_INCLUDED
