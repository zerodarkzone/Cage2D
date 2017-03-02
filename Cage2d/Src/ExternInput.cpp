#include "Cage2d/stdafx.h"
#include "Cage2d/ExternInput.hpp"
#define EPSILON_ 0.01
namespace Cage2d
{
	ExternInput::ExternInput( ) : _mousePosition( 0, 0 ), _mouseMoved( false )
	{
		std::fill( std::begin( _keys ), std::end( _keys ), false );
	}

	bool ExternInput::isKeyPressed( Key key ) const
	{
		return _keys[static_cast<int>(key)];
	}

	Key ExternInput::GetKeyPressed( ) const
	{
		for( int i = 0; i < ALLEGRO_KEY_MAX; ++i )
		{
			if( _keys[i] )
			{
				return static_cast<Key>(i);
			}
		}
		return Key::NO_KEY;
	}

	alx::Event ExternInput::GetKeyEvent( ) const
	{
		if( !_keyEvents.empty( ) )
		{
			return _keyEvents.front( );
		}
		return alx::Event( );
	}

	alx::Event ExternInput::GetEvent( ) const
	{
		if( !_events.empty( ) )
		{
			auto ev = _events.front( );
			return ev;
		}
		return alx::Event( );
	}

	alx::Point<double> ExternInput::GetMousePosition( ) const
	{
		alx::MouseState ms;
		alx::Point<int> newPos( ms.getMouseX( ), ms.getMouseY( ) );

		if( _mousePosition == newPos )
			_mouseMoved = false;
		else
			_mouseMoved = true;

		_mousePosition = newPos;
		return static_cast<alx::Point<double>>( _mousePosition );
	}

	bool ExternInput::hasMouseMoved( ) const
	{
		GetMousePosition( );
		return _mouseMoved;
	}

	bool ExternInput::isMousePressed( ) const
	{
		return _mousePressed;
	}

	void ExternInput::Pop( )
	{
		if( !_keyEvents.empty( ) )
		{
			_keyEvents.pop( );
		}
		if( !_events.empty( ) )
		{
			_events.pop( );
		}
	}


	void ExternInput::ProcessEvent( alx::Event ev )
	{
		if( ev.getType( ) == ALLEGRO_EVENT_KEY_DOWN )
		{
			_keys[ev.getKeyboardKeycode( )] = true;
			_keyEvents.push( ev );
		}
		else if( ev.getType( ) == ALLEGRO_EVENT_KEY_UP )
		{
			_keys[ev.getKeyboardKeycode( )] = false;
			_keyEvents.push( ev );
		}
		else
		{
			if( ev.getType( ) == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN )
			{
				_mousePressed = true;
			}
			if( ev.getType( ) == ALLEGRO_EVENT_MOUSE_BUTTON_UP )
			{
				_mousePressed = false;
			}
			_events.push( ev );
		}
	}

	void ExternInput::ResetKeyPressed( )
	{
		std::fill( std::begin( _keys ), std::end( _keys ), false );
	}

	ExternInput::~ExternInput( )
	{
		while( !_events.empty( ) )
		{
			_events.pop( );
		}
		while( !_keyEvents.empty( ) )
		{
			_keyEvents.pop( );
		}
	}
}
