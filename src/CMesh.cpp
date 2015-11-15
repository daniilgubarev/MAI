#include "CMesh.h"

CMesh::CMesh()
{
	VertexBufferID = 0;
	IndexBufferID  = 0;
}

CMesh::~CMesh()
{}

std::vector<glm::vec3>& CMesh::VertexBufferLock()
{
	return VertexBuffer;
}

void CMesh::VertexBufferUnlock()
{
	if (!VertexBufferID)
		glGenBuffers(1, &VertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(decltype(VertexBuffer)::value_type) * VertexBuffer.size(),
		&VertexBuffer[0],
		GL_STATIC_DRAW
		);
}

std::vector<uint16_t>& CMesh::IndexBufferLock()
{
	return IndexBuffer;
}

void CMesh::IndexBufferUnlock()
{
	if (!IndexBufferID)
		glGenBuffers(1, &IndexBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(decltype(IndexBuffer)::value_type) * IndexBuffer.size(),
		&IndexBuffer[0],
		GL_STATIC_DRAW
		);
}
