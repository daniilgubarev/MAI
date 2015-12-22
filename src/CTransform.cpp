#include "CTransform.h"

CTransform::CTransform()
{
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Rotation = glm::quat();
}

CTransform::~CTransform()
{
}

void CTransform::SetPosition(glm::vec3 position)
{
	Position = position;
}

void CTransform::SetRotation(glm::vec3 axis, float angle)
{
	Rotation = glm::normalize(glm::angleAxis(angle, glm::normalize(axis)));
}

void CTransform::MovePosition(glm::vec3 move)
{
	Position += move;
}

void CTransform::AddRotation(glm::vec3 axis, float angle)
{
	Rotation = glm::normalize(glm::angleAxis(angle, glm::normalize(axis))) * Rotation;
}

void CTransform::LookAtPoint(glm::vec3 point)
{
	glm::vec3 direction = glm::normalize(point - Position);

	LookAtDirection(direction);
}

void CTransform::LookAtDirection(glm::vec3 direction)
{
	direction = glm::normalize(direction);

	float dot = glm::dot(glm::vec3(0, 0, 1), direction);

	if (fabs(dot - (-1.0f)) < 0.000001f)
	{
		Rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		return;
	}
	else if (fabs(dot - (1.0f)) < 0.000001f)
	{
		Rotation = glm::quat();
		return;
	}

	float angle = acosf(dot);

	glm::vec3 cross = glm::normalize(glm::cross(glm::vec3(0, 0, 1), direction));

	Rotation = glm::normalize(glm::angleAxis(angle, cross));
}

glm::mat4 CTransform::GetTransformMatrix()
{
	return glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(Rotation);
}

glm::mat4 CTransform::GetTranslateMatrix()
{
	return glm::translate(glm::mat4(1.0f), Position);
}

glm::mat4 CTransform::GetRotationMatrix()
{
	return glm::toMat4(Rotation);
}

glm::vec3 CTransform::GetAxisX()
{
	glm::mat4 mat = glm::toMat4(Rotation);;

	return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}

glm::vec3 CTransform::GetAxisY()
{
	glm::mat4 mat = glm::toMat4(Rotation);;

	return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
}

glm::vec3 CTransform::GetAxisZ()
{
	glm::mat4 mat = glm::toMat4(Rotation);;

	return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}
