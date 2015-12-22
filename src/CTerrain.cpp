#include <algorithm>

#include "CTerrain.h"

CTerrain::CTerrain(float cellSize, float heightScale,
		const std::string& heightMapFilename,
		const std::string& coeffTexFilename,
		const std::string& megaTexFilename,
		const std::string& vertexShadefFilename,
		const std::string& fragmentShadefFilename)
{
	CellSize = cellSize;
	MaxHeight = 0.0f;

	LoadFromHeightmap(heightMapFilename, heightScale);
	CalculateNormals();

	CoeffTexture.Load(coeffTexFilename);
	MegaTexture.Load(megaTexFilename);

	Shader.LoadShader(vertexShadefFilename, CShaderProgram::ST_VERTEX);
	Shader.LoadShader(fragmentShadefFilename, CShaderProgram::ST_FRAGMENT);

	Shader.LinkShaders();
}

CTerrain::~CTerrain()
{
}

void CTerrain::LoadDiffuseTexture(int index, const std::string& filename)
{
	DiffuseTextures[index].Load(filename);
}

bool CTerrain::GetHeight(const glm::vec3& pos, float& height) const
{
	glm::vec3 origin(pos.x, 0.0f, pos.z);
	glm::vec3 direction(0.0f, 1.0f, 0.0f);
	glm::vec3 result;

	const glm::vec3* vertex = (const glm::vec3*)Vertices.ConstLock();

	int col = int(floorf(pos.x / CellSize));
	int row = int(floorf(pos.z / CellSize));

	if (col > VertexPerRow - 1 || row > VertexPerColumn - 1)
		return false;

	const glm::vec3* v0 = (glm::vec3*)(&(vertex[row * VertexPerRow + col]));
	const glm::vec3* v1 = (glm::vec3*)(&(vertex[row * VertexPerRow + col + 1]));
	const glm::vec3* v2 = (glm::vec3*)(&(vertex[(row + 1) * VertexPerRow + col]));
	const glm::vec3* v3 = (glm::vec3*)(&(vertex[(row + 1) * VertexPerRow + col + 1]));

	if (glm::intersectLineTriangle(origin, direction, *v0, *v1, *v2, result))
	{
		height = result.x; // because glm
	}
	else if (glm::intersectLineTriangle(origin, direction, *v1, *v2, *v3, result))
	{
		height = result.x;
	} // На этом месте Серега хавал слойку с сыром "Хачапури"
	else
	{
		return false;
	}

	return true;
}

bool CTerrain::RayIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3& result) const
{
	direction = glm::normalize(direction);

	direction *= CellSize;
	
	glm::vec3 v0 = origin;
	glm::vec3 v1 = origin + direction;

	float maxX = (VertexPerRow - 1) * CellSize;
	float maxZ = (VertexPerColumn - 1) * CellSize;

	while (true)
	{
		if ((v0.x > 0.0f && v1.x < 0.0f) ||
			(v0.y > 0.0f && v1.y < 0.0f) ||
			(v0.z > 0.0f && v1.z < 0.0f) ||
			(v0.x < maxX && v1.x > maxX) ||
			(v0.y < MaxHeight && v1.y > MaxHeight) ||
			(v0.z < maxZ && v1.z > maxZ))
		{
			return false;
		}

		float h1, h2;

		if (GetHeight(v0, h1) && GetHeight(v1, h2) &&
			v0.y > h1 && v1.y < h2)
		{
			int x0 = int(floorf(v0.x / CellSize)),
				z0 = int(floorf(v0.z / CellSize)),
				x1 = int(floorf(v1.x / CellSize)),
				z1 = int(floorf(v1.z / CellSize));

			if (!IntersectQuad(x0, z0, origin, direction, result))
			{
				if (!IntersectQuad(x1, z1, origin, direction, result))
				{
					if (!IntersectQuad(x0, z1, origin, direction, result))
					{
						IntersectQuad(x1, z0, origin, direction, result);
					}
				}
			} 

			return true;
		}

		v0 = v1;
		v1 += direction;
	}
}

void CTerrain::Draw(CGraphic& graphic)
{
	graphic.UseShaderProgram(Shader);

	graphic.SetTexture(MegaTexture, 0);
	graphic.SetTexture(CoeffTexture, 1);
	graphic.SetTexture(DiffuseTextures[0], 2);
	graphic.SetTexture(DiffuseTextures[1], 3);
	graphic.SetTexture(DiffuseTextures[2], 4);
	graphic.SetTexture(DiffuseTextures[3], 5);

	graphic.SetVertexAttribArray(Vertices, 0);
	graphic.SetVertexAttribArray(UVCoords, 1);
	graphic.SetVertexAttribArray(Normals, 2);

	Shader.SetUniform("megaTexture", 0);
	Shader.SetUniform("coeffTexture", 1);
	Shader.SetUniform("diffuseTexture0", 2);
	Shader.SetUniform("diffuseTexture1", 3);
	Shader.SetUniform("diffuseTexture2", 4);
	Shader.SetUniform("diffuseTexture3", 5);

	Shader.SetUniform("matVP", graphic.GetActiveCamera()->GetViewProjMatrix());

	graphic.DrawIndexedArrays(Indexes);
}

