#pragma once

namespace Cage2d
{
	template<typename _Tile>
	Cage2d::TiledMap<_Tile>::TiledMap( ) : _width( 0 ), _height( 0 ), _tile_width( 0 ), _tile_height( 0 ), _numLayers( 0 ), _numObjects( 0 ),
		Width( _width ), Height( _height ), TileWidth( _tile_width ), TileHeight( _tile_height ),
		NumberOfLayers( _numLayers ), NumberOfObjects( _numObjects )
	{}

	template<typename _Tile>
	Cage2d::TiledMap<_Tile>::~TiledMap( )
	{
		DEBUG_MSG( "Tiled Map Destroyed" );
	}

	template<typename _Tile>
	void TiledMap<_Tile>::Draw( ) const
	{
		al_hold_bitmap_drawing( true );
		for( const auto& layer : _layers )
		{
			for( const auto& tile : layer.second )
			{
				if( tile->Alive( ) )
				{
					tile->Draw( );
				}
			}
		}
		al_hold_bitmap_drawing( false );
	}

	template<typename _Tile>
	const TiledMap<>::Layer<_Tile>& TiledMap<_Tile>::GetLayer( const std::string& layer ) const
	{
		return _layers.at( layer );
	}

	template<typename _Tile>
	const TiledMap<>::Layers<_Tile>& TiledMap<_Tile>::GetLayers( ) const
	{
		return _layers;
	}

	template<typename _Tile>
	const TiledMap<>::Layer<Entity>& TiledMap<_Tile>::GetObjectLayer( const std::string& layer ) const
	{
		return _objectLayers.at( layer );
	}

	template<typename _Tile>
	const TiledMap<>::Layers<Entity>& TiledMap<_Tile>::GetObjectLayers( ) const
	{
		return _objectLayers;
	}

	template<typename _Tile>
	int TiledMap<_Tile>::pixelToIndexint( int x, int y )
	{
		int _x = x / _tile_width;
		int _y = y / _tile_height;
		int index = ( _y * _width ) + _x; //col * cols + row
		return index;
	}

	template<typename _Tile>
	const sp::SpriteSheet& TiledMap<_Tile>::GetSpriteSheet( ) const
	{
		return _spriteSheet;
	}

	template<typename _Tile>
	std::vector<SmrPtr<Entity>>& TiledMap<_Tile>::GetEntities( )
	{
		return _entities;
	}

	template<typename _Tile>
	bool Cage2d::TiledMap<_Tile>::LoadMap( const std::string& fileName )
	{
		Tmx::Map map;
		map.ParseFile( fileName );

		_width = map.GetWidth( );
		_height = map.GetHeight( );
		_tile_width = map.GetTileWidth( );
		_tile_height = map.GetTileHeight( );
		_numLayers = map.GetNumTileLayers( );
		_numObjects = map.GetNumObjectGroups( );
		int mapSize = _width * _height;

		std::string file;

		auto found = fileName.find_last_of( "/\\" );
		if( found == std::string::npos )
		{
			DEBUG_MSG( "Can't found Tiled Map" );
			file = map.GetTileset( 0 )->GetImage( )->GetSource( );
		}
		else
		{
			std::string path = fileName.substr( 0, found );
			path.push_back( '/' );
			file = path + map.GetTileset( 0 )->GetImage( )->GetSource( );
		}
		DEBUG_MSG( file );
		DEBUG_MSG( _width );
		DEBUG_MSG( _height );
		_spriteSheet = sp::SpriteSheet( file, _tile_width, _tile_height, mapSize,
										map.GetTileset( 0 )->GetSpacing( ), map.GetTileset( 0 )->GetSpacing( ) );

		DEBUG_MSG( _spriteSheet.getSpriteSize( ).getWidth( ) );

		_gids.resize( _numLayers );

		auto layers = map.GetTileLayers( );

		int ind = 0;
		for( auto& layer : layers )
		{
			_layers.insert( _pair<_Tile>( layer->GetName( ), std::vector<SmrPtr<_Tile>>( ) ) );
			_gids[ind].clear( );
			const auto& list = layer->GetProperties( ).GetList( );
			for( int j = 0; j < layer->GetHeight( ); j++ )
			{
				for( int i = 0; i < layer->GetWidth( ); i++ )
				{
					int tid = layer->GetTileId( i, j );
					tid = tid != 0 ? tid : -1;
					_gids[ind].push_back( tid );

					if( tid != -1 )
					{
						int index = matrixToArrayIndex( j, i, _width );
						const auto& tile = _Tile::Create( );
						auto sprite = _spriteSheet.GetSpriteAt( tid );
						sprite.setScale( 1.02, 1.02 );
						tile->Load( sprite );
						tile->SetTag( index );
						tile->SetName( "Tile" + std::to_string( index ) );
						tile->SetPosition( _tile_width * ( index % _width ), _tile_height * ( index / _width ) );
						if( !list.empty( ) )
						{
							for( const auto& prop : list )
							{
								tile->AddProperty( prop.first, prop.second );
							}
						}
						const auto& propList = map.GetTileset( 0 )->GetTile( tid )->GetProperties( ).GetList( );
						if( propList.empty( ) )
						{
							for( const auto& prop : propList )
							{
								tile->ChangeProperty( prop.first, prop.second );
							}
						}
						_layers.at( layer->GetName( ) ).push_back( tile );
						_entities.push_back( tile );

					}
				}
			}
			ind++;
		}
		DEBUG_MSG( _gids[0].size( ) );
		ind = 0;
		const auto& objlayers = map.GetObjectGroups( );

		for( auto& objGrp : objlayers )
		{
			_objectLayers.insert( _pair<Entity>( objGrp->GetName( ), std::vector<SmrPtr<Entity>>( ) ) );
			const auto& list = objGrp->GetProperties( ).GetList( );
			for( auto& obj : objGrp->GetObjects( ) )
			{
				int w = obj->GetWidth( );
				int h = obj->GetHeight( );
				int x = ( obj->GetX( ) + w / 2 - _tile_width / 2 );
				int y = ( obj->GetY( ) + h / 2 - _tile_height / 2 );
				const auto& object = Entity::Create( );
				object->SetName( "Object" + std::to_string( ind ) );
				object->SetPosition( x, y );
				if( !list.empty( ) )
				{
					for( const auto& pro : list )
					{
						object->AddProperty( pro.first, pro.second );
					}
				}
				const auto& objpro = obj->GetProperties( ).GetList( );
				if( !objpro.empty( ) )
				{
					for( const auto& pro : objpro )
					{
						object->ChangeProperty( pro.first, pro.second );
					}
				}

				const auto& b = Body::Create( x - w / 2, y - h / 2, w, h );
				b->SetType( Body::Type::STATIC );
				b->SetCollisionMask( 0 );
				b->SetGroupMask( 0 );
				b->Solid( false );
				b->Gravity( false );
				object->AddBody( b );

				_objectLayers.at( objGrp->GetName( ) ).push_back( object );
				_entities.push_back( object );
			}
			if( !objGrp->GetProperties( ).Empty( ) )
			{
				DEBUG_MSG( "Property" );
				const auto& list = objGrp->GetProperties( ).GetList( );
				for( auto it = list.begin( ); it != list.end( ); ++it )
				{
					DEBUG_MSG( ( it->first ) );
					DEBUG_MSG( ( it->second ) );
				}
			}
			ind++;
		}



		return true;
	}
}
