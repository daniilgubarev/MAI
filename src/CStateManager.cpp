#include <cstddef>

#include "CStateManager.h"

CStateManager::CStateManager()
{
}

CStateManager::~CStateManager()
{
	for (size_t i = 0; i < States.size(); i++)
	{
		delete States[i];
	}
}

void CStateManager::AddState(CGameState* state)
{
	States.push_back(state);
}

bool CStateManager::UpdateAll(float dt)
{
	for (size_t i = 0; i < States.size(); i++)
	{
		if (!States[i]->Update(dt))
		{
			delete States[i];

			States.erase(States.begin() + i);
		}
	}

	if (!States.size())
		return false;

	return true;
}

void CStateManager::RenderAll(float dt)
{
	for (size_t i = 0; i < States.size(); i++)
	{
		States[i]->Render(dt);
	}
}
