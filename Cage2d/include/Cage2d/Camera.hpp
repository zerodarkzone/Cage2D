#pragma once
#include <typeinfo>

namespace Cage2d
{
	class Camera
	{
		alx::Point<double> _position;

		double _resX1, _resY1, _resX2, _resY2, _scale;
		alx::Bitmap _buffer;
		std::vector<std::function<void( )>> _drawFuncs;
		ReadOnlyProperty<double, Camera> X;
	public:
		Camera( double x = 0.0, double y = 0.0, double width = INT_MAX, double height = INT_MAX );
		Camera( const alx::Point<double>& pos, alx::Size<double> size = alx::Size<double>( INT_MAX, INT_MAX ) );

		alx::Point<double> GetPosition( ) const;
		double GetPositionX( ) const;
		double GetPositionY( ) const;
		double GetScale( ) const;

		void SetPosition( double x, double y );
		void SetPosition( const alx::Point<double>& position );
		void SetPositionX( double x );
		void SetPositionY( double y );
		void SetRestrictions( double x1, double y1, double x2, double y2 );
		void SetSize( double width, double height );
		void SetSize( const alx::Size<double>& size );
		void SetWidth( double width );
		void SetHeight( double height );
		void SetScale( double scale );
		void ClearFunctions( );
		void Draw( alx::Display& window );

		Camera& operator << ( const std::function<void( )>& func );
		void AddDrawFunction( const std::function<void( )>& func );
		void AddBackDrawFunction( const std::function<void( )>& func );

		template<typename Func, typename Arg>
		void AddDrawFunction( const Func& fun, const Arg& arg )
		{
			_drawFuncs.push_back( std::bind( fun, arg ) );
		}

		template<typename Func, typename Arg>
		void AddBackDrawFunction( const Func& fun, const Arg& arg )
		{
			_drawFuncs.insert( _drawFuncs.begin( ), std::bind( fun, arg ) );
		}

		template<typename ...Funcs>
		void Draw( alx::Display& window, const Funcs& ...funcs )
		{
			if( _buffer )
			{
				_buffer.setTarget( );
				al_clear_to_color( alx::Color( 0, 0, 0, 0 ) );
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
				std::vector<std::function<void( )>> f = { funcs... };
				unsigned int len = sizeof...( funcs );
				for( unsigned int i = 0; i < len; ++i )
				{
					f[i]( );
				}
				transform.setIdentity( );
				transform.setCurrent( );
				alx::Display::setTarget( window );
				_buffer.draw( 0, 0 );
			}
		}

		~Camera( );
	};
}
