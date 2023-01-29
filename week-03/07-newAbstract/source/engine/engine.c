#include "engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Tconfig *config)
{
	mouse_input(window, config);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, 1);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, 1);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	/*
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		config->perspective = !(config->perspective);
	*/

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}
void mouse_input(GLFWwindow *window, Tconfig *config)
{
	/*
	double xpos, ypos;
	float xoffset = xpos - config->lastX;
	float yoffset = config->lastY - ypos; 
	float sensitivity = 0.1f;
	
	glfwGetCursorPos(window, &xpos, &ypos);

	if (config->firstMouse)
	{
		config->lastX = xpos;
		config->lastY = ypos;
		config->firstMouse = 0;
	}
	
	config->lastX = xpos;
	config->lastY = ypos;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	config->yaw   += xoffset;
	config->pitch += yoffset;
	
	if(config->pitch > 89.0f)
		config->pitch = 89.0f;
	if(config->pitch < -89.0f)
		config->pitch = -89.0f;
		*/
}

GLFWwindow *prepareGLFW(int width, int height)
{
	GLFWwindow* window;
	if (glfwInit())
	{
		window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
		stbi_set_flip_vertically_on_load(1);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		/*
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		*/
	}
	
	return window;
}

int prepareGLEW()
{
	GLenum err;
	glewExperimental = 1;
	err=glewInit();
	if(err!=GLEW_OK) {
		printf("glewInit failed: %s", glewGetErrorString(err));
		exit(1);
		return 1;
	}
	return 0;
}
