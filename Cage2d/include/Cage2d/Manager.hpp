#pragma once

namespace Cage2d
{
	class Manager
	{
	public:
		virtual ~Manager( ) = 0;
		virtual void DrawAll( ) const = 0;
		virtual void UpdateAll( ) = 0;
	};

	inline Manager::~Manager( ) {}
}
