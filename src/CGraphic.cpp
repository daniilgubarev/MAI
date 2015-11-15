#include <iostream>

#include "CGraphic.h"

CGraphic::CGraphic()
{
	Window = nullptr;
}

CGraphic::~CGraphic()
{}

bool CGraphic::Init(std::string windowTitle,
	int windowWidth, int windowHeight,
	bool fullscreen)
{	
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init() error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_DisplayMode displayMode;

	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
	{
		std::cerr << "SDL_GetDesktopDisplayMode() failed" << std::endl;
		return false;
	}

	std::cout << "displayMode.w : " << displayMode.w << std::endl
			  << "displayMode.h : " << displayMode.h << std::endl;

	if (windowWidth <= 0 || windowHeight <= 0)
	{
		windowWidth  = displayMode.w;
		windowHeight = displayMode.h;
	}

	int winPosX = 0;
	int winPosY = 0;
	Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

	if (fullscreen)
	{
		windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else
	{
		winPosX = (displayMode.w - windowWidth)  / 2;
		winPosY = (displayMode.h - windowHeight) / 2;
	}

	Window = SDL_CreateWindow(windowTitle.c_str(),
		winPosX, winPosY,
		windowWidth, windowHeight,
		windowFlags);

	SDL_GL_CreateContext(Window);

	// Инициализируем GLEW
	glewExperimental = true; // Флаг необходим в Core-режиме OpenGL
	
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Невозможно инициализировать GLEW" << std::endl;
		return false;
	}

 	glEnable(GL_DEPTH_TEST);
 	glDepthFunc(GL_LESS);

	return true;
}

void CGraphic::SwapBuffres()
{
	SDL_GL_SwapWindow(Window);
}

bool CGraphic::SetVertexAttribPointer(const CVertexAttribArray& attribArray, int layoutIndex)
{
	GLenum type;

	switch (attribArray.GetAttribType())
	{
		case CVertexAttribArray::AT_FLOAT:
		{
			type = GL_FLOAT;
			break;
		}

		case CVertexAttribArray::AT_UNSIGNED_BYTE:
		{
			type = GL_UNSIGNED_BYTE;
			break;
		}

		default:
		{
			std::cerr << "Wrong attribArray.AttribType" << std::endl;
			return false;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, attribArray.GetAttribBufferID());

	glVertexAttribPointer(
		layoutIndex,					// Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
		attribArray.GetAttribSize(),	// Размер
		type,							// Тип
		attribArray.IsNormalized(),		// Указывает, что значения не нормализованы
		0,								// Шаг
		(void*)0						// Смещение массива в буфере
	);

	if (glGetError() != GL_NO_ERROR)
		return false;

	return true;
}