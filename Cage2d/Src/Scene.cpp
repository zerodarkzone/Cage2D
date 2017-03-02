#include "Cage2d/stdafx.h"

#include "Cage2d/Scene.hpp"
#include "Cage2d/TiledMap.hpp"
#include "Cage2d/Camera.hpp"

#include <mutex>

namespace Cage2d
{

	Scene::Scene( ) : _entityManager( ), _collisionManager( ),
		_doneWithScene( false ), _useCamera( true ), _scenePaused( false ), _entitiesPaused( false ),
		Paused( _scenePaused ), EntitiesPaused( _entitiesPaused ), _mainCamera( nullptr ), cond( false ), threadReady( false ), flag( false ), m( ), cv( ),
		_mouseEvents( false ), _keyBoardEvents( false )
	{}

	void Scene::UseCamera( bool use )
	{
		_useCamera = use;
	}

	bool Scene::UseCamera( )
	{
		return _useCamera;
	}

	Camera& Scene::GetMainCamera( )
	{
		return *_mainCamera;
	}

	Camera& Scene::GetMainCamera( const KeyScene& )
	{
		return *_mainCamera;
	}

	void Scene::RegisterKeyBoardEvents( bool ke )
	{
		_keyBoardEvents = ke;
	}

	void Scene::RegisterMouseEvents( bool Me )
	{
		_mouseEvents = Me;
	}

	bool Scene::Init( )
	{
		if( !Entity::Init( ) )
		{
			return false;
		}
		auto _drawFunc = [this] ( )->void {_entityManager.DrawAll( ); };
		auto _cDrawFunc = [this] ( )->void {_collisionManager.DrawAll( ); };
		_mainCamera = smCamera( new( std::nothrow )Camera( ) );
		if( !_mainCamera )
		{
			return false;
		}
		_mainCamera->SetSize( Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT );
		_mainCamera->SetPosition( Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2 );
		*_mainCamera << _drawFunc << _cDrawFunc;
		return true;
	}

	void Scene::Draw( ) const
	{
		//std::lock_guard<std::mutex> lm( m );
		_mainCamera->Draw( GAME.GetMainWindow( ) );
	}

	void Scene::Update( )
	{}

	void Scene::SceneUpdate( const KeyScene& )
	{
		if( _scenePaused )
		{
			return;
		}
		StartScene( );
		if( !_entitiesPaused )
		{
			std::lock_guard<std::mutex> lm( m );
			_entityManager.UpdateAll( );
			cond = true;
			cv.notify_one( );
		}
		if( _keyBoardEvents || _mouseEvents )
		{
			auto& _currentEvent = GAME.GetInput( );
			if( _keyBoardEvents )
			{
				auto _keyboardEvent = _currentEvent.GetKeyEvent( );
				if( _keyboardEvent.getType( ) == ALLEGRO_EVENT_KEY_DOWN )
				{
					this->KeyPressedEvent( _currentEvent );
				}
				else if( _keyboardEvent.getType( ) == ALLEGRO_EVENT_KEY_UP )
				{
					this->KeyReleasedEvent( _currentEvent );
				}
				if( _keyboardEvent.getType( ) == ALLEGRO_EVENT_KEY_CHAR )
				{
					this->KeyCharEvent( _currentEvent );
				}
			}
			if( _mouseEvents )
			{
				if( _currentEvent.hasMouseMoved( ) )
				{
					MouseMovedEvent( _currentEvent );
					if( _currentEvent.isMousePressed( ) )
					{
						MouseDraggedEvent( _currentEvent );
					}
				}
				switch( _currentEvent.GetEvent( ).getType( ) )
				{
				case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
					MouseReleasedEvent( _currentEvent );
					break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
					MousePressedEvent( _currentEvent );
					break;
				case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
					MouseEnteredEvent( _currentEvent );
					break;
				case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
					MouseExitedEvent( _currentEvent );
					break;
				}
			}
		}
		this->Update( );
	}

	bool Scene::DoneWithScene( )
	{
		return _doneWithScene;
	}

