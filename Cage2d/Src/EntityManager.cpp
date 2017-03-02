#include "Cage2d/stdafx.h"
#include "Cage2d/Entity.hpp"
#include "Cage2d/EntityManager.hpp"

namespace Cage2d
{
	EntityManager::EntityManager( ) : _order_count( INT_MIN )
	{}

	void EntityManager::AddEntity( const std::string& name, Entity *entity, int order )
	{
		if( entity )
		{
			if( !_entities.empty( ) )
			{
				for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
				{
					if( it->second.get( ) == entity )
					{
						return;
					}
				}
			}
			if( order == INT_MAX )
			{
				Node node{ name, _order_count };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order = ++_order_count;
					}
				}
				entity->SetOrder( _order_count++ );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, SmrPtr<Entity>( entity ) ) );
			}
			else
			{
				Node node{ name, order };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order++;
					}
				}
				entity->SetOrder( node.order );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, SmrPtr<Entity>( entity ) ) );
			}
		}
	}

	void EntityManager::AddEntity( const std::string& name, const SmrPtr<Entity>& entity, int order )
	{
		if( entity )
		{
			if( !_entities.empty( ) )
			{
				for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
				{
					if( it->second == entity )
					{
						return;
					}
				}
			}
			if( order == INT_MAX )
			{
				Node node{ name, _order_count };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order = ++_order_count;
					}
				}
				entity->SetOrder( _order_count++ );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, entity ) );
			}
			else
			{
				Node node{ name, order };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order++;
					}
				}
				entity->SetOrder( node.order );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, entity ) );
			}
		}
	}

	void EntityManager::AddEntity( const SmrPtr<Entity>& entity, int order )
	{
		if( entity )
		{
			if( !_entities.empty( ) )
			{
				for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
				{
					if( it->second == entity )
					{
						return;
					}
				}
			}
			if( order == INT_MAX )
			{
				Node node{ entity->GetName( ), _order_count };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order = ++_order_count;
					}
				}
				entity->SetOrder( _order_count++ );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, entity ) );
			}
			else
			{
				Node node{ entity->GetName( ), entity->GetOrder( ) };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order++;
					}
				}
				entity->SetOrder( node.order );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, entity ) );
			}
		}
	}

	void EntityManager::AddEntity( Entity *entity, int order )
	{
		if( entity )
		{
			if( !_entities.empty( ) )
			{
				for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
				{
					if( it->second.get( ) == entity )
					{
						return;
					}
				}
			}
			if( order == INT_MAX )
			{
				Node node{ entity->GetName( ), _order_count };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order = ++_order_count;
					}
				}
				entity->SetOrder( _order_count++ );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, SmrPtr<Entity>( entity ) ) );
			}
			else
			{
				Node node{ entity->GetName( ), entity->GetOrder( ) };
				if( !_entities.empty( ) )
				{
					if( _entities.count( node ) )
					{
						node.order++;
					}
				}
				entity->SetOrder( node.order );
				_entities.insert( std::pair<Node, SmrPtr<Entity>>( node, SmrPtr<Entity>( entity ) ) );
			}
		}
	}

	void EntityManager::RemoveEntity( const std::string& name )
	{
		if( !_entities.empty( ) )
		{
			for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
			{
				if( it->first.name == name )
				{
					it = _entities.erase( it );
					if( it == _entities.end( ) )
					{
						break;
					}
				}
			}
		}
	}

	void EntityManager::RemoveEntity( int order )
	{
		if( !_entities.empty( ) )
		{
			for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
			{
				if( it->first.order == order )
				{
					it = _entities.erase( it );
					break;
				}
			}
		}
	}

	void EntityManager::RemoveEntity( const SmrPtr<Entity>& entity )
	{
		if( !_entities.empty( ) )
		{
			for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
			{
				if( it->second.get( ) == entity.get( ) )
				{
					it = _entities.erase( it );
					break;
				}
			}
		}
	}

	int EntityManager::GetEntityCount( ) const
	{
		return static_cast<int>( _entities.size( ) );
	}

	SmrPtr<Entity> EntityManager::Get( const std::string& name )
	{
		if( !_entities.empty( ) )
		{
			for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
			{
				if( it->first.name == name )
				{
					return it->second;
				}
			}
		}
		return nullptr;
	}

	SmrPtr<Entity> EntityManager::Get( int order )
	{
		if( !_entities.empty( ) )
		{
			for( auto it = _entities.begin( ); it != _entities.end( ); ++it )
			{
				if( it->first.order == order )
				{
					return it->second;
				}
			}
		}
		return nullptr;
	}

	void EntityManager::DrawAll( ) const
	{
		if( !_entities.empty( ) )
		{
			auto it = _entities.begin( );
			while( it != _entities.end( ) )
			{
				it->second->Draw( );
				++it;
			}
		}
	}

	void EntityManager::UpdateAll( )
	{
		if( !_entities.empty( ) )
		{
			std::map<Node, SmrPtr<Entity>>::const_iterator it = _entities.begin( );
			while( it != _entities.end( ) )
			{
				if( it->second->Alive( ) )
				{
					it->second->Update( );
					++it;
				}
				else
				{
					DEBUG_MSG( "Erase entity" );
					it = _entities.erase( it );
				}
			}
		}
	}

	EntityManager::~EntityManager( )
	{}
}
