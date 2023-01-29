#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int x, y, i, j, k;
	Tobject light, tile_floor, crate, grass, glass_window;
	Tcamera camera;
	Tmodel crash;
	float delta, lastFrame, currentFrame, angle, distance;
	unsigned int tileTexture, crateTexture, crateTextureSpec, grassTexture, glassTexture, crashTexture;
	unsigned int lightShader, primitiveShader, modelShader, alphaShader;
	vec3 lightPos = {0.0f, 2.5f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor = {0.9f, 0.8f, 0.7f};
	vec3 globalAmbience = {0.5f, 0.4f, 0.3f};
	vec3 modelPos = {1.0f, 0.0f, 0.0f};
	vec3 crashSize = {0.01f, 0.01f, 0.01f};
	vec3 grassSize = {1.38f, 1.38f, 1.38f};
	vec3 glassSize = {1.0f, 2.0f, 1.0f};
	vec4 temp;
	
	vec3 cratePos[] =
	{
		{ 1.0f, -2.0f, -5.0f},
		
		{-3.0f, -2.0f, -3.0f},
		{-4.0f, -2.0f, -3.0f},
		{-4.0f, -1.0f, -3.0f},
		{-4.0f, -2.0f, -2.0f},
		
		{ 5.0f, -2.0f,  0.0f},
		{ 4.0f, -2.0f, -1.0f},
		{-5.0f, -2.0f,  2.0f},
	};

	vec4 grassPos[] =
	{
		{ 1.0f, -1.81f, -4.49f, 0},
		{-3.0f, -1.81f, -2.49f, 0},
		{-4.0f, -1.81f, -1.49f, 0},
		{ 1.5f, -1.81f, -1.0f, 0},
		{ 4.0f, -1.81f, -0.49f, 0},
		{-2.0f, -1.81f,  1.0f, 0},
		{ 0.5f, -1.81f,  2.0f, 0},
		{ 0.0f, -1.5f,  5.0f, 1},
		{ 2.0f, -1.5f,  3.0f, 1},
		{-2.0f, -1.5f, -5.0f, 1},
	};
	
	window = prepareGLFW(400, 400);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 2);
	
	createShader(&lightShader, "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createShader(&primitiveShader, "./source/shaders/perspective.vert", "./source/shaders/surfaceLight.frag");
	createShader(&modelShader, "./source/shaders/modPer.vert", "./source/shaders/surfaceLight.frag");
	createShader(&alphaShader, "./source/shaders/perspective.vert", "./source/shaders/fullTrans.frag");
	
	createTexture(&tileTexture, "./source/images/grass_02.png", 1);
	createTexture(&crateTexture, "./source/images/crate_03.png", 1);
	createTexture(&crateTextureSpec, "./source/images/container2_specular.png", 1);
	createTexture(&crashTexture, "./source/images/color_pallete.png", 1);
	createTexture(&grassTexture, "./source/images/foliage_01.png", 2);
	createTexture(&glassTexture, "./source/images/window_01.png", 2);
	
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	instance_create_quad(&tile_floor, 0, 0, 0, 100, 100, 1);
	instance_create_cube(&crate, 0, 0, 0, 100, 100, 100, 1);
	instance_create_quad(&grass, 0, 0, 0, 100, 100, 1);
	instance_create_quad(&glass_window, 0, -1.5, 5, 100, 100, 1);
	
	load_model(&crash, "./source/models/crashbandicoot.obj", 0.0f, -2.57f, 0.0f, 1);
	
	prepare_light(&primitiveShader, &lightShader, 0, lightColor);
	prepare_light(&modelShader, &lightShader, 0, lightColor);
	prepare_light(&alphaShader, &lightShader, 0, lightColor);
	
	setInt(&primitiveShader, "material.diffuse", 0);
	setInt(&primitiveShader, "material.specular", 1);
	setFloat(&primitiveShader, "material.shininess", 32.0f);
	setVec4(&primitiveShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&primitiveShader, "light[0].ambient", globalAmbience[0]*5, globalAmbience[1]*5, globalAmbience[2]*5);
	setInt(&primitiveShader, "n_lights", 1);
	
	setInt(&modelShader, "material.diffuse", 0);
	setFloat(&modelShader, "material.shininess", 32.0f);
	setVec4(&modelShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&modelShader, "light[0].ambient", globalAmbience[0]*2.5f, globalAmbience[1]*2.5f, globalAmbience[2]*2.5f);
	setInt(&modelShader, "n_lights", 1);
	
	setInt(&alphaShader, "material.diffuse", 0);
	setFloat(&alphaShader, "material.shininess", 32.0f);
	setVec4(&alphaShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&alphaShader, "light[0].ambient", globalAmbience[0]*2.5f, globalAmbience[1]*2.5f, globalAmbience[2]*2.5f);
	setInt(&alphaShader, "n_lights", 1);
	
	glm_scale(light.model, lightSize);
	glm_scale(crash.model, crashSize);
	glm_scale(glass_window.model, glassSize);
	
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
		
		setVec3(&lightShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&primitiveShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
		instance_draw(light, &lightShader, camera);
		
		bind_texture(&primitiveShader, tileTexture, 0);
		tile_floor.pos[1] = -2.51;
		for (y = -10; y < 10; y++)
		{
			for (x = -10; x < 10; x++)
			{
				tile_floor.pos[0] = x;
				tile_floor.pos[2] = y;
				glm_mat4_identity(tile_floor.model);
				glm_translate(tile_floor.model, tile_floor.pos);
				glm_rotate(tile_floor.model, glm_rad(90.0f), modelPos);
				instance_draw(tile_floor, &primitiveShader, camera);
			}
		}
		
		bind_texture(&primitiveShader, crateTexture, 0);
		bind_texture(&primitiveShader, crateTextureSpec, 1);
		for (i = 0; i < sizeof(cratePos) / sizeof(cratePos[0]); i++)
		{
			glm_mat4_identity(crate.model);
			glm_translate(crate.model, cratePos[i]);
			glm_rotate(crate.model, glm_rad(90.0f), modelPos);
			instance_draw(crate, &primitiveShader, camera);
		}
		
		bind_texture(&modelShader, crashTexture, 0);
		draw_model(&crash, &modelShader, camera);
		
		for (i = 0; i < (sizeof(grassPos) / sizeof(grassPos[0])) - 1; i ++)
		{
			for (j = 0; j < (sizeof(grassPos) / sizeof(grassPos[0])) - i - 1; j ++)
			{
				if (glm_vec3_distance(camera.pos, grassPos[j]) < glm_vec3_distance(camera.pos, grassPos[j+1]))
				{
					glm_vec4_copy(grassPos[j], temp);
					glm_vec4_copy(grassPos[j+1], grassPos[j]);
					glm_vec4_copy(temp, grassPos[j+1]);
				}
			}
		}
		
		for (i = 0; i < sizeof(grassPos) / sizeof(grassPos[0]); i++)
		{
			if (grassPos[i][3])
			{
				bind_texture(&alphaShader, glassTexture, 0);
				glm_mat4_identity(glass_window.model);
				glm_translate(glass_window.model, grassPos[i]);
				glm_scale(glass_window.model, glassSize);
				instance_draw(glass_window, &alphaShader, camera);
			}
			else
			{
				bind_texture(&alphaShader, grassTexture, 0);
				glm_mat4_identity(grass.model);
				glm_translate(grass.model, grassPos[i]);
				glm_scale(grass.model, grassSize);
				instance_draw(grass, &alphaShader, camera);
			}
		}
		
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	return 0;
}
