#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <math.h>
#include <GLFW/glfw3.h>

typedef struct _tvertShader{
	const char *vertexShaderSource;
	unsigned int vertexShader;
	
	int succes;
	char infoLog[512];
}TvertShader;

typedef struct _tfragShader{
	const char *fragmentShaderSource;
	unsigned int fragmentShader;
	
	int succes;
	char infoLog[512];
}TfragShader;

typedef struct _tshaderProg{
	unsigned int shaderProgram;
	int succes;
	char infoLog[512];
}TshaderProg;

typedef struct _tobjects{
	unsigned int VAO[5]; //vertex array object
	unsigned int VBO[5]; //vertex buffer object
	unsigned int EBO; // Element buffer object
}Tobjects;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


GLFWwindow *prepareGLFW(int width, int height);
int prepareGLEW();

char *readVertexShader(char *directory);
char *readFragmentShader(char *directory);
int prepareFragShader(TfragShader *shaders, char *directory);
int prepareVertShader(TvertShader *shaders, char *directory);
int attachProgShader(TshaderProg *program, TvertShader vertShader, TfragShader fragShader);

int main()
{
	GLFWwindow* window;
	TvertShader vertShader;
	
	TfragShader blueFragShader;
	TfragShader redFragShader;
	TfragShader vecColFragShader;
	TfragShader cycleFragShader;
	TfragShader customFragShader;
	
	TshaderProg programBlue;
	TshaderProg programRed;
	TshaderProg programVecCol;
	TshaderProg programCycle;
	TshaderProg programCustom;
	
	Tobjects objects;
	
	window = prepareGLFW(512, 512);
	
	prepareGLEW();
	
	prepareVertShader(&vertShader, "shaders/shader.vert");
	
	prepareFragShader(&blueFragShader, "shaders/blue.frag");
	prepareFragShader(&redFragShader, "shaders/red.frag");
	prepareFragShader(&vecColFragShader, "shaders/vecCol.frag");
	prepareFragShader(&cycleFragShader, "shaders/cycle.frag");
	prepareFragShader(&customFragShader, "shaders/custom.frag");
	
	attachProgShader(&programBlue, vertShader, blueFragShader);
	attachProgShader(&programRed, vertShader, redFragShader);
	attachProgShader(&programVecCol, vertShader, vecColFragShader);
	attachProgShader(&programCycle, vertShader, cycleFragShader);
	attachProgShader(&programCustom, vertShader, customFragShader);
	/*
	printf("%s", config.shaders.vertexShaderSource);
	printf("%s", config.shaders.fragmentShaderSource);
	*/
	
	GLfloat squares[4][24] =
	{
		{
			-0.9f, -0.1f, 0.0f,
			-0.7f, -0.1f, 0.0f,
			-0.7f,  0.1f, 0.0f,
			-0.9f,  0.1f, 0.0f,
		},
		{
			-0.6f, -0.1f, 0.0f,
			-0.4f, -0.1f, 0.0f,
			-0.4f,  0.1f, 0.0f,
			-0.6f,  0.1f, 0.0f,
		},
		{
			-0.3f, -0.1f, 0.0f,
			-0.1f, -0.1f, 0.0f,
			-0.1f,  0.1f, 0.0f,
			-0.3f,  0.1f, 0.0f,
		},
		{
			// position				// color
			0.0f, -0.1f, 0.0f,		1.0f, 0.0f, 0.0f,
			0.2f, -0.1f, 0.0f,		0.0f, 1.0f, 0.0f,
			0.2f,  0.1f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.0f,  0.1f, 0.0f,		0.0f, 0.0f, 0.0f
		},
	};
	
	GLuint indices[] =
	{
		0, 1, 2,
		3, 2, 0,
	};
	
	glGenVertexArrays(4, objects.VAO);
	glGenBuffers(4, objects.VBO);
	glGenBuffers(1, &(objects.EBO));
	
	glBindVertexArray(objects.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[0]), squares[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(objects.VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[1]), squares[1], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glEnableVertexAttribArray(0);

	glBindVertexArray(objects.VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[2]), squares[2], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(objects.VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[3]), squares[3], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	//GL_FILL or GL_LINE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLfloat timeValue;
	GLfloat greenValue;
	GLfloat blueValue;
	GLint vertexColorLocation;
	
	while (!glfwWindowShouldClose(window))
	{
		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		blueValue = (sin(timeValue) / 4.0f) + 0.5f;
		vertexColorLocation = glGetUniformLocation(programCycle.shaderProgram, "ourColor");
		processInput(window);
		/* Render here */
		glClearColor(0.0f, 0.15f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(programRed.shaderProgram);
		glBindVertexArray(objects.VAO[0]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(programVecCol.shaderProgram);
		glBindVertexArray(objects.VAO[1]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(programCycle.shaderProgram);
		glUniform4f(vertexColorLocation, 0.3f, greenValue, blueValue, 1.0f);
		glBindVertexArray(objects.VAO[2]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(programCustom.shaderProgram);
		glBindVertexArray(objects.VAO[3]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteVertexArrays(3, objects.VAO);
	glDeleteBuffers(3, objects.VBO);
	
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

char *readVertexShader(char *directory)
{
	FILE *file;
	long size = 0;
	char *vertexShaderSource;
	
	file = fopen(directory, "rb");
	if(vertexShaderSource == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen(directory, "r");
	vertexShaderSource = memset(malloc(size), '\0', size);
	fread(vertexShaderSource, 1, size-1, file);
	fclose(file);
	return vertexShaderSource;
}

char *readFragmentShader(char *directory)
{
	FILE *file;
	long size = 0;
	char *fragmentShaderSource;
	
	file = fopen(directory, "rb");
	if(fragmentShaderSource == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen(directory, "r");
	fragmentShaderSource = memset(malloc(size), '\0', size);
	fread(fragmentShaderSource, 1, size-1, file);
	fclose(file);
	return fragmentShaderSource;
}

int prepareFragShader(TfragShader *shader, char *directory)
{
	shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shader->fragmentShaderSource = readFragmentShader(directory);
	glShaderSource(shader->fragmentShader, 1, &(shader->fragmentShaderSource), NULL);
	glCompileShader(shader->fragmentShader);
	
	glGetShaderiv(shader->fragmentShader, GL_COMPILE_STATUS, &(shader->succes));
	if (!(shader->succes))
	{
		glGetShaderInfoLog(shader->fragmentShader, 512, NULL, shader->infoLog);
		printf("ERROR EN EL FRAGMENT SHADER ::%s", shader->infoLog);
	}
	else
	{
		printf("fragment shader cargado con exito\n");
	}
	return 0;
}

int prepareVertShader(TvertShader *shader, char *directory)
{
	shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shader->vertexShaderSource = readVertexShader(directory);
	
	glShaderSource(shader->vertexShader, 1, &(shader->vertexShaderSource), NULL);
	glCompileShader(shader->vertexShader);
	
	glGetShaderiv(shader->vertexShader, GL_COMPILE_STATUS, &(shader->succes));
	if (!(shader->succes))
	{
		glGetShaderInfoLog(shader->vertexShader, 512, NULL, shader->infoLog);
		printf("ERROR EN EL VERTEX SHADER ::%s\n", shader->infoLog);
	}
	else
	{
		printf("vertex shader cargado con exito\n");
	}
	return 0;
}

int attachProgShader(TshaderProg *program, TvertShader vertShader, TfragShader fragShader)
{
	program->shaderProgram = glCreateProgram();
	
	glAttachShader(program->shaderProgram, vertShader.vertexShader);
	glAttachShader(program->shaderProgram, fragShader.fragmentShader);
	glLinkProgram(program->shaderProgram);
	
	glGetProgramiv(program->shaderProgram, GL_LINK_STATUS, &(program->succes));
	if (!(program->succes))
	{
		glGetProgramInfoLog(program->shaderProgram, 512, NULL, program->infoLog);
		printf("ERROR EN EL PROGRAM SHADER ::%s", program->infoLog);
	}
	else
	{
		printf("program shader cargado con exito\n");
	}
	glDeleteShader(vertShader.vertexShader);
	glDeleteShader(fragShader.fragmentShader);
	return 0;
}
