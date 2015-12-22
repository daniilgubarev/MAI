#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <vector>
#include <algorithm>

#include "CommonHeaders.h"

#include "CGraphic.h"
#include "CVertexAttribArray.h"
#include "CInput.h"
#include "CShaderProgram.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CTransform.h"
#include "CStateManager.h"
#include "CCameraManager.h"
#include "CCubeRenderer.h"

int main(int argc, char *argv[])
{
	CGraphic graphic;

	graphic.Init("OpenGL + SDL", 800, 600, false);

	std::cout << glGetString(GL_VERSION) << std::endl;

	CInput input;

	CTerrain terrain(
		1.0f,
		0.003f,
		"img/heightMap.mtf",
		"img/coefTex.mtf",
		"img/megaTex.png",
		"fx/terrain.vert",
		"fx/terrain.frag"
	);

	terrain.LoadDiffuseTexture(0, "img/snow0.png");
	terrain.LoadDiffuseTexture(1, "img/rock0.png");
	terrain.LoadDiffuseTexture(2, "img/grass1.png");
	terrain.LoadDiffuseTexture(3, "img/rock1.png");

	glm::vec3 cameraPosition = glm::vec3(-10.0f, 5.0f, -10.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	CCamera camera(
		cameraPosition,
		cameraTarget,
		800, 600,
		0.1, 100000.0,
		M_PI / 4
	);

	graphic.SetActiveCamera(&camera);

	CStateManager stateManager;

	CGameState* cubeRend = new CCubeRenderer(&graphic, &terrain);

	stateManager.AddState(new CCameraManager(&camera, &input, &terrain));
	stateManager.AddState(cubeRend);

	while (input.Update() &&
		!input.IsKeyPressed(SDL_SCANCODE_SPACE) &&
		!input.IsKeyPressed(SDL_SCANCODE_RETURN))
	{

		// Draw Cube
		/*graphic.UseShaderProgram(shader);

		graphic.SetVertexAttribArray(vertexBuffer, 0);
		graphic.SetVertexAttribArray(uvBuffer, 1);

		graphic.SetTexture(texture, 0);*/

		//cubeTransform.AddRotation({1.0f, 1.0f, 1.0f}, 0.01f);

		//shader.SetUniform("textureSampler", 0);

		stateManager.UpdateAll(1.0f);


		graphic.Clear();
		stateManager.RenderAll(1.0f);
		graphic.DrawRenderableObjects();

		//graphic.DrawArrays(6 * 2 * 3); // Начиная с вершины 0, всего 3 вершины -> один треугольник
		//graphic.DrawIndexedArrays(indexes);

		// ***********/
		// Draw Terrain
		terrain.Draw(graphic);
		// **********

		graphic.SwapBuffres();
	}

	

	SDL_Quit();

	return 0;
}
