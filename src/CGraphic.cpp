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

 	glEnable(GL_CULL_FACE);
 	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	GL_CHECK();
	/*glDisable ( GL_CULL_FACE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) ;*/

	return true;
}

void CGraphic::Clear()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CGraphic::SwapBuffres()
{
	SDL_GL_SwapWindow(Window);
}

bool CGraphic::SetVertexAttribArray(const CVertexAttribArray& attribArray, int layoutIndex)
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

	glEnableVertexAttribArray(layoutIndex);
	GL_CHECK()

	glBindBuffer(GL_ARRAY_BUFFER, attribArray.GetAttribBufferID());
	GL_CHECK()

	glVertexAttribPointer(
		layoutIndex,					// Атрибут 0.
		attribArray.GetAttribSize(),	// Размер
		type,							// Тип
		attribArray.IsNormalized(),		// Указывает, что значения не нормализованы
		0,								// Шаг
		(void*)0						// Смещение массива в буфере
	);
	GL_CHECK()

	return true;
}

bool CGraphic::SetTexture(const CTexture& texture, int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	GL_CHECK()

	glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
	GL_CHECK()

	return true;
}

void CGraphic::SetActiveCamera(CCamera* camera)
{
	ActiveCamera = camera;
}

CCamera* CGraphic::GetActiveCamera() const
{
	return ActiveCamera;
}

void CGraphic::AddRenderableObjects(SRenderable* renderable)
{
	RenderableObjects.push_back(renderable);
}

void CGraphic::DrawArrays(int vertexCount)
{
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	GL_CHECK()
}

void CGraphic::DrawIndexedArrays(const CIndexBuffer& indexBuffer)
{
	GLenum type;

	switch (indexBuffer.GetIndexType())
	{
		case CIndexBuffer::IT_UINT:
		{
			type = GL_UNSIGNED_INT;
			break;
		}

		case CIndexBuffer::IT_USHORT:
		{
			type = GL_UNSIGNED_SHORT;
			break;
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.GetIndexBufferID());
	GL_CHECK()

	glDrawElements(
		GL_TRIANGLES,
		indexBuffer.GetIndexCount(),
		type,
		(void*)0
	);
	GL_CHECK()
}

void CGraphic::DrawRenderableObjects()
{
	for (size_t i = 0; i < RenderableObjects.size(); i++)
	{
		SRenderable* r = RenderableObjects[i];

		// Set Shader
		UseShaderProgram(*(r->Shader));

		// Set Textures
		for (size_t texIndex = 0; texIndex < r->Textures.size(); texIndex++)
		{
			SetTexture(*(r->Textures[texIndex].first), texIndex);

			r->Shader->SetUniform(r->Textures[texIndex].second, texIndex);
		}

		// Set Uniforms
		for (size_t uniformIndex = 0; uniformIndex < r->Uniforms.size(); uniformIndex++)
		{
			SRenderable::SUniform* u = r->Uniforms[uniformIndex];

			switch (u->Type)
			{
				case SRenderable::UT_INT:
				{
					r->Shader->SetUniform(u->Name, u->Int);
					break;
				}
				case SRenderable::UT_MAT4:
				{
					r->Shader->SetUniform(u->Name, u->Mat4);
					break;
				}
				case SRenderable::UT_VEC3:
				{
					r->Shader->SetUniform(u->Name, u->Vec3);
					break;
				}
			}
		}

		// Set AttribArrays
		for (size_t layout = 0; layout < r->AttribArrays.size(); layout++)
		{
			SetVertexAttribArray(*(r->AttribArrays[layout]), layout);
		}

		DrawIndexedArrays(*r->IndexBuffer);
	}
}

bool CGraphic::UseShaderProgram(const CShaderProgram& program)
{
	glUseProgram(program.GetProgramID());
	GL_CHECK()

	return true;
}
