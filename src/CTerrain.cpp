#include "CTerrain.h"

CTerrain::CTerrain(float cellSize, float heightScale,
		const std::string& heightMapFilename,
		const std::string& coeffTexFilename,
		const std::string& megaTexFilename,
		const std::string& vertexShadefFilename,
		const std::string& fragmentShadefFilename)
{
	CellSize = cellSize;

	LoadFromHeightmap(heightMapFilename, heightScale);

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

float CTerrain::GetHeight(float x, float z) const
{
	glm::vec3 orig(x, 0.0f, z);
	glm::vec3 dir(0.0f, 1.0f, 0.0f);
	glm::vec3 result;

	const glm::vec3* vertex = (const glm::vec3*)Vertices.ConstLock();

	int col = int(floorf(x / CellSize));
	int row = int(floorf(z / CellSize));

	const glm::vec3* v0 = (glm::vec3*)(&vertex[row * VertexPerRow + col]);
	const glm::vec3* v1 = (glm::vec3*)(&vertex[row * VertexPerRow + col + 1]);
	const glm::vec3* v2 = (glm::vec3*)(&vertex[(row + 1) * VertexPerRow + col]);
	const glm::vec3* v3 = (glm::vec3*)(&vertex[(row + 1) * VertexPerRow + col + 1]);

	if (glm::intersectLineTriangle(orig, dir, *v0, *v1, *v2, result))
	{
		return result.y;
	}
	else if (glm::intersectLineTriangle(orig, dir, *v1, *v2, *v3, result))
	{
		return result.y;
	} // На этом месте Серега хавал слойку с сыром "Хачапури"

	return 0.0f;
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

	Shader.SetUniformInteger("megaTexture", 0);
	Shader.SetUniformInteger("coeffTexture", 1);
	Shader.SetUniformInteger("diffuseTexture0", 2);
	Shader.SetUniformInteger("diffuseTexture1", 3);
	Shader.SetUniformInteger("diffuseTexture2", 4);
	Shader.SetUniformInteger("diffuseTexture3", 5);

	Shader.SetUniformMatrix("matVP", graphic.GetActiveCamera()->GetViewProjMatrix());

	graphic.DrawIndexedArrays(Indexes);
}

void CTerrain::LoadFromHeightmap(const std::string& filename, float heightScalling)
{
	CTexture heightMap;

	heightMap.Load(filename);

	VertexPerRow = heightMap.GetWidth();
	VertexPerColumn = heightMap.GetHeight();

	CTexture::SPixelGRAYSCALE8* pixels;

	pixels = (CTexture::SPixelGRAYSCALE8*)heightMap.ConstLock();

	if (!pixels)
		return;

	Vertices.Init(sizeof(glm::vec3) * heightMap.GetHeight() * heightMap.GetWidth(),
		3, CVertexAttribArray::AT_FLOAT, false);

	UVCoords.Init(sizeof(glm::vec2) * heightMap.GetHeight() * heightMap.GetWidth(),
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
				vertex[n].y = pixels[n].G * heightScalling;

				/*std::cout << vertex[n].x << " x "
						  << vertex[n].z
						  <<std::endl;*/

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

			/*std::cout
			<< v11 << "\t" << v12 << std::endl
			<< v21 << "\t" << v22 << std::endl 
			<< "--------------"
			<< std::endl;*/

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
}
