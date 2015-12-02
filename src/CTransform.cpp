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
	Rotation = glm::quat(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
}

void CTransform::MovePosition(glm::vec3 move)
{
	Position += move;
}

void CTransform::AddRotation(glm::vec3 axis, float angle)
{
}

void CTransform::LookAtPoint(glm::vec3 point)
{
	Rotation = glm::quat(0.0f, glm::normalize(point - Position));
}

void CTransform::LookAtDirection(glm::vec3 direction)
{
	Rotation = glm::quat(0.0f, glm::normalize(direction));
}

glm::mat4 CTransform::GetTransformMatrix()
{
}

glm::mat4 CTransform::GetRotationMatrix()
{
}

glm::vec3 CTransform::GetAxisX()
{
}

glm::vec3 CTransform::GetAxisY()
{
}

glm::vec3 CTransform::GetAxisZ()
{
}
