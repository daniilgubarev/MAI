#pragma once

class CGameState
{
public:
	virtual ~CGameState() {};

	virtual bool Update(float dt) = 0;
	virtual void Render(float dt) = 0;
};