void CTerrain::LoadFromHeightmap(const std::string& filename, float heightScalling)
{
	CTexture heightMap;

	heightMap.Load(filename);

	VertexPerRow = heightMap.GetWidth();
	VertexPerColumn = heightMap.GetHeight();

	CTexture::SPixelGrayscaleU16* pixels;

	pixels = (CTexture::SPixelGrayscaleU16*)heightMap.ConstLock();

	if (!pixels)
		return;

	Vertices.Init(sizeof(glm::vec3) * VertexPerColumn * VertexPerRow,
		3, CVertexAttribArray::AT_FLOAT, false);

	UVCoords.Init(sizeof(glm::vec2) * VertexPerColumn * VertexPerRow,
		2, CVertexAttribArray::AT_FLOAT, false);

	glm::vec3* vertex = (glm::vec3*)Vertices.Lock();
	glm::vec2* uv = (glm::vec2*)UVCoords.Lock();

		for (int i = 0; i < VertexPerColumn; i++)
		{
			for (int j = 0; j < VertexPerRow; j++)
			{
				int n = i * VertexPerRow + j;

				vertex[n].x = j * CellSize;
				vertex[n].z = i * CellSize;
				vertex[n].y = (uint16_t(pixels[n].G >> 8) | uint16_t(pixels[n].G << 8)) * heightScalling;

				MaxHeight = std::max(MaxHeight, vertex[n].y);

				uv[n].x = float(j) / float(VertexPerRow);
				uv[n].y = float(i) / float(VertexPerColumn);
			}
		}

	Vertices.Unlock();
	UVCoords.Unlock();

	Indexes.Init((VertexPerColumn - 1) * (VertexPerRow - 1) * 6, CIndexBuffer::IT_UINT);

	size_t currIndex = 0;
	GLuint* index = (GLuint*)Indexes.Lock();

	for (int z = 0; z < VertexPerColumn - 1; z++)
	{
		for (int x = 0; x < VertexPerRow - 1; x++)
		{
			GLuint v11 = z * VertexPerRow + x;
			GLuint v12 = v11 + 1;
			GLuint v21 = v11 + VertexPerRow;
			GLuint v22 = v21 + 1;

			index[currIndex + 0] = v11;
			index[currIndex + 1] = v21;
			index[currIndex + 2] = v12;

			index[currIndex + 3] = v12;
			index[currIndex + 4] = v21;
			index[currIndex + 5] = v22;

			currIndex += 6;
		}
	}

	Indexes.Unlock();
}

void CTerrain::CalculateNormals()
{
	Normals.Init(sizeof(glm::vec3) * VertexPerColumn * VertexPerRow,
		3, CVertexAttribArray::AT_FLOAT, false);

	glm::vec3* normal = (glm::vec3*)Normals.Lock();
	glm::vec3* vertex = (glm::vec3*)Vertices.ConstLock();

	for (int z = 0; z < VertexPerColumn - 1; z++)
	{
		for (int x = 0; x < VertexPerRow - 1; x++)
		{
			int n = z * VertexPerRow + x;

			GLuint v11 = z * VertexPerRow + x;
			GLuint v12 = v11 + 1;
			GLuint v21 = v11 + VertexPerRow;
			GLuint v22 = v21 + 1;

			glm::vec3 a = vertex[v11];
			glm::vec3 b = vertex[v12];
			glm::vec3 c = vertex[v21];
			glm::vec3 d = vertex[v22];

			normal[n] = glm::abs(
						glm::normalize(
							glm::normalize(glm::cross(c - a, b - a))
							+ glm::normalize(glm::cross(b - d, c - d))
						)
						);

			normal[n] /= (normal[n].x + normal[n].y + normal[n].z);
		}
	}

	Normals.Unlock();
}

bool CTerrain::IntersectQuad(int x, int z, const glm::vec3& origin, const glm::vec3& direction, glm::vec3& result) const
{
	const glm::vec3* vertex = (glm::vec3*)Vertices.ConstLock();

	const glm::vec3* v0 = &vertex[z * VertexPerRow + x];
	const glm::vec3* v1 = &vertex[z * VertexPerRow + x + 1];
	const glm::vec3* v2 = &vertex[(z + 1) * VertexPerRow + x];
	const glm::vec3* v3 = &vertex[(z + 1) * VertexPerRow + x + 1];

	if (glm::intersectRayTriangle(origin, direction, *v0, *v1, *v2, result))
	{
		return true;
	}

	if (glm::intersectRayTriangle(origin, direction, *v1, *v2, *v3, result))
	{
		return true;
	}

	return false;
}
