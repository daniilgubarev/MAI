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
#include "CGBuffer.h"

int main(int argc, char *argv[])
{
	CGraphic graphic;

	const int screenWidth = 800;
	const int screenHeight = 600;

	graphic.Init("OpenGL + SDL", screenWidth, screenHeight, false);

	std::cout << glGetString(GL_VERSION) << std::endl;

	CInput input;

	CTerrain terrain(
		1.0f,
		0.003f,
		"img/heightMap.mtf",
		"img/coefTex.mtf",
		"img/megaTex.png",
		"fx/terrain.vert",
		"fx/terrain_gbuf.frag"
	);

	terrain.LoadDiffuseTexture(0, "img/snow0.png");
	terrain.LoadDiffuseTexture(1, "img/rock0.png");
	terrain.LoadDiffuseTexture(2, "img/grass1.png");
	terrain.LoadDiffuseTexture(3, "img/rock1.png");

	glm::vec3 cameraPosition = glm::vec3(10.0f, 400.0f, 10.0f);
	glm::vec3 cameraTarget = glm::vec3(200.0f, 200.0f, 200.0f);

	CCamera camera(
		cameraPosition,
		cameraTarget,
		screenWidth, screenHeight,
		0.1, 100000.0,
		M_PI / 4
	);

	graphic.SetActiveCamera(&camera);

	CStateManager stateManager;

	stateManager.AddState(new CCameraManager(&camera, &input, &terrain));
	//stateManager.AddState(new CCubeRenderer(&graphic, &terrain));

	CGBuffer gBuffer(screenWidth, screenHeight);

	CShaderProgram defferRenderShader;
	CShaderProgram defferRenderLightShader;

	defferRenderShader.LoadShader("fx/screen_quad.vert", CShaderProgram::ST_VERTEX);
	defferRenderShader.LoadShader("fx/gbuffer_color.frag", CShaderProgram::ST_FRAGMENT);

	defferRenderLightShader.LoadShader("fx/screen_quad.vert", CShaderProgram::ST_VERTEX);
	defferRenderLightShader.LoadShader("fx/gbuffer_light.frag", CShaderProgram::ST_FRAGMENT);
	
	if (!defferRenderShader.LinkShaders())
		return 0;

	if (!defferRenderLightShader.LinkShaders())
		return 0;

	while (input.Update() &&
		!input.IsKeyPressed(SDL_SCANCODE_SPACE) &&
		!input.IsKeyPressed(SDL_SCANCODE_RETURN))
	{
		stateManager.UpdateAll(1.0f);

		gBuffer.BindFramebuffer();

		graphic.Clear();

		stateManager.RenderAll(1.0f);

		graphic.DrawRenderableObjects();
		
		// Draw Terrain
		terrain.Draw(graphic);
		// **********

		gBuffer.UnbindFramebuffer();

		gBuffer.BindTextures();

		graphic.Clear();

		graphic.UseShaderProgram(defferRenderLightShader);

		defferRenderLightShader.SetUniform("viewPos", camera.GetPosition());

		glm::vec3 lightPosition[] = {
			{300.0f, 150.0f, 512.0f},
			{512.0f, 150.0f, 512.0f},
			{512.0f, 150.0f, 300.0f}
		};

		glm::vec3 lightColor[] = {
			{1.0f, 0.2f, 0.2f},
			{1.0f, 1.0f, 0.8f},
			{0.2f, 1.0f, 0.2f}
		};

		float lightRadius[] = {
			300.0f,
			300.0f,
			300.0f
		};

		for (size_t i = 0; i < 3; i++)
		{
			defferRenderLightShader.SetUniform("lightPosition", lightPosition[i]);
			defferRenderLightShader.SetUniform("lightColor", lightColor[i]);
			defferRenderLightShader.SetUniform("lightRadius", lightRadius[i]);

			graphic.DrawQuad();
		}

		graphic.SwapBuffres();
	}

	SDL_Quit();

	return 0;
}
