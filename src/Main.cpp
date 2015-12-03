#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <vector>
#include <algorithm>

#include "GL_common.h"

#include "CGraphic.h"
#include "CVertexAttribArray.h"
#include "CInput.h"
#include "CShaderProgram.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CTerrain.h"

// Массив из 3 векторов, которые являются вершинами треугольника
static const GLfloat vertexBufferData[] = {
	// front
	-1.0, -1.0,  1.0,
	1.0, -1.0,  1.0,
	1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// back
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0
};

static const GLfloat colorBufferData[] = {
	// front colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};

 static const GLfloat uvBufferData[] = {
	0.000059f, 1.0f-0.000004f,
	0.000103f, 1.0f-0.336048f,
	0.335973f, 1.0f-0.335903f,
	1.000023f, 1.0f-0.000013f,
	0.667979f, 1.0f-0.335851f,
	0.999958f, 1.0f-0.336064f,
	0.667979f, 1.0f-0.335851f,
	0.336024f, 1.0f-0.671877f
};

 static const uint32_t cubeIndexes[] = {
	 // front
	 0, 1, 2,
	 2, 3, 0,
	 // top
	 1, 5, 6,
	 6, 2, 1,
	 // back
	 7, 6, 5,
	 5, 4, 7,
	 // bottom
	 4, 0, 3,
	 3, 7, 4,
	 // left
	 4, 5, 1,
	 1, 0, 4,
	 // right
	 3, 2, 6,
	 6, 7, 3,
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

	CVertexAttribArray vertexBuffer(sizeof(glm::vec3) * 8, 3, CVertexAttribArray::AT_FLOAT, false);
	CVertexAttribArray uvBuffer(sizeof(glm::vec2) * 8, 2, CVertexAttribArray::AT_FLOAT, false);
	CIndexBuffer indexes(6 * 2 * 3, CIndexBuffer::IT_UINT);

 	// fill vertices
	char* bufferData = (char*)vertexBuffer.Lock();

		memcpy(bufferData, vertexBufferData, sizeof(vertexBufferData));

	vertexBuffer.Unlock();

 	bufferData = (char*)uvBuffer.Lock();

		memcpy(bufferData, uvBufferData, sizeof(uvBufferData));

	uvBuffer.Unlock();

	bufferData = (char*)indexes.Lock();

		memcpy(bufferData, cubeIndexes, sizeof(cubeIndexes));

	indexes.Unlock();

	CTerrain terrain(
		1.0f,
		0.5f,
		"img/heightMap.png",
		"img/coefTex.mtf",
		"img/megaTex.png",
		"fx/terrain.vert",
		"fx/terrain.frag"
	);

	terrain.LoadDiffuseTexture(0, "img/snow0.png");
	terrain.LoadDiffuseTexture(1, "img/rock0.png");
	terrain.LoadDiffuseTexture(2, "img/grass1.png");
	terrain.LoadDiffuseTexture(3, "img/rock0.png");

	CTexture texture;

	if (!texture.Load("img/image.bmp"))
 		std::cerr << "fail load img/image.bmp" << std::endl;

	glm::vec3 cameraPosition = glm::vec3(1024.0f, 200.0f, 1024.0f);
	glm::vec3 cameraTarget = glm::vec3(512.0f, 0.0f, 512.0f);

	CCamera camera(
		cameraPosition,
		cameraTarget,
		800, 600,
		0.1, 100000.0,
		M_PI / 4
	);

	graphic.SetActiveCamera(&camera);

	while (input.Update() &&
		!input.IsKeyPressed(SDL_SCANCODE_SPACE) &&
		!input.IsKeyPressed(SDL_SCANCODE_RETURN))
	{
		glm::mat4 matView = camera.GetViewMatrix();
		glm::vec3 right(matView[0][0], matView[1][0], matView[2][0]);
		glm::vec3 up   (matView[0][1], matView[1][1], matView[2][1]);
		glm::vec3 front(matView[0][2], matView[1][2], matView[2][2]);

		if (input.IsKeyPressed(SDL_SCANCODE_LSHIFT))
			cameraPosition += up * 0.01f * cameraPosition.y;
		if (input.IsKeyPressed(SDL_SCANCODE_LCTRL))
			cameraPosition -= up * 0.01f * cameraPosition.y;
		if (input.IsKeyPressed(SDL_SCANCODE_UP))
			cameraPosition -= front * 0.01f * cameraPosition.y;
		if (input.IsKeyPressed(SDL_SCANCODE_DOWN))
			cameraPosition += front * 0.01f * cameraPosition.y;
		if (input.IsKeyPressed(SDL_SCANCODE_LEFT))
			cameraPosition += right * 0.01f * cameraPosition.y;
		if (input.IsKeyPressed(SDL_SCANCODE_RIGHT))
			cameraPosition -= right * 0.01f * cameraPosition.y;

		camera.SetPosition(cameraPosition);
		camera.LookAt(cameraTarget);

		graphic.Clear();

		// Draw Cube
		/*graphic.UseShaderProgram(shader);

		graphic.SetVertexAttribArray(vertexBuffer, 0);
		graphic.SetVertexAttribArray(uvBuffer, 1);

		graphic.SetTexture(texture, 0);

		shader.SetUniformInteger("textureSampler", 0);
		shader.SetUniformMatrix("MVP", camera.GetViewProjMatrix());

		//graphic.DrawArrays(6 * 2 * 3); // Начиная с вершины 0, всего 3 вершины -> один треугольник
		graphic.DrawIndexedArrays(indexes);*/
		// ***********/

		// Draw Terrain
		terrain.Draw(graphic);
		// **********

		graphic.SwapBuffres();
	}

	SDL_Quit();

	return 0;
}
