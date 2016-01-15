#version 330 core

out vec2 TexCoords;

const vec2 quadVertices[4] = vec2[4](
	vec2(-1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
);

const vec2 quadUV[4] = vec2[4](
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0)
);

void main()
{
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
    TexCoords = quadUV[gl_VertexID];
}
