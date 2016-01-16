#pragma once

#include "CommonHeaders.h"

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

	void SetUniform(const std::string& matrixName, const glm::mat4& matrix);
	void SetUniform(const std::string& vectorName, const glm::vec3& vector);
	void SetUniform(const std::string& integerName, int value);
	void SetUniform(const std::string& floatName, float value);

	GLuint GetProgramID() const;

private:
	GLuint ProgramID;
};
