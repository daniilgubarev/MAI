#include "CVertexAttribArray.h"

CVertexAttribArray::CVertexAttribArray(int bufferSize, int attribSize, EAttribType attribType, bool normalized) :
	BufferSize(bufferSize),
	AttribSize(attribSize),
	AttribType(attribType),
	Normalized(normalized)
{
	Buffer = new char[BufferSize];

	glGenBuffers(1, &AttribBufferID);
}

CVertexAttribArray::~CVertexAttribArray()
{
	delete[] (char*)Buffer;
}

void* CVertexAttribArray::Lock()
{
	return Buffer;
}

bool CVertexAttribArray::Unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, AttribBufferID);
	
	if (glGetError() != GL_NO_ERROR)
		return false;

	glBufferData(
		GL_ARRAY_BUFFER,
		BufferSize,
		Buffer,
		GL_STATIC_DRAW
	);

	if (glGetError() != GL_NO_ERROR)
		return false;

	return true;
}

GLuint CVertexAttribArray::GetAttribBufferID() const
{
	return AttribBufferID;
}

int CVertexAttribArray::GetBufferSize() const
{
	return BufferSize;
}

int CVertexAttribArray::GetAttribSize() const
{
	return AttribSize;
}

CVertexAttribArray::EAttribType CVertexAttribArray::GetAttribType() const
{
	return AttribType;
}

bool CVertexAttribArray::IsNormalized() const
{
	return Normalized;
}
