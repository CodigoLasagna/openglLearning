#ifndef ELEMENTS_H
#define ELEMENTS_H
#pragma once

#include <cglm/cglm.h>
#include "shaders.h"
#include <GLFW/glfw3.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
	
typedef struct _tconfig{
	int screenWidth, screenHeight;
	int wireframe;
}Tconfig;


typedef struct _tobject{
	unsigned int VAO; /*Vertex array object*/
	unsigned int VBO; /*Vertex buffer object*/
	unsigned int EBO; /*Element buffer object*/
	int width, height, thickness;
	vec3 pos;
	int type;
	int indices_n;
	mat4 model;
}Tobject;

typedef struct _tmodel{
	unsigned int *VAO, *VBO, *EBO, *NBO, *TBO;
	unsigned int max_meshes;
	unsigned int *indices;
	mat4 model;
	float x, y, z;
	int type;
	vec3 pos;
}Tmodel;

typedef struct _tcamera{
	float currentFrame;
	float lastFrame;
	float deltaTime;
	
	int width;
	int height;
	float nWidth;
	float nHeight;
	float scale;
	int perspective;
	mat4 model;
	mat4 view;
	mat4 projection;
	
	vec3 pos;
	vec3 front;
	vec3 up;
	
	float speed;
	int firstMouse;
	
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;
	
	GLuint frameBuffer, renderBufferColor, renderBufferDepth, renderBufferStencil;
}Tcamera;

typedef struct _trenderer{
	unsigned int shader;
	Tobject screen;
	unsigned int frameBuffer;
	unsigned int texcolBuffer;
	unsigned int renderBuffer;
}Trenderer;

int instance_create_quad(Tobject *ID, float x, float y, float z, int width, int height, int nAttributes);
int instance_create_cube(Tobject *ID, float x, float y, float z, int width, int height, int thickness, int nAttributes);
int instance_draw(Tobject ID, unsigned int *shader, Tcamera camera);
int instance_move(Tobject *ID);

int prepare_light(unsigned int *surface_shader, unsigned int *light_shader, int n_light, vec3 color);
int load_model(Tmodel *model, char *directory, float x, float y, float z, int type);
int draw_model(Tmodel *model, unsigned int *shader, Tcamera camera);

int init_camera(Tcamera *camera, int width, int height, float scale);
int run_camera(Tcamera *camera, GLFWwindow *window);
int apply_camera(unsigned int *shader, Tcamera camera);

int prepare_renderer(Trenderer *renderer);
int run_rederer(Trenderer *renderer, Tcamera camera);

int prepare_uniformblockData(unsigned int *shader, char *name);
int prepare_uniformblockMatrices(unsigned int *uboBuffer, Tcamera camera);
int run_uniformblockMatrices(unsigned int uboBuffer, Tcamera camera);

#endif
