#pragma once

#include "CommonHeaders.h"

class CGBuffer
{
public:
	CGBuffer(uint32_t width, uint32_t height);
	~CGBuffer();

	void BindFramebuffer();
	void UnbindFramebuffer();
	void BindTextures();

private:
	GLuint FrameBuffer;

	GLuint TexPosition;
	GLuint TexNormal;
	GLuint TexColor;
	GLuint TexDepth;
};
