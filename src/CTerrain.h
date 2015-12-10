#include <string>

#include "GL_common.h"

#include "CGraphic.h"
#include "CVertexAttribArray.h"
#include "CIndexBuffer.h"
#include "CTexture.h"
#include "CShaderProgram.h"

class CTerrain
{
public:
	CTerrain(float cellSize, float heightScale,
		const std::string& heightMapFilename,
		const std::string& coeffTexFilename,
		const std::string& megaTexFilename,
		const std::string& vertexShadefFilename,
		const std::string& fragmentShadefFilename);
	~CTerrain();

	void LoadDiffuseTexture(int index, const std::string& filename);

	bool GetHeight(const glm::vec3& pos, float& height) const;
	bool RayIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3& result) const;

	void Draw(CGraphic& graphic);

private:
	void LoadFromHeightmap(const std::string& filename, float heightScalling);
	void CalculateNormals();

	bool IntersectQuad(int x, int z, const glm::vec3& origin, const glm::vec3& direction, glm::vec3& result) const;

	CVertexAttribArray Vertices;
	CVertexAttribArray Normals;
	CVertexAttribArray UVCoords;

	CIndexBuffer Indexes;

	CTexture CoeffTexture;
	CTexture MegaTexture;
	CTexture DiffuseTextures[4];

	CShaderProgram Shader;

	int VertexPerColumn;
	int VertexPerRow;

	float CellSize;
	float MaxHeight;
};
