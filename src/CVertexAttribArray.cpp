#include "CVertexAttribArray.h"

CVertexAttribArray::CVertexAttribArray()
{
	Buffer = nullptr;
	AttribBufferID = 0;
}

CVertexAttribArray::CVertexAttribArray(int bufferSize, int attribSize, EAttribType attribType, bool normalized)
{
	Init(bufferSize, attribSize, attribType, normalized);

	glGenBuffers(1, &AttribBufferID);
}

CVertexAttribArray::~CVertexAttribArray()
{
	if (Buffer)
		delete[] (char*)Buffer;

	if (AttribBufferID)
		glDeleteBuffers(1, &AttribBufferID);
}

bool CVertexAttribArray::Init(int bufferSize, int attribSize, EAttribType attribType, bool normalized)
{
	BufferSize = bufferSize;
	AttribSize = attribSize;
	AttribType = attribType;
	Normalized = normalized;

	Buffer = new char[BufferSize];
	IsLocked = false;

	glGenBuffers(1, &AttribBufferID);
	GL_CHECK();

	return true;
}

const void* CVertexAttribArray::ConstLock() const
{
	return Buffer;
}

void* CVertexAttribArray::Lock()
{
	IsLocked = true;
	return Buffer;
}

bool CVertexAttribArray::Unlock()
{
	if (!IsLocked)
		return false;

	glBindBuffer(GL_ARRAY_BUFFER, AttribBufferID);

	glBufferData(
		GL_ARRAY_BUFFER,
		BufferSize,
		Buffer,
		GL_STATIC_DRAW
	);
	GL_CHECK();

	IsLocked = false;

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
