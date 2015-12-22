#pragma once

#include "CGameState.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CInput.h"

class CCameraManager : public CGameState
{
public:
	CCameraManager(CCamera* camera, CInput* input, CTerrain* terrain);
	~CCameraManager();

	bool Update(float dt) override;
	void Render(float dt) override;

private:
	CCamera*  Camera;
	CInput*	  Input;
	CTerrain* Terrain;
};
