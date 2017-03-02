#pragma once
#include "Entity.hpp"
#include "SpriteSheet.hpp"
#include <tmxparser/Tmx.h>

namespace Cage2d
{
	template<typename _Tile>class TiledMap :
		public Entity
	{
	public:
		template <typename T>
		using Layers = std::map < std::string, std::vector<SmrPtr<T>> >;
		template <typename T>
		using Layer = std::vector < SmrPtr<T> >;
	private:
		template <typename T>
		using _pair = std::pair < std::string, std::vector<SmrPtr<T>> >;
		friend class Scene;
		int _width, _height, _tile_width, _tile_height, _numLayers, _numObjects;
		sp::SpriteSheet _spriteSheet;
		Layers<_Tile> _layers;
		Layers<Entity> _objectLayers;
		std::vector<std::vector<int>> _gids;
		std::vector<SmrPtr<Entity>> _entities;
		TiledMap( );
	public:
		ReadOnlyProperty<int> Width, Height, TileWidth, TileHeight, NumberOfLayers, NumberOfObjects;
		CREATE_FUNC( TiledMap )
			bool LoadMap( const std::string& fileName );
		void Draw( ) const override;
		const Layer<_Tile>& GetLayer( const std::string& layer ) const;
		const Layers<_Tile>& GetLayers( ) const;
		const Layer<Entity>& GetObjectLayer( const std::string& layer ) const;
		const Layers<Entity>& GetObjectLayers( ) const;
		const sp::SpriteSheet& GetSpriteSheet( ) const;
		int pixelToIndexint( int x, int y );

		~TiledMap( );
	private:
		std::vector<SmrPtr<Entity>>& GetEntities( );
	};
}

#include "TiledMapImp.hxx"
