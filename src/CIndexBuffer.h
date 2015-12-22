#pragma once

#include "CommonHeaders.h"

class CIndexBuffer
{
public:
	enum EIndexType
	{
		IT_UINT,
		IT_USHORT
	};

	CIndexBuffer();
	CIndexBuffer(int indexCount, EIndexType indexType);
	~CIndexBuffer();

	bool Init(int indexCount, EIndexType indexType);

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
