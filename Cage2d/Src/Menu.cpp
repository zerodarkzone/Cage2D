#include "Cage2d/stdafx.h"
#include "Cage2d/Menu.hpp"
#include "Cage2d/MenuItem.hpp"

namespace Cage2d
{
	Menu::Menu( alx::Color color ) : Menu( "", color )
	{}

	Menu::Menu( const std::string& name, alx::Color color ) :
		_name( name ), __enteredMenuItem( false ), _isSongPlaying( false ), _buttonSound( "" ), MaxElements( 11 ), _action( false ), _color( color )
	{}

	void Menu::SetScapeAction( const std::function<void( )>& action )
	{
		_scapeAction = action;
	}


	void Menu::PlayMusic( const std::string& name )
	{
		ServiceLocator::GetAudio( )->PlaySong( name, true, 1.6 );
		_isSongPlaying = true;
	}

	void Menu::HandleEvents( alx::Display& window )
	{
		auto it = _menuItems.begin( );
		( *it ).ChangeSelect( true );
		alx::EventQueue event_queue;
		alx::Timer timer( 1 / 24.0 );
		event_queue << alx::Mouse::getEventSource( ) << alx::Keyboard::getEventSource( ) << timer << window;
		alx::Event event;
		timer.start( );

		while( true )
		{
			event = event_queue.waitForEvent( );
			if( event.getType( ) == ALLEGRO_EVENT_DISPLAY_CLOSE )
			{
				if( _scapeAction )
				{
					_scapeAction( );
				}
				return;
			}
			else if( event.getType( ) == ALLEGRO_EVENT_KEY_UP )
			{
				switch( event.getKeyboardKeycode( ) )
				{
				case ALLEGRO_KEY_ESCAPE:
					if( _scapeAction )
					{
						_scapeAction( );
					}
					return;
				case ALLEGRO_KEY_UP:
					ManageEvents( it, ALLEGRO_KEY_UP );
					ServiceLocator::GetAudio( )->PlaySound( _buttonSound );
					break;
				case ALLEGRO_KEY_DOWN:
					ManageEvents( it, ALLEGRO_KEY_DOWN );
					ServiceLocator::GetAudio( )->PlaySound( _buttonSound );
					break;
				case ALLEGRO_KEY_ENTER:
					RunAction( );
					if( _action )
					{
						ServiceLocator::GetAudio( )->PlaySound( _buttonSound );
						return;
					}
					break;
				default:
					break;
				}
			}
			else if( event.getType( ) == ALLEGRO_EVENT_MOUSE_AXES )
			{
				ManageEvents( it, ALLEGRO_EVENT_MOUSE_AXES, event.getMouseX( ), event.getMouseY( ) );
			}
			else if( event.getType( ) == ALLEGRO_EVENT_MOUSE_BUTTON_UP )
			{
				switch( event.getMouseButton( ) )
				{
				case 1: //LEFT
					RunAction( );
					if( _action )
					{
						ServiceLocator::GetAudio( )->PlaySound( _buttonSound );
						return;
					}
					break;
				default:
					break;
				}
			}
			else if( event.getType( ) == ALLEGRO_EVENT_TIMER && event.getTimer( ) == timer )
			{
				ShowMenu( );
			}
		}
	}

	void Menu::RunAction( )
	{
		if( !_menuItems.empty( ) )
		{
			for( auto& item : _menuItems )
			{
				if( item.IsSelected( ) )
				{
					item.Action( );
					item.ChangeSelect( false );
					_action = true;
					GAME.ClearEventQueue( );
					return;
				}
			}
		}
	}

	void Menu::ShowMenu( )
	{
		al_clear_to_color( _color );
		if( !_menuItems.empty( ) )
		{
			for( auto& item : _menuItems )
			{
				item.Draw( );
			}
		}
		al_flip_display( );
	}

	void Menu::AddMenuItem( const MenuItem& menuItem )
	{
		if( _menuItems.size( ) >= MaxElements )
		{
			return;
		}
		_menuItems.push_back( menuItem );
		OrganizeItems( );
	}

	Menu& Menu::operator << ( const MenuItem& menuItem )
	{
		AddMenuItem( menuItem );
		return *this;
	}

	void Menu::SetItemSound( const std::string& name )
	{
		_buttonSound = name;
	}

