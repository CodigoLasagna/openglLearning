#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D ourTexture;
uniform float wind;
uniform vec3 color;

void main()
{
	float amount, constant;
	vec2 TexSize = textureSize(ourTexture, 0);
	vec2 pixel_size = 1.0f / vec2(TexSize);
	vec2 p = TexCoords.xy;
	constant = 0.3f;
	amount = wind*constant;
	float direction = 1.0f;
	float yoffset = 0.0f;
	float xoffset = 0.0f;
	float notion;
	int x = 0, y = 0;
	vec4 outline = vec4(0.05f, 0.8f, 0.6f, 1.0f);
	
	/*
	yoffset = (floor(p.y*TexSize.y))/TexSize.y;
	p.x -= (floor(yoffset * sin(amount) * TexSize.x)/TexSize.x);
	*/
	notion = direction*constant;
	xoffset = sin(amount+notion);
	yoffset += floor(p.y*TexSize.y)/TexSize.y;
	p.x -= floor((xoffset*yoffset)*TexSize.x)/TexSize.x;
	
	
	vec4 myTex = texture2D(ourTexture, p);
	vec4 checkTex;
	vec2 pixel_off;
	for (y = -1; y <= 1; y++)
	{
		for (x = -1; x <= 1; x++)
		{
			pixel_off = vec2(float(x), float(y));
			checkTex = texture(ourTexture, p + pixel_off * pixel_size);
			if (checkTex.a == 1.0f)
			{
				if (myTex.a == 0.0f)
				{
					outline.rgb = color.rgb;
					myTex = outline;
				}
			}
		}
	}
	
	FragColor = myTex;
}
