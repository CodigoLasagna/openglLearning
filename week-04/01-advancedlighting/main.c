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
	unsigned int floorDiffuse, crateDiffuse;
	unsigned int lightShader, modelShader, objectShader, instanceShader;
	unsigned int uboMatrices;
	unsigned int tilesAmount = 1024;
	float tilesAmountSquared = 0;
	mat4 tilesMatrices[1024];
	unsigned int cratesAmount = 15;
	mat4 cratesMatrices[15];
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
	vec3 lightColor = {1.0f, 1.0f, 1.0f};
	vec3 floorRot = {1.0f, 0.0f, 0.0f};
	vec3 crateRot = {1.0f, 0.0f, 0.0f};
	vec3 bushRot = {0.0f, 1.0f, 0.0f};
	
	window = prepareGLFW(64, 64);
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
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 1);
	createTexture(&floorDiffuse, "./source/images/grass_02.png", 1, 1);
	createTexture(&crateDiffuse, "./source/images/crate_03.png", 1, 1);
	
	instance_create_cube(&light, 0.0f, 5.0f, 0.0f, 100, 100, 100, 1.0f, 0);
	instance_create_quad(&tile_floor, 0.0f, 0.0f, 0.0f, 100, 100, 1.0f, 2);
	instance_create_cube(&crate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 1.0f, 2);
	
	prepare_light(&modelShader	 , &lightShader, 0, lightColor, 0.6f, 0.022f, 0.0019f);
	prepare_light(&objectShader	 , &lightShader, 0, lightColor, 0.6f, 0.0028f, 0.000014f);
	prepare_light(&instanceShader, &lightShader, 0, lightColor, 0.3f, 0.007f, 0.0002f);
	
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
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	
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
			tile_floor.pos[1] = -5.51f;
			tile_floor.pos[2] = y;
			glm_translate(tile_floor.model, tile_floor.pos);
			glm_rotate(tile_floor.model, glm_rad(-90.0f), floorRot);
			glm_mat4_copy(tile_floor.model, tilesMatrices[i]);
			i++;
		}
	}
	
	srand(time(NULL));
	for (i = 0; i < cratesAmount; i++)
	{
		glm_vec3_fill(crate.scale, ((random()%30)/20.0f)+0.5f);
		glm_mat4_identity(crate.model);
		crate.pos[0] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		crate.pos[1] = -5.5f+crate.scale[0]/2;
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

	while (!glfwWindowShouldClose(window))
	{
		/*extra options*/
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		camera.currentFrame = (float)glfwGetTime();
		camera.deltaTime = camera.currentFrame - camera.lastFrame;
		camera.lastFrame = camera.currentFrame;
		
		run_camera(&camera, window);
		run_uniformblockMatrices(uboMatrices, camera);
		run_rederer(&render, camera);
		processInput(window, &config);
		
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glm_mat4_identity(light.model);
		glm_translate(light.model, light.pos);
		glm_scale(light.model, light.scale);
		light.pos[0] = (sin(glfwGetTime()/2)*10);
		
		setVec4(&instanceShader, "light[0].lightVector", light.pos[0], light.pos[1], light.pos[2], 0.0f);
		setVec3(&instanceShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
		instance_draw(light, &lightShader, camera);
		draw_skybox(skybox, camera);
		
		bind_texture(&instanceShader, floorDiffuse, 0);
		useShader(&instanceShader);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (tile_floor.indices_n * 3), GL_UNSIGNED_INT, 0, tilesAmount);
		
		bind_texture(&instanceShader, crateDiffuse, 0);
		useShader(&instanceShader);
		glBindVertexArray(crate.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (crate.indices_n * 3), GL_UNSIGNED_INT, 0, cratesAmount);
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

