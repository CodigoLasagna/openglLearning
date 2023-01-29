#pragma once

#include "../src/init.h"
#include "../src/shaders.h"
#include "../src/textures.h"

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>


int init()
{
	GLFWwindow* window;
	Tshader shader;
	
	Ttexture texture1, texture2;
	
	Tobjects objects;

	Tconfig config;
	
	window = prepareGLFW(512, 512);
	
	prepareGLEW();
	
	createShader(&shader, "./src/shaders/per_proj.vert", "./src/shaders/texDuo.frag");
	
	createTexture(&texture1, "./src/imgs/crate_01.png");
	createTexture(&texture2, "./src/imgs/awesomeface.png");

	GLfloat cube[] =
	{
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 0.0f,
		
		-0.5f, -0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,
		
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 0.0f,
		
		 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,		0.0f, 0.0f,
		
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,		0.0f, 0.0f
	};
	
	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3,
		
		4, 5, 7,
		5, 6, 7,
		
		8, 9, 11,
		9, 10, 11,
		
		12, 13, 15,
		13, 14, 15,
		
		16, 17, 19,
		17, 18, 19,
		
		20, 21, 23,
		21, 22, 23,
	};
	vec3 cubePositions[] = {
		{ 0.0f,  0.0f,  0.0f}, 
		{ 2.0f,  5.0f, -15.0f}, 
		{-1.5f, -2.2f, -2.5f},  
		{-3.8f,  4.0f, -12.3f},  
		{ 2.4f, -0.4f, -3.5f},  
		{ 3.7f, -3.0f, -7.5f},  
		{-2.6f, -1.0f, -2.5f},  
		{ 1.5f,  2.0f, -2.5f}, 
		{ 2.5f,  0.2f, -1.5f}, 
		{-2.3f,  1.0f, -1.5f}  
	};
	
	glGenVertexArrays(1, objects.VAO);
	glGenBuffers(1, objects.VBO);
	glGenBuffers(1, &(objects.EBO));
	
	glBindVertexArray(objects.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, objects.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.EBO);
	
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLint modelLoc, viewLoc, i = 0, angle  = 20.0f;

	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, &config);
		/* Render here */
		glClearColor(0.0f, 0.15f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		mat4 view =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		mat4 projection =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		
		viewLoc = glGetUniformLocation(shader.ID, "view");
		modelLoc = glGetUniformLocation(shader.ID, "model");
		
		glm_perspective(glm_rad(45.0f), 128.0f / 72.0f, 0.1f, 100.0f, projection);
		glm_translate(view, (vec3) {0.0f, 0.0f, -3.0f});
		
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		
		useShader(shader);
		setInt(shader, "texture1", 0);
		setInt(shader, "texture2", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);
		for (i = 0; i < 10; i++)
		{
			mat4 model =
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
			glm_translate(model, cubePositions[i]);
			angle = 10.0f * (i+0.1);
			glm_rotate(model, (float) glfwGetTime() * glm_rad(angle), (vec3) {0.8f, 0.3f, 0.5f});
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *) model);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		}
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, objects.VAO);
	glDeleteBuffers(1, objects.VBO);
	
	return 0;
}
