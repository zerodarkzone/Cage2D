#pragma once
#include "Cage2d.hpp"

class AppDelegate :
	public Cage2d::Application
{
public:
	AppDelegate() {}
	~AppDelegate() {}

	void Run() override;

private:
	bool ApplicationInit() override;
};

