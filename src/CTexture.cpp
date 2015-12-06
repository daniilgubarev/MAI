#include <iostream>
#include <fstream>
#include <cstring>

#include "GL_common.h"

#include "SDL2/SDL_image.h"

#include "CTexture.h"

CTexture::CTexture()
{
	TextureID = 0;
	PixelData = nullptr;
	Width = 0;
	Height = 0;
}

CTexture::~CTexture()
{
	if (PixelData)
		delete[] (char*)PixelData;

	glDeleteTextures(1, &TextureID);
}

bool CTexture::Load(const std::string& filename)
{
	if (filename.substr(filename.size() - 4) == ".mtf")
	{
		if (!LoadFromFileMTF(filename))
			return false;
	}
	else
	{
		if (!LoadFromFileSDL(filename))
			return false;
	}

	if (!LoadToOpenGL())
		return false;

	return true;
}

uint32_t CTexture::GetWidth() const
{
	return Width;
}

uint32_t CTexture::GetHeight() const
{
	return Height;
}

CTexture::EPixelFormat CTexture::GetPixelFormat() const
{
	return PixelFormat;
}

int CTexture::GetPixelSize() const
{
	switch (PixelFormat)
	{
		case PF_BGR:
		case PF_RGB:
			return 3; break;

		case PF_BGRA:
		case PF_RGBA:
		case PF_ABGR:
		case PF_ARGB:
		case PF_GRAYSCALE32:
			return 4; break;

		case PF_GRAYSCALE8:
			return 1; break;

		case PF_GRAYSCALE16:
			return 2; break;

		default:
			return 0; break;
	}
}

const void* CTexture::ConstLock() const
{
	return PixelData;
}

void* CTexture::Lock()
{
	return PixelData;
}

bool CTexture::Unlock()
{
	return LoadToOpenGL();
}

GLuint CTexture::GetTextureID() const
{
	return TextureID;
}

bool CTexture::LoadFromFileSDL(const std::string& filename)
{
	SDL_Surface* surface = nullptr;

	surface = IMG_Load(filename.c_str());

	if (!surface)
		return false;

	Width  = surface->w;
	Height = surface->h;

	switch (surface->format->format)
	{
		case SDL_PIXELFORMAT_RGB888:
		case SDL_PIXELFORMAT_RGB24:
			PixelFormat = PF_RGB; break;

		case SDL_PIXELFORMAT_BGR888:
		case SDL_PIXELFORMAT_BGR24:
			PixelFormat = PF_BGR; break;

		case SDL_PIXELFORMAT_RGBA8888:
			PixelFormat = PF_RGBA; break;

		case SDL_PIXELFORMAT_ARGB8888:
			PixelFormat = PF_ARGB; break;

		case SDL_PIXELFORMAT_BGRA8888:
			PixelFormat = PF_BGRA; break;

		case SDL_PIXELFORMAT_ABGR8888:
			PixelFormat = PF_ABGR; break;

		case SDL_PIXELFORMAT_INDEX8:
			PixelFormat = PF_GRAYSCALE8; break;

		default:
		{
			std::cout << "Unknown pixelformat : "
					  << SDL_GetPixelFormatName(surface->format->format)
					  << std::endl;
			return false;
		}
	}

	int pixelDataSize = surface->pitch * surface->h;

	PixelData = new char[pixelDataSize];

	memcpy(PixelData, surface->pixels, pixelDataSize);

	SDL_FreeSurface(surface);

	return true;
}

bool CTexture::LoadFromFileMTF(const std::string& filename)
{
	std::ifstream file;

	file.open(filename.c_str(), std::ios_base::binary | std::ios_base::in);

	if (!file.is_open())
		return false;

	file.read((char*)&Width, sizeof(Width));
	file.read((char*)&Height, sizeof(Height));
	file.read((char*)&PixelFormat, sizeof(EPixelFormat));

	int pixelDataSize = Width * Height * GetPixelSize();

	PixelData = new char[pixelDataSize];

	file.read((char*)PixelData, pixelDataSize);

	return true;
}

bool CTexture::LoadToOpenGL()
{
	GLint internalFormat;
	GLenum format;

	switch (PixelFormat)
	{
		case PF_RGB:	internalFormat = GL_RGB;	format = GL_RGB; break;
		case PF_BGR:	internalFormat = GL_RGB;	format = GL_BGR; break;
		case PF_RGBA:
		case PF_ARGB:	internalFormat = GL_RGBA;	format = GL_RGBA; break;
		case PF_BGRA:
		case PF_ABGR:	internalFormat = GL_RGBA;	format = GL_BGRA; break;

		case PF_GRAYSCALE8:
		case PF_GRAYSCALE16:
		case PF_GRAYSCALE32:
		{
			std::cout << "PF_GRAYSCALE texture not be loaded to OpenGL"
					  << std::endl;

			return true;
		}

		default: return false;
	}

	if (TextureID == 0)
		glGenTextures(1, &TextureID);
	
	glBindTexture(GL_TEXTURE_2D, TextureID);
	GL_CHECK()

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, Width, Height, 0, format, GL_UNSIGNED_BYTE, PixelData);
	GL_CHECK()

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	GL_CHECK()

	return true;
}
