#include "Cage2d/stdafx.h"
#include "Cage2d/Body.hpp"
#include "Cage2d/Entity.hpp"

namespace Cage2d
{
	Body::Body( bool solid ) : _vel( 0.0, 0.0 ), _accel( 0.0, 0.0 ), _iVel( 0.0, 0.0 ),
		_rect( ), _oRect( ), _alive( true ), _solid( solid ), _grav( true ), _collision( false ), _forces( ), _impulses( ), _mass( 1.0 ), _entity( nullptr ),
		_groupMask( UINT16_MAX ), _collisionMask( UINT16_MAX ), _draw( true ), _dirX( 0 ), _dirY( 0 ), _type( Body::Type::STATIC )
	{
		std::fill( std::begin( _coll ), std::end( _coll ), false );
	}

	Body::~Body( )
	{
		//std::cout << "Body" << std::endl;
	}

	Body::Body( const alx::Rect<double>& rect, bool solid ) : Body( solid )
	{
		_rect = _oRect = rect;
	}

	Body::Body( const alx::Point<double>& pos, const alx::Size<double>& size, bool solid ) : Body( alx::Rect<double>( pos, size ), solid )
	{}

	Body::Body( double x, double y, double w, double h, bool solid ) : Body( alx::Point<double>( x, y ), alx::Size<double>( w, h ), solid )
	{}

	Body& Body::operator=( const Body& b )
	{
		this->_accel = b._accel;
		this->_alive = b._alive;
		this->_collision = b._collision;
		this->_collisionMask = b._collisionMask;
		this->_dirX = b._dirX;
		this->_dirY = b._dirY;
		this->_draw = b._draw;
		this->_entity = nullptr;
		this->_forces = b._forces;
		this->_grav = b._grav;
		this->_gravity = b._gravity;
		this->_groupMask = b._groupMask;
		this->_impulses = b._impulses;
		this->_iVel = b._iVel;
		this->_mass = b._mass;
		this->_oRect = b._oRect;
		this->_rect = b._rect;
		this->_solid = b._solid;
		this->_type = b._type;
		this->_vel = b._vel;
		return *this;
	}

	Body::Body( const Body& b )
	{
		*this = b;
	}

	void Body::Update( )
	{
		if( !_collision )
		{
			_oRect = _rect;
		}
		if( _type == Type::DYNAMIC )
		{
			alx::Size<double> t_a( 0.0, 0.0 );
			for( auto& f : _forces )
			{
				t_a += f;
			}
			double a_x = ( ( _coll[Dir::LEFT] && t_a.getWidth( ) < 0 ) ? 0.0 : ( _coll[Dir::RIGHT] && t_a.getWidth( ) > 0 ) ? 0.0 : t_a.getWidth( ) );
			double a_y = ( ( _coll[Dir::UP] && t_a.getHeight( ) < 0 ) ? 0.0 : ( _coll[Dir::DOWN] && t_a.getHeight( ) > 0 ) ? 0.0 : t_a.getHeight( ) );
			t_a.set( a_x, a_y );

			alx::Size<double> t_i( 0.0, 0.0 );
			for( auto& i : _impulses )
			{
				t_i += i;
			}
			_vel += t_i;
			//std::cout << t_i.getHeight() << std::endl;
			_impulses.clear( );

			if( _collision )
			{
				if( _coll[Dir::LEFT] )
				{
					_vel.setWidth( ( _vel.getWidth( ) > 0 ? _vel.getWidth( ) : 0 ) );
					//t_a.setWidth( ( t_a.getWidth() > 0 ? t_a.getWidth() : 0 ) );
				}
				else if( _coll[Dir::RIGHT] )
				{
					_vel.setWidth( ( _vel.getWidth( ) < 0 ? _vel.getWidth( ) : 0 ) );
					//t_a.setWidth( ( t_a.getWidth() < 0 ? t_a.getWidth() : 0 ) );
				}
				if( _coll[Dir::UP] )
				{
					//_vel.setHeight( ( _vel.getHeight() > 0 ? _vel.getHeight() : 0 ) );
					if( _grav && _vel.getHeight( ) < 0 && _iVel.getHeight( ) < 0 )
					{
						_vel.setHeight( -( _vel.getHeight( ) + _accel.getHeight( ) ) * 0.25 );
					}
					else
					{
						_vel.setHeight( ( _vel.getHeight( ) > 0 ? _vel.getHeight( ) : 0 ) );
					}
					//t_a.setHeight( ( t_a.getHeight() > 0 ? t_a.getHeight() : 0 ) );
				}
				else if( _coll[Dir::DOWN] )
				{
					_vel.setHeight( ( _vel.getHeight( ) < 0 ? _vel.getHeight( ) : 0 ) );
					//t_a.setHeight( ( t_a.getHeight() > 0 ? t_a.getHeight() : 0 ) );
				}
			}


			_accel = t_a + ( _grav && !_coll[Dir::DOWN] ? alx::Size<double>( 0.0, _gravity ) : 0.0 );
			_vel += _accel;
			_rect.setCenter( _rect.getCenter( ) + _vel );
		}
		if( _entity )
		{
			_entity->SetPosition( _rect.getCenter( ) );
		}
	}

	void Body::Draw( )
	{
		if( _draw )
		{
			al_draw_filled_rectangle( _rect.getLeft( ), _rect.getTop( ), _rect.getRight( ), _rect.getBottom( ), al_map_rgba( 255, 0, 125, 105 ) );
			al_draw_rectangle( _rect.getLeft( ), _rect.getTop( ), _rect.getRight( ), _rect.getBottom( ), al_map_rgb( 255, 0, 105 ), 2 );
		}
	}

	void Body::Draw( bool draw )
	{
		_draw = draw;
	}

	void Body::Solid( bool solid )
	{
		_solid = solid;
	}

	bool Body::Solid( )
	{
		return _solid;
	}

	void Body::Alive( bool alive )
	{
		this->_alive = alive;
	}

	bool Body::Alive( ) const
	{
		return this->_alive;
	}

	void Body::Gravity( bool gravity )
	{
		_grav = gravity;

	}

	void Body::Gravity( double gravity )
	{
		_gravity = gravity;
	}

	double Body::Gravity( )
	{
		return _gravity;
	}

	void Body::ApplyForce( double forceX, double forceY )
	{
		ApplyForce( alx::Size<double>( forceX, forceY ) );
	}

	void Body::ApplyForce( const alx::Size<double>& force )
	{
		_forces.push_back( force / _mass );
	}

	void Body::ApplyImpulse( double impulseX, double impulseY )
	{
		ApplyImpulse( alx::Size<double>( impulseX, impulseY ) );
	}

	void Body::ApplyImpulse( const alx::Size<double>& impulse )
	{
		_impulses.push_back( impulse / _mass );
	}

	void Body::SetVelocity( const alx::Size<double>& velocity )
	{
		_iVel = _vel = velocity;
		double w = _vel.getWidth( );
		_dirX = w < 0.0001 && w > -0.0001 ? 0.0 : w > 0.0001 ? 1.0 : -1.0;
		double h = _vel.getHeight( );
		_dirY = h < 0.0001 && h > -0.0001 ? 0.0 : h > 0.0001 ? 1.0 : -1.0;
	}

	void Body::SetVelocity( double x, double y )
	{
		SetVelocity( alx::Size<double>( x, y ) );
	}

	void Body::SetVelocityX( double x )
	{
		SetVelocity( x, this->_vel.getY( ) );
	}

	void Body::SetVelocityY( double y )
	{
		SetVelocity( _vel.getX( ), y );
	}

	alx::Size<double> Body::GetVelocity( ) const
	{
		return _vel;
	}

	double Body::GetVelotityX( ) const
	{
		return _vel.getX( );
	}

	double Body::GetVelocityY( ) const
	{
		return _vel.getY( );
	}

	void Body::SetMass( double mass )
	{
		_mass = mass;
	}

	double Body::GetMass( ) const
	{
		return _mass;
	}

	void Body::SetGroupMask( uint16_t mask )
	{
		_groupMask = mask;
	}

	void Body::SetCollisionMask( uint16_t mask )
	{
		_collisionMask = mask;
	}

	uint16_t Body::GetGroupMask( ) const
	{
		return _groupMask;
	}

	uint16_t Body::GetCollisionMask( ) const
	{
		return _collisionMask;
	}

	alx::Point<double> Body::GetPosition( ) const
	{
		return _rect.getCenter( );
	}

	alx::Size<double> Body::GetSize( ) const
	{
		return _rect.getSize( );
	}

	alx::Rect<double> Body::GetRect( ) const
	{
		return _rect;
	}

	void Body::SetEntity( Entity *entity )
	{
		_entity = entity;
	}

