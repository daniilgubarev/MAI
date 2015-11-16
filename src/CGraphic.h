#pragma once

#include <string>

#include "GL_common.h"
#include "CVertexAttribArray.h"
#include "CShaderProgram.h"

class CGraphic
{
public:
	CGraphic();
	~CGraphic();

	bool Init(std::string windowTitle,
		int windowWidth, int windowHeight,
		bool fullscreen);

	void SwapBuffres();

	bool SetVertexAttribPointer(const CVertexAttribArray& attribArray, int layoutIndex);

	bool UseShaderProgram(const CShaderProgram& program);

private:
	SDL_Window* Window;
};