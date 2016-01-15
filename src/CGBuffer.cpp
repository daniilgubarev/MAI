#include <iostream>

#include "CGBuffer.h"

CGBuffer::CGBuffer(uint32_t width, uint32_t height)
{
	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	// - Position color buffer
	glGenTextures(1, &TexPosition);
	glBindTexture(GL_TEXTURE_2D, TexPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexPosition, 0);

	// - Normal color buffer
	glGenTextures(1, &TexNormal);
	glBindTexture(GL_TEXTURE_2D, TexNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, TexNormal, 0);

	// - Color + Specular color buffer
	glGenTextures(1, &TexColor);
	glBindTexture(GL_TEXTURE_2D, TexColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, TexColor, 0);

	// - Depth
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glDrawBuffers(3, attachments);

	GL_CHECK()
}

CGBuffer::~CGBuffer()
{

}

void CGBuffer::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	GL_CHECK()
}

void CGBuffer::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK()
}

void CGBuffer::BindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexPosition);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TexNormal);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TexColor);

	GL_CHECK()
}
