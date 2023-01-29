#pragma once

#include "../src/init.h"
#include "../src/shaders.h"
#include "../src/textures.h"

#include <cglm/util.h>
#include <math.h>
#include <cglm/cglm.h>


int init()
{
	GLFWwindow* window;
	vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
	mat4 trans = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	
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
	Tshader shaderTexRotScale;
	
	Ttexture texture1;
	Ttexture texture2;
	
	Tobjects objects;
	Tconfig config;
	
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
	createShader(&shaderTexRotScale, "./src/shaders/scaleRot.vert", "./src/shaders/tex.frag");
	
	createTexture(&texture1, "./src/imgs/crate_01.png");
	createTexture(&texture2, "./src/imgs/awesomeface.png");
	
	GLfloat squares[15][33] =
	{
		{
		  //  x     y      z
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
		{
			 // position			// texture coords
			-0.1f,-0.1f, 0.0f,		1.0f, 0.0f,
			 0.1f,-0.1f, 0.0f,		0.0f, 0.0f,
			 0.1f, 0.1f, 0.0f,		0.0f, 1.0f,
			-0.1f, 0.1f, 0.0f,		1.0f, 1.0f
		},
	};
	
	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	
	glGenVertexArrays(15, objects.VAO);
	glGenBuffers(15, objects.VBO);
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

	glBindVertexArray(objects.VAO[10]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares[10]), squares[10], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
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
		
		GLint transformLoc = glGetUniformLocation(shaderTexRotScale.ID, "transform");
		mat4 trans = 
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		glm_translate(trans, (vec3){0.4f, 0.5f, 0.0f});
		glm_rotate(trans, -(float)timeValue / 4, (vec3) {0.0f, 0.0f, 1.0f});
		glm_scale(trans, (vec3){0.8f, 0.8f, 1.0f});
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float *)trans);
		processInput(window, &config);
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
		
		useShader(shaderTexRotScale);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glBindVertexArray(objects.VAO[10]);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteVertexArrays(15, objects.VAO);
	glDeleteBuffers(15, objects.VBO);
	
	return 0;
}
