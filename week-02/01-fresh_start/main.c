#include "./source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int i;
	Tobject cube;
	Tobject light[4];
	Tcamera camera;
	float delta, lastFrame, currentFrame, angle;
	unsigned int objsShader, lightShader[4], texture, specularTexture, emmisionTexture;
	unsigned int lightVAO;
	vec3 lightPos[4];
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor[] =
	{
		{0.0f, 0.2f, 1.0f},
		{1.0f, 0.0f, 0.2f},
		{0.1f, 1.0f, 0.1f},
		{0.5f, 0.5f, 0.1f},
	};
	vec3 cubeAngle = {1.0f, 0.0f, 0.0f};

	vec3 globalAmbience = {0.0f, 0.1f, 0.2f};
	
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
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 2);
	
	createShader(&objsShader, "./source/shaders/perspective.vert", "./source/shaders/lightmapEmmisiveMultipleLights.frag");
	createShader(&lightShader[0], "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createShader(&lightShader[1], "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createShader(&lightShader[2], "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createShader(&lightShader[3], "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createTexture(&texture, "./source/images/crate_03.png");
	createTexture(&specularTexture, "./source/images/container2_specular.png");
	createTexture(&emmisionTexture, "./source/images/crate_03_emmision.png");
	
	instance_create_cube(&cube, 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light[0], 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light[1], 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light[2], 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light[3], 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	
	bind_texture(&objsShader, texture, "material.diffuse", 0);
	bind_texture(&objsShader, specularTexture, "material.specular", 1);
	bind_texture(&objsShader, emmisionTexture, "material.emmision", 2);
	setFloat(&objsShader, "material.shininess", 32.0f);
	
	prepare_light(&objsShader, &lightShader[0], 0, lightColor[0]);
	prepare_light(&objsShader, &lightShader[1], 1, lightColor[1]);
	prepare_light(&objsShader, &lightShader[2], 2, lightColor[2]);
	prepare_light(&objsShader, &lightShader[3], 3, lightColor[3]);
	
	setInt(&objsShader, "n_lights", 4);
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
		
		lightPos[0][0] = sin(glfwGetTime()) * 2.0f;
		lightPos[0][1] = cos(glfwGetTime()) * 2.0f;
		lightPos[0][2] = sin(glfwGetTime() / 4.0f) * 1.0f;
		
		lightPos[1][0] = cos(glfwGetTime()) * 2.0f;
		lightPos[1][1] = cos(glfwGetTime() / 4.0f) * 2.0f;
		lightPos[1][2] = sin(glfwGetTime()) * 2.0f;
		
		lightPos[2][0] = sin(glfwGetTime() * 2);
		lightPos[2][1] = sin(glfwGetTime() / 2) * 2.0f;
		lightPos[2][2] = cos(glfwGetTime() / 2) * 2.0f;
		
		lightPos[3][0] = sin(glfwGetTime() / 2) * 2.0f;
		lightPos[3][1] = cos(glfwGetTime() / 2) * 2.0f;
		lightPos[3][2] = 0.0f;
		for (i = 0; i < 4; i++)
		{
			glm_mat4_identity(light[i].model);
			glm_translate(light[i].model, lightPos[i]);
			glm_scale(light[i].model, lightSize);
			instance_draw(light[i], &lightShader[i], camera);
		}
		
		setVec4(&objsShader, "light[0].lightVector", lightPos[0][0], lightPos[0][1], lightPos[0][2], 0.0f);
		setVec4(&objsShader, "light[1].lightVector", lightPos[1][0], lightPos[1][1], lightPos[1][2], 0.0f);
		setVec4(&objsShader, "light[2].lightVector", lightPos[2][0], lightPos[2][1], lightPos[2][2], 0.0f);
		setVec4(&objsShader, "light[3].lightVector", lightPos[3][0], lightPos[3][1], lightPos[3][2], 0.0f);
		
		setVec3(&objsShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
		for(i = 0; i < 10; i++)
		{
			glm_mat4_identity(cube.model);
			glm_translate(cube.model, cubePositions[i]);
			angle = 5.0f * (i + 1);
			glm_rotate(cube.model, glm_rad(90), cubeAngle);
			setFloat(&objsShader, "material.emmisionStr", sin(glfwGetTime() * (angle / 20)) + 1.0f);
			glm_rotate(cube.model, glm_rad((float)glfwGetTime() * angle), lightPos[0]);
			
			instance_draw(cube, &objsShader, camera);
		}
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
