#define STB_IMAGE_IMPLEMENTATION
#include "textures.h"
#include "shaders.h"
void createTexture(unsigned int *ID, char *filedir, int type)
{
	unsigned char *data;
	int width, height, nrChannels;
	glGenTextures(1, ID);
	glBindTexture(GL_TEXTURE_2D, *ID);
	/* set the texture wrapping/filtering options (on the currently bound texture object)*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/* load and generate the texture*/
	data = stbi_load(filedir, &width, &height, &nrChannels, 0);
	if (data)
	{
		switch(type)
		{
			case 0:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

int bind_texture(unsigned int *ID, unsigned int texture, int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_2D, texture);
	return 0;
}
