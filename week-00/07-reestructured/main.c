#include "src/shaders.h"
#include "src/images.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <math.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

typedef struct _tobjects{
	unsigned int VAO[10]; //vertex array object
	unsigned int VBO[10]; //vertex buffer object
	unsigned int EBO; // Element buffer object
}Tobjects;

GLFWwindow *prepareGLFW(int width, int height);
int prepareGLEW();

int main()
{
	GLFWwindow* window;
	
	Tshader shaderBlue;
	Tshader shaderRed;
	Tshader shaderVecCol;
	Tshader shaderCycle;
	Tshader shaderCustom;
	Tshader shaderTest;
	Tshader shaderTex;
	Tshader shaderTexColor;
	Tshader shaderTexDuo;
	Tshader shaderTexDuoColor;
	
	Ttexture texture1;
	Ttexture texture2;
	
	Tobjects objects;
	
	window = prepareGLFW(512, 512);
	
	prepareGLEW();
	
	createShader(&shaderBlue,        "./src/shaders/shader.vert", "./src/shaders/blue.frag");
	createShader(&shaderRed,         "./src/shaders/shader.vert", "./src/shaders/red.frag");
	createShader(&shaderVecCol,      "./src/shaders/shader.vert", "./src/shaders/vecCol.frag");
	createShader(&shaderCycle,       "./src/shaders/shader.vert", "./src/shaders/cycle.frag");
	createShader(&shaderCustom,      "./src/shaders/shader.vert", "./src/shaders/custom.frag");
	createShader(&shaderTest,        "./src/shaders/shader.vert", "./src/shaders/green.frag");
	createShader(&shaderTex,         "./src/shaders/shader.vert", "./src/shaders/tex.frag");
	createShader(&shaderTexColor,    "./src/shaders/shader.vert", "./src/shaders/texColor.frag");
	createShader(&shaderTexDuo,      "./src/shaders/shader.vert", "./src/shaders/texDuo.frag");
	createShader(&shaderTexDuoColor, "./src/shaders/shader.vert", "./src/shaders/texDuoColor.frag");
	
	createTexture(&texture1, "./src/imgs/crate_01.png");
	createTexture(&texture2, "./src/imgs/awesomeface.png");
	
	GLfloat squares[10][33] =
	{
		{
			-0.9f, 0.9f, 0.0f,
			-0.7f, 0.9f, 0.0f,
			-0.7f, 0.7f, 0.0f,
			-0.9f, 0.7f, 0.0f
		},
		{
			-0.6f,  0.9f, 0.0f,
			-0.4f,  0.9f, 0.0f,
			-0.4f,  0.7f, 0.0f,
			-0.6f,  0.7f, 0.0f 
		},
		{
			-0.3f,  0.9f, 0.0f,
			-0.1f,  0.9f, 0.0f,
			-0.1f,  0.7f, 0.0f,
			-0.3f,  0.7f, 0.0f
		},
		{
			 // position				// color
			 0.0f,  0.9f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.2f,  0.9f, 0.0f,		0.0f, 1.0f, 0.0f,
			 0.2f,  0.7f, 0.0f,		0.0f, 0.0f, 1.0f,
			 0.0f,  0.7f, 0.0f,		0.0f, 0.0f, 0.0f
		},
		{
			 0.3f,  0.9f, 0.0f,
			 0.5f,  0.9f, 0.0f,
			 0.5f,  0.7f, 0.0f,
			 0.3f,  0.7f, 0.0f
		},
		{
			 0.6f,  0.9f, 0.0f,
			 0.8f,  0.9f, 0.0f,
			 0.8f,  0.7f, 0.0f,
			 0.6f,  0.7f, 0.0f
		},
		{
			 // position			// texture coords
			-0.9f, 0.6f, 0.0f,		1.0f, 0.0f,
			-0.7f, 0.6f, 0.0f,		0.0f, 0.0f,
			-0.7f, 0.4f, 0.0f,		0.0f, 1.0f,
			-0.9f, 0.4f, 0.0f,		1.0f, 1.0f
		},
		{
			 // position				// color		// texture coords
			-0.6f, 0.6f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
			-0.4f, 0.6f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
			-0.4f, 0.4f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
			-0.6f, 0.4f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f
		},
		{
			 // position			// texture coords
			-0.3f, 0.6f, 0.0f,		1.0f, 0.0f,
			-0.1f, 0.6f, 0.0f,		0.0f, 0.0f,
			-0.1f, 0.4f, 0.0f,		0.0f, 1.0f,
			-0.3f, 0.4f, 0.0f,		1.0f, 1.0f
		},
		{
			 // position				// color		// texture coords
			 0.0f, 0.6f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
			 0.2f, 0.6f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
			 0.2f, 0.4f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
			 0.0f, 0.4f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f
		},
	};
	
	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	
	glGenVertexArrays(10, objects.VAO);
	glGenBuffers(10, objects.VBO);
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(objects.VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[4]), squares[4], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glEnableVertexAttribArray(0);

	glBindVertexArray(objects.VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[5]), squares[5], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(objects.VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[6]), squares[6], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);

	glBindVertexArray(objects.VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[7]), squares[7], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	
	glBindVertexArray(objects.VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[8]), squares[8], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	
	glBindVertexArray(objects.VAO[9]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[9]), squares[9], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	GLfloat timeValue;
	GLfloat greenValue;
	GLfloat blueValue;
	GLint vertexColorLocation;
	
	while (!glfwWindowShouldClose(window))
	{
		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		blueValue = (sin(timeValue) / 4.0f) + 0.5f;
		vertexColorLocation = glGetUniformLocation(shaderCycle.ID, "ourColor");
		processInput(window);
		/* Render here */
		glClearColor(0.0f, 0.15f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		useShader(shaderRed);
		glBindVertexArray(objects.VAO[0]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderVecCol.ID);
		glBindVertexArray(objects.VAO[1]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderCycle.ID);
		glUniform4f(vertexColorLocation, 0.3f, greenValue, blueValue, 1.0f);
		glBindVertexArray(objects.VAO[2]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderCustom.ID);
		glBindVertexArray(objects.VAO[3]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderBlue.ID);
		glBindVertexArray(objects.VAO[4]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		useShader(shaderTest);
		glBindVertexArray(objects.VAO[5]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		useShader(shaderTex);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glBindVertexArray(objects.VAO[6]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		useShader(shaderTexColor);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glBindVertexArray(objects.VAO[7]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		useShader(shaderTexDuo);
		setInt(shaderTexDuo, "texture1", 0);
		setInt(shaderTexDuo, "texture2", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);
		glBindVertexArray(objects.VAO[8]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		useShader(shaderTexDuoColor);
		setInt(shaderTexDuo, "texture1", 0);
		setInt(shaderTexDuo, "texture2", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);
		glBindVertexArray(objects.VAO[9]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteVertexArrays(8, objects.VAO);
	glDeleteBuffers(8, objects.VBO);
	
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

