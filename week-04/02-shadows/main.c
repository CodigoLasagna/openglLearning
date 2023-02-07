#include "source/engine/engine.h"
#include <time.h>

int main()
{
	Tconfig config;
	GLFWwindow *window;
	Tobject light;
	Tcamera camera;
	Trenderer render;
	Tskybox skybox;
	Tobject tile_floor, crate, probCrate;
	unsigned int floorDiffuse, crateDiffuse;
	unsigned int lightShader, modelShader, objectShader, instanceShader, simpleDepthShader, debugDepthShader, insShadowMapShader;
	unsigned int uboMatrices;
	unsigned int tilesAmount = 128;
	const unsigned SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float tilesAmountSquared = 0;
	mat4 tilesMatrices[128];
	unsigned int cratesAmount = 4;
	mat4 cratesMatrices[4];
	float radius = 30.0f;
	float offset = 10.5f;
	float angle, displacement;
	int x, y, z, i, j, k;
	vec3 pos;
	float scale;
	vec3 scaleGen;
	float rotAngle;
	unsigned int ibCrates, ibTiles, ibBushes;
	unsigned int tempVao;
	size_t vec4size;
	vec3 angleDir = {0.4f, 0.6f, 0.8f};
	vec3 lightColor = {0.7f, 0.8f, 1.0f};
	vec3 floorRot = {1.0f, 0.0f, 0.0f};
	vec3 crateRot = {1.0f, 0.0f, 0.0f};
	vec3 bushRot = {0.0f, 1.0f, 0.0f};
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	unsigned int seed;
	
	unsigned int depthMapFBO;
	unsigned int depthMap;
	float nearPlane = 1.0f, farPlane = 10.5f;
	mat4 lightProjection;
	mat4 lightView;
	mat4 lightSpaceMatrix;
	vec3 view[3] =
	{
		{-2.0f, 4.0f, -1.0f},
		{ 0.0f, 0.0f,  0.0f},
		{ 0.0f, 1.0f,  0.0f},
	};
	
	window = prepareGLFW(600, 800);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	prepareGLEW();
	init_config(&config, 1920, 1080);
	init_camera(&camera, config, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&modelShader, "./source/shaders/vertexShaders/modPer.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/newInstancedPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	createShader(&debugDepthShader, "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/debuQuadDepth.frag");
	createShader(&simpleDepthShader, "./source/shaders/vertexShaders/shadowMapthDepth.vert", "./source/shaders/fragmentShaders/empty.frag");
	createShader(&insShadowMapShader, "./source/shaders/vertexShaders/insShadowMap2.0.vert", "./source/shaders/fragmentShaders/surfaceLightShadows.frag");
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 1);
	createTexture(&floorDiffuse, "./source/images/grass_02.png", 1, 1);
	createTexture(&crateDiffuse, "./source/images/crate_03.png", 1, 1);
	
	instance_create_cube(&light, 0.0f, 3.0f, 5.0f, 100, 100, 100, 0.5f, 0);
	instance_create_quad(&tile_floor, 0.0f, 0.0f, 0.0f, 100, 100, 1.0f, 2);
	instance_create_cube(&crate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 0.5f, 2);
	instance_create_cube(&probCrate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 0.5f, 2);
	
	prepare_light(&modelShader	 , &lightShader, 0, lightColor, 0.6f, 0.022f, 0.0019f);
	prepare_light(&objectShader	 , &lightShader, 0, lightColor, 0.6f, 0.0028f, 0.000014f);
	prepare_light(&instanceShader, &lightShader, 0, lightColor, 0.3f, 0.007f, 0.0002f);
	prepare_light(&insShadowMapShader, &lightShader, 0, lightColor, 0.3f, 0.007f, 0.0002f);
	
	setInt(&modelShader, "material.diffuse", 0);
	setFloat(&modelShader, "material.shininess", 32.0f);
	setVec4(&modelShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0);
	setInt(&modelShader, "n_lights", 1);
	
	setInt(&objectShader, "material.diffuse", 0);
	setFloat(&objectShader, "material.shininess", 32.0f);
	setVec4(&objectShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0);
	setInt(&objectShader, "n_lights", 1);
	
	setInt(&instanceShader, "material.diffuse", 0);
	setFloat(&instanceShader, "material.shininess", 0.0f);
	setVec4(&instanceShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0); /*nota mental, 0 = luz de punto, 1 = luz direccional;*/
	setInt(&instanceShader, "n_lights", 1);

	setInt(&insShadowMapShader, "material.diffuse", 0);
	setFloat(&insShadowMapShader, "material.shininess", 0.0f);
	setVec4(&insShadowMapShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0);
	setInt(&insShadowMapShader, "n_lights", 1);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	prepare_uniformblockData(&insShadowMapShader, "Matrices");
	
	prepare_uniformblockMatrices(&uboMatrices, camera);
	
	prepare_renderer(&render, config);
	tilesAmountSquared = sqrt((float)tilesAmount)/2.0f;
	
	i = 0;
	for (y = -tilesAmountSquared; y < tilesAmountSquared; y++)
	{
		for (x = -tilesAmountSquared; x < tilesAmountSquared; x++)
		{
			glm_mat4_identity(tile_floor.model);
			tile_floor.pos[0] = x;
			tile_floor.pos[1] = -2.5f;
			tile_floor.pos[2] = y;
			glm_translate(tile_floor.model, tile_floor.pos);
			glm_rotate(tile_floor.model, glm_rad(-90.0f), floorRot);
			glm_mat4_copy(tile_floor.model, tilesMatrices[i]);
			i++;
		}
	}
	

	seed = time(NULL);
	srand(1675313989);
	/*1675313989*/
	for (i = 0; i < cratesAmount; i++)
	{
		glm_vec3_fill(crate.scale, ((random()%20)/10.0f)+0.8f);
		glm_mat4_identity(crate.model);
		crate.pos[0] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		crate.pos[1] = -2.5f+crate.scale[0]/2;
		crate.pos[2] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		glm_translate(crate.model, crate.pos);
		crateRot[0] = 1;
		crateRot[2] = 0;
		glm_rotate(crate.model, glm_rad(90.0f), crateRot);
		crateRot[0] = 0;
		crateRot[2] = 1;
		glm_rotate(crate.model, glm_rad(random()%360), crateRot);
		glm_scale(crate.model, crate.scale);
		glm_mat4_copy(crate.model, cratesMatrices[i]);
	}
	
	glGenBuffers(1, &ibTiles);
	glBindBuffer(GL_ARRAY_BUFFER, ibTiles);
	glBufferData(GL_ARRAY_BUFFER, tilesAmount * sizeof(mat4), &tilesMatrices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(tile_floor.VAO);
	vec4size = sizeof(vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (1 * vec4size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (2 * vec4size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (3 * vec4size));
	
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
	
	
	glGenBuffers(1, &ibCrates);
	glBindBuffer(GL_ARRAY_BUFFER, ibCrates);
	glBufferData(GL_ARRAY_BUFFER, cratesAmount * sizeof(mat4), &cratesMatrices[0], GL_STATIC_DRAW);
	glBindVertexArray(crate.VAO);
	vec4size = sizeof(vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (1 * vec4size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (2 * vec4size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (3 * vec4size));
	
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
	
	glGenFramebuffers(1, &depthMapFBO);
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	nearPlane = 0.0001f, farPlane = 25.0f;

	setInt(&insShadowMapShader, "material.diffuse", 0);
	setInt(&insShadowMapShader, "shadowMap", 1);
	
	
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		camera.currentFrame = (float)glfwGetTime();
		camera.deltaTime = camera.currentFrame - camera.lastFrame;
		camera.lastFrame = camera.currentFrame;
		
		light.pos[0] = sin(glfwGetTime()/2)*4;
		light.pos[2] = cos(glfwGetTime()/2)*4;
		useShader(&simpleDepthShader);
		glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane, lightProjection);
		glm_lookat(light.pos, view[1], view[2], lightView);
		glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);
		setMat4(&simpleDepthShader, "lightSpaceMatrix", lightSpaceMatrix);
		setVec3(&lightShader, "color", lightColor[0], lightColor[1], lightColor[2]);
		setVec3(&insShadowMapShader, "light[0].diffuse", lightColor[0], lightColor[1], lightColor[2]);
		setVec3(&insShadowMapShader, "light[0].ambient", lightColor[0], lightColor[1], lightColor[2]);
		setFloat(&(render.shader), "iTime", (float) glfwGetTime());
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		useShader(&simpleDepthShader);
		bind_texture(&simpleDepthShader, floorDiffuse, 0);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (tile_floor.indices_n * 3), GL_UNSIGNED_INT, 0, tilesAmount);
		
		bind_texture(&simpleDepthShader, crateDiffuse, 0);
		glBindVertexArray(crate.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (crate.indices_n * 3), GL_UNSIGNED_INT, 0, cratesAmount);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		run_uniformblockMatrices(uboMatrices, camera);
		run_camera(&camera, window);
		run_renderer(&render, camera);
		processInput(window, &config);
		
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm_mat4_identity(light.model);
		glm_translate(light.model, light.pos);
		glm_scale(light.model, light.scale);
		instance_draw(light, &lightShader, camera);
		
		bind_texture(&insShadowMapShader, depthMap, 1);
		setVec3(&insShadowMapShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec4(&insShadowMapShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0.0f);
		setMat4(&insShadowMapShader, "lightSpaceMatrix", lightSpaceMatrix);
		
		bind_texture(&insShadowMapShader, floorDiffuse, 0);
		useShader(&insShadowMapShader);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (tile_floor.indices_n * 3), GL_UNSIGNED_INT, 0, tilesAmount);
		
		bind_texture(&insShadowMapShader, crateDiffuse, 0);
		glBindVertexArray(crate.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (crate.indices_n * 3), GL_UNSIGNED_INT, 0, cratesAmount);
		
		useShader(&debugDepthShader);
		setFloat(&debugDepthShader, "near_plane", nearPlane);
		setFloat(&debugDepthShader, "far_plane", farPlane);
		#if 0
		render.texcolBuffer = depthMap;
		run_rendererShader(&render, &debugDepthShader, camera);
		#endif
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

