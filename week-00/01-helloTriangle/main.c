#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>

typedef struct _tshaders{
	const char *vertexShaderSource;
	unsigned int vertexShader;
	unsigned int VBO;
	unsigned int VAO;
	
	const char *fragmentShaderSource;
	unsigned int fragmentShader;
	
	int succes;
	char infoLog[512];
	
	unsigned int shaderProgram;
}Tshaders;

char *readVertexShader();
char *readFragmentShader();
int renderer();


int main(void)
{
	GLFWwindow* window;
	Tshaders shaders;
	
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
	
	glewExperimental=true;
	GLenum err=glewInit();
	if(err!=GLEW_OK) {
		// Problem: glewInit failed, something is seriously wrong.
		printf("glewInit failed: %s", glewGetErrorString(err));
		exit(1);
	}
	GLfloat verts[] =
	{
		-0.5f, -0.5, 0.0,
		 0.5f, -0.5, 0.0,
		 0.0f,  0.5, 0.0
	};
	
	glGenBuffers(1, &shaders.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, shaders.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	shaders.vertexShaderSource = readVertexShader();
	shaders.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
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
	
	glGenVertexArrays(1, &shaders.VAO);
	
	glBindVertexArray(shaders.VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, shaders.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	
	
	glDeleteShader(shaders.vertexShader);
	glDeleteShader(shaders.fragmentShader);
	
	glUseProgram(shaders.shaderProgram);
	glBindVertexArray(shaders.VAO);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		renderer();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		
		/* Poll for and process events */
		glfwPollEvents();
		
	}
	
	glfwTerminate();
	return 0;
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
