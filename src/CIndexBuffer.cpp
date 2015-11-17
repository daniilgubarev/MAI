#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer(int indexCount, EIndexType indexType) :
	IndexCount(indexCount),
	IndexType(indexType)
{
	switch (IndexType)
	{
		case IT_UINT:
		{
			BufferSize = IndexCount * sizeof(GLuint);
			break;
		}

		case IT_USHORT:
		{
			BufferSize = IndexCount * sizeof(GLushort);
			break;
		}
	}

	Buffer = new char[BufferSize];

	glGenBuffers(1, &IndexBufferID);
}

CIndexBuffer::~CIndexBuffer()
{
	delete[] (char*)Buffer;
}

void* CIndexBuffer::Lock()
{
	return Buffer;
}

bool CIndexBuffer::Unlock()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	
	if (glGetError() != GL_NO_ERROR)
		return false;

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		BufferSize,
		Buffer,
		GL_STATIC_DRAW
	);

	if (glGetError() != GL_NO_ERROR)
		return false;

	return true;
}

GLuint CIndexBuffer::GetIndexBufferID() const
{
	return IndexBufferID;
}

int CIndexBuffer::GetIndexCount() const
{
	return IndexCount;
}

CIndexBuffer::EIndexType CIndexBuffer::GetIndexType() const
{
	return IndexType;
}
