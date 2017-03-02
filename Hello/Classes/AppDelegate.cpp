#include "AppDelegate.hpp"
#include "HelloScene.hpp"

bool AppDelegate::ApplicationInit()
{
	// DON'T MODIFY THIS ORDER
	GAME.SetScreenSize( 600, 400 );
	GAME.ShowFPS( true );
	GAME.ShowMenu( false );
	if ( GAME.Init() )
	{
		// Here goes game creation and initialization
		const auto &mainScene = HelloScene::Create();
		GAME.RunningScene( mainScene );
		GAME.BackGroundColor( { "black" } );
		GAME.SetWindowTitle("Hello");
		return true;
	}
	return false;
}

void AppDelegate::Run()
{
	if ( ApplicationInit() )
	{
		GAME.Run();
	}
}
