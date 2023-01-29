#include "./source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int i;
	Tobject cube, light1, light2;
	Tcamera camera;
	float delta, lastFrame, currentFrame, angle;
	unsigned int objsShader, lightShader, texture, specularTexture, emmisionTexture;
	unsigned int lightVAO;
	vec3 lightPos1 = {0.9f, 2.0f, 0.0f};
	vec3 lightPos2 = {2.0f, 0.0f, 1.0f};
	vec3 lightSize = {0.3f, 0.3f, 0.3f};
	vec3 lightColor1 = {0.2f, 0.9f, 0.5f};
	vec3 lightColor2 = {0.9f, 0.2f, 0.5f};
	vec3 cubeAngle = {1.0f, 0.0f, 0.0f};
	
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
	
	window = prepareGLFW(400, 400);
	lastFrame = 0;
	
	
	prepareGLEW();
	createShader(&objsShader, "./source/shaders/perspective.vert", "./source/shaders/lightmapEmmisiveMultipleLights.frag");
	createShader(&lightShader, "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createTexture(&texture, "./source/images/crate_03.png");
	createTexture(&specularTexture, "./source/images/container2_specular.png");
	createTexture(&emmisionTexture, "./source/images/crate_03_emmision.png");
	
	instance_create_cube(&cube, 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light1, 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light2, 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	
	init_camera(&camera, 1366, 768, 2);
	
	glm_mat4_identity(light1.model);
	glm_translate(light1.model, lightPos1);
	glm_scale(light1.model, lightSize);
	
	glm_mat4_identity(light2.model);
	glm_translate(light2.model, lightPos2);
	glm_scale(light2.model, lightSize);
	
	glm_rotate(cube.model, 89.90f, cubeAngle);
	
	
	bind_texture(&objsShader, texture, "material.diffuse", 0);
	bind_texture(&objsShader, specularTexture, "material.specular", 1);
	bind_texture(&objsShader, emmisionTexture, "material.emmision", 2);
	
	setVec3(&objsShader, "light[0].diffuse", lightColor1[0], lightColor1[1], lightColor1[2]);
	setVec3(&objsShader, "light[0].specular", lightColor1[0], lightColor1[1], lightColor1[2]);
	
	setVec3(&objsShader, "light[1].diffuse", lightColor2[0], lightColor2[1], lightColor2[2]);
	setVec3(&objsShader, "light[1].specular", lightColor2[0], lightColor2[1], lightColor2[2]);
	
	setInt(&objsShader, "n_lights", 2);
	
	setVec3(&objsShader, "light[0].ambient", 0.2f, 0.2f, 0.2f);
	setVec3(&objsShader, "light[1].ambient", 0.2f, 0.2f, 0.2f);
	setFloat(&objsShader, "material.shininess", 32.0f);
	
	setFloat(&objsShader, "light[0].constant", 1.0f);
	setFloat(&objsShader, "light[0].linear", 0.09f);
	setFloat(&objsShader, "light[0].quadratic", 0.032f);
	
	setFloat(&objsShader, "light[1].constant", 1.0f);
	setFloat(&objsShader, "light[1].linear", 0.09f);
	setFloat(&objsShader, "light[1].quadratic", 0.032f);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		
		glClearColor(0.050f, 0.046f, 0.120f, 1.0f);
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, camera.frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		run_camera(&camera, window, delta);
		
		/*
		lightPos[0] = 0.2f;
		lightPos[1] = 1.0f;
		lightPos[2] = 0.3f;
		*/
		lightPos1[0] = sin(glfwGetTime()) * 2.0f;
		lightPos1[1] = cos(glfwGetTime()) * 2.0f;
		lightPos1[2] = sin(glfwGetTime() / 4.0f) * 1.0f;

		lightPos2[0] = cos(glfwGetTime()) * 2.0f;
		lightPos2[1] = cos(glfwGetTime() / 4.0f) * 2.0f;
		lightPos2[2] = sin(glfwGetTime()) * 2.0f;
		glm_mat4_identity(light1.model);
		glm_translate(light1.model, lightPos1);
		glm_scale(light1.model, lightSize);

		glm_mat4_identity(light2.model);
		glm_translate(light2.model, lightPos2);
		glm_scale(light2.model, lightSize);
		
		/*
		setVec3(&objsShader, "material.ambient", 0.3f, 0.7f, 0.6f);
		setVec3(&objsShader, "material.diffuse", 0.3f, 0.7f, 0.6f);
		setVec3(&objsShader, "material.specular", 0.5f, 0.5f, 0.5f);
		*/
		
		/*
		setVec3(&objsShader, "lightPos", lightPos[0], lightPos[1], lightPos[2]);
		setVec3(&objsShader, "light.position", lightPos[0], lightPos[1], lightPos[2]);
		*/
		setVec4(&objsShader, "light[0].lightVector", lightPos1[0], lightPos1[1], lightPos1[2], 0.0f);
		setVec4(&objsShader, "light[1].lightVector", lightPos2[0], lightPos2[1], lightPos2[2], 0.0f);
		/*
		spotlight
		setVec3(&objsShader, "light.position", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&objsShader, "light.direction", camera.front[0], camera.front[1], camera.front[2]);
		setFloat(&objsShader, "light.cutOff", cos(glm_rad(12.5f)));
		setFloat(&objsShader, "light.outerCutOff", cos(glm_rad(17.5f)));
		*/
		setVec3(&objsShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
		
		setFloat(&objsShader, "material.emmisionStr", sin(glfwGetTime() * 2.0f) + 1.0f);
		
		
		
		/*
		setVec3(&objsShader, "lightColor", 1.0f, 1.0f, 1.0f);
		*/
		
		for(i = 0; i < 10; i++)
		{
			glm_mat4_identity(cube.model);
			glm_translate(cube.model, cubePositions[i]);
			angle = 10.0f * (i + 1);
			glm_rotate(cube.model, glm_rad(angle * glfwGetTime()), lightColor1);
			
			instance_draw(cube, &objsShader, camera, texture, 1);
		}
		
		setVec3(&lightShader, "color", lightColor1[0], lightColor1[1], lightColor1[2]);
		instance_draw(light1, &lightShader, camera, texture, 0);
		setVec3(&lightShader, "color", lightColor2[0], lightColor2[1], lightColor2[2]);
		instance_draw(light2, &lightShader, camera, texture, 0);
		
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
