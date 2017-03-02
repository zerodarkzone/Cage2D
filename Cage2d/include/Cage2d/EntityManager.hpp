#pragma once
#include "Cage2d/Manager.hpp"

namespace Cage2d
{
	class EntityManager :
		public Manager
	{

	public:
		EntityManager( );
		~EntityManager( );

		void AddEntity( const std::string& name, Entity *entity, int order = INT_MAX );
		void AddEntity( const std::string& name, const SmrPtr<Entity>& entity, int order = INT_MAX );
		void AddEntity( const SmrPtr<Entity>& entity, int order = INT_MAX );
		void AddEntity( Entity *entity, int order = INT_MAX );
		void RemoveEntity( const std::string& name );
		void RemoveEntity( int order );
		void RemoveEntity( const SmrPtr<Entity>& entity );

		int GetEntityCount( ) const;

		SmrPtr<Entity> Get( const std::string& name );
		SmrPtr<Entity> Get( int order );

		void DrawAll( ) const override;
		void UpdateAll( ) override;

		friend class Game;
	private:
		struct Node
		{
			std::string name;
			int order;
			friend bool operator <( Node const& lhs, Node const& rhs )
			{
				return lhs.order < rhs.order;
			}
		};

		int _order_count;
		mutable std::map<Node, SmrPtr<Entity>> _entities;
	};
}
