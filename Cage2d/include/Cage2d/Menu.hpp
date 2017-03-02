#pragma once

namespace Cage2d
{
	class MenuItem;

	class Menu
	{
		std::string _name;
		bool __enteredMenuItem;
		bool _isSongPlaying;
		std::string _buttonSound;
		unsigned int MaxElements;
		bool _action;
		std::list<MenuItem> _menuItems;
		void OrganizeItems( );
		std::function<void( )> _scapeAction;
		alx::Color _color;
		void RunAction( );
		void ManageEvents( std::list<MenuItem>::iterator& it, int _case, int x = 0, int y = 0 );
	protected:
		void ShowMenu( );
	public:
		Menu( alx::Color color = alx::Color( "black" ) );
		Menu( const std::string& name, alx::Color color = alx::Color( "black" ) );
		virtual void HandleEvents( alx::Display& window );
		void AddMenuItem( const MenuItem& menuItem );
		Menu& operator << ( const MenuItem& menuItem );
		void SetScapeAction( const std::function<void( )>& action );
		void PlayMusic( const std::string& name );
		void SetItemSound( const std::string& name );

		std::string GetName( );
		virtual ~Menu( );
	};
}
