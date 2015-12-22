#pragma once

#include <vector>

#include "CGameState.h"

class CStateManager
{
public:
	CStateManager();
	~CStateManager();

	void AddState(CGameState* state);

	bool UpdateAll(float dt);
	void RenderAll(float dt);

private:
	std::vector<CGameState*> States;
};
