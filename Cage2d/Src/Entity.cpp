#include "Cage2d/stdafx.h"
#include "Cage2d/Entity.hpp"
#include "Cage2d/Camera.hpp"

namespace Cage2d
{
	Entity::Entity( ) : _alive( true ), _sprite( new sp::Sprite( ) ), _pos( alx::Point<double>( ) ), _isLoaded( false ), _tag( 0 ), _order( 0 ), _name( "" )
	{
		//_body = SmrPtr<Body>( new Body() );
	}

	Entity::Entity( const sp::Sprite& sprite ) : Entity( )
	{
		Entity::Load( sprite );
	}

	Entity::Entity( const SmrPtr<sp::Sprite> &sprite ) : Entity( )
	{
		Entity::Load( sprite );
	}

	Entity::Entity( sp::Sprite *sprite )
	{
		Entity::Load( sprite );
	}

	Entity::Entity( const std::string& filename ) : Entity( )
	{
		Entity::Load( filename );
	}

	bool Entity::Init( )
	{
		return true;
	}

	void Entity::Load( const std::string& filename )
	{
		_image = BFC.LoadBitmap( filename );
		if( !_image )
		{
			_filename = "";
			_isLoaded = false;
		}
		else
		{
			_filename = filename;
			_sprite->setBitmap( _image );
			_isLoaded = true;
		}
	}

	void Entity::Load( const sp::Sprite& sprite )
	{
		_image = sprite.getBitmap( );
		if( !_image )
		{
			_filename = "";
			_isLoaded = false;
		}
		else
		{
			_filename = "";
			*_sprite = sprite;
			_isLoaded = true;
		}
	}

	void Entity::Load( const SmrPtr<sp::Sprite>& sprite )
	{
		_image = sprite->getBitmap( );
		if( !_image )
		{
			_filename = "";
			_isLoaded = false;
		}
		else
		{
			_filename = "";
			_sprite = sprite;
			_isLoaded = true;
		}
	}
	void Entity::Load( sp::Sprite *sprite )
	{
		_image = sprite->getBitmap( );
		if( !_image )
		{
			_filename = "";
			_isLoaded = false;
		}
		else
		{
			_filename = "";
			_sprite = SmrPtr<sp::Sprite>( sprite );
			_isLoaded = true;
		}
	}

	void Entity::Draw( ) const
	{
		if( _isLoaded )
		{
			const Camera& camera = GAME.GetMainCamera( );
			double scale = camera.GetScale( );
			if( _sprite->X + _sprite->Width * scale > camera.GetPositionX( ) - ( Game::SCREEN_WIDTH / 2 ) * ( 1 / scale ) &&
				_sprite->X - _sprite->Width * scale < camera.GetPositionX( ) + ( Game::SCREEN_WIDTH / 2 ) * ( 1 / scale ) &&
				_sprite->Y + _sprite->Height * scale > camera.GetPositionY( ) - ( Game::SCREEN_HEIGHT / 2 ) * ( 1 / scale ) &&
				_sprite->Y - _sprite->Height * scale < camera.GetPositionY( ) + ( Game::SCREEN_HEIGHT / 2 ) * ( 1 / scale ) )
			{
				_sprite->draw( );
			}
		}
		/*if( _body )
		{
			_body->Draw( );
		}*/
	}

	void Entity::Update( )
	{
		if( _sprite )
			_sprite->update( );
	}

	void Entity::SetPosition( double x, double y )
	{
		if( _isLoaded )
		{
			_sprite->setPosition( x, y );
		}
		_pos.set( x, y );
	}

	void Entity::SetPosition( const alx::Point<double>& pos )
	{
		if( _isLoaded )
		{
			_sprite->setPosition( pos );
		}
		_pos = pos;
	}

	void Entity::SetPositionX( double x )
	{
		if( _isLoaded )
		{
			_sprite->setPosition( x, _sprite->Y );
		}
		_pos.setX( x );
	}

	void Entity::SetPositionY( double y )
	{
		if( _isLoaded )
		{
			_sprite->setPosition( _sprite->X, y );
		}
		_pos.setY( y );
	}

	void Entity::Move( double dx, double dy )
	{
		if( _isLoaded )
		{
			_sprite->move( dx, dy );
		}
		_pos += alx::Point<double>( dx, dy );
	}

	void Entity::Move( const alx::Point<double>& dist )
	{
		if( _isLoaded )
		{
			_sprite->move( dist );
		}
		_pos += dist;
	}

	alx::Point<double> Entity::GetPosition( ) const
	{
		if( _isLoaded )
		{
			return _sprite->getPosition( );
		}
		return _pos;
	}

	double Entity::GetPositionX( ) const
	{
		if( _isLoaded )
		{
			return _sprite->X;
		}
		return _pos.getX( );
	}

	double Entity::GetPositionY( ) const
	{
		if( _isLoaded )
		{
			return _sprite->Y;
		}
		return _pos.getY( );
	}

	bool Entity::IsLoaded( ) const
	{
		return _isLoaded;
	}

	bool Entity::HandlePreContactCollision( const Cage2d::Entity *other )
	{
		UNREFERENCED_PARAMETER( other );
		return true;
	}

	bool Entity::HandleContactCollision( const Cage2d::Entity *other, const bool( &colDir )[4] )
	{
		UNREFERENCED_PARAMETER( other );
		UNREFERENCED_PARAMETER( colDir );
		return true;
	}

