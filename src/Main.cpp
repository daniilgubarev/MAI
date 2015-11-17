#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <vector>
#include <algorithm>

#include "GL_common.h"

#include "LoadShader.h"

#include "CGraphic.h"
#include "CVertexAttribArray.h"
#include "CInput.h"
#include "CShaderProgram.h"
#include "CTexture.h"

// Массив из 3 векторов, которые являются вершинами треугольника
static const GLfloat vertexBufferData[] = {
	-1.0f,-1.0f,-1.0f, // Треугольник 1 : начало
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // Треугольник 1 : конец
	1.0f, 1.0f,-1.0f, // Треугольник 2 : начало
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // Треугольник 2 : конец
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

static const GLfloat colorBufferData[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

 static const GLfloat uvBufferData[] = {
	0.000059f, 1.0f-0.000004f,
	0.000103f, 1.0f-0.336048f,
	0.335973f, 1.0f-0.335903f,
	1.000023f, 1.0f-0.000013f,
	0.667979f, 1.0f-0.335851f,
	0.999958f, 1.0f-0.336064f,
	0.667979f, 1.0f-0.335851f,
	0.336024f, 1.0f-0.671877f,
	0.667969f, 1.0f-0.671889f,
	1.000023f, 1.0f-0.000013f,
	0.668104f, 1.0f-0.000013f,
	0.667979f, 1.0f-0.335851f,
	0.000059f, 1.0f-0.000004f,
	0.335973f, 1.0f-0.335903f,
	0.336098f, 1.0f-0.000071f,
	0.667979f, 1.0f-0.335851f,
	0.335973f, 1.0f-0.335903f,
	0.336024f, 1.0f-0.671877f,
	1.000004f, 1.0f-0.671847f,
	0.999958f, 1.0f-0.336064f,
	0.667979f, 1.0f-0.335851f,
	0.668104f, 1.0f-0.000013f,
	0.335973f, 1.0f-0.335903f,
	0.667979f, 1.0f-0.335851f,
	0.335973f, 1.0f-0.335903f,
	0.668104f, 1.0f-0.000013f,
	0.336098f, 1.0f-0.000071f,
	0.000103f, 1.0f-0.336048f,
	0.000004f, 1.0f-0.671870f,
	0.336024f, 1.0f-0.671877f,
	0.000103f, 1.0f-0.336048f,
	0.336024f, 1.0f-0.671877f,
	0.335973f, 1.0f-0.335903f,
	0.667969f, 1.0f-0.671889f,
	1.000004f, 1.0f-0.671847f,
	0.667979f, 1.0f-0.335851f
};

int main(int argc, char *argv[])
{
	CGraphic graphic;

	graphic.Init("OpenGL + SDL", 800, 600, false);

	std::cout << glGetString(GL_VERSION) << std::endl;

	CInput input;
	CShaderProgram shader;

	shader.LoadShader("fx/pos_uv_TO_uv.vert", CShaderProgram::ST_VERTEX);
	shader.LoadShader("fx/uv_TO_color.frag", CShaderProgram::ST_FRAGMENT);

	shader.LinkShaders();

	graphic.UseShaderProgram(shader);

	CVertexAttribArray vertexBuffer(sizeof(glm::vec3) * 12 * 3, 3, CVertexAttribArray::AT_FLOAT, false);
	//CVertexAttribArray colorBuffer(sizeof(glm::vec3) * 12 * 3, 3, CVertexAttribArray::AT_FLOAT, false);
	CVertexAttribArray uvBuffer(sizeof(glm::vec2) * 12 * 3, 2, CVertexAttribArray::AT_FLOAT, false);

 	// fill vertices
 	char* bufferData = (char*)vertexBuffer.Lock();

 	memcpy(bufferData, vertexBufferData, sizeof(vertexBufferData));

	vertexBuffer.Unlock();

 	// fill colors
	/*bufferData = (char*)colorBuffer.Lock();

	memcpy(bufferData, colorBufferData, sizeof(colorBufferData));

 	colorBuffer.Unlock();*/

 	bufferData = (char*)uvBuffer.Lock();

	memcpy(bufferData, uvBufferData, sizeof(uvBufferData));

 	uvBuffer.Unlock();

 	CTexture texture;

 	if (!texture.Load("img/image.bmp"))
 		std::cerr << "fail load img/image.bmp" << std::endl;

 	graphic.SetTexture(texture, 0);

	// Setting matrix
	glm::mat4 matProjection = glm::perspective(
		float(M_PI) / 4.0f,
		static_cast<float>(400) / static_cast<float>(300),
		0.1f,
		100.0f);

	glm::vec3 cameraPosition = -glm::vec3(3.0f, 4.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 matView = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 matModel = glm::mat4(1.0f);
	glm::mat4 matMVP = matProjection * matView * matModel;

	shader.SetUniformMatrix("MVP", matMVP);
	shader.SetUniformInteger("textureSampler", 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	graphic.SetVertexAttribArray(vertexBuffer, 0);
	graphic.SetVertexAttribArray(uvBuffer, 1);

	// Вывести треугольник!
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3); // Начиная с вершины 0, всего 3 вершины -> один треугольник

	graphic.SwapBuffres();

	while (input.Update() &&
		!input.IsKeyPressed(SDL_SCANCODE_SPACE) &&
		!input.IsKeyPressed(SDL_SCANCODE_RETURN))
		SDL_Delay(1);

	SDL_Quit();

	return 0;
}
