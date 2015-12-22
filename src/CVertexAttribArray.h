#pragma once

#include "CommonHeaders.h"

class CVertexAttribArray
{
public:
	enum EAttribType
	{
		AT_FLOAT,
		AT_UNSIGNED_BYTE
	};

	CVertexAttribArray();
	CVertexAttribArray(int bufferSize, int attribSize, EAttribType attribType, bool normalized);
	~CVertexAttribArray();

	bool Init(int bufferSize, int attribSize, EAttribType attribType, bool normalized);

	const void* ConstLock() const;
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
	bool IsLocked;
	
	int BufferSize;
	int AttribSize;
	EAttribType AttribType;
	bool Normalized;
};
