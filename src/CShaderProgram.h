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

	void SetUniformMatrix(const std::string& matrixName, const glm::mat4& matrix);
	void SetUniformInteger(const std::string& integerName, int value);

	GLuint GetProgramID() const;

private:
	GLuint ProgramID;
};