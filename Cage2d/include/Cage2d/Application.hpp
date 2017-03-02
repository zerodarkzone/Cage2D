#pragma once
namespace Cage2d
{
	class Application
	{
	public:
		Application( );
		virtual ~Application( );
		virtual void Run( ) = 0;
	protected:
		virtual bool ApplicationInit( ) = 0;
	};
}
