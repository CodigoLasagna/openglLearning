#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int i, j;
	Tobject light;
	Tcamera camera;
	Tmodel apple;
	float delta, lastFrame, currentFrame, angle;
	unsigned int lightShader, texture, specularTexture, emmisionTexture, outlineShader;
	unsigned int texture_backpack;
	unsigned int modShader;
	unsigned int lightVAO;
	vec3 lightPos = {1.9f, 1.9f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor = {1.0f, 1.0f, 1.0f};
	vec3 globalAmbience = {0.4f, 0.4f, 0.4f};
	vec3 modelSize1 = {1.0f, 1.0f, 1.0f};
	vec3 modelSize2 = {1.07f, 1.07f, 1.07f};
	vec3 modelPos = {0.0f, 1.0f, 0.0f};
	mat4 model;
	
	window = prepareGLFW(400, 400);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 2);
	
	createShader(&modShader, "./source/shaders/modPer.vert", "./source/shaders/surfaceLight.frag");
	createShader(&lightShader, "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createShader(&outlineShader, "./source/shaders/modPer.vert", "./source/shaders/outline.frag");
	createTexture(&texture, "./source/images/crystal_diffuse.png", 1);
	createTexture(&specularTexture, "./source/images/crystal_bright.png", 1);
	createTexture(&emmisionTexture, "./source/images/crystal_emissive.png", 1);
	
	load_model(&apple, "./source/models/crystal.fbx", 0.0f, 0.0f, 0.0f, 1);
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	
	bind_texture(&modShader, texture, "material.diffuse", 0);
	bind_texture(&modShader, specularTexture, "material.specular", 1);
	/*
	bind_texture(&modShader, emmisionTexture, "material.emmision", 2);
	*/
	prepare_light(&modShader, &lightShader, 0, lightColor);
	
	setFloat(&modShader, "material.shininess", 32.0f);
	setVec4(&modShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	
	setVec3(&modShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	
	setInt(&modShader, "n_lights", 1);
	
	
	glm_scale(light.model, lightSize);
	
	
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		run_camera(&camera, window, delta);
		
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, camera.frameBuffer);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		setVec3(&modShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&outlineShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
		glStencilMask(0xFF);
		instance_draw(light, &lightShader, camera);
		
		glm_mat4_identity(apple.model);
		glm_rotate(apple.model, glm_rad(glfwGetTime() * 10), modelPos);
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glm_scale(apple.model, modelSize1);
		draw_model(&apple, &modShader, camera);
		
		glm_scale(apple.model, modelSize2);
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		draw_model(&apple, &outlineShader, camera);
		
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	return 0;
}
