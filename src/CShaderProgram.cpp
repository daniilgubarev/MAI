#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "CShaderProgram.h"

CShaderProgram::CShaderProgram()
{
	ProgramID = glCreateProgram();
}

CShaderProgram::~CShaderProgram()
{}

bool CShaderProgram::LoadShader(const std::string& shaderFilename, EShaderType shaderType)
{
	GLuint shaderID;

	switch (shaderType)
	{
		case ST_VERTEX:
		{
			shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		}

		case ST_FRAGMENT:
		{
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}
	}

	std::string shaderCode;
	std::ifstream shaderFile(shaderFilename.c_str(), std::ios::in);

	if (shaderFile.is_open())
	{
		std::string line = "";

		while (getline(shaderFile, line))
		{
			shaderCode += "\n";
			shaderCode += line;
		}

		shaderFile.close();
	}
	else
	{
		std::cout << "Не удалось открыть " << shaderFilename << std::endl;

		return false;
	}

	GLint compileResult = GL_FALSE;
	int compileLogLength;

	std::cout << "Компиляция шейдера : " << shaderFilename << std::endl;

	const char* shaderSourcePointer = shaderCode.c_str();

	glShaderSource(shaderID, 1, &shaderSourcePointer , NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &compileLogLength);

	if (compileLogLength > 1)
	{
		std::vector<char> shaderErrorMessage(compileLogLength);

		glGetShaderInfoLog(shaderID, compileLogLength, NULL, &shaderErrorMessage[0]);

		std::cout << &shaderErrorMessage[0] << std::endl;

		return false;
	}

	glAttachShader(ProgramID, shaderID);

	/*
		"If a shader object is deleted while it is attached
		to a program object, it will be flagged for deletion,
		and deletion will not occur until glDetachShader is
		called to detach it from all program objects to which
		it is attached."
	*/
	glDeleteShader(shaderID);

	return true;
}

bool CShaderProgram::LinkShaders()
{
	GLint linkResult;
	int linkLogLength;

	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &linkResult);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &linkLogLength);

	if (linkLogLength > 1)
	{
		std::vector<char> linkErrorMessage(std::max(linkLogLength, int(1)));

		glGetProgramInfoLog(ProgramID, linkLogLength, NULL, &linkErrorMessage[0]);

		std::cout << &linkErrorMessage[0] << std::endl;

		return false;
	}

	return true;
}

void CShaderProgram::SetUniformMatrix(const std::string& matrixName, const glm::mat4& matrix)
{
	GLuint shaderMatrixID = glGetUniformLocation(ProgramID, "MVP");
	GL_CHECK();

	glUniformMatrix4fv(shaderMatrixID, 1, GL_FALSE, &matrix[0][0]);
	GL_CHECK();
}

void CShaderProgram::SetUniformInteger(const std::string& integerName, int value)
{
	GLuint shaderIntegerID = glGetUniformLocation(ProgramID, integerName.c_str());
	GL_CHECK();

	glUniform1i(shaderIntegerID, value);
	GL_CHECK();
}

GLuint CShaderProgram::GetProgramID() const
{
	return ProgramID;
}