	void Body::SetType( Type type )
	{
		_type = type;
	}

	Body::Type Body::GetType( ) const
	{
		return _type;
	}

	bool Body::Collision( const Body& other )
	{
		bool _otherStatic = false;
		if( other.GetType( ) == Type::STATIC/* || other._dirX == _dirX || other._dirY == _dirY */ )
		{
			_otherStatic = true;
		}
		_collision = false;
		std::fill( std::begin( _coll ), std::end( _coll ), false );

		if( _solid && other._solid && ( ( _collisionMask & other._groupMask ) != 0 ) )
		{
			if( _entity && other._entity && _rect.intersects( other._rect ) && ( _collision = _entity->HandlePreContactCollision( ) ) )
			{
				bool _occollision = false;
				if( _otherStatic && ( ( other._collisionMask & _groupMask ) != 0 ) )
				{
					_occollision = other._entity->HandlePreContactCollision( );
				}
				_collision = true;
				if( DownCollision( other ) )
				{
					_coll[Dir::DOWN] = true;
				}
				else if( UpCollision( other ) )
				{
					_coll[Dir::UP] = true;
				}
				else if( LeftCollision( other ) )
				{
					_coll[Dir::LEFT] = true;
				}
				else if( RightCollision( other ) )
				{
					_coll[Dir::RIGHT] = true;
				}
				else
				{
					_collision = false;
				}
				if( _collision )
				{
					_collision = _entity->HandleContactCollision( other._entity, _coll );
				}

				if( _collision )
				{
					bool ocol[4] = { 0 };
					if( _otherStatic && _occollision )
					{
						ocol[Dir::DOWN] = _coll[Dir::UP];
						ocol[Dir::UP] = _coll[Dir::DOWN];
						ocol[Dir::RIGHT] = _coll[Dir::LEFT];
						ocol[Dir::LEFT] = _coll[Dir::RIGHT];
						_occollision = other._entity->HandleContactCollision( _entity, ocol );
					}
					if( _coll[Dir::LEFT] )
					{
						//_dir[Dir::LEFT] = false;
						double dx = std::abs( other._rect.getRight( ) - _rect.getLeft( ) ) + 0.05;
						_rect.setCenterX( _rect.getCenterX( ) + dx );
						if( _entity )
						{
							_entity->SetPosition( _rect.getCenter( ) );
						}
						DEBUG_MSG( "LEFT" );
					}
					if( _coll[Dir::RIGHT] )
					{
						//_dir[Dir::RIGHT] = false;
						double dx = std::abs( _rect.getRight( ) - other._rect.getLeft( ) ) + 0.05;
						_rect.setCenterX( _rect.getCenterX( ) - dx );
						if( _entity )
						{
							_entity->SetPosition( _rect.getCenter( ) );
						}
						DEBUG_MSG( "RIGHT" );
					}
					if( _coll[Dir::DOWN] )
					{
						//_dir[Dir::DOWN] = false;
						double dx = std::abs( _rect.getBottom( ) - other._rect.getTop( ) ) + 0.05;
						_rect.setCenterY( _rect.getCenterY( ) - dx );
						if( _entity )
						{
							_entity->SetPosition( _rect.getCenter( ) );
						}
						//std::cout << "DOWN" << std::endl;
					}
					if( _coll[Dir::UP] )
					{
						//_dir[Dir::UP] = false;
						double dx = std::abs( other._rect.getBottom( ) - _rect.getTop( ) ) + 0.1;
						_rect.setCenterY( _rect.getCenterY( ) + dx );
						if( _entity )
						{
							_entity->SetPosition( _rect.getCenter( ) );
						}
						DEBUG_MSG( "UP" );
					}

					bool collided = false;
					for( bool col_dir : _coll )
					{
						collided |= col_dir;
					}
					if( !collided )
					{
						_rect = _oRect;
					}

					_entity->ResolveCollision( other._entity, _coll );
					if( _otherStatic )
					{
						if( _occollision )
						{
							other._entity->ResolveCollision( _entity, ocol );
						}
					}
				}
				else
				{
					std::fill( std::begin( _coll ), std::end( _coll ), false );
				}
			}
		}
		return _collision;
	}

	const bool( &Body::Collision( ) const )[4]
	{
		return _coll;
	}

		void Body::ResetCollisions( )
	{
		std::fill( std::begin( _coll ), std::end( _coll ), false );
	}

	double Body::_gravity = 6 * 0.098;
}
