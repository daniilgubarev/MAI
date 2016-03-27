#pragma once

#include "CGameState.h"
#include "CTerrain.h"
#include "CScene.h"

class CCubeRenderer : public CGameState
{
public:
	CCubeRenderer(CScene* scene, const CTerrain* terrain);
	~CCubeRenderer();

	bool Update(float dt) override;
	void Render(float dt) override;

private:
	CShaderProgram Shader;

	CVertexAttribArray* VertexBuffer;
	CVertexAttribArray* UVBuffer;

	CIndexBuffer* Indexes;

	CTexture Texture;

	std::vector<SRenderable*> Cubes;

	CScene* Scene;
};
