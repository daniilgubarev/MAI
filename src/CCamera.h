#pragma once

#include "GL_common.h"

class CCamera
{
public:
	CCamera();
	CCamera(
		const glm::vec3& position,
		const glm::vec3& target,
		int screenWidth, int screenHeight,
		float nearPlane, float farPlane,
		float fov
	);
	
	~CCamera();

	void SetPosition(const glm::vec3& position);
	void SetDirection(const glm::vec3& direction);
	void SetResolution(int screenWidth, int screenHeight);
	void SetDepthRange(float nearPlane, float farPlane);
	void SetFOV(float fov);

	void LookAt(const glm::vec3& target);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjMatrix();
	glm::mat4 GetViewProjMatrix();

private:
	glm::vec3 Position;
	glm::vec3 Direction;

	int ScreenWidth;
	int ScreenHeight;

	float FOV;
	bool IsOrtho;

	float NearPlane;
	float FarPlane;
};