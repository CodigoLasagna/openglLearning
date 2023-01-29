#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	Tobject light;
	Tcamera camera;
	Trenderer render;
	Tskybox skybox;
	Tmodel apple;
	unsigned int appleDiffuse, appleSpec;
	unsigned int lightShader, modelShader, instanceShader;
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
	vec3 appleRot = {0.0f, 1.0f, 0.0f};
	
	window = prepareGLFW(400, 400);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/perspective.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&modelShader, "./source/shaders/vertexShaders/modPer.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/instancedModPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	
	createTexture(&appleDiffuse, "./source/images/apple.png", 1);
	createTexture(&appleSpec, "./source/images/apple_specular.png", 1);
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 0);
	
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	load_model(&apple, "./source/models/apple3.obj", 0.0f, 0.0f, 0.0f, 1);
	
	prepare_light(&modelShader, &lightShader, 0, lightColor);
	prepare_light(&instanceShader, &lightShader, 0, lightColor);
	
	setInt(&modelShader, "material.diffuse", 0);
	setFloat(&modelShader, "material.shininess", 32.0f);
	setVec4(&modelShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&modelShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	setInt(&modelShader, "n_lights", 1);
	
	setInt(&instanceShader, "material.diffuse", 0);
	setFloat(&instanceShader, "material.shininess", 256.0f);
	setVec4(&instanceShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&instanceShader, "light[0].ambient", globalAmbience[0]*15, globalAmbience[1]*15, globalAmbience[2]*15);
	setInt(&instanceShader, "n_lights", 1);
	
	glm_scale(light.model, lightSize);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	
	prepare_uniformblockMatrices(&uboMatrices, camera);
	
	prepare_renderer(&render);
	
	for (i = 0; i < amount; i++)
	{
		glm_mat4_identity(apple.model);
		angle = (float)i / (float)amount * 360.0f;
		
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		pos[0] = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		pos[1] = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		pos[2] = cos(angle) * radius + displacement;
		glm_translate(apple.model, pos);
		
		scale = (rand() % 20) / 20.0f + 0.05f;
		scaleGen[0] = scale;
		scaleGen[1] = scale;
		scaleGen[2] = scale;
		glm_scale(apple.model, scaleGen);
		
		rotAngle = (rand() % 360);
		glm_rotate(apple.model, rotAngle, angleDir);
		
		glm_mat4_copy(apple.model, modelMatrices[i]);
	}
	
	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(mat4), &modelMatrices[0], GL_STATIC_DRAW);
	
	for (i = 0; i < apple.max_meshes; i++)
	{
		glBindVertexArray(apple.VAO[i]);
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
	}
	
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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		draw_skybox(skybox, camera);
		
		instance_draw(light, &lightShader, camera);
		
		bind_texture(&instanceShader, appleDiffuse, 0);
		bind_texture(&instanceShader, appleSpec, 1);
		useShader(&instanceShader);
		for (i = 0; i < apple.max_meshes; i++)
		{
			glBindVertexArray(apple.VAO[i]);
			glDrawElementsInstanced(GL_TRIANGLES, (apple.indices[i] * 3), GL_UNSIGNED_INT, 0, amount);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	return 0;
}

