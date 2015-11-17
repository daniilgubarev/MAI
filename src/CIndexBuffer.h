#pragma once

#include "GL_common.h"

class CIndexBuffer
{
public:
	enum EIndexType
	{
		IT_UINT,
		IT_USHORT
	};

	CIndexBuffer(int indexCount, EIndexType indexType);
	~CIndexBuffer();

	void* Lock();
	bool Unlock();

	GLuint GetIndexBufferID() const;
	int GetIndexCount() const;
	EIndexType GetIndexType() const;

private:
	GLuint IndexBufferID;
	
	void* Buffer;
	
	int BufferSize;
	int IndexCount;
	EIndexType IndexType;
};