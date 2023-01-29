#ifndef OPENGL_H
#define OPENGL_H
#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "myobjs.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_input(GLFWwindow *window, Tconfig *config);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Tconfig *config);

GLFWwindow *prepareGLFW(int width, int height);
int prepareGLEW();

#include <stdio.h>
#include <stdlib.h>

#endif
