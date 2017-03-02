#include "Cage2d/stdafx.h"
#include "Cage2d/Game.hpp"
#include "Cage2d/Scene.hpp"
#include "Cage2d/SplashScreen.hpp"
#include "Cage2d/Menu.hpp"
#include "Cage2d/MenuItem.hpp"
#include "Cage2d/Camera.hpp"
#include "Cage2d/ALSoundProvider.hpp"

namespace Cage2d
{
	Game::Game( ) : _gameState( GameState::Uninitialized ), _event_queue( false ), _backGroundColor( "black" ), _externInput( ),
		_defaultCamera( nullptr ), _mainCamera( nullptr ), _showFPS( false ), _showMenu( false ), _runningScene( nullptr ),
		DeltaTime( this ), Width( this ), Height( this ), HalfWidth( this ), HalfHeight( this )
	{
		gameTime = 0;
		render = false;
		frames = 0;
		gameFPS = 0;
		_badInit = false;
	}

	bool Game::Init( float fps )
	{
		_deltaTime = 1.0 / fps;
		if( !InitAllegro( ) )
		{
			Close( );
			_badInit = true;
			return false;
		}
		_defaultCamera = SmrPtr<Camera>( new( std::nothrow )Camera( ) );
		if( !_defaultCamera )
		{
			Close( );
			return false;
		}
		_defaultCamera->SetSize( SCREEN_WIDTH, SCREEN_HEIGHT );
		_defaultCamera->SetScale( 1.0 );
		SetMainCamera( *_defaultCamera );
		IAudioProvider *soundProvider = new ALSoundProvider( );
		ServiceLocator::RegisterServiceLocator( soundProvider );
		return true;
	}

	void Game::Run( void )
	{
		ChangeGameState( GameState::ShowingSplash );

		gameTime = al_current_time( );
		_timer.start( );
		while( !IsExiting( ) )
		{
			GameLoop( );
		}
		Close( );
	}

