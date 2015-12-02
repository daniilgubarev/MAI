#version 330 core

in vec3 fragPosition;
in vec2 fragUV;

uniform sampler2D megaTexture;
uniform sampler2D coeffTexture;
uniform sampler2D diffuseTexture0;
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;

out vec3 color;

void main(void)
{
	vec4 megaColor = texture2D(megaTexture, fragUV);
	vec4 coeffs = texture2D(coeffTexture, fragUV);

	vec4 diffuseColor = texture2D(diffuseTexture0, fragPosition.xz) * coeffs.a;
	diffuseColor	 += texture2D(diffuseTexture1, fragPosition.xz) * coeffs.r;
	diffuseColor	 += texture2D(diffuseTexture2, fragPosition.xz) * coeffs.g;
	diffuseColor	 += texture2D(diffuseTexture3, fragPosition.xz) * coeffs.b;

	color = vec3(1.0, 1.0, 1.0);//diffuseColor.rgb * (1.0 - megaColor.a) + megaColor.rgb * megaColor.a;
}

