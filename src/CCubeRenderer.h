#pragma once

#include "CGameState.h"
#include "CTerrain.h"

class CCubeRenderer : public CGameState
{
public:
	CCubeRenderer(CGraphic* graphic, const CTerrain* terrain);
	~CCubeRenderer();

	bool Update(float dt) override;
	void Render(float dt) override;

private:
	CShaderProgram Shader;

	CVertexAttribArray* VertexBuffer;
	CVertexAttribArray* UVBuffer;

	CIndexBuffer* Indexes;

	CTexture Texture;

	std::vector<SRenderable> Cubes;

	CGraphic* Graphic;
};
