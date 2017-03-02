#pragma once
#include "Cage2d.hpp"

class HelloScene :
	public Cage2d::Scene
{
	HelloScene();
public:
	CREATE_FUNC( HelloScene )
	bool Init() override;
	virtual void Update() override;
	~HelloScene();
private:
	enum class State
	{
		Paused, Playing, Exiting, Uninitialized
	};
	void ChangeState( State newState );
	State _state;
	void ShowMenu();
};

