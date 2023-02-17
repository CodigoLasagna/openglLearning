#ifndef TEXTURES_H
#define TEXTURES_H
#pragma once

#include <stb/stb_image.h>
#include <GL/glew.h>

#include "elements.h"
#include "shaders.h"

typedef struct _tskybox{
	unsigned int texture;
	unsigned int shader;
	Tobject cube;
}Tskybox;

void createTexture(unsigned int *ID, char *filedir, int type, int repeat);
int bind_texture(unsigned int texture, int layer);
int bind_cubemap(unsigned int texture, int layer);
int create_skybox(Tskybox *skybox, char *folder, int type);
int draw_skybox(Tskybox skybox, Tcamera camera);

#endif
