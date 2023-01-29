#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	Tobject light;
	Tcamera camera;
	Trenderer render;
	Tskybox skybox;
	Tobject tile_floor, myquad;
	unsigned int floorDiffuse;
	unsigned int lightShader, modelShader, objectShader, instanceShader;
	unsigned int uboMatrices;
	unsigned int amount = 2500;
	unsigned int tilesAmount = 2500;
	mat4 modelMatrices[2500];
	mat4 tilesMatrices[2500];
	float radius = 30.0f;
	float offset = 10.5f;
	float angle, displacement;
	int x, y, z, i, j, k;
	vec3 pos;
	float scale;
	vec3 scaleGen;
	float rotAngle;
	unsigned int instanceBuffer;
	unsigned int tempVao;
	size_t vec4size;
	vec3 angleDir = {0.4f, 0.6f, 0.8f};
	vec3 lightPos = {0.0f, 25.0f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor = {1.0f, 0.8f, 0.7f};
	vec3 globalAmbience = {0.3f, 0.25f, 0.1f};
	vec3 floorRot = {1.0f, 0.0f, 0.0f};
	
	window = prepareGLFW(64, 64);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/normPers.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&modelShader, "./source/shaders/vertexShaders/modPer.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/newInstancedPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/normPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 1);
	createTexture(&floorDiffuse, "./source/images/grass_02.png", 1, 1);
	
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	instance_create_quad(&tile_floor, 0.0f, -0.0f, 0.0f, 100, 100, 2);
	/*instance_create_quad(&myquad, 0.0f, -3.0f, 0.0f, 100, 100, 2);*/
	
	prepare_light(&modelShader, &lightShader, 0, lightColor, 0.022f, 0.0019f);
	prepare_light(&objectShader, &lightShader, 0, lightColor, 0.022f, 0.0019f);
	prepare_light(&instanceShader, &lightShader, 0, lightColor, 0.009f, 0.0004f);
	
	setInt(&modelShader, "material.diffuse", 0);
	setFloat(&modelShader, "material.shininess", 32.0f);
	setVec4(&modelShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&modelShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	setInt(&modelShader, "n_lights", 1);
	
	setInt(&objectShader, "material.diffuse", 0);
	setFloat(&objectShader, "material.shininess", 32.0f);
	setVec4(&objectShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&objectShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	setInt(&objectShader, "n_lights", 1);
	
	setInt(&instanceShader, "material.diffuse", 0);
	setFloat(&instanceShader, "material.shininess", 0.0f);
	setVec4(&instanceShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&instanceShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	setInt(&instanceShader, "n_lights", 1);
	
	glm_scale(light.model, lightSize);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	
	prepare_uniformblockMatrices(&uboMatrices, camera);
	
	prepare_renderer(&render);
	
	i = 0;
	for (y = -sqrt(tilesAmount)/2; y < sqrt(tilesAmount)/2; y++)
	{
		for (x = -sqrt(tilesAmount)/2; x < sqrt(tilesAmount)/2; x++)
		{
			glm_mat4_identity(tile_floor.model);
			pos[0] = x;
			pos[1] = -5.0f;
			pos[2] = y;
			glm_translate(tile_floor.model, pos);
			glm_rotate(tile_floor.model, glm_rad(-90.0f), floorRot);
			glm_mat4_copy(tile_floor.model, tilesMatrices[i]);
			i++;
		}
	}
	
	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
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

	while (!glfwWindowShouldClose(window))
	{
		/*extra options*/
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		camera.currentFrame = (float)glfwGetTime();
		camera.deltaTime = camera.currentFrame - camera.lastFrame;
		camera.lastFrame = camera.currentFrame;
		
		run_camera(&camera, window);
		run_rederer(&render, camera);
		run_uniformblockMatrices(uboMatrices, camera);
		processInput(window, &config);
		
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		instance_draw(light, &lightShader, camera);
		draw_skybox(skybox, camera);
		
		bind_texture(&instanceShader, floorDiffuse, 0);
		useShader(&instanceShader);
		glBindVertexArray(tile_floor.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, (tile_floor.indices_n * 3), GL_UNSIGNED_INT, 0, tilesAmount);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

