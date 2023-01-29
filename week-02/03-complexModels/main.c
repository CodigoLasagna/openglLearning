#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int i, j;
	Tobject light[2];
	Tcamera camera;
	Tmodel apple;
	float delta, lastFrame, currentFrame, angle;
	unsigned int lightShader, lightShader2, texture, specularTexture, emmisionTexture;
	unsigned int texture_backpack;
	unsigned int modShader;
	unsigned int lightVAO;
	vec3 lightPos = {1.9f, 1.9f, 0.0f};
	vec3 lightPos2 = {-1.9f, -1.9f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor = {1.0f, 1.0f, 1.0f};
	vec3 lightColor2 = {0.0f, 0.05f, 0.1f};
	vec3 globalAmbience = {0.05f, 0.15f, 0.3f};
	vec3 modelSize = {1.0f, 1.0f, 1.0f};
	vec3 modelPos = {0.0f, 1.0f, 0.0f};
	mat4 model;
	
	window = prepareGLFW(400, 400);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 2);
	
	createShader(&modShader, "./source/shaders/modPer.vert", "./source/shaders/lightmapEmmisiveMultipleLights.frag");
	createShader(&lightShader, "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createShader(&lightShader2, "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createTexture(&texture, "./source/images/apple.png", 0);
	createTexture(&specularTexture, "./source/images/apple_specular.png", 0);
	createTexture(&emmisionTexture, "./source/images/Night_lights_2K.png", 0);
	
	load_model(&apple, "./source/models/apple3.obj", 0.0f, 0.0f, 0.0f, 1);
	instance_create_cube(&light[0], lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	instance_create_cube(&light[1], lightPos2[0], lightPos2[1], lightPos2[2], 100, 100, 100, 1);
	
	bind_texture(&modShader, texture, "material.diffuse", 0);
	bind_texture(&modShader, specularTexture, "material.specular", 1);
	/*
	bind_texture(&modShader, emmisionTexture, "material.emmision", 2);
	*/
	prepare_light(&modShader, &lightShader, 0, lightColor);
	prepare_light(&modShader, &lightShader2, 1, lightColor2);
	
	setFloat(&modShader, "material.shininess", 32.0f);
	setVec4(&modShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec4(&modShader, "light[1].lightVector", lightPos2[0], lightPos2[1], lightPos2[2], 0.0f);
	
	setVec3(&modShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	setVec3(&modShader, "light[1].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	
	setInt(&modShader, "n_lights", 1);
	
	
	glm_scale(light[0].model, lightSize);
	glm_scale(light[1].model, lightSize);
	
	
	glm_mat4_identity(model);
	glm_scale(model, modelSize);
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		
		glClearColor(0.1f, 0.0f, 0.2f, 1.0f);
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, camera.frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		run_camera(&camera, window, delta);
		
		instance_draw(light[0], &lightShader, camera);
		
		setVec3(&modShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		glm_rotate(apple.model, glm_rad(0.3f), modelPos);
		
		draw_model(&apple, &modShader, camera);
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	return 0;
}
