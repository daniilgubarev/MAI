#include "GL_common.h"

class CTransform
{
public:
	CTransform();
	~CTransform();

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 axis, float angle);

	void MovePosition(glm::vec3 move);
	void AddRotation(glm::vec3 axis, float angle);

	void LookAtPoint(glm::vec3 point);
	void LookAtDirection(glm::vec3 direction);

	glm::mat4 GetTransformMatrix();
	glm::mat4 GetTranslateMatrix();
	glm::mat4 GetRotationMatrix();

	glm::vec3 GetAxisX();
	glm::vec3 GetAxisY();
	glm::vec3 GetAxisZ();

private:
	glm::vec3 Position;
	glm::quat Rotation;
};
