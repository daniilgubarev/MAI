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

	//coeffs /= (coeffs.a + coeffs.r + coeffs.g + coeffs.b);

	vec4 diffuseColor = texture2D(diffuseTexture0, fragPosition.xz * 0.2) * coeffs.r;
	diffuseColor	 += texture2D(diffuseTexture1, fragPosition.xz * 0.2) * coeffs.g;
	diffuseColor	 += texture2D(diffuseTexture2, fragPosition.xz * 0.2) * coeffs.b;
	diffuseColor	 += texture2D(diffuseTexture3, fragPosition.xz * 0.2) * coeffs.a;

	color = diffuseColor.rgb * (1.0 - (megaColor.a + 0.1)) + megaColor.rgb * (megaColor.a + 0.1);
}

