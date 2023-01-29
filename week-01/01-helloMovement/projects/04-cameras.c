#pragma once

#include "../src/init.h"
#include "../src/shaders.h"
#include "../src/textures.h"

#include <math.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>


int init()
{
	GLFWwindow* window;
	Tconfig config;
	Tshader shader;
	
	Ttexture texture1, texture2;
	
	Tobjects objects;
	int inputRes[] = {1366, 768};
	//int outputRes[] = {1366, 768};
	int outputRes[] = {340, 192};
	
	window = prepareGLFW(inputRes[0], inputRes[1]);
	config.cameraSpeed = 0.05f;
	config.walk_w = 0;
	config.walk_s = 0;
	config.walk_a = 0;
	config.walk_d = 0;
	config.perspective = 1;
	
	
	prepareGLEW();
	
	createShader(&shader, "./src/shaders/per_proj.vert", "./src/shaders/dither2.frag");
	
	createTexture(&texture1, "./src/imgs/crate_03.png");
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
	vec3 cameraPos = {0.0f, 0.0f, 5.0f};
	vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
	vec3 up = {0.0f, 1.0f, 0.0f};
	vec3 cameraDirection;
	vec3 cameraUp;
	vec3 cameraRight;
	vec3 cameraFront = {0.0f, 0.0f, -1.0f};
	vec3 cameraForward;
	vec3 Helper;
	vec3 direction;

	glm_vec3_sub(cameraPos, cameraTarget, cameraDirection);
	glm_normalize(cameraDirection);
	glm_cross(up, cameraDirection, cameraRight);
	glm_cross(cameraDirection, cameraRight, cameraUp);
	
	
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

	GLint modelLoc, viewLoc, i = 0;
	GLfloat angle = 20.0f, radius = 10.0f, camX, camZ;
	GLfloat deltaTime = 0.0f, lastFrame = 0.0f, currentFrame;
	config.yaw = -90.0f;
	config.pitch = 0.0f;
	config.lastX =  720.0f / 2.0;
	config.lastY =  720.0 / 2.0;

	glEnable(GL_DEPTH_TEST);
	GLuint frameBuffer;
	

	GLuint rboColorId;
	glGenRenderbuffers(1, &rboColorId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, inputRes[0], inputRes[1]);
	
	// create a 4x MSAA renderbuffer object for depthbuffer
	GLuint rboDepthId;
	glGenRenderbuffers(1, &rboDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, inputRes[0], inputRes[1]);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
	                          GL_COLOR_ATTACHMENT0, // 2. color attachment point
	                          GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
	                          rboColorId);          // 4. rbo ID
	
	// attach depthbuffer image to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
	                          GL_DEPTH_ATTACHMENT,  // 2. depth attachment point
	                          GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
	                          rboDepthId);          // 4. rbo ID
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, &config);
		currentFrame = glfwGetTime();
		direction[0] = cos(glm_rad(config.yaw)) * cos(glm_rad(config.pitch));
		direction[1] = sin(glm_rad(config.pitch));
		direction[2] = sin(glm_rad(config.yaw)) * cos(glm_rad(config.pitch));
		glm_normalize_to(direction, cameraFront);
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		config.cameraSpeed = 2.5f*deltaTime;
		if(config.walk_w)
		{
			glm_vec3_muladds(cameraFront, config.cameraSpeed, cameraPos);
		}
		if(config.walk_s)
		{
			glm_vec3_muladds(cameraFront, -config.cameraSpeed, cameraPos);
		}
		if(config.walk_a)
		{
			glm_vec3_cross(cameraUp, cameraFront, Helper);
			glm_normalize(Helper);
			glm_vec3_muladds(Helper, config.cameraSpeed, cameraPos);
		}
		if(config.walk_d)
		{
			glm_cross(cameraUp, cameraFront, Helper);
			glm_vec3_normalize(Helper);
			glm_vec3_muladds(Helper, -config.cameraSpeed, cameraPos);
		}
		glm_vec3_add(cameraPos, cameraFront, cameraForward);
		/* Render here */
		glClearColor(0.050f, 0.046f, 0.120f, 1.0f);
		glViewport(0, 0, outputRes[0], outputRes[1]);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		mat4 view =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		camX = sin(glfwGetTime())* radius;
		camZ = cos(glfwGetTime())* radius;
		//glm_lookat((vec3){camX, 0.0f, camZ}, (vec3){0.0f, 0.0f, 0.0f}, cameraUp, view);
		glm_lookat(cameraPos, cameraForward, cameraUp, view);
		mat4 projection =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		
		viewLoc = glGetUniformLocation(shader.ID, "view");
		modelLoc = glGetUniformLocation(shader.ID, "model");
		
		if (config.perspective)
		{
			glm_perspective(glm_rad(45.0f), ((float) inputRes[0] / (float) inputRes[1]), 0.1f, 100.0f, projection);
		}
		else
		{
			glm_ortho(-((float)inputRes[0]/100)/2, ((float)inputRes[0]/100)/2, -((float)inputRes[1]/100)/2, ((float)inputRes[1]/100)/2, 0.1f, 100.0f, projection);
		}
		
		glm_translate(view, (vec3) {0.0f, 0.0f, -3.0f});
		
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		
		useShader(shader);
		setInt(shader, "texture1", 0);
		//setInt(shader, "texture2", 1);
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
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, outputRes[0], outputRes[1], 0, 0, inputRes[0], inputRes[1], GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, objects.VAO);
	glDeleteBuffers(1, objects.VBO);
	
	return 0;
}
