#include "Cage2d/stdafx.h"
#include "Cage2d/Application.hpp"

namespace Cage2d
{
	Application::Application( )
	{}


	Application::~Application( )
	{
		delete &GAME;
	}
}