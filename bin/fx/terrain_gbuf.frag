#version 330 core

layout (location = 0) out vec3 texPosition;
layout (location = 1) out vec3 texNormal;
layout (location = 2) out vec3 texDiffuse;

in vec4 fragPosition;
in vec2 fragUV;
in vec3 fragNormal;

uniform sampler2D megaTexture;
uniform sampler2D coeffTexture;
uniform sampler2D diffuseTexture0;
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;

out vec3 color;

const float scalling = 0.02;

vec4 GetColor(in sampler2D tex, in vec4 coord, vec3 norm)
{
	return texture2D(tex, coord.xz * scalling);
}

vec4 GetColorTriPlanar(in sampler2D tex, in vec4 coord, in vec3 norm)
{
	// in wNorm is the world-space normal of the fragment
	vec3 blending = abs(norm);
	blending = normalize(max(blending, 0.00001)); // Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= b;

	// in float scale
	vec4 xaxis = texture2D(tex, coord.yz * scalling);
	vec4 yaxis = texture2D(tex, coord.xz * scalling);
	vec4 zaxis = texture2D(tex, coord.xy * scalling);
	
	vec4 color = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

	return color;
}

void main(void)
{
	vec4 megaColor = texture2D(megaTexture, fragUV);
	vec4 coeffs = texture2D(coeffTexture, fragUV);

	vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 diffuseColorTriPlanar = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	float triPlanarCoeff = smoothstep(90.0, 100.0, fragPosition.w);

	diffuseColor += GetColor(diffuseTexture0, fragPosition, fragNormal) * coeffs.r;
	diffuseColor += GetColor(diffuseTexture1, fragPosition, fragNormal) * coeffs.g;
	diffuseColor += GetColor(diffuseTexture2, fragPosition, fragNormal) * coeffs.b;
	diffuseColor += GetColor(diffuseTexture3, fragPosition, fragNormal) * coeffs.a;
	//diffuseColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	diffuseColorTriPlanar += GetColorTriPlanar(diffuseTexture0, fragPosition, fragNormal) * coeffs.r;
	diffuseColorTriPlanar += GetColorTriPlanar(diffuseTexture1, fragPosition, fragNormal) * coeffs.g;
	diffuseColorTriPlanar += GetColorTriPlanar(diffuseTexture2, fragPosition, fragNormal) * coeffs.b;
	diffuseColorTriPlanar += GetColorTriPlanar(diffuseTexture3, fragPosition, fragNormal) * coeffs.a;
	//diffuseColorTriPlanar = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	diffuseColor = (1.0 - triPlanarCoeff) * diffuseColor + triPlanarCoeff * diffuseColorTriPlanar;

	color = diffuseColor.rgb * (1.0 - (megaColor.a + 0.1)) + megaColor.rgb * (megaColor.a + 0.1);

	texPosition = fragPosition.xyz;
	texNormal   = fragNormal;
	texDiffuse  = color.rgb;
}

