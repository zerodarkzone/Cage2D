#include "Cage2d/stdafx.h"
#include "Cage2d/Body.hpp"
#include "Cage2d/CollisionManager.hpp"


namespace Cage2d
{
	CollisionManager::CollisionManager( ) : _showBodies( false )
	{}

	CollisionManager::~CollisionManager( )
	{
		_bList.clear( );
		_sBList.clear( );
		_dBList.clear( );
	}

#ifdef USE_JLIST_DOUBLE
	void CollisionManager::UpdateAll( )
	{
		if( !_bList.isEmpty( ) )
		{
			auto d = _dBList.begin( );
			while( d != _dBList.end( ) )
			{
				if( ( *d )->Alive( ) )
				{
					( *d )->Update( );
				}
				else
				{
					auto body = _dBList.remove( d );
					_bList.ObjectRemove( body );
				}
				++d;
			}
			//JList::DoubleChainedList<Body*>::MyIterator<Body*> d = _dBList.begin();
			d = _dBList.begin( );
			while( d != _dBList.end( ) )
			{
				//bool _remove = false;
				/*if ((*d)->Alive( )) { (*d)->Update( ); }
				else
				{
					_remove = true;
					auto body = _dBList.remove( d );
					_bList.ObjectRemove( body );
					++d;
				}*/

				for( auto b : _bList )
				{
					if( ( *d ) == b )
					{
						continue;
					}
					if( ( *d )->Collision( *b ) )
					{
						break;
					}
				}

				//if (!_remove) { ++d; }
				++d;
			}
			auto it = _sBList.begin( );
			while( it != _sBList.end( ) )
			{
				if( ( *it )->Alive( ) )
				{
					( *it )->Update( );
				}
				else
				{
					auto body = _sBList.remove( it );
					_bList.ObjectRemove( body );
				}
				++it;
			}
		}
	}
#else
	void CollisionManager::UpdateAll( )
	{
		if( !_bList.empty( ) )
		{
			auto d = _dBList.begin( );
			while( d != _dBList.end( ) )
			{
				if( ( *d )->Alive( ) )
				{
					( *d )->Update( );
					++d;
				}
				else
				{
					auto body = *d;
					d = _dBList.erase( d );
					_bList.remove( body );
				}
			}
			//JList::DoubleChainedList<Body*>::MyIterator<Body*> d = _dBList.begin();
			d = _dBList.begin( );
			while( d != _dBList.end( ) )
			{
				//bool _remove = false;
				/*if ((*d)->Alive( )) { (*d)->Update( ); }
				else
				{
				_remove = true;
				auto body = _dBList.remove( d );
				_bList.ObjectRemove( body );
				++d;
				}*/

				for( auto b : _bList )
				{
					if( ( *d ) == b )
					{
						continue;
					}
					if( ( *d )->Collision( *b ) )
					{
						break;
					}
				}

				//if (!_remove) { ++d; }
				++d;
			}
			auto it = _sBList.begin( );
			while( it != _sBList.end( ) )
			{
				if( ( *it )->Alive( ) )
				{
					( *it )->Update( );
				}
				else
				{
					auto body = *it;
					it = _sBList.erase( it );
					_bList.remove( body );
				}
				++it;
			}
		}
	}
#endif // _USE_JLIST_DOUBLE



	void CollisionManager::DrawAll( ) const
	{
		if( _showBodies )
		{
			for( auto& b : _bList )
			{
				b->Draw( );
			}
		}
	}

	void CollisionManager::AddBody( Body *body )
	{
		if( body )
		{
			if( body->GetType( ) == Body::Type::STATIC )
			{
				_sBList.push_back( body );
			}
			else if( body->GetType( ) == Body::Type::DYNAMIC || body->GetType( ) == Body::Type::KINEMATIC )
			{
				_dBList.push_back( body );
			}
			_bList.push_back( body );
		}
	}

	void CollisionManager::RemoveBody( Body *body )
	{
		if( body )
		{
			if( body->GetType( ) == Body::Type::STATIC )
			{
#ifdef USE_JLIST_DOUBLE
				_sBList.ObjectRemove( body );
#else
				_sBList.remove( body );
#endif // _USE_JLIST_DOUBLE
			}
			else if( body->GetType( ) == Body::Type::DYNAMIC || body->GetType( ) == Body::Type::KINEMATIC )
			{
#ifdef USE_JLIST_DOUBLE
				_dBList.ObjectRemove( body );
#else
				_dBList.remove( body );
#endif // _USE_JLIST_DOUBLE
			}
#ifdef USE_JLIST_DOUBLE
			_bList.ObjectRemove( body );
#else
			_bList.remove( body );
#endif // _USE_JLIST_DOUBLE
		}
	}

	void CollisionManager::AddBody( const SmrPtr<Body>& body )
	{
		if( body )
		{
			if( body->GetType( ) == Body::Type::STATIC )
			{
				_sBList.push_back( body.get( ) );
			}
			else if( body->GetType( ) == Body::Type::DYNAMIC || body->GetType( ) == Body::Type::KINEMATIC )
			{
				_dBList.push_back( body.get( ) );
			}
			_bList.push_back( body.get( ) );
		}
	}

	void CollisionManager::RemoveBody( const SmrPtr<Body>& body )
	{
		if( body )
		{
			if( body->GetType( ) == Body::Type::STATIC )
			{
#ifdef USE_JLIST_DOUBLE
				_sBList.ObjectRemove( body.get( ) );
#else
				_sBList.remove( body.get( ) );
#endif // _USE_JLIST_DOUBLE
			}
			else if( body->GetType( ) == Body::Type::DYNAMIC || body->GetType( ) == Body::Type::KINEMATIC )
			{
#ifdef USE_JLIST_DOUBLE
				_dBList.ObjectRemove( body.get( ) );
#else
				_dBList.remove( body.get( ) );
#endif // _USE_JLIST_DOUBLE
			}
#ifdef USE_JLIST_DOUBLE
			_bList.ObjectRemove( body.get( ) );
#else
			_bList.remove( body.get( ) );
#endif // _USE_JLIST_DOUBLE
		}
	}

	void CollisionManager::ShowBodies( bool show )
	{
	    for( auto& b : _bList )
        {
            b->Draw( show );
		}
		_showBodies = show;
	}
}
