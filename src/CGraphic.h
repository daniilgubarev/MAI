#pragma once

#include <string>

#include "GL_common.h"
#include "CVertexAttribArray.h"
#include "CShaderProgram.h"
#include "CTexture.h"
#include "CIndexBuffer.h"

class CGraphic
{
public:
	CGraphic();
	~CGraphic();

	bool Init(std::string windowTitle,
		int windowWidth, int windowHeight,
		bool fullscreen);

	void SwapBuffres();

	bool SetVertexAttribArray(const CVertexAttribArray& attribArray, int layoutIndex);
	bool SetTexture(const CTexture& texture, int index);

	void DrawArrays(int vertexCount);
	void DrawElements(const CIndexBuffer& indexBuffer);

	bool UseShaderProgram(const CShaderProgram& program);

private:
	SDL_Window* Window;
};