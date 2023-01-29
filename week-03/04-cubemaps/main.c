#include "source/engine/engine.h"

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int x, y, i, j, k;
	Tobject light, tile_floor, crate, grass, glass_window, screen, cubeMapObj;
	Tcamera camera;
	Tmodel crash, apple;
	float delta, lastFrame, currentFrame, angle, distance;
	unsigned int tileTexture, crateTexture, crateTextureSpec, grassTexture, glassTexture, crashTexture, cubeMapTexture, appleDiffuse, appleSpec;
	unsigned int lightShader, primitiveShader, modelShader, alphaShader, screenShader, skyboxShader, mirrorShader, refractionShader;
	vec3 lightPos = {0.0f, 2.5f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 lightColor = {0.9f, 0.8f, 0.7f};
	vec3 globalAmbience = {0.5f, 0.4f, 0.3f};
	vec3 modelPos = {1.0f, 0.0f, 0.0f};
	vec3 crashSize = {0.01f, 0.01f, 0.01f};
	vec3 grassSize = {1.38f, 1.38f, 1.38f};
	vec3 glassSize = {1.0f, 2.0f, 1.0f};
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
	
	createShader(&lightShader, "./source/shaders/vertexShaders/perspective.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&primitiveShader, "./source/shaders/vertexShaders/perspective.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&modelShader, "./source/shaders/vertexShaders/modPer.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&alphaShader, "./source/shaders/vertexShaders/perspective.vert", "./source/shaders/fragmentShaders/fullTrans.frag");
	createShader(&screenShader, "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&skyboxShader, "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	createShader(&mirrorShader, "./source/shaders/vertexShaders/reflectPers.vert", "./source/shaders/fragmentShaders/surfaceMirror.frag");
	createShader(&refractionShader, "./source/shaders/vertexShaders/reflectPers.vert", "./source/shaders/fragmentShaders/surfaceRefraction.frag");
	
	createTexture(&tileTexture, "./source/images/grass_02.png", 1);
	createTexture(&crateTexture, "./source/images/crate_03.png", 1);
	createTexture(&crateTextureSpec, "./source/images/container2_specular.png", 1);
	createTexture(&crashTexture, "./source/images/color_pallete.png", 1);
	createTexture(&grassTexture, "./source/images/foliage_01.png", 2);
	createTexture(&glassTexture, "./source/images/window_01.png", 2);
	createTexture(&appleDiffuse, "./source/images/apple.png", 1);
	createTexture(&appleSpec, "./source/images/apple_specular.png", 1);
	
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	instance_create_quad(&tile_floor, 0, 0, 0, 100, 100, 1);
	instance_create_cube(&crate, 0, 0, 0, 100, 100, 100, 1);
	instance_create_quad(&grass, 0, 0, 0, 100, 100, 1);
	instance_create_quad(&glass_window, 0, -1.5, 5, 100, 100, 1);
	instance_create_quad(&screen, 0, 0, 0, 200.0f, 200.0f, 1);
	instance_create_cube(&cubeMapObj, 0, 0, 0, 100000, 100000, 100000, 1);
	
	load_model(&crash, "./source/models/crashbandicoot.obj", 0.0f, -2.57f, 0.0f, 1);
	load_model(&apple, "./source/models/apple3.obj", -2.0f, 0.0f, 0.0f, 1);
	
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
	
	while (!glfwWindowShouldClose(window))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		bind_texture(&screenShader, textureColorbuffer, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.nWidth, camera.nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, camera.width, camera.height);
		instance_draw(screen, &screenShader, camera);
		setFloat(&screenShader, "iTime", (float) glfwGetTime());
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		run_camera(&camera, window, delta);
		
		/*
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, camera.frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		*/
		
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
		
		setVec3(&lightShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&primitiveShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&modelShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&mirrorShader, "cameraPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&refractionShader, "cameraPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
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
		
		for (i = 0; i < sizeof(applePos) / sizeof(applePos[0]); i ++)
		{
			glm_mat4_identity(apple.model);
			applePos[i][1] = (-sin(glm_rad((float) glfwGetTime() * 100 + (100*i)))) / 2 - 1.5f;
			glm_translate(apple.model, applePos[i]);
			glm_scale(apple.model, appleSize);
			glm_rotate(apple.model, glm_rad(glfwGetTime() * 50 + (100*i)), appleRot);
			switch(i)
			{
				default:
					bind_texture(&modelShader, appleDiffuse, 0);
					bind_texture(&modelShader, appleSpec, 1);
					draw_model(&apple, &modelShader, camera);
				break;
				case 1:
					glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
					draw_model(&apple, &mirrorShader, camera);
				break;
				case 2:
					glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
					draw_model(&apple, &refractionShader, camera);
				break;
			}
		}
		
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
		
		
		/*
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		*/
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	return 0;
}
