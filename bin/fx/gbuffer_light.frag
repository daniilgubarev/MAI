#version 420 core

in vec2 TexCoords;

out vec3 FragColor;

layout(binding = 0) uniform sampler2D texPosition;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texDiffuse;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float lightRadius = 400.0;

void main()
{
    // Retrieve data from G-buffer
    vec3 fragPos = texture(texPosition, TexCoords).rgb;
    vec3 fragNormal = texture(texNormal, TexCoords).rgb;
    vec3 fragDiffuse = texture(texDiffuse, TexCoords).rgb;

    float coef = 1.0 - smoothstep(0.0, lightRadius, distance(fragPos, lightPosition));
    
    // Then calculate lighting as usual
    vec3 diffuse = fragDiffuse * 0.1;

    vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(lightPosition - fragPos);

    diffuse += max(dot(fragNormal, lightDir), 0.2) * fragDiffuse * lightColor * coef;
    
    FragColor = diffuse;
}
