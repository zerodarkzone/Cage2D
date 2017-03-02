#pragma once
namespace Cage2d
{
	class ExternInput
	{
	public:
		ExternInput( );
		bool isKeyPressed( Key key ) const;
		Key GetKeyPressed( ) const;
		alx::Event GetKeyEvent( ) const;
		alx::Event GetEvent( ) const;
		alx::Point<double> GetMousePosition( ) const;
		bool hasMouseMoved( ) const;
		bool isMousePressed( ) const;
		void Pop( );

		~ExternInput( );
		friend class Game;
	private:
		void ProcessEvent( alx::Event ev );
		void ResetKeyPressed( );
		std::queue<alx::Event> _keyEvents;
		std::queue<alx::Event> _events;
		mutable alx::Point<int> _mousePosition;
		bool _keys[ALLEGRO_KEY_MAX];
		mutable bool _mouseMoved;
		mutable bool _mousePressed;
	};
}