	void Entity::ResolveCollision( const Cage2d::Entity *other, const bool( &colDir )[4] )
	{
		UNREFERENCED_PARAMETER( other );
		UNREFERENCED_PARAMETER( colDir );
	}

	void Entity::AddBody( const SmrPtr<Body>& body )
	{
		if( body )
		{
			this->_body = body;
			this->_body->_entity = this;
		}
	}

	void Entity::AddBody( const alx::Rect<double>& rect, bool solid )
	{
		this->_body = SmrPtr<Body>( new Body( rect, solid ) );
		this->_body->_entity = this;
	}

	void Entity::AddBody( const alx::Point<double>& pos, const alx::Size<double>& size, bool solid )
	{
		this->_body = SmrPtr<Body>( new Body( pos, size, solid ) );
		this->_body->_entity = this;
	}

	void Entity::AddBody( double x, double y, double w, double h, bool solid )
	{
		this->_body = SmrPtr<Body>( new Body( x, y, w, h, solid ) );
		this->_body->_entity = this;
	}

	const SmrPtr<Body>& Entity::GetBody( )
	{
		return _body;
	}

	SmrPtr<Body> Entity::GetBody( ) const
	{

		return SmrPtr<Body>( new Body( this->_body.get( ) ) );
	}

	bool Entity::HasBody( ) const
	{
		return ( _body && _body->_entity != nullptr );
	}

	int Entity::GetTag( ) const
	{
		return _tag;
	}

	void Entity::SetTag( int tag )
	{
		_tag = tag;
	}

	int Entity::GetOrder( ) const
	{
		return _order;
	}

	void Entity::SetOrder( int order )
	{
		_order = order;
	}

	std::string Entity::GetName( ) const
	{
		return _name;
	}

	void Entity::SetName( std::string name )
	{
		_name = name;
	}

	sp::Sprite& Entity::GetSprite( )
	{
		return *_sprite;
	}

	const std::map<std::string, std::string>& Entity::GetProperties( ) const
	{
		return _properties;
	}

	std::string Entity::GetProperty( const std::string& name ) const
	{
		auto it = _properties.find( name );
		if( it != _properties.end( ) )
		{
			return it->second;
		}
		return "";
	}

	bool Entity::HasProperty( ) const
	{
		return !_properties.empty( );
	}

	void Entity::AddProperty( const std::string& name, const std::string& value )
	{
		_properties.insert( std::pair<std::string, std::string>( name, value ) );
	}

	void Entity::ChangeProperty( const std::string& name, const std::string& value )
	{
		_properties[name] = value;
	}

	bool Entity::Alive( )
	{
		return _alive;
	}

	void Entity::Alive( bool alive )
	{
		this->_alive = alive;
	}

	Entity& Entity::operator=( const Entity& E )
	{
		this->_alive = E._alive;
		if( E.HasBody( ) )
		{
			this->_body = Body::Create( *E._body );
			this->_body->_entity = this;
		}
		this->_filename = E._filename;
		this->_image = E._image;
		this->_isLoaded = E._isLoaded;
		this->_name = E._name;
		this->_order = E._order;
		this->_pos = E._pos;
		this->_properties = E._properties;
		this->_sprite = E._sprite;
		this->_tag = E._tag;
		return *this;
	}

	Entity& Entity::operator=( Entity&& E )
	{
		if( this != &E )
		{
			this->_alive = std::move( E._alive );
			if( E.HasBody( ) )
			{
				this->_body->_entity = nullptr;
				this->_body = nullptr;
				E._body->_entity = nullptr;
				this->_body = E._body;
				this->_body->_entity = this;
			}
			this->_filename = std::move( E._filename );
			this->_image = std::move( E._image );
			this->_isLoaded = std::move( E._isLoaded );
			this->_name = std::move( E._name );
			this->_order = std::move( E._order );
			this->_pos = std::move( E._pos );
			this->_properties = std::move( E._properties );
			this->_sprite = std::move( E._sprite );
			this->_tag = std::move( E._tag );
		}
		return *this;
	}

	bool& Entity::GetCollision( )
	{
		return _body->_collision;
	}

	alx::Rect<double>& Entity::GetBodyRect( )
	{
		return _body->_rect;
	}

	alx::Rect<double>& Entity::GetBodyOldRect( )
	{
		return _body->_oRect;
	}

	Entity::Entity( const Entity& E )
	{
		*this = E;
	}

	Entity::Entity( Entity&& E ) : _alive( std::move( E._alive ) ), _properties( std::move( E._properties ) ), _sprite( std::move( E._sprite ) ),
		_image( std::move( E._image ) ), _pos( std::move( E._pos ) ), _filename( std::move( E._filename ) ), _isLoaded( std::move( E._isLoaded ) ),
		_tag( std::move( E._tag ) ), _order( std::move( E._order ) ), _name( std::move( E._name ) )
	{
		if( E.HasBody( ) )
		{
			this->_body->_entity = nullptr;
			this->_body = nullptr;
			E._body->_entity = nullptr;
			this->_body = E._body;
			this->_body->_entity = this;
		}
	}

	Entity::~Entity( )
	{
		//std::cout << "entity destroyed" << std::endl;
	}

}
