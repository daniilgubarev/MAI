#version 330 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec3 vertNormal;

out vec3 fragPosition;
out vec2 fragUV;
out vec3 fragNormal;

uniform mat4 matVP;

void main(void)
{
	gl_Position = matVP * vec4(vertPosition, 1);

	fragPosition = vertPosition;
	fragUV = vertUV;
	fragNormal = vertNormal;
}
