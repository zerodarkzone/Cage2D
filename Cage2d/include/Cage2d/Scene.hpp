#pragma once


namespace Cage2d
{
	template <typename _Tyle = Entity>class TiledMap;
	class Menu;
	class MenuItem;
	class Scene;

	using smEntity = SmrPtr<Entity>;
	using smMenu = SmrPtr<Menu>;
	using smMenuItem = SmrPtr<MenuItem>;
	using smScene = SmrPtr<Scene>;
	using smCamera = SmrPtr<Camera>;
	template <typename L>
	using smTiledMap = SmrPtr<TiledMap<L>>;
	using smSprite = SmrPtr<sp::Sprite>;
	using smSpriteSheet = SmrPtr<sp::SpriteSheet>;
	using smAnimatedSprite = SmrPtr<sp::AnimatedSprite>;
	using smAnimation = SmrPtr<sp::Animation>;

	template<typename T>
	struct identity
	{
		using type = T;
	};


	class KeyScene
	{
    private:
		friend class Game;
		KeyScene( ) {}
	};
	class Scene :
		public Entity
	{
	protected:
		Scene( );
		void RemoveEntity( const std::string& name );
		void RemoveEntity( int order );
		void RemoveEntity( const smEntity& entity );
		template <class _Tile> void RemoveTiledMap( const smTiledMap<_Tile>& tiledMap );
		smEntity GetEntity( const std::string& name );
		smEntity GetEntity( int order );
		void UseCamera( bool use );
		void DoneWithScene( bool done );
		Camera& GetMainCamera( );

		/// Selects whether or not the scene can register keyboard events
		void RegisterKeyBoardEvents( bool ke );
		/// Selects whether or not the scene can register mouse events
		void RegisterMouseEvents( bool Me );

		/// Mouse events, you must override this methods to catch mouse input
		virtual void MousePressedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void MouseReleasedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void MouseEnteredEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void MouseExitedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void MouseMovedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void MouseDraggedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}

		/// Keyboard events, you must override this methods to catch keyboard input
		virtual void KeyPressedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void KeyReleasedEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}
		virtual void KeyCharEvent( const Cage2d::ExternInput& input )
		{
			UNREFERENCED_PARAMETER( input )
		}

	public:
		CREATE_FUNC( Scene )

		virtual bool Init( ) override;

		bool DoneWithScene( );
		virtual void Draw( ) const override;
		virtual void Update( ) override;
		void SceneUpdate( const KeyScene& );

		void AddEntity( const std::string& name, Entity *entity, int order = INT_MAX );
		void AddEntity( const std::string& name, const smEntity& entity, int order = INT_MAX );
		void AddEntity( const smEntity& entity );
		void AddEntity( Entity *entity );

		template <class _Tile> void AddTiledMap( const smTiledMap<_Tile>& tiledMap, const std::string& name = "" );
		template <class _Tile> void AddTiledMap( TiledMap<_Tile> *tiledMap, const std::string& name = "" );

		bool UseCamera( );
		void ShowBodies( bool show );

		void Pause( bool pause );
		void StartScene( );
		void PauseEntities( bool pauseEntities );
		Camera& GetMainCamera( const KeyScene& );

		virtual ~Scene( );
	private:
		EntityManager _entityManager;
		CollisionManager _collisionManager;
		bool _doneWithScene;
		bool _useCamera;
		bool _scenePaused;
		bool _entitiesPaused;
		ReadOnlyProperty<bool> Paused;
		ReadOnlyProperty<bool> EntitiesPaused;

		smCamera _mainCamera;

		std::atomic_bool cond;
		bool threadReady;
		std::atomic<bool> flag;
		mutable std::mutex m;
		std::condition_variable cv;
		std::thread thr;
		friend class __cond_ready;
		bool _mouseEvents;
		bool _keyBoardEvents;
	};

	template <class _Tile>
	void Scene::RemoveTiledMap( const smTiledMap<_Tile>& tiledMap )
	{
		if( tiledMap )
		{
			for( auto& ent : tiledMap->GetEntities( ) )
			{
				if( ent->HasBody( ) )
				{
					_collisionManager.RemoveBody( ent->GetBody( ) );
				}
			}
			smEntity tm = tiledMap;
			_entityManager.RemoveEntity( tm );
		}
	}

	template <class _Tile>
	void Scene::AddTiledMap( const smTiledMap<_Tile>& tiledMap, const std::string& name )
	{
		if( tiledMap )
		{
			for( auto& ent : tiledMap->GetEntities( ) )
			{
				if( ent->HasBody( ) )
				{
					_collisionManager.AddBody( ent->GetBody( ) );
				}
			}
			if( name == "" )
			{
				_entityManager.AddEntity( tiledMap->GetName( ), tiledMap );
			}
			else
			{
				_entityManager.AddEntity( name, tiledMap );
				tiledMap->SetName( name );
			}
		}
	}
	template <class _Tile>
	void Scene::AddTiledMap( TiledMap<_Tile> *tiledMap, const std::string& name )
	{
		if( tiledMap )
		{
			for( auto& ent : tiledMap->GetEntities( ) )
			{
				if( ent->HasBody( ) )
				{
					_collisionManager.AddBody( ent->GetBody( ) );
				}
			}
			if( name == "" )
			{
				_entityManager.AddEntity( tiledMap->GetName( ), tiledMap );
			}
			else
			{
				_entityManager.AddEntity( name, tiledMap );
				tiledMap->SetName( name );
			}
		}
	}

	class __cond_ready
	{
		Cage2d::Scene *_scene;
	public:
		__cond_ready( Cage2d::Scene *scene ) : _scene( scene ) {}
		bool operator()( )
		{
			return _scene->cond;
		}
	};
}


