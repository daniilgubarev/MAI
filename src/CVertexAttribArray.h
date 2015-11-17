#pragma once

#include "GL_common.h"

class CVertexAttribArray
{
public:
	enum EAttribType
	{
		AT_FLOAT,
		AT_UNSIGNED_BYTE
	};

	CVertexAttribArray(int bufferSize, int attribSize, EAttribType attribType, bool normalized);
	~CVertexAttribArray();

	void* Lock();
	bool Unlock();

	GLuint GetAttribBufferID() const;
	int GetBufferSize() const;
	int GetAttribSize() const;
	EAttribType GetAttribType() const;
	bool IsNormalized() const;

private:
	GLuint AttribBufferID;
	
	void* Buffer;
	
	int BufferSize;
	int AttribSize;
	EAttribType AttribType;
	bool Normalized;
};