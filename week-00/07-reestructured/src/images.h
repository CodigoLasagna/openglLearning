#pragma once
#ifndef IMAGES_H
#define IMAGES_H

#include <stb/stb_image.h>
#include <GL/glew.h>

typedef struct _ttexture{
	unsigned int ID;
	int width, height, nrChannels;
}Ttexture;

void createTexture(Ttexture *texture, char *directory);

#endif
