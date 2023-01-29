#include "../src/init.h"
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		config->walk_w = 0;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		config->walk_d = 0;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		config->walk_a = 0;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		config->walk_s = 0;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		config->walk_w = 1;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		config->walk_s = 1;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		config->walk_a = 1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		config->walk_d = 1;
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		config->perspective = !(config->perspective);

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
}
void mouse_input(GLFWwindow *window, Tconfig *config)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (config->firstMouse)
	{
		config->lastX = xpos;
		config->lastY = ypos;
		config->firstMouse = 0;
	}
	
	float xoffset = xpos - config->lastX;
	float yoffset = config->lastY - ypos; 
	config->lastX = xpos;
	config->lastY = ypos;
	
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	config->yaw   += xoffset;
	config->pitch += yoffset;
	
	if(config->pitch > 89.0f)
		config->pitch = 89.0f;
	if(config->pitch < -89.0f)
		config->pitch = -89.0f;
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
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	}
	
	return window;
}

int prepareGLEW()
{
	glewExperimental = 1;
	GLenum err=glewInit();
	if(err!=GLEW_OK) {
		printf("glewInit failed: %s", glewGetErrorString(err));
		exit(1);
		return 1;
	}
	return 0;
}

