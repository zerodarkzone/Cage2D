#pragma once
#include "Body.hpp"

namespace Cage2d
{
	class Entity
	{
	public:
		/**
		*  \brief  Variadic template function, the arguments must be the same as the constructors.
		*   Entity( )
		*   Entity( const sp::Sprite &sprite )
		*   Entity( const std::string &filename )
		**/
		CREATE_FUNC( Entity )

		Entity( const Entity& E );
		Entity( Entity&& E );
		virtual ~Entity( );

		virtual bool Init( );
		virtual void Load( const std::string& filename );
		virtual void Load( const sp::Sprite& sprite );
		virtual void Load( const SmrPtr<sp::Sprite>& sprite );
		virtual void Load( sp::Sprite *sprite );
		virtual void Draw( ) const;
		virtual void Update( );

		virtual void SetPosition( double x, double y );
		virtual void SetPosition( const alx::Point<double>& pos );
		virtual void SetPositionX( double x );
		virtual void SetPositionY( double y );
		virtual void Move( double dx, double dy );
		virtual void Move( const alx::Point<double>& dist );
		virtual alx::Point<double> GetPosition( ) const;
		virtual double GetPositionX( ) const;
		virtual double GetPositionY( ) const;
		virtual bool IsLoaded( ) const;
		virtual bool HandlePreContactCollision( const Cage2d::Entity *other );
		virtual bool HandleContactCollision( const Cage2d::Entity *other, const bool( &colDir )[4] );
		virtual void ResolveCollision( const Cage2d::Entity *other, const bool( &colDir )[4] );

		void AddBody( const SmrPtr<Body>& body );
		void AddBody( const alx::Rect<double>& rect, bool solid = true );
		void AddBody( const alx::Point<double>& pos, const alx::Size<double>& size, bool solid = true );
		void AddBody( double x, double y, double w, double h, bool solid = true );
		const SmrPtr<Body>& GetBody( );
		SmrPtr<Body> GetBody( ) const;
		bool HasBody( ) const;

		int GetTag( ) const;
		void SetTag( int tag );

		int GetOrder( ) const;
		void SetOrder( int order );

		std::string GetName( ) const;
		void SetName( std::string name );
		sp::Sprite& GetSprite( );

		const std::map<std::string, std::string>& GetProperties( ) const;
		std::string GetProperty( const std::string& name ) const;
		bool HasProperty( ) const;
		void AddProperty( const std::string& name, const std::string& value );
		void ChangeProperty( const std::string& name, const std::string& value );
		bool Alive( );
		void Alive( bool alive );

		Entity& operator = ( const Entity& E );
		Entity& operator = ( Entity&& E );

	protected:
		Entity( );
		Entity( const sp::Sprite& sprite );
		Entity( const SmrPtr<sp::Sprite> &sprite );
		Entity( sp::Sprite *sprite );
		Entity( const std::string& filename );		
		bool _alive;
		SmrPtr<Body> _body;
		std::map<std::string, std::string> _properties;
		bool& GetCollision( );
		alx::Rect<double>& GetBodyRect( );
		alx::Rect<double>& GetBodyOldRect( );
		SmrPtr<sp::Sprite> _sprite;
		alx::Bitmap _image;

	private:
		alx::Point<double> _pos;
		std::string _filename;
		bool _isLoaded;
		int _tag;
		int _order;
		std::string _name;
	};
}
