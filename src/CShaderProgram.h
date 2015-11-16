#pragma once

#include "GL_common.h"

class CShaderProgram
{
public:
	enum EShaderType
	{
		ST_VERTEX,
		ST_FRAGMENT
	};

	CShaderProgram();
	~CShaderProgram();

	bool LoadShader(const std::string& shaderFilename, EShaderType shaderType);
	bool LinkShaders();

	void SetUniformMatrix(std::string matrixName, const glm::mat4& matrix);

	GLuint GetProgramID() const;

private:
	GLuint ProgramID;
};