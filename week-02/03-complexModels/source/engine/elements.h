#ifndef ELEMENTS_H
#define ELEMENTS_H
#pragma once

#include <cglm/cglm.h>
#include "shaders.h"
#include "textures.h"
#include <GLFW/glfw3.h>

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

typedef struct _tconfig{
	float currentFrame;
	float lastFrame;
	float deltaTime;
}Tconfig;

typedef struct _tcamera{
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
	
	GLuint frameBuffer, renderBufferColor, renderBufferDepth;
}Tcamera;

int instance_create_quad(Tobject *ID, float x, float y, float z, int width, int height, int nAttributes);
int instance_create_cube(Tobject *ID, float x, float y, float z, int width, int height, int thickness, int nAttributes);
int instance_draw(Tobject ID, unsigned int *shader, Tcamera camera);
int instance_move(Tobject *ID);
int init_camera(Tcamera *camera, int width, int height, float scale);
int run_camera(Tcamera *camera, GLFWwindow *window, float delta);
int apply_camera(unsigned int *shader, Tcamera camera);

#endif
