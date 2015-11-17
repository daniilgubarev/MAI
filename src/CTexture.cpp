#include <iostream>
#include <cstring>

#include "GL_common.h"

#include "CTexture.h"

CTexture::CTexture()
{
	PixelData = nullptr;
	Width = 0;
	Height = 0;
}

CTexture::~CTexture()
{
	if (PixelData)
		delete[] (char*)PixelData;
}

bool CTexture::Load(const std::string& filename)
{
	if (!LoadFromFileSDL(filename))
		return false;

	if (!LoadToOpenGL())
		return false;

	return true;
}

GLuint CTexture::GetTextureID() const
{
	return TextureID;
}

bool CTexture::LoadFromFileSDL(const std::string& filename)
{
	SDL_Surface* surface = nullptr;

	surface = SDL_LoadBMP(filename.c_str());

	if (!surface)
		return false;

	Width  = surface->w;
	Height = surface->h;

	switch (surface->format->format)
	{
		case SDL_PIXELFORMAT_RGB888:
		case SDL_PIXELFORMAT_RGB24:
		{
			PixelFormat = PF_RGB;
			break;
		}

		case SDL_PIXELFORMAT_BGR888:
		case SDL_PIXELFORMAT_BGR24:
		{
			PixelFormat = PF_BGR;
			break;
		}
	}

	int pixelDataSize = surface->pitch * surface->h;

	PixelData = new char[pixelDataSize];

	memcpy(PixelData, surface->pixels, pixelDataSize);

	SDL_FreeSurface(surface);

	return true;
}

bool CTexture::LoadToOpenGL()
{
	GLenum format;

	switch (PixelFormat)
	{
		case PF_RGB:
		{
			format = GL_RGB;
			break;
		}

		case PF_BGR:
		{
			format = GL_BGR;
			break;
		}
	}

	glGenTextures(1, &TextureID);
	
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, format, GL_UNSIGNED_BYTE, PixelData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glGetError() != GL_NO_ERROR)
		return false;

	return true;
}
