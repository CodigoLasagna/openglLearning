#ifndef SHADERS_H
#define SHADERS_H
#pragma once

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <stdio.h>

char *readVertexShader(char *directory);
char *readFragmentShader(char *directory);

void createShader(unsigned int *ID, char *vertexPath, char *fragmentPath);
void useShader(unsigned int *ID);

void setBool(unsigned int *ID, char *name, bool value);
void setInt(unsigned int *ID, char *name, int value);
void setFloat(unsigned int *ID, char *name, float value);

#endif
