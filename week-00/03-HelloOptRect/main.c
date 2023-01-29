#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

typedef struct _tshaders{
	const char *vertexShaderSource;
	unsigned int vertexShader;
	
	const char *fragmentShaderSource;
	unsigned int fragmentShader;
	
	int succes;
	char infoLog[512];
	
	unsigned int shaderProgram;
}Tshaders;

typedef struct _tobjects{
	unsigned int VAO[2]; //vertex array object
	unsigned int VBO[2]; //vertex buffer object
	unsigned int EBO; // Element buffer object
}Tobjects;

typedef struct _tconfig{
	GLFWwindow* window;
	Tshaders shaders;
	Tobjects objects;
}Tconfig;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


Tconfig prepareGLFW(Tconfig config);
int prepareGLEW();

char *readVertexShader();
char *readFragmentShader();
Tconfig prepareShaders(Tconfig config);

int main()
{
	Tconfig config;
	
	config = prepareGLFW(config);
	
	prepareGLEW();
	
	config = prepareShaders(config);
	
	/*
	printf("%s", config.shaders.vertexShaderSource);
	printf("%s", config.shaders.fragmentShaderSource);
	*/
	
	GLfloat squareOne[] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};
	GLfloat squareTwo[] =
	{
		 0.9f,  0.1f, 0.0f,
		 0.9f, -0.1f, 0.0f,
		 0.7f, -0.1f, 0.0f,
		 0.7f,  0.1f, 0.0f
	};
	
	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3,
		
		4, 5, 7,
		5, 6, 7
	};
	
	glGenVertexArrays(2, config.objects.VAO);
	glGenBuffers(2, config.objects.VBO);
	glGenBuffers(1, &(config.objects.EBO));
	
	glBindVertexArray(config.objects.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, config.objects.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareOne), squareOne, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.objects.EBO);
	glEnableVertexAttribArray(0);
	
	
	glBindVertexArray(config.objects.VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, config.objects.VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareTwo), squareTwo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.objects.EBO);
	glEnableVertexAttribArray(0);
	
	
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	//GL_FILL or GL_LINE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	while (!glfwWindowShouldClose(config.window))
	{
		processInput(config.window);
		/* Render here */
		glClearColor(0.0f, 0.15f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(config.shaders.shaderProgram);
		glBindVertexArray(config.objects.VAO[0]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(config.objects.VAO[1]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(config.window);
		
		/* Poll for and process events */
		glfwPollEvents();
	}
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
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
}

Tconfig prepareGLFW(Tconfig config)
{
	if (glfwInit())
	{
		config.window = glfwCreateWindow(256, 256, "Hello World", NULL, NULL);
		if (!config.window)
		{
			glfwTerminate();
		}
		glfwMakeContextCurrent(config.window);
		glfwSetFramebufferSizeCallback(config.window, framebuffer_size_callback);
	}
	
	return config;
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

char *readVertexShader()
{
	FILE *file;
	long size = 0;
	char *vertexShaderSource;
	
	file = fopen("shaders/shader.vert", "rb");
	if(vertexShaderSource == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen("shaders/shader.vert", "r");
	vertexShaderSource = memset(malloc(size), '\0', size);
	fread(vertexShaderSource, 1, size-1, file);
	fclose(file);
	return vertexShaderSource;
}

char *readFragmentShader()
{
	FILE *file;
	long size = 0;
	char *fragmentShaderSource;
	
	file = fopen("shaders/shader.frag", "rb");
	if(fragmentShaderSource == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen("shaders/shader.frag", "r");
	fragmentShaderSource = memset(malloc(size), '\0', size);
	fread(fragmentShaderSource, 1, size-1, file);
	fclose(file);
	return fragmentShaderSource;
}

Tconfig prepareShaders(Tconfig config)
{
	config.shaders.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	config.shaders.vertexShaderSource = readVertexShader();
	
	glShaderSource(config.shaders.vertexShader, 1, &(config.shaders.vertexShaderSource), NULL);
	glCompileShader(config.shaders.vertexShader);
	
	glGetShaderiv(config.shaders.vertexShader, GL_COMPILE_STATUS, &(config.shaders.succes));
	if (!(config.shaders.succes))
	{
		glGetShaderInfoLog(config.shaders.vertexShader, 512, NULL, config.shaders.infoLog);
		printf("ERROR EN EL VERTEX SHADER ::%s\n", config.shaders.infoLog);
	}
	else
	{
		printf("vertex shader cargado con exito\n");
	}

	config.shaders.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	config.shaders.fragmentShaderSource = readFragmentShader();
	glShaderSource(config.shaders.fragmentShader, 1, &(config.shaders.fragmentShaderSource), NULL);
	glCompileShader(config.shaders.fragmentShader);
	
	glGetShaderiv(config.shaders.fragmentShader, GL_COMPILE_STATUS, &(config.shaders.succes));
	if (!(config.shaders.succes))
	{
		glGetShaderInfoLog(config.shaders.fragmentShader, 512, NULL, config.shaders.infoLog);
		printf("ERROR EN EL FRAGMENT SHADER ::%s", config.shaders.infoLog);
	}
	else
	{
		printf("fragment shader cargado con exito\n");
	}
	
	config.shaders.shaderProgram = glCreateProgram();
	glAttachShader(config.shaders.shaderProgram, config.shaders.vertexShader);
	glAttachShader(config.shaders.shaderProgram, config.shaders.fragmentShader);
	glLinkProgram(config.shaders.shaderProgram);
	
	glGetProgramiv(config.shaders.shaderProgram, GL_LINK_STATUS, &(config.shaders.succes));
	if (!(config.shaders.succes))
	{
		glGetProgramInfoLog(config.shaders.shaderProgram, 512, NULL, config.shaders.infoLog);
		printf("ERROR EN EL PROGRAM SHADER ::%s", config.shaders.infoLog);
	}
	else
	{
		printf("program shader cargado con exito\n");
	}
	glDeleteShader(config.shaders.vertexShader);
	glDeleteShader(config.shaders.fragmentShader);
	return config;
}
