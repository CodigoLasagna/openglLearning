#ifndef SHADERS_H
#define SHADERS_H
#pragma once

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <stdio.h>
#include <cglm/cglm.h>


char *readShaderFromFile(char *directory);

void createShader(unsigned int *ID, char *vertexPath, char *fragmentPath);
void createShader_geometry(unsigned int *ID, char *vertexPath, char *fragmentPath, char *geometryPath);
void useShader(unsigned int *ID);

void setBool(unsigned int *ID, char *name, bool value);
void setInt(unsigned int *ID, char *name, int value);
void setFloat(unsigned int *ID, char *name, float value);
void setVec3(unsigned int *ID, char *name, float v1, float v2, float v3);
void setVec4(unsigned int *ID, char *name, float v1, float v2, float v3, float v4);
void setMat4(unsigned int *ID, char *name, mat4 mat);

#endif
