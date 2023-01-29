#define STB_IMAGE_IMPLEMENTATION
#include "textures.h"

void createTexture(unsigned int *ID, char *filedir, int type, int repeat)
{
	unsigned char *data;
	int width, height, nrChannels;
	glGenTextures(1, ID);
	glBindTexture(GL_TEXTURE_2D, *ID);
	/* set the texture wrapping/filtering options (on the currently bound texture object)*/
	if (repeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
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

int create_skybox(Tskybox *skybox, char *folder, int type)
{
	int i, width, height, nrChannels;
	char helper[64];
	unsigned char *data;
	char *textureFaces[] =
	{
		"left.jpg",
		"right.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg",
	};
	
	instance_create_cube(&(skybox->cube), 0, 0, 0, 100000, 100000, 100000, 1);
	
	glGenTextures(1, &(skybox->texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
	
	for (i = 0; i < 6; i++)
	{
		strcpy(helper, folder);
		strcat(helper, textureFaces[i]);
		data = stbi_load(helper, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("No se ha podido cargar la textura\n");
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	setInt(&(skybox->shader), "skybox", 0);
	return 0;
}
int draw_skybox(Tskybox skybox, Tcamera camera)
{
	instance_draw(skybox.cube, &skybox.shader, camera);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture);
	return 0;
}
