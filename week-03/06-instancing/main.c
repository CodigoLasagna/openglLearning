#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	Tobject light, screen, cubeMapObj;
	Tcamera camera;
	Tmodel apple;
	float delta, lastFrame, currentFrame, distance;
	unsigned int cubeMapTexture, appleDiffuse, appleSpec;
	unsigned int lightShader, modelShader, instanceShader, screenShader, skyboxShader;
	unsigned int ubiLight, ubiModel, ubiCubeMap, ubiInstance;
	unsigned int uboMatrices;
	unsigned int amount = 2000;
	mat4 modelMatrices[2000];
	float radius = 10.0f;
	float offset = 2.5f;
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
	vec3 appleSize = {0.25f, 0.25f, 0.25f};
	vec3 applePos[] =
	{
		{-2.0f, -1.0f, 0.0f},
		{1.0f, -1.5f, 1.0f},
		{0.5f, -1.5f, -2.0f},
		{2.5f, -1.5f, -1.5f},
	};
	vec3 appleRot = {0.0f, 1.0f, 0.0f};
	vec4 temp;
	unsigned int frameBuffer;
	unsigned int textureColorbuffer;
	unsigned int textureColorbufferHalf;
	unsigned int renderBuffer;
	mat4 newView;
	mat3 tempView;
	
	int cmWidth, cmHeight, cmNrchannels;
	unsigned char *data;
	char *textureFaces[] =
	{
		"./source/images/cubemaps/skybox_00/left.jpg",
		"./source/images/cubemaps/skybox_00/right.jpg",
		"./source/images/cubemaps/skybox_00/top.jpg",
		"./source/images/cubemaps/skybox_00/bottom.jpg",
		"./source/images/cubemaps/skybox_00/front.jpg",
		"./source/images/cubemaps/skybox_00/back.jpg",
	};
	
	window = prepareGLFW(400, 400);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 2);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/perspective.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&modelShader, "./source/shaders/vertexShaders/modPer.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/instancedModPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&screenShader, "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&skyboxShader, "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	
	createTexture(&appleDiffuse, "./source/images/apple.png", 1);
	createTexture(&appleSpec, "./source/images/apple_specular.png", 1);
	
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	instance_create_quad(&screen, 0, 0, 0, 200.0f, 200.0f, 1);
	instance_create_cube(&cubeMapObj, 0, 0, 0, 100000, 100000, 100000, 1);
	
	load_model(&apple, "./source/models/apple3.obj", 0.0f, 0.0f, 0.0f, 1);
	
	prepare_light(&modelShader, &lightShader, 0, lightColor);
	prepare_light(&instanceShader, &lightShader, 0, lightColor);
	
	setInt(&modelShader, "material.diffuse", 0);
	setFloat(&modelShader, "material.shininess", 32.0f);
	setVec4(&modelShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&modelShader, "light[0].ambient", globalAmbience[0]*2.5f, globalAmbience[1]*2.5f, globalAmbience[2]*2.5f);
	setInt(&modelShader, "n_lights", 1);
	
	setInt(&instanceShader, "material.diffuse", 0);
	setFloat(&instanceShader, "material.shininess", 32.0f);
	setVec4(&instanceShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	setVec3(&instanceShader, "light[0].ambient", globalAmbience[0]*2.5f, globalAmbience[1]*2.5f, globalAmbience[2]*2.5f);
	setInt(&instanceShader, "n_lights", 1);
	
	glm_scale(light.model, lightSize);
	glm_scale(apple.model, appleSize);
	
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1366.0f, 768.0f, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1366.0f, 768.0f);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR EL FRAMEBUFFER NO ESTA COMPLETO\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glGenTextures(1, &cubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	
	for (i = 0; i < sizeof(textureFaces)/sizeof(textureFaces[0]); i++)
	{
		data = stbi_load(textureFaces[i], &cmWidth, &cmHeight, &cmNrchannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, cmWidth, cmHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("No se ha podido cargar la textura\n");
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	setInt(&skyboxShader, "skybox", 0);
	
	ubiLight = glGetUniformBlockIndex(lightShader, "Matrices");
	ubiCubeMap = glGetUniformBlockIndex(skyboxShader, "Matrices");
	ubiInstance = glGetUniformBlockIndex(instanceShader, "Matrices");
	
	glUniformBlockBinding(lightShader, ubiLight, 0);
	glUniformBlockBinding(ubiCubeMap, skyboxShader, 0);
	glUniformBlockBinding(ubiInstance, instanceShader, 0);
	
	glGenBuffers(1, &uboMatrices);
	
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(mat4));
	
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), (float*)(camera.projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), (float*)(camera.view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
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
		
		scale = (rand() % 20) / 100.0f + 0.05f;
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
		/*Matrices buffer*/
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(mat4));
		
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), (float*)(camera.projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), (float*)(camera.view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		/*textureToRender*/
		bind_texture(&screenShader, textureColorbuffer, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.nWidth, camera.nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		
		/*render textureScene*/
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, camera.width, camera.height);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		instance_draw(screen, &screenShader, camera);
		setFloat(&screenShader, "iTime", (float) glfwGetTime());
		
		/*extra options*/
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		run_camera(&camera, window, delta);
		
		/*render scene*/
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		
		glDepthMask(GL_FALSE);
		useShader(&skyboxShader);
		
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		instance_draw(cubeMapObj, &skyboxShader, camera);
		glDepthMask(GL_TRUE);
		
		instance_draw(light, &lightShader, camera);
		
		bind_texture(&instanceShader, appleDiffuse, 0);
		bind_texture(&instanceShader, appleSpec, 1);
		useShader(&instanceShader);
		for (i = 0; i < apple.max_meshes; i++)
		{
			glBindVertexArray(apple.VAO[i]);
			glDrawElementsInstanced(GL_TRIANGLES, (apple.indices[i] * 3), GL_UNSIGNED_INT, 0, amount);
		}
		/*
		bind_texture(&modelShader, appleDiffuse, 0);
		bind_texture(&modelShader, appleSpec, 1);
		for (i = 0; i < amount; i++)
		{
			setMat4(&modelShader, "model", modelMatrices[i]);
			draw_model(&apple, &modelShader, camera);
		}
		*/
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	return 0;
}
