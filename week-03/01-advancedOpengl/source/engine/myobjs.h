#ifndef MYOBJS_H
#define MYOBJS_H
#pragma once
#include "elements.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

typedef struct _tmodel{
	unsigned int *VAO, *VBO, *EBO, *NBO, *TBO;
	unsigned int max_meshes;
	unsigned int *indices;
	mat4 model;
	float x, y, z;
	int type;
	vec3 pos;
}Tmodel;

int prepare_light(unsigned int *surface_shader, unsigned int *light_shader, int n_light, vec3 color);
int load_model(Tmodel *model, char *directory, float x, float y, float z, int type);
int draw_model(Tmodel *model, unsigned int *shader, Tcamera camera);

#endif
