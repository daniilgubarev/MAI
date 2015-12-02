#include "CCamera.h"

CCamera::CCamera()
{
}

CCamera::CCamera(
	const glm::vec3& position,
	const glm::vec3& target,
	int screenWidth, int screenHeight,
	float nearPlane, float farPlane,
	float fov)
{
	SetPosition(position);
	SetResolution(screenWidth, screenHeight);
	SetDepthRange(nearPlane, farPlane);
	SetFOV(fov);
	LookAt(target);
}

CCamera::~CCamera()
{
}

void CCamera::SetPosition(const glm::vec3& position)
{
	Position = position;
}

void CCamera::SetDirection(const glm::vec3& direction)
{
	Direction = direction;
}

void CCamera::SetResolution(int screenWidth, int screenHeight)
{
	ScreenWidth  = screenWidth;
	ScreenHeight = screenHeight;
}

void CCamera::SetDepthRange(float nearPlane, float farPlane)
{
	NearPlane = nearPlane;
	FarPlane  = farPlane;
}

void CCamera::SetFOV(float fov)
{
	FOV = fov;
}

void CCamera::LookAt(const glm::vec3& target)
{
	Direction = normalize(target - Position);
}

glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(
		Position,
		Position + Direction,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
}

glm::mat4 CCamera::GetProjMatrix()
{
	return glm::perspective(
		FOV,
		static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight),
		NearPlane,
		FarPlane
	);
}

glm::mat4 CCamera::GetViewProjMatrix()
{
	return GetProjMatrix() * GetViewMatrix();
}
