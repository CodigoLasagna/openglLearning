#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform float iTime;

uniform sampler2D screenTexture;


void main()
{
	float amount = 0.0f;
	vec3 col;
	amount = (1.0f + sin(iTime*6.0f)) * 0.5f;
	amount *= 1.0f + sin(iTime*16.0f) * 0.5f;
	amount *= 1.0f + sin(iTime*19.0f) * 0.5f;
	amount *= 1.0f + sin(iTime*27.0f) * 0.5f;
	amount = pow(amount, 3.0f);
	amount *= 0.05f;
	
	col.r = texture(screenTexture, vec2(TexCoords.x+amount, TexCoords.y+(amount/2.0f))).r;
	col.g = texture(screenTexture, TexCoords).g;
	col.b = texture(screenTexture, vec2(TexCoords.x-amount, TexCoords.y)).b;
	
	col *= (1.0f - amount * 0.5f);
	
	FragColor = vec4(col, 1.0f);
}