	void Scene::ShowBodies( bool show )
	{
		_collisionManager.ShowBodies( show );
	}

	void Scene::Pause( bool pause )
	{
		std::lock_guard<std::mutex> lg( m );
		_scenePaused = pause;
	}

	void Scene::StartScene( )
	{
		if( !flag )
		{
			thr = std::thread( [this] ( ) {
				{
					std::unique_lock<std::mutex> lk( m );
					cv.wait( lk, [&] {return threadReady; } );
				}
				//std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
				while( !_doneWithScene )
				{
					std::unique_lock<std::mutex> lk( m );
					cv.wait_for( lk, std::chrono::milliseconds( 17 ), __cond_ready( this ) );
					if( !_entitiesPaused )
					{
						_collisionManager.UpdateAll( );
					}
					cond = false;
				}
				std::lock_guard<std::mutex> lm( m );
				return;
			} );
			flag = true;
			{
				std::lock_guard<std::mutex> lm( m );
				threadReady = true;
				cv.notify_one( );
			}
			DEBUG_MSG( "Called Once" );
		}
	}

	void Scene::PauseEntities( bool pauseEntities )
	{
		std::lock_guard<std::mutex> lg( m );
		_entitiesPaused = pauseEntities;
	}

	void Scene::AddEntity( const std::string& name, Entity *entity, int order )
	{
		_entityManager.AddEntity( name, entity, order );
		if( entity->HasBody( ) )
		{
			_collisionManager.AddBody( entity->GetBody( ) );
		}
	}

	void Scene::AddEntity( const std::string& name, const smEntity& entity, int order )
	{
		_entityManager.AddEntity( name, entity, order );
		if( entity->HasBody( ) )
		{
			_collisionManager.AddBody( entity->GetBody( ) );
		}
	}

	void Scene::AddEntity( const smEntity& entity )
	{
		_entityManager.AddEntity( entity );
		if( entity->HasBody( ) )
		{
			_collisionManager.AddBody( entity->GetBody( ) );
		}
	}

	void Scene::AddEntity( Entity *entity )
	{
		_entityManager.AddEntity( entity );
		if( entity->HasBody( ) )
		{
			_collisionManager.AddBody( entity->GetBody( ) );
		}
	}

	void Scene::RemoveEntity( const std::string& name )
	{
		auto entity = GetEntity( name );
		std::lock_guard<std::mutex> lm( m );
		_entityManager.RemoveEntity( entity );
		if( entity->HasBody( ) )
		{
			_collisionManager.RemoveBody( entity->GetBody( ) );
		}
	}

	void Scene::RemoveEntity( int order )
	{
		auto entity = GetEntity( order );
		std::lock_guard<std::mutex> lm( m );
		_entityManager.RemoveEntity( order );
		if( entity->HasBody( ) )
		{
			_collisionManager.RemoveBody( entity->GetBody( ) );
		}
	}

	void Scene::RemoveEntity( const smEntity& entity )
	{
		std::lock_guard<std::mutex> lm( m );
		if( entity->HasBody( ) )
		{
			_collisionManager.RemoveBody( entity->GetBody( ) );
		}
		_entityManager.RemoveEntity( entity );
	}

	smEntity Scene::GetEntity( const std::string& name )
	{
		return _entityManager.Get( name );
	}

	smEntity Scene::GetEntity( int order )
	{
		return _entityManager.Get( order );
	}

	void Scene::DoneWithScene( bool done )
	{
		DEBUG_MSG( "Ending Scene" );
		{
			std::lock_guard<std::mutex> lg( m );
			_doneWithScene = done;
			cond = true;
			cv.notify_all( );
		}
		DEBUG_MSG( "Scene Ended" );
	}

	Scene::~Scene( )
	{
		{
			Scene::DoneWithScene( true );
		}
		DEBUG_MSG( "Scene Destroyed" );
		if( thr.joinable( ) )
		{
			//thr.detach();
			thr.join( );
			DEBUG_MSG( "Thread Joined" );
		}
	}
}
