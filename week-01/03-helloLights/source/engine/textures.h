#ifndef TEXTURES_H
#define TEXTURES_H
#pragma once

#include <stb/stb_image.h>
#include <GL/glew.h>

void createTexture(unsigned int *ID, char *filedir);
int bind_texture(unsigned int *ID, unsigned int texture, char *name, int layer);

#endif
