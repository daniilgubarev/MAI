#pragma once

#include <unordered_set>

#include "CCamera.h"
#include "SRenderable.h"

class CScene
{
	friend class CGraphic;

public:
	CScene();
	~CScene();

	SRenderable* NewRenderableObject();
	void DeleteRenderableObject(SRenderable* renderable);

	void SetActiveCamera(CCamera* camera);
	CCamera* GetActiveCamera() const;

private:
	CCamera* ActiveCamera;
	std::unordered_set<SRenderable*> RenderableObjects;
};