#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct _tshaders{
	const char *vertexShaderSource;
	unsigned int vertexShader;
	unsigned int VBO; //vertex buffer object
	unsigned int VAO; //vertex array object
	
	const char *fragmentShaderSource;
	unsigned int fragmentShader;
	
	int succes;
	char infoLog[512];
	
	unsigned int shaderProgram;
}Tshaders;

typedef struct _tobjects{
	unsigned int EBO;
}Tobjects;

char *readVertexShader();
char *readFragmentShader();
int renderer();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main(void)
{
	GLFWwindow* window;
	Tshaders shaders;
	Tobjects objects;
	
	/* Initialize the library */
	if (!glfwInit())
		return -1;
		
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(256, 256, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	glewExperimental = 1;
	GLenum err=glewInit();
	if(err!=GLEW_OK) {
		// Problem: glewInit failed, something is seriously wrong.
		printf("glewInit failed: %s", glewGetErrorString(err));
		exit(1);
	}
	GLfloat verts[] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	
	
	shaders.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shaders.vertexShaderSource = readVertexShader();
	glShaderSource(shaders.vertexShader, 1, &(shaders.vertexShaderSource), NULL);
	glCompileShader(shaders.vertexShader);
	
	glGetShaderiv(shaders.vertexShader, GL_COMPILE_STATUS, &(shaders.succes));
	if (!(shaders.succes))
	{
		glGetShaderInfoLog(shaders.vertexShader, 512, NULL, shaders.infoLog);
		printf("ERROR EN EL SHADER VERTEX ::%s", shaders.infoLog);
	}
	
	shaders.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaders.fragmentShaderSource = readFragmentShader();
	glShaderSource(shaders.fragmentShader, 1, &(shaders.fragmentShaderSource), NULL);
	glCompileShader(shaders.fragmentShader);
	
	glGetShaderiv(shaders.fragmentShader, GL_COMPILE_STATUS, &(shaders.succes));
	if (!(shaders.succes))
	{
		glGetShaderInfoLog(shaders.fragmentShader, 512, NULL, shaders.infoLog);
		printf("ERROR EN EL SHADER FRAGMENT ::%s", shaders.infoLog);
	}
	
	shaders.shaderProgram = glCreateProgram();
	glAttachShader(shaders.shaderProgram, shaders.vertexShader);
	glAttachShader(shaders.shaderProgram, shaders.fragmentShader);
	glLinkProgram(shaders.shaderProgram);
	
	glGetProgramiv(shaders.shaderProgram, GL_LINK_STATUS, &(shaders.succes));
	if (!(shaders.succes))
	{
		glGetProgramInfoLog(shaders.shaderProgram, 512, NULL, shaders.infoLog);
		printf("ERROR EN EL SHADER PROGRAM ::%s", shaders.infoLog);
	}
	glDeleteShader(shaders.vertexShader);
	glDeleteShader(shaders.fragmentShader);
	
	glGenVertexArrays(1, &shaders.VAO);
	glBindVertexArray(shaders.VAO);
	
	glGenBuffers(1, &(shaders.VBO));
	glBindBuffer(GL_ARRAY_BUFFER, shaders.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &(objects.EBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	//GL_FILL or GL_LINE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		/* Render here */
		glClearColor(0.0f, 0.15f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaders.shaderProgram);
		glBindVertexArray(shaders.VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		
		/* Poll for and process events */
		glfwPollEvents();
		
	}
	
	glDeleteVertexArrays(1, &(shaders.VAO));
	glDeleteBuffers(1, &(shaders.VBO));
	glDeleteBuffers(1, &(objects.EBO));
	glDeleteProgram(shaders.shaderProgram);
	
	glfwTerminate();
	return 0;
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int renderer()
{
	
	
	return 0;
}
char *readVertexShader()
{
	FILE* vertShaderFile;
	char *vertexShaderSource;
	int i = 0;
	int c;
	vertShaderFile = fopen("shaders/shader.vert", "r");
	if (vertShaderFile)
	{
		while((c = fgetc(vertShaderFile)) != EOF)
		{
			vertexShaderSource[i++] = (char)c;
		}
		vertexShaderSource[i] = '\0';
		fclose(vertShaderFile);
	}
	printf("%s", vertexShaderSource);
	return vertexShaderSource;
}

char *readFragmentShader()
{
	FILE* fragShaderFile;
	char *fragmentShaderSource;
	int i = 0;
	int c;
	fragShaderFile = fopen("shaders/shader.frag", "r");
	if (fragShaderFile)
	{
		while((c = fgetc(fragShaderFile)) != EOF)
		{
			fragmentShaderSource[i++] = (char)c;
		}
		fragmentShaderSource[i] = '\0';
		fclose(fragShaderFile);
	}
	printf("%s", fragmentShaderSource);
	return fragmentShaderSource;
}
