#pragma once

#include <string>
#include <vector>

#include "CommonHeaders.h"
#include "CVertexAttribArray.h"
#include "CShaderProgram.h"
#include "CTexture.h"
#include "CIndexBuffer.h"
#include "CCamera.h"
#include "SRenderable.h"
#include "CScene.h"

class CGraphic
{
public:
	CGraphic();
	~CGraphic();

	bool Init(std::string windowTitle,
		int windowWidth, int windowHeight,
		bool fullscreen);

	void Clear();
	void SwapBuffres();

	bool SetVertexAttribArray(const CVertexAttribArray& attribArray, int layoutIndex);
	bool SetTexture(const CTexture& texture, int index);

	void DrawArrays(int vertexCount);
	void DrawIndexedArrays(const CIndexBuffer& indexBuffer);
	void DrawScene(CScene* scene);

	void DrawQuad();

	bool UseShaderProgram(const CShaderProgram& program);

private:
	CCamera* ActiveCamera;
	std::vector<SRenderable*> RenderableObjects;

	SDL_Window* Window;
};
