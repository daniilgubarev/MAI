#include <string>
#include <vector>

#include "CommonHeaders.h"

#include "CVertexAttribArray.h"
#include "CIndexBuffer.h"
#include "CShaderProgram.h"
#include "CTexture.h"
#include "CTransform.h"

struct SRenderable
{
	enum EUniformType
	{
		UT_MAT4,
		UT_VEC3,
		UT_INT
	};

	struct SUniform
	{
		std::string Name;
		EUniformType Type;

		glm::mat4 Mat4;
		glm::vec3 Vec3;
		int Int;
	};

	CTransform Transform;

	std::vector<CVertexAttribArray*> AttribArrays;
	CIndexBuffer* IndexBuffer;

	CShaderProgram* Shader;

	std::vector<std::pair<CTexture*, std::string>> Textures;
	std::vector<SUniform*> Uniforms;
};
