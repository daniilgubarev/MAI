#version 420 core

in vec2 TexCoords;

out vec3 FragColor;

layout(binding = 0) uniform sampler2D texPosition;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texDiffuse;

void main()
{
    // Retrieve data from G-buffer
    vec3 fragPos = texture(texPosition, TexCoords).rgb;
    vec3 fragNormal = texture(texNormal, TexCoords).rgb;
    vec3 fragDiffuse = texture(texDiffuse, TexCoords).rgb;

    FragColor = fragNormal;
}