	bool Game::IsExiting( )
	{
		if( _gameState == GameState::Exiting )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	alx::Display& Game::GetMainWindow( )
	{
		if( _mainWindow )
		{
			return _mainWindow;
		}
		throw std::runtime_error( "No available game display" );
	}

	const ExternInput& Game::GetInput( )
	{
		return _externInput;
	}

	void Game::SetMainCamera( Camera& camera )
	{
		_mainCamera = &camera;
	}

	Camera& Game::GetMainCamera( )
	{
		if( _mainCamera )
		{
			return *_mainCamera;
		}
		throw std::runtime_error( "Camera doesn't exist" );
	}

	void Game::GameLoop( )
	{
		alx::Event _currentEvent = _event_queue.waitForEvent( );
		switch( _gameState )
		{
		case GameState::ShowingSplash:
			ShowSplashScreen( );
			break;
		case GameState::ShowingMenu:
			DEBUG_MSG( "Showing Menu" );
			ShowMenu( );
			break;
		case GameState::Playing:
			if( _currentEvent.getType( ) != ALLEGRO_EVENT_MOUSE_AXES &&
				_currentEvent.getType( ) != ALLEGRO_EVENT_TIMER )
			{
				_externInput.ProcessEvent( _currentEvent );
			}
			if( _currentEvent.getType( ) == ALLEGRO_EVENT_DISPLAY_CLOSE )
			{
				ChangeGameState( GameState::Exiting );
			}
			else if( _currentEvent.getType( ) == ALLEGRO_EVENT_KEY_DOWN )
			{
			}
			else if( _currentEvent.getType( ) == ALLEGRO_EVENT_KEY_UP )
			{
				switch( _currentEvent.getKeyboardKeycode( ) )
				{
				case ALLEGRO_KEY_ESCAPE:
					if( _runningScene ) // fix
					{
						ChangeGameState( GameState::ShowingMenu );
					}
					else
					{
						ChangeGameState( GameState::Exiting );
					}
					break;
				default:
					break;
				}
			}
			else if( _currentEvent.getType( ) == ALLEGRO_EVENT_TIMER && _currentEvent.getTimer( ) == _timer )
			{
				render = true;
				if( al_current_time( ) - gameTime >= 1 )
				{
					gameTime = al_current_time( );
					gameFPS = frames;
					frames = 0;
				}
				//UPDATE GAME LOGIC
				if( _runningScene && _runningScene->DoneWithScene( ) )
				{
					SetMainCamera( *_defaultCamera );
					_runningScene = SmrPtr<Cage2d::Scene>( nullptr );
					if( !_sceneStack.empty( ) )
					{
						RunningScene( _sceneStack.top( ) );
						_sceneStack.pop( );
					}
				}
				else if( _runningScene )
				{
					_runningScene->SceneUpdate( {} );
				}
				_externInput.Pop( );
			}
			if( render && _event_queue.isEmpty( ) )
			{
				al_clear_to_color( _backGroundColor );
				// UPDATE FPS===========
				frames++;
				render = false;
				// Render Game
				if( _runningScene && !_runningScene->DoneWithScene( ) && _runningScene->UseCamera( ) )
				{
					_runningScene->Draw( );
				}
				else
				{
					GetMainCamera( ).Draw( GetMainWindow( ) );
				}

				if( _showFPS )
				{
					font18.drawf( 5, 5, 0, alx::Color( "yellow" ), "FPS: %i", gameFPS ); // display FPS on screen
				}
				al_flip_display( );
			}
			break;
		default:
			break;
		}
	}

	void Game::ShowSplashScreen( )
	{
		SplashScreen splashScreen;
		splashScreen.Show( GetMainWindow( ) );
		ChangeGameState( GameState::ShowingMenu );
		ClearEventQueue( );
	}

	void Game::ShowMenu( )
	{
		if( !_showMenu )
		{
			ChangeGameState( GameState::Playing );
			return;
		}
		Menu mainMenu( "Main Menu" );
		mainMenu.SetScapeAction( [this] {ChangeGameState( GameState::Exiting ); } );

		MenuItem play( "PLAY" );
		play.SetColor( { 255, 255, 255 } );
		play.SetFont( "Data/Fonts/arial.ttf", 190 );
		play.SetAction( [this] {ChangeGameState( GameState::Playing ); } );
		mainMenu << play;

		MenuItem exit( "EXIT" );
		exit.SetColor( { 255, 255, 255 } );
		exit.SetFont( "Data/Fonts/arial.ttf", 190 );
		exit.SetAction( [this] {ChangeGameState( GameState::Exiting ); } );
		mainMenu << exit;

		mainMenu.SetItemSound( "Data/Sounds/button-21.wav" );
		mainMenu.PlayMusic( "Data/Sounds/fondo.wav" );
		mainMenu.HandleEvents( GetMainWindow( ) );
		ClearEventQueue( );
	}

	void Game::ShowMenu( bool show )
	{
		_showMenu = show;
	}

	void Game::ClearEventQueue( )
	{
		_event_queue.clear( );
		_externInput.ResetKeyPressed( );
	}

	auto Game::GetGameState( )->GameState
	{
		return _gameState;
	}

	//Uninitialized, ShowingSplash, Paused,
	//ShowingMenu, Playing, Exiting, Saving, Loading

	auto Game::ChangeGameState( GameState newGameState )->void
	{
		switch( _gameState )
		{
		case Game::GameState::Uninitialized:
			DEBUG_MSG( "Initializing Game" );
			break;
		case Game::GameState::ShowingSplash:
			break;
		case Game::GameState::ShowingMenu:
			break;
		case Game::GameState::Playing:
			break;
		case Game::GameState::Exiting:
			DEBUG_MSG( "This Cannot Happen" );
			break;
		case Game::GameState::Saving:
			break;
		case Game::GameState::Loading:
			break;
		default:
			break;
		}
		_gameState = newGameState;
		switch( _gameState )
		{
		case Game::GameState::Uninitialized:
			DEBUG_MSG( "This Cannot Happen" );
			break;
		case Game::GameState::ShowingSplash:
			break;
		case Game::GameState::ShowingMenu:
			render = false;
			break;
		case Game::GameState::Playing:
			break;
		case Game::GameState::Exiting:
			DEBUG_MSG( "Leaving Game" );
			break;
		case Game::GameState::Saving:
			break;
		case Game::GameState::Loading:
			break;
		default:
			break;
		}
		ClearEventQueue( );
	}

	void Game::Close( )
	{
		delete ServiceLocator::GetAudio( );
	}

	Game::~Game( )
	{
		delete &BFC;
		delete &SFC;
		if( !_badInit )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_destroy_timer( _timer.get( ) );
			al_destroy_event_queue( _event_queue.get( ) );
		}
		DEBUG_MSG( "Game Destroyed" );
	}

	void Game::RunningScene( const SmrPtr <Scene>& scene )
	{
		_runningScene = scene;
		SetMainCamera( _runningScene->GetMainCamera( {} ) );
	}

	SmrPtr<Scene> Game::ReplaceScene( const SmrPtr <Scene>& scene )
	{
		if( _runningScene )
		{
			SmrPtr<Scene> old = _runningScene;
			_runningScene = scene;
			SetMainCamera( _runningScene->GetMainCamera( {} ) );
			return old;
		}
		return nullptr;
	}

