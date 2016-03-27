#include "CommonHeaders.h"
#include "CScene.h"

CScene::CScene()
{
	ActiveCamera = nullptr;
}

CScene::~CScene()
{
	for (std::unordered_set<SRenderable*>::const_iterator it = RenderableObjects.begin();
		 it != RenderableObjects.end();
		 it++)
	{
		delete *it;
	}
}

SRenderable* CScene::NewRenderableObject()
{
	SRenderable* newRenderable = new SRenderable;

	RenderableObjects.insert(newRenderable);

	return newRenderable;
}

void CScene::DeleteRenderableObject(SRenderable* renderable)
{
	RenderableObjects.erase(renderable);
}

void CScene::SetActiveCamera(CCamera* camera)
{
	ActiveCamera = camera;
}

CCamera* CScene::GetActiveCamera() const
{
	return ActiveCamera;
}