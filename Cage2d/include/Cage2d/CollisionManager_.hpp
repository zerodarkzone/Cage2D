#pragma once
#include "Cage2d/Manager.hpp"
//#define USE_JLIST_DOUBLE
//#define USE_EASTL_LIST

#ifdef USE_JLIST_DOUBLE
#include "co/edu/unal/list/JList.hpp"
#endif // USE_JLIST_DOUBLE
namespace Cage2d
{
	class CollisionManager :
		public Manager
	{
		bool _showBodies;
#ifdef USE_JLIST_DOUBLE
		JList::DoubleChainedList<Body *> _bList;
		JList::DoubleChainedList<Body *> _dBList;
		JList::DoubleChainedList<Body *> _sBList;
#else
#ifdef USE_EASTL_LIST
		eastl::list<Body *> _bList;
		eastl::list<Body *> _dBList;
		eastl::list<Body *> _sBList;
#else
		std::list<Body *> _bList;
		std::list<Body *> _dBList;
		std::list<Body *> _sBList;
#endif // _USE_EASTL_LIST
#endif // _USE_JLIST_DOUBLE

	public:
		CollisionManager( );
		~CollisionManager( );

		void UpdateAll( ) override;
		void DrawAll( ) const override;
		void AddBody( Body *body );
		void AddBody( const SmrPtr<Body>& body );
		void RemoveBody( Body *body );
		void RemoveBody( const SmrPtr<Body>& body );
		void ShowBodies( bool show = true );
	};
}
