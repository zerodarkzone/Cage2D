
#include "HelloScene.hpp"

using namespace Cage2d;

HelloScene::HelloScene()
{
	ChangeState( State::Uninitialized );
}

bool HelloScene::Init()
{
	if ( !Scene::Init() )
	{
		return false;
	}
	auto cage2d = Cage2d::Entity::Create( "Data/Sprites/Cage2d.png" );
	cage2d->GetSprite().setScale( 0.3 );
	cage2d->SetPosition( GameApp.Width - cage2d->GetSprite().getSize().getWidth() / 2, cage2d->GetSprite().getSize().getHeight() / 2 );
	AddEntity( "Cage2D", cage2d );
	alx::Font font( "Data/Fonts/arial.ttf", 89 );
	auto fun = [ = ]
	{
		font.draw( GameApp.HalfWidth, ( GameApp.Height - font.getHeight() ) / 2, ALLEGRO_ALIGN_CENTRE, alx::Color( "white" ), "Hello World!" );
	};
	GetMainCamera().AddBackDrawFunction( fun );
	GetMainCamera().SetScale( 1.0 );
	GetMainCamera().SetPosition( Game::HSCREEN_WIDTH, Game::HSCREEN_HEIGHT );
	return true;
}

void HelloScene::Update()
{
	switch ( _state )
	{
	case HelloScene::State::Paused:
		ShowMenu();
		break;
	case HelloScene::State::Playing:
	{
		// UPDATE KEYBOARD INPUT
		auto _currentEvent = GAME.GetInput().GetKeyEvent();
		if ( _currentEvent.getType() == ALLEGRO_EVENT_KEY_UP )
		{
			switch ( _currentEvent.getKeyboardKeycode() )
			{
			case ALLEGRO_KEY_ESCAPE:
				ChangeState( State::Uninitialized );
				break;
			case ALLEGRO_KEY_ENTER:
				ChangeState( State::Paused );
				break;
			default:
				break;
			}
		}
	}
	break;
	case HelloScene::State::Exiting:
		break;
	case HelloScene::State::Uninitialized:
		ChangeState( State::Playing );
		break;
	default:
		break;
	}
}

void HelloScene::ChangeState( State newState )
{
	switch ( _state )
	{
	case HelloScene::State::Paused:
		PauseEntities( false );
		break;
	case HelloScene::State::Playing:
		PauseEntities( true );
		break;
	case HelloScene::State::Exiting:
		break;
	case HelloScene::State::Uninitialized:
		PauseEntities( false );
		break;
	default:
		break;
	}
	_state = newState;
	switch ( _state )
	{
	case HelloScene::State::Paused:
		break;
	case HelloScene::State::Playing:
		if ( !ServiceLocator::GetAudio()->IsSongPlaying() )
		{
			ServiceLocator::GetAudio()->PlaySong( "Data/Sounds/06_Leito.wav", true, 1.3 );
		}
		break;
	case HelloScene::State::Exiting:
		ServiceLocator::GetAudio()->StopAllSounds();
		DoneWithScene( true );
		break;
	case HelloScene::State::Uninitialized:
		break;
	default:
		break;
	}
}

void HelloScene::ShowMenu()
{
	Menu SceneMenu( "Scene Menu" );
	SceneMenu.SetScapeAction( [this] {ChangeState( State::Exiting ); } );

	MenuItem Resume( "RESUME" );
	Resume.SetColor( { 255, 255, 255 } );
	Resume.SetFont( "Data/Fonts/arial.ttf", 88 );
	Resume.SetAction( [this] {ChangeState( State::Playing ); } );
	SceneMenu << Resume;

	MenuItem MainMenu( "MAIN MENU" );
	MainMenu.SetColor( { 255, 255, 255} );
	MainMenu.SetFont( "Data/Fonts/arial.ttf", 88 );
	MainMenu.SetAction( [this] {ChangeState( State::Uninitialized ); GAME.ChangeGameState( Game::GameState::ShowingMenu ); } );
	SceneMenu << MainMenu;

	SceneMenu.SetItemSound( "Data/Sounds/button-21.wav" );
	SceneMenu.HandleEvents( GAME.GetMainWindow() );
}


HelloScene::~HelloScene()
{
}
