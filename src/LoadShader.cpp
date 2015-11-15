#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL/glew.h>

GLuint LoadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    // Создаем шейдеры
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Загружаем код Вершинного Шейдера из файла
	std::string vertexShaderCode;
	std::ifstream vertexShaderFile(vertexShaderFilename.c_str(), std::ios::in);
	
	if (vertexShaderFile.is_open())
	{
		std::string line = "";
		
		while (getline(vertexShaderFile, line))
		{
			vertexShaderCode += "\n";
			vertexShaderCode += line;
		}

		vertexShaderFile.close();
	}
	else
	{
		std::cout << "Не удалось открыть " << vertexShaderFilename << std::endl;
	}

    // Загружаем код Фрагментного шейдера из файла
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderFile(fragmentShaderFilename.c_str(), std::ios::in);
	
	if (fragmentShaderFile.is_open())
	{
		std::string line = "";

		while(getline(fragmentShaderFile, line))
		{
			fragmentShaderCode += "\n";
			fragmentShaderCode += line;
		}

		fragmentShaderFile.close();
	}
	else
	{
		std::cout << "Не удалось открыть " << fragmentShaderFilename << std::endl;
	}

	GLint Result = GL_FALSE;
	int compileLogLength;

    // Компилируем Вершинный шейдер
	std::cout << "Компиляция шейдера : " << vertexShaderFilename << std::endl;

	const char* vertexSourcePointer = vertexShaderCode.c_str();

	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

    // Выполняем проверку Вершинного шейдера
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);

	if (compileLogLength > 1)
	{
		std::vector<char> VertexShaderErrorMessage(compileLogLength);

		glGetShaderInfoLog(vertexShaderID, compileLogLength, NULL, &VertexShaderErrorMessage[0]);

		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}

    // Компилируем Фрагментный шейдер
	std::cout << "Компиляция шейдера : " << fragmentShaderFilename << std::endl;

	const char* fragmentSourcePointer = fragmentShaderCode.c_str();

	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

    // Проверяем Фрагментный шейдер
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &compileLogLength);

	if (compileLogLength > 1)
	{
		std::vector<char> FSErrorMesage(compileLogLength);

		glGetShaderInfoLog(fragmentShaderID, compileLogLength, NULL, &FSErrorMesage[0]);

		std::cout << &FSErrorMesage[0] << std::endl;
	}

    // Создаем шейдерную программу и привязываем шейдеры к ней
	std::cout << "Создаем шейдерную программу и привязываем шейдеры к ней" << std::endl;

	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);

	glLinkProgram(ProgramID);

    // Проверяем шейдерную программу
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &compileLogLength);

	if (compileLogLength > 1)
	{
		std::vector<char> ProgramErrorMessage(std::max(compileLogLength, int(1)));

		glGetProgramInfoLog(ProgramID, compileLogLength, NULL, &ProgramErrorMessage[0]);

		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return ProgramID;
}
