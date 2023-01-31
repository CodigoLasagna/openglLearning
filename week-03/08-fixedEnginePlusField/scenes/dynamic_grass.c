#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	Tobject light;
	Tcamera camera;
	Trenderer render;
	Tskybox skybox;
	Tobject grass;
	unsigned int grassDiffuse;
	unsigned int lightShader, modelShader, objectShader, foliageShader;
	unsigned int uboMatrices;
	unsigned int amount = 2500;
	mat4 modelMatrices[2500];
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
	vec3 lightPos = {0.0f, 2.5f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor = {0.9f, 0.8f, 0.7f};
	vec3 globalAmbience = {0.5f, 0.4f, 0.3f};
	float wind;
	
	window = prepareGLFW(64, 64);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/perspective.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&modelShader, "./source/shaders/vertexShaders/modPer.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/persGroundFoliage.vert", "./source/shaders/fragmentShaders/fullTrans.frag");
	createShader(&foliageShader, "./source/shaders/vertexShaders/persGroundFoliage.vert", "./source/shaders/fragmentShaders/pixelArtTransp.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 0);
	createTexture(&grassDiffuse, "./source/images/foliage_01.png", 2);
	
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	instance_create_quad(&grass, 0.0f, 1.5f, -4.94f, 320, 320, 1);
	
	prepare_light(&modelShader, &lightShader, 0, lightColor, 0.022f, 0.0019f);
	prepare_light(&objectShader, &lightShader, 0, lightColor, 0.022f, 0.0019f);
	prepare_light(&foliageShader, &lightShader, 0, lightColor, 0.022f, 0.0019f);
	
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
	
	setInt(&foliageShader, "material.diffuse", 0);
	setFloat(&foliageShader, "material.shininess", 32.0f);
	setVec4(&foliageShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&foliageShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	setInt(&foliageShader, "n_lights", 1);
	
	glm_scale(light.model, lightSize);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&foliageShader, "Matrices");
	
	prepare_uniformblockMatrices(&uboMatrices, camera);
	
	prepare_renderer(&render);
	
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
		
		/*
		draw_skybox(skybox, camera);
		*/
		if (((int)glfwGetTime()%2) == 0)
		{
			if (glfwGetTime() > ((int)glfwGetTime())+0.98f)
			{
				setVec3(&foliageShader, "color", (float)((rand()%11)/11.0f), (float)((rand()%11)/11.0f), (float)((rand()%11)/11.0f));
			}
		}
		
		wind = sin((float)glfwGetTime());
		setFloat(&foliageShader, "wind", wind);
		instance_draw(light, &lightShader, camera);
		
		bind_texture(&foliageShader, grassDiffuse, 0);
		instance_draw(grass, &foliageShader, camera);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

