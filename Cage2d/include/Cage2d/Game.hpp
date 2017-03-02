#pragma once
#include "KeyCodes.hpp"
#include "ExternInput.hpp"

namespace Cage2d
{

	class Scene;
	class Camera;

	class Game
	{
		std::stack<SmrPtr<Scene>> _sceneStack;

	public:
		static ReadOnlyProperty<int> SCREEN_WIDTH;
		static ReadOnlyProperty<int> SCREEN_HEIGHT;
		static ReadOnlyProperty<int> HSCREEN_WIDTH;
		static ReadOnlyProperty<int> HSCREEN_HEIGHT;		

		enum class GameState
		{
			Uninitialized, ShowingSplash, ShowingMenu,
			Playing, Exiting, Saving, Loading
		};

		INSTANCE( Game );
		~Game( );

		bool Init( float fps = 60.0 );
		void Run( );

		alx::Display& GetMainWindow( );
		const ExternInput& GetInput( );
		void SetMainCamera( Camera& camera );
		Camera& GetMainCamera( );
		void ShowFPS( bool show );
		void ShowMenu( bool show );
		void ClearEventQueue( );

		GameState GetGameState( );
		void ChangeGameState( GameState newGameState );

		void SetScreenSize( int width, int height );
		void SetWindowTitle( const std::string& title );

		SmrPtr<Scene> ReplaceScene( const SmrPtr<Scene>& scene );
		SmrPtr<Scene> RunningScene( );
		void RunningScene( const SmrPtr<Scene>& scene );
		void PushScene( const SmrPtr<Scene>& scene );
		void PopScene( );
		void BackGroundColor( const alx::Color& color );

	private:
		Game( );

		Game( Game const& ) = delete;
		void operator=( Game const& ot ) = delete;


		bool IsExiting( );
		void GameLoop( );
		bool InitAllegro( );
		void Close( );

		void ShowSplashScreen( );
		void ShowMenu( );

		GameState _gameState;
		alx::Display _mainWindow;
		alx::Timer _timer;
		alx::EventQueue _event_queue;
		alx::Font font18;
		alx::Color _backGroundColor;

		ExternInput _externInput;

		double gameTime;
		bool render;
		int frames;
		int gameFPS;
		float _deltaTime;

		SmrPtr<Camera> _defaultCamera;
		Camera *_mainCamera;

		bool _showFPS;
		bool _showMenu;

		SmrPtr<Scene> _runningScene;

		bool _badInit;

		static int& ScreenWidth( )
		{
			static int _SCREEN_WIDTH = 1366;
			return _SCREEN_WIDTH;
		}

		static int& ScreenHeight( )
		{
			static int _SCREEN_HEIGHT = 768;
			return _SCREEN_HEIGHT;
		}
	public:		
		GET( Game, float, DeltaTime, { return _deltaTime; } );
		GET( Game, int, Width, { return Game::ScreenWidth( ); } );
		GET( Game, int, Height, { return Game::ScreenHeight( ); } );
		GET( Game, int, HalfWidth, { return Game::ScreenWidth( ) / 2; } );
		GET( Game, int, HalfHeight, { return Game::ScreenHeight( ) / 2; } );
	};
}
