#pragma once

#include <string>

#include "GL_common.h"
#include "CVertexAttribArray.h"

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

private:
	SDL_Window* Window;
};