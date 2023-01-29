#ifndef INIT_H
#define INIT_H
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct _tobjects{
	unsigned int VAO[15]; //vertex array object
	unsigned int VBO[15]; //vertex buffer object
	unsigned int EBO; // Element buffer object
}Tobjects;

typedef struct _tconfig{
	int walk_w;
	int walk_s;
	int walk_a;
	int walk_d;
	float cameraSpeed;
	int firstMouse;
	float yaw, pitch, lastX, lastY;
	int perspective;
}Tconfig;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_input(GLFWwindow *window, Tconfig *config);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Tconfig *config);

GLFWwindow *prepareGLFW(int width, int height);
int prepareGLEW();

#endif
