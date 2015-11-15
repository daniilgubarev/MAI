#pragma once

#include <vector>

#include "GL_common.h"
#include "CGraphic.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	std::vector<glm::vec3>& VertexBufferLock();
	void VertexBufferUnlock();

	std::vector<uint16_t>& IndexBufferLock();
	void IndexBufferUnlock();

	GLuint GetVertexBufferID() const;
	GLuint GetIndexBufferID() const;

private:
	GLuint VertexBufferID;
	GLuint IndexBufferID;

	std::vector<glm::vec3> VertexBuffer;
	std::vector<uint16_t> IndexBuffer;
};