#pragma once

#include "GL_common.h"

class CTexture
{
public:
	enum EPixelFormat
	{
		PF_RGB,
		PF_BGR
	};

	CTexture();
	~CTexture();

	bool Load(const std::string& filename);

	GLuint GetTextureID() const;

private:
	bool LoadFromFileSDL(const std::string& filename);
	bool LoadToOpenGL();

	GLuint TextureID;

	void* PixelData;

	EPixelFormat PixelFormat;
	int Width;
	int Height;
};