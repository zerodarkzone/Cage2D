#include "Cage2d/stdafx.h"
#include "Cage2d/SplashScreen.hpp"

namespace Cage2d
{
	SplashScreen::SplashScreen( )
	{
		_image = BFC.LoadBitmap( "Data/Splash/Cage2d.png" );
		if( !_image )
		{
			std::cerr << "couldn't load splash image " << std::endl;
			throw BitmapLoadException( "couldn't load splash image " );
		}
	}
	void SplashScreen::Show( alx::Display& renderWindow )
	{
		if( !_image )
		{
			throw InvalidBitmapException( "Invalid Splash Bitmap " );
		}
		int delay = 0;
		alx::Timer timer( 1 / 24.0 );
		alx::EventQueue event_queue;
		event_queue << alx::Mouse::getEventSource( ) << alx::Keyboard::getEventSource( ) << timer << renderWindow;

		int x = Game::SCREEN_WIDTH / 2 - _image.getWidth( ) / 2;
		int y = Game::SCREEN_HEIGHT / 2 - _image.getHeight( ) / 2;

		bool render = false;
		timer.start( );
		while( true )
		{
			alx::Event ev = event_queue.waitForEvent( );
			if( ev.getType( ) == ALLEGRO_EVENT_DISPLAY_CLOSE ||
				ev.getType( ) == ALLEGRO_EVENT_KEY_UP ||
				ev.getType( ) == ALLEGRO_EVENT_MOUSE_BUTTON_UP )
			{
				fade_out( 8 );
				return;
			}
			else if( ev.getType( ) == ALLEGRO_EVENT_TIMER && ev.getTimer( ) == timer )
			{
				if( ++delay > 72 )
				{
					fade_out( 8 );
					return;
				}
				render = true;
			}

			if( render && event_queue.isEmpty( ) )
			{
				render = false;
				al_clear_to_color( alx::Color( "black" ) );

				_image.draw( x, y );

				al_flip_display( );
			}
		}
	}
}
