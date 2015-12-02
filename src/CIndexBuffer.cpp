#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer()
{
	IndexBufferID = 0;
}

CIndexBuffer::CIndexBuffer(int indexCount, EIndexType indexType)
{
	IndexBufferID = 0;

	Init(indexCount, indexType);
}

CIndexBuffer::~CIndexBuffer()
{
	delete[] (char*)Buffer;
}

bool CIndexBuffer::Init(int indexCount, EIndexType indexType)
{
	IndexCount = indexCount;
	IndexType = indexType;

	if (IndexBufferID == 0)
		glGenBuffers(1, &IndexBufferID);

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

	return true;
}

void* CIndexBuffer::Lock()
{
	return Buffer;
}

bool CIndexBuffer::Unlock()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	GL_CHECK();

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		BufferSize,
		Buffer,
		GL_STATIC_DRAW
	);
	GL_CHECK();

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
