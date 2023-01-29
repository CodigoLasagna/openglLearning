#pragma once
#ifndef SHADERS_H
#define SHADERS_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <stdio.h>

typedef struct _tshader{
	unsigned int ID;
}Tshader;

char *readVertexShader(char *directory);
char *readFragmentShader(char *directory);

void createShader(Tshader *shader, char *vertexPath, char *fragmentPath);
void useShader(Tshader shader);
void setBool(Tshader shader, char *name, bool value);
void setInt(Tshader shader, char *name, int value);
void setFloat(Tshader shader, char *name, float value);

#endif
