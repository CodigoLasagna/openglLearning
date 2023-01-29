#version 330 core
out vec4 FragColor;
in float color;
in vec2 TexCoord;

uniform sampler2D ourTexture;
vec4 tex_color;

const int indexMatrix8x8[64] = int[](0,  32, 8,  40, 2,  34, 10, 42,
                                     48, 16, 56, 24, 50, 18, 58, 26,
                                     12, 44, 4,  36, 14, 46, 6,  38,
                                     60, 28, 52, 20, 62, 30, 54, 22,
                                     3,  35, 11, 43, 1,  33, 9,  41,
                                     51, 19, 59, 27, 49, 17, 57, 25,
                                     15, 47, 7,  39, 13, 45, 5,  37,
                                     63, 31, 55, 23, 61, 29, 53, 21);

float indexValue() {
    int x = int(mod(gl_FragCoord.x, 8));
    int y = int(mod(gl_FragCoord.y, 8));
    return indexMatrix8x8[(x + y * 8)] / 64.0;
}

float dither(float color) {
    float closestColor;

	if (color >= 0.75)
	{
		closestColor = 0.75;
	}
	else
	{
		if (color >= 0.5)
		{
			closestColor = 0.5;
		}
		else
		{
			if (color >= 0.25)
			{
				closestColor = 0.25;
			}
			else
			{
				closestColor = 0;
			}
		}
	}
    float secondClosestColor = 1.0f - (closestColor+0.25f);
    float d = indexValue();
    float distance = abs(closestColor - color);
    return (distance < d) ? closestColor : secondClosestColor;
}


void main () {
	tex_color = texture(ourTexture, TexCoord);
	FragColor = vec4(dither(tex_color.r), dither(tex_color.g), dither(tex_color.b), 1.0f);
}
