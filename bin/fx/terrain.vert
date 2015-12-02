#version 330 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertUV;

out vec3 fragPosition;
out vec2 fragUV;

uniform mat4 matVP;

void main(void)
{
	gl_Position = matVP * vec4(vertPosition, 1);

	fragPosition = gl_Position.xyz;
	fragUV = vertUV;
}
