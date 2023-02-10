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
	unsigned int floorDiffuse, crateDiffuse, crateSpecular;
	unsigned int lightShader, objectShader, instanceShader, debugDepthShader;
	unsigned int depthCubeShader, pointShadowShader;
	unsigned int uboMatrices;
	unsigned int tilesAmount = 128;
	const unsigned SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float tilesAmountSquared = 0;
	mat4 tilesMatrices[128];
	unsigned int cratesAmount = 3;
	mat4 cratesMatrices[3];
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
	GLenum face;
	
	unsigned int depthMapFBO;
	unsigned int depthCubeMap;
	float nearPlane = 1.0f, farPlane = 10.5f;
	mat4 lightProjection;
	mat4 lightView;
	mat4 lightSpaceMatrix;
	float aspect, near, far;
	mat4 shadowProj;
	mat4 shadowTransforms[6];
	mat4 matHelper;
	vec3 vecHelper;
	vec3 view[3] =
	{
		{-2.0f, 4.0f, -1.0f},
		{ 0.0f, 0.0f,  0.0f},
		{ 0.0f, 1.0f,  0.0f},
	};
	vec3 lookatVecs[6] =
	{
		{ 1.0f, 0.0f,  0.0f},
		{-1.0f, 0.0f,  0.0f},
		{ 0.0f, 1.0f,  0.0f},
		{ 0.0f,-1.0f,  0.0f},
		{ 0.0f, 0.0f,  1.0f},
		{ 0.0f, 0.0f, -1.0f},
	};
	
	window = prepareGLFW(800, 600);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	prepareGLEW();
	init_config(&config, 1920, 1080);
	init_camera(&camera, config, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/newInstancedPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	createShader(&debugDepthShader, "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/debuQuadDepth.frag");
	createShader_geometry(&depthCubeShader, "./source/shaders/pointShadowDepthMap/vertex.vert", "./source/shaders/pointShadowDepthMap/fragment.frag", "./source/shaders/pointShadowDepthMap/geometry.geo");
	createShader(&pointShadowShader, "./source/shaders/rendPointShadow/vertext.vert", "./source/shaders/rendPointShadow/fragmentTest.frag");
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 1);
	createTexture(&floorDiffuse, "./source/images/grass_02.png", 1, 1);
	createTexture(&crateDiffuse, "./source/images/crate_03.png", 1, 1);
	createTexture(&crateSpecular, "./source/images/container2_specular.png", 1, 1);
	
	instance_create_cube(&light, 0.0f, 1.0f, 0.0f, 100, 100, 100, 0.5f, 0);
	instance_create_quad(&tile_floor, 0.0f, 0.0f, 0.0f, 100, 100, 1.0f, 2);
	instance_create_cube(&crate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 0.5f, 2);
	instance_create_cube(&probCrate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 0.5f, 2);
	
	prepare_light(&objectShader	 , &lightShader, 0, lightColor, 0.6f, 0.0028f, 0.000014f);
	prepare_light(&instanceShader, &lightShader, 0, lightColor, 0.3f, 0.007f, 0.0002f);
	prepare_light(&pointShadowShader, &lightShader, 0, lightColor, 0.3f, 0.007f, 0.0002f);
	
	setInt(&objectShader, "material.diffuse", 0);
	setInt(&objectShader, "material.specular", 1);
	setFloat(&objectShader, "material.shininess", 32.0f);
	setVec4(&objectShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0);
	setInt(&objectShader, "n_lights", 1);
	
	setInt(&instanceShader, "material.diffuse", 0);
	setFloat(&instanceShader, "material.shininess", 32.0f);
	setVec4(&instanceShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0);
	setInt(&instanceShader, "n_lights", 1);

	setInt(&pointShadowShader, "material.diffuse", 0);
	setInt(&pointShadowShader, "material.specular", 2);
	setFloat(&pointShadowShader, "material.shininess", 32.0f);
	setVec3(&pointShadowShader, "light[0].diffuse", lightColor[0], lightColor[1], lightColor[2]);
	setVec3(&pointShadowShader, "light[0].ambient", lightColor[0]*0.7, lightColor[1]*0.7, lightColor[2]*0.7);
	setInt(&pointShadowShader, "n_lights", 1);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	prepare_uniformblockData(&pointShadowShader, "Matrices");
	
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
			tile_floor.pos[1] = -2.5001f;
			tile_floor.pos[2] = y;
			glm_translate(tile_floor.model, tile_floor.pos);
			glm_rotate(tile_floor.model, glm_rad(-90.0f), floorRot);
			glm_mat4_copy(tile_floor.model, tilesMatrices[i]);
			i++;
		}
	}
	

	seed = time(NULL);
	srand(1675999174);
	for (i = 0; i < cratesAmount; i++)
	{
		glm_vec3_fill(crate.scale, ((random()%20)/20.0f)+0.5f);
		glm_mat4_identity(crate.model);
		crate.pos[0] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		crate.pos[1] = -2.5f+crate.scale[0]/2;
		crate.pos[2] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		glm_translate(crate.model, crate.pos);
		crateRot[0] = 1;
		crateRot[2] = 0;
		glm_rotate(crate.model, glm_rad(-90.0f), crateRot);
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
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	aspect = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;
	near = 0.01f;
	far = 35.0f;
	
	glm_perspective(glm_rad(90.0f), aspect, near, far, shadowProj);
	
	setInt(&pointShadowShader, "depthMap", 1);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		camera.currentFrame = (float)glfwGetTime();
		camera.deltaTime = camera.currentFrame - camera.lastFrame;
		camera.lastFrame = camera.currentFrame;
		
		/*
		light.pos[0] = sin(glfwGetTime()/2)*6;
		*/
		light.pos[0] = sin(glfwGetTime()/2.0f)*6.0f;
		light.pos[1] = sin(glfwGetTime()*2.0f);
		light.pos[2] = cos(glfwGetTime()/2.0f)*6.0f;
		
		glm_perspective(glm_rad(90.0f), aspect, near, far, shadowProj);
		
		glm_vec3_add(light.pos, lookatVecs[0], vecHelper);
		glm_lookat(light.pos, vecHelper, lookatVecs[3], matHelper);
		glm_mul(shadowProj, matHelper, shadowTransforms[0]);
		
		glm_vec3_add(light.pos, lookatVecs[1], vecHelper);
		glm_lookat(light.pos, vecHelper, lookatVecs[3], matHelper);
		glm_mul(shadowProj, matHelper, shadowTransforms[1]);
		
		glm_vec3_add(light.pos, lookatVecs[2], vecHelper);
		glm_lookat(light.pos, vecHelper, lookatVecs[4], matHelper);
		glm_mul(shadowProj, matHelper, shadowTransforms[2]);
		
		glm_vec3_add(light.pos, lookatVecs[3], vecHelper);
		glm_lookat(light.pos, vecHelper, lookatVecs[5], matHelper);
		glm_mul(shadowProj, matHelper, shadowTransforms[3]);
		
		glm_vec3_add(light.pos, lookatVecs[4], vecHelper);
		glm_lookat(light.pos, vecHelper, lookatVecs[3], matHelper);
		glm_mul(shadowProj, matHelper, shadowTransforms[4]);
		
		glm_vec3_add(light.pos, lookatVecs[5], vecHelper);
		glm_lookat(light.pos, vecHelper, lookatVecs[3], matHelper);
		glm_mul(shadowProj, matHelper, shadowTransforms[5]);
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		
		useShader(&depthCubeShader);
		setMat4(&depthCubeShader, "shadowMatrices[0]", shadowTransforms[0]);
		setMat4(&depthCubeShader, "shadowMatrices[1]", shadowTransforms[1]);
		setMat4(&depthCubeShader, "shadowMatrices[2]", shadowTransforms[2]);
		setMat4(&depthCubeShader, "shadowMatrices[3]", shadowTransforms[3]);
		setMat4(&depthCubeShader, "shadowMatrices[4]", shadowTransforms[4]);
		setMat4(&depthCubeShader, "shadowMatrices[5]", shadowTransforms[5]);
		setFloat(&depthCubeShader, "far_plane", far);
		setVec3(&depthCubeShader, "lightPos", light.pos[0], light.pos[1], light.pos[2]);
		
		useShader(&depthCubeShader);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (tile_floor.indices_n * 3), GL_UNSIGNED_INT, 0, tilesAmount);
		
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
		
		setVec3(&lightShader, "color", lightColor[0], lightColor[1], lightColor[2]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		instance_draw(light, &lightShader, camera);
		
		setMat4(&pointShadowShader, "projection", camera.projection);
		setMat4(&pointShadowShader, "view", camera.view);
		setVec4(&pointShadowShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0.0f);
		setVec3(&pointShadowShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setFloat(&pointShadowShader, "far_plane", far);
		
		bind_texture(&pointShadowShader, floorDiffuse, 0);
		bind_texture(&pointShadowShader, floorDiffuse, 2);
		useShader(&pointShadowShader);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (tile_floor.indices_n * 3), GL_UNSIGNED_INT, 0, tilesAmount);
		
		bind_texture(&pointShadowShader, crateDiffuse, 0);
		bind_texture(&pointShadowShader, crateSpecular, 2);
		glBindVertexArray(crate.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (crate.indices_n * 3), GL_UNSIGNED_INT, 0, cratesAmount);
		
		
		/*
		useShader(&debugDepthShader);
		setFloat(&debugDepthShader, "near_plane", nearPlane);
		setFloat(&debugDepthShader, "far_plane", farPlane);
		*/
		
		#if 0
		render.texcolBuffer = depthMap;
		run_rendererShader(&render, &debugDepthShader, camera);
		#endif
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