	void Menu::OrganizeItems( )
	{
		int space = Game::SCREEN_HEIGHT / ( ( *_menuItems.begin( ) )._rect.getHeight( ) ) * 15 / _menuItems.size( );

		int middle = (int) _menuItems.size( ) / 2;
		auto mid = _menuItems.begin( );
		std::advance( mid, middle );
		if( _menuItems.size( ) % 2 == 0 )
		{
			( *mid ).SetBounds( ( Game::SCREEN_WIDTH / 2 ) - ( *mid )._rect.getWidth( ) / 2,
				( Game::SCREEN_HEIGHT / 2 ),
								( Game::SCREEN_WIDTH / 2 ) + ( *mid )._rect.getWidth( ) / 2,
								( Game::SCREEN_HEIGHT / 2 ) + ( *mid )._rect.getHeight( ) );
		}
		else
		{
			( *mid ).SetBounds( ( Game::SCREEN_WIDTH / 2 ) - ( *mid )._rect.getWidth( ) / 2,
				( Game::SCREEN_HEIGHT / 2 ) - ( *mid )._rect.getHeight( ) / 2,
								( Game::SCREEN_WIDTH / 2 ) + ( *mid )._rect.getWidth( ) / 2,
								( Game::SCREEN_HEIGHT / 2 ) + ( *mid )._rect.getHeight( ) / 2 );
		}

		auto it = mid;
		auto end = _menuItems.end( );
		end--;
		while( it != end )
		{
			auto prev = it++;
			( *it ).SetBounds( ( Game::SCREEN_WIDTH / 2 ) - ( *it )._rect.getWidth( ) / 2,
				( *prev )._rect.getBottom( ) + space,
							   ( Game::SCREEN_WIDTH / 2 ) + ( *it )._rect.getWidth( ) / 2,
							   ( *prev )._rect.getBottom( ) + space + ( *it )._rect.getHeight( ) );
		}
		it = mid;
		auto begin = _menuItems.begin( );
		while( it != begin )
		{
			auto prev = it--;
			( *it ).SetBounds( ( Game::SCREEN_WIDTH / 2 ) - ( *it )._rect.getWidth( ) / 2,
				( *prev )._rect.getTop( ) - space - ( *it )._rect.getHeight( ),
							   ( Game::SCREEN_WIDTH / 2 ) + ( *it )._rect.getWidth( ) / 2,
							   ( *prev )._rect.getTop( ) - space );
		}

	}

	void Menu::ManageEvents( std::list<MenuItem>::iterator& it, int _case, int x, int y )
	{
		switch( _case )
		{
		case ALLEGRO_KEY_DOWN:
			if( !_menuItems.empty( ) )
			{
				for( auto itr = _menuItems.begin( ); itr != _menuItems.end( ); ++itr )
				{
					( *itr ).ChangeSelect( false );
				}
			}
			++it;
			if( it != _menuItems.end( ) )
			{
				( *it ).ChangeSelect( true );
			}
			else
			{
				it = _menuItems.begin( );
				( *it ).ChangeSelect( true );
			}
			break;
		case ALLEGRO_KEY_UP:
			if( !_menuItems.empty( ) )
			{
				for( auto itr = _menuItems.begin( ); itr != _menuItems.end( ); ++itr )
				{
					( *itr ).ChangeSelect( false );
				}
			}
			if( it == _menuItems.begin( ) )
			{
				it = _menuItems.end( );
			}
			--it;
			( *it ).ChangeSelect( true );
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			if( !_menuItems.empty( ) )
			{
				auto itr = _menuItems.begin( );
				auto itr2 = itr;
				int count = 0;
				while( itr != _menuItems.end( ) )
				{
					if( ( *itr ).InsideBounds( x, y ) )
					{
						if( !__enteredMenuItem )
						{
							ServiceLocator::GetAudio( )->PlaySound( _buttonSound );
							__enteredMenuItem = true;
						}
						count++;
						( *itr ).ChangeSelect( true );
						itr2 = itr;
					}
					else
					{
						( *itr ).ChangeSelect( false );
					}
					++itr;
				}
				if( count == 0 )
				{
					__enteredMenuItem = false;
				}
				it = itr2;
			}
			break;
		default:
			break;
		}
	}

	std::string Menu::GetName( )
	{
		return _name;
	}

	Menu::~Menu( )
	{
		if( _isSongPlaying )
		{
			ServiceLocator::GetAudio( )->StopSong( );
		}
		GAME.ClearEventQueue( );
		_menuItems.clear( );
	}
}
