#include "CCameraManager.h"

CCameraManager::CCameraManager(CCamera* camera, CInput* input, CTerrain* terrain)
{
	Camera  = camera;
	Input   = input;
	Terrain = terrain;
}

CCameraManager::~CCameraManager()
{
}

bool CCameraManager::Update(float dt)
{
	glm::mat4 matView = Camera->GetViewMatrix();

	glm::vec3 right(matView[0][0], matView[1][0], matView[2][0]);
	glm::vec3 up   (0.0f, 1.0f, 0.0f);
	glm::vec3 front(matView[0][2], matView[1][2], matView[2][2]);

	glm::vec3 cameraPosition = Camera->GetPosition();
	glm::vec3 cameraTarget   = glm::vec3(512.0f, 20.0f, 512.0f);//cameraPosition + Camera->GetDirection();

	if (Input->IsKeyPressed(SDL_SCANCODE_LSHIFT))
		cameraPosition += up * 0.01f * cameraPosition.y;
	if (Input->IsKeyPressed(SDL_SCANCODE_LCTRL))
		cameraPosition -= up * 0.01f * cameraPosition.y;
	if (Input->IsKeyPressed(SDL_SCANCODE_UP))
		cameraPosition -= front * 0.01f * cameraPosition.y;
	if (Input->IsKeyPressed(SDL_SCANCODE_DOWN))
		cameraPosition += front * 0.01f * cameraPosition.y;
	if (Input->IsKeyPressed(SDL_SCANCODE_LEFT))
		cameraPosition -= right * 0.01f * cameraPosition.y;
	if (Input->IsKeyPressed(SDL_SCANCODE_RIGHT))
		cameraPosition += right * 0.01f * cameraPosition.y;

	float height;
	glm::vec3 minPosAtTerrain = cameraPosition - up;

	if (Terrain && Terrain->GetHeight(minPosAtTerrain, height) && minPosAtTerrain.y < height)
		cameraPosition.y = height + up.y;

	Camera->SetPosition(cameraPosition);
	Camera->LookAt(cameraTarget);

	return true;
}

void CCameraManager::Render(float dt)
{

}