	void Game::PushScene( const SmrPtr <Scene>& scene )
	{
		if( scene )
		{
			_sceneStack.push( scene );
		}
	}

	void Game::PopScene( )
	{
		_sceneStack.pop( );
		SetMainCamera( *_defaultCamera );
	}

	SmrPtr<Scene> Game::RunningScene( )
	{
		return _runningScene;
	}

	void Game::BackGroundColor( const alx::Color& color )
	{
		_backGroundColor = color;
	}

	void Game::ShowFPS( bool show )
	{
		_showFPS = show;
	}

	void Game::SetWindowTitle( const std::string& title )
	{
		GetMainWindow( ).setWindowTitle( title.c_str( ) );
	}

	bool Game::InitAllegro( )
	{
		if( _gameState != GameState::Uninitialized )
		{
			return false;
		}
		if( !al_init( ) )
		{
			std::cerr << "Couldn't initialize allegro" << std::endl;
			return false;
		}

		al_set_new_bitmap_flags( ALLEGRO_MIPMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_VIDEO_BITMAP );
		al_set_new_display_option( ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST );
		al_set_new_display_option( ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST );
		al_set_new_display_option( ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST );
		al_set_new_bitmap_flags( ALLEGRO_MIPMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_VIDEO_BITMAP );
		al_set_new_display_option( ALLEGRO_SUPPORT_NPOT_BITMAP, 1, ALLEGRO_SUGGEST );
		al_set_new_display_option( ALLEGRO_CAN_DRAW_INTO_BITMAP, 1, ALLEGRO_SUGGEST );
		//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
		al_set_new_display_flags( ALLEGRO_OPENGL );
		_mainWindow = alx::Display( al_create_display( SCREEN_WIDTH, SCREEN_HEIGHT ), false );
		if( !_mainWindow )
		{
			std::cerr << "Couldn't create display" << std::endl;
			return false;
		}

		al_set_app_name( "GameEngine" );

		if( !al_install_audio( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't install audio" << std::endl;;
			return false;
		}

		if( !al_install_keyboard( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't install keyboard" << std::endl;;
			return false;
		}
		if( !al_install_joystick( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't install joystick" << std::endl;;
			return false;
		}
		if( !al_install_mouse( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't install mouse" << std::endl;;
			return false;
		}
		if( !al_init_acodec_addon( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't initiate a-codec ad-on" << std::endl;;
			return false;
		}
		if( !al_init_image_addon( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't initiate image ad-on" << std::endl;;
			return false;
		}
		if( !al_init_font_addon( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't initiate font ad-on" << std::endl;;
			return false;
		}
		if( !al_init_ttf_addon( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't initiate true type font ad-on" << std::endl;;
			return false;
		}
		if( !al_init_primitives_addon( ) )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't initiate primitives ad-on" << std::endl;;
			return false;
		}

		al_reserve_samples( 0 );
		alx::Mixer::getDefault( ).setQuality( ALLEGRO_MIXER_QUALITY_CUBIC );


		_timer = alx::Timer( al_create_timer( _deltaTime ), false );
		if( !_timer )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't create timer" << std::endl;;
			return false;
		}
		_event_queue = alx::EventQueue( al_create_event_queue( ), false );
		if( !_event_queue )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_destroy_timer( _timer.get( ) );
			al_uninstall_system( );
			std::cerr << "Couldn't create event queue" << std::endl;;
			return false;
		}
		_event_queue << alx::Keyboard::getEventSource( ) << alx::Mouse::getEventSource( ) << _mainWindow << _timer;

		font18 = alx::Font( "Data/Fonts/arial.ttf", 18, 0 );
		if( !font18 )
		{
			al_destroy_display( _mainWindow.get( ) );
			al_destroy_timer( _timer.get( ) );
			al_destroy_event_queue( _event_queue.get( ) );
			std::cerr << "Couldn't load font" << std::endl;
			al_uninstall_system( );
			return false;
		}
		return true;
	}

	void Game::SetScreenSize( int width, int height )
	{
		Game::ScreenWidth( ) = width;
		Game::ScreenHeight( ) = height;
	}

	ReadOnlyProperty<int> Game::SCREEN_WIDTH( Game::ScreenWidth( ) );
	ReadOnlyProperty<int> Game::SCREEN_HEIGHT( Game::ScreenHeight( ) );
	ReadOnlyProperty<int> Game::HSCREEN_WIDTH( Game::ScreenWidth( ), [&] {static int h_sw = Game::SCREEN_WIDTH / 2;  return h_sw; } );
	ReadOnlyProperty<int> Game::HSCREEN_HEIGHT( Game::ScreenHeight( ), [&] {static int h_sh = Game::SCREEN_HEIGHT / 2; return h_sh; } );
}
