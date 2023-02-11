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
	Tobject tile_floor, crate;
	const unsigned SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int floorDiffuse, floorSpecular, crateDiffuse, crateSpecular;
	unsigned int lightShader, objectShader, instanceShader;
	unsigned int depthCubeShader, pointShadowShader;
	unsigned int uboMatrices;
	unsigned int tilesAmount = 61440;
	float tilesAmountSquared = 0;
	mat4 tilesMatrices[61440];
	unsigned int cratesAmount = 61440;
	mat4 cratesMatrices[61440];
	float objectsHeight = -5.5f;
	int x, y, z;
	unsigned int i, j, k;
	unsigned int ibCrates, ibTiles;
	size_t vec4size;
	vec3 angleDir = {0.4f, 0.6f, 0.8f};
	vec3 lightColor = {0.5f, 0.6f, 1.0f};
	vec3 adjustRot = {1.0f, 0.0f, 0.0f};
	vec3 crateRot = {0.0f, 0.0f, 1.0f};
	unsigned int seed;
	GLenum face;
	
	unsigned int depthMapFBO;
	unsigned int depthCubeMap;
	float aspect, near, far;
	mat4 shadowProj;
	mat4 shadowTransforms[6];
	mat4 matHelper;
	vec3 vecHelper;
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
	
	prepareGLEW();
	init_config(&config, 1920, 1080);
	init_camera(&camera, config, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/newInstancedPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	createShader_geometry(&depthCubeShader, "./source/shaders/pointShadowDepthMap/vertex.vert", "./source/shaders/pointShadowDepthMap/fragment.frag", "./source/shaders/pointShadowDepthMap/geometry.geo");
	createShader(&pointShadowShader, "./source/shaders/rendPointShadow/vertext.vert", "./source/shaders/rendPointShadow/fragmentTest.frag");
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 1);
	createTexture(&floorDiffuse, "./source/images/grass_02.png", 1, 1);
	createTexture(&floorSpecular, "./source/images/grass_02_specular.png", 1, 1);
	createTexture(&crateDiffuse, "./source/images/crate_03.png", 1, 1);
	createTexture(&crateSpecular, "./source/images/container2_specular.png", 1, 1);
	
	instance_create_cube(&light, 0.0f, 10.0f, 0.0f, 100, 100, 100, 0.5f, 0);
	instance_create_quad(&tile_floor, 0.0f, 0.0f, 0.0f, 100, 100, 1.0f, 2);
	instance_create_cube(&crate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 0.5f, 2);
	
	prepare_lightobj(&lightShader, lightColor);
	prepare_material(&objectShader, 1, 32.0f);
	prepare_material(&instanceShader, 1, 32.0f);
	prepare_material(&pointShadowShader, 1, 32.0f);
	prepare_material_lum(&objectShader , 0, false, lightColor, 0.6f, 0.0028f, 0.000014f); /* 0.007  0.0002*/
	prepare_material_lum(&instanceShader, 0, false, lightColor, 0.3f, 0.007f, 0.0002f);
	prepare_material_lum(&pointShadowShader, 0, false, lightColor, 0.9f, 0.0014f, 0.000007f);

	setInt(&pointShadowShader, "depthMap", 10);
	
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
			tile_floor.pos[1] = objectsHeight-0.001f;
			tile_floor.pos[2] = y;
			glm_translate(tile_floor.model, tile_floor.pos);
			glm_rotate(tile_floor.model, glm_rad(-90.0f), adjustRot);
			glm_mat4_copy(tile_floor.model, tilesMatrices[i]);
			i++;
		}
	}
	
	seed = time(NULL);
	srand(1675999174);
	for (i = 0; i < cratesAmount; i++)
	{
		glm_vec3_fill(crate.scale, ((random()%30)/40.0f)+0.5f);
		glm_mat4_identity(crate.model);
		crate.pos[0] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		crate.pos[1] = objectsHeight+crate.scale[0]/2;
		crate.pos[2] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		glm_translate(crate.model, crate.pos);
		adjustRot[0] = 1;
		adjustRot[2] = 0;
		glm_rotate(crate.model, glm_rad(-90.0f), adjustRot);
		adjustRot[0] = 0;
		adjustRot[2] = 1;
		glm_rotate(crate.model, glm_rad(random()%360), adjustRot);
		glm_scale(crate.model, crate.scale);
		glm_mat4_copy(crate.model, cratesMatrices[i]);
	}
	
	instanced_object_buffer(&ibTiles , &tile_floor, tilesAmount, tilesMatrices);
	instanced_object_buffer(&ibCrates , &crate, cratesAmount, cratesMatrices);
	
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
	far = 256.0f;
	
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		camera.currentFrame = (float)glfwGetTime();
		camera.deltaTime = camera.currentFrame - camera.lastFrame;
		camera.lastFrame = camera.currentFrame;
		
		light.pos[0] = sin(glfwGetTime()/2.0f)*9.0f;
		light.pos[1] = sin(glfwGetTime()*2.0f);
		light.pos[2] = cos(glfwGetTime()/2.0f)*9.0f;
		
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
		setVec3(&depthCubeShader, "lightPos", light.pos);
		
		useShader(&depthCubeShader);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) (tile_floor.indices_n), GL_UNSIGNED_INT, 0, tilesAmount);
		
		glBindVertexArray(crate.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) (crate.indices_n), GL_UNSIGNED_INT, 0, cratesAmount);
		
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
		
		useShader(&pointShadowShader);
		
		bind_cubemap(depthCubeMap, 10);
		instance_draw(light, &lightShader, camera);
		
		setVec3(&pointShadowShader, "light[0].position", light.pos);
		setVec3(&pointShadowShader, "viewPos", camera.pos);
		setFloat(&pointShadowShader, "far_plane", far);
		
		bind_texture(floorDiffuse, 0);
		bind_texture(floorSpecular, 1);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) (tile_floor.indices_n), GL_UNSIGNED_INT, 0, tilesAmount);
		
		bind_texture(crateDiffuse, 0);
		bind_texture(crateSpecular, 1);
		glBindVertexArray(crate.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) (crate.indices_n), GL_UNSIGNED_INT, 0, cratesAmount);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
