#include "source/engine/engine.h"
#include <time.h>

int main()
{
	Tconfig config;
	GLFWwindow *window;
	Tcamera camera;
	Trenderer render;
	Tskybox skybox;
	TshadowCM shadowCM;
	Tobject light;
	Tobject tile_floor, crate;
	unsigned int floorDiffuse, floorSpecular, crateDiffuse, crateSpecular;
	unsigned int lightShader, objectShader, instanceShader;
	unsigned int uboMatrices;
	unsigned int ibCrates, ibTiles;
	unsigned int tilesAmount = 400;
	unsigned int cratesAmount = 8;
	unsigned int i, j;
	float tilesAmountSquared = 0;
	float objectsHeight = -5.5f;
	int x, y, z;
	mat4 tilesMatrices[400];
	mat4 cratesMatrices[8];
	vec3 lightColor = {0.5f, 0.6f, 1.0f};
	vec3 adjustRot = {1.0f, 0.0f, 0.0f};
	vec3 crateRot = {0.0f, 0.0f, 1.0f};
	unsigned int seed;
	
	window = prepareGLFW(800, 600);
	prepareGLEW();
	
	init_config(&config, 1920, 1080);
	init_camera(&camera, config, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/newInstancedPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	createShader_geometry(&(shadowCM.depth_shader), "./source/shaders/pointShadowDepthMap/vertex.vert", "./source/shaders/pointShadowDepthMap/fragment.frag", "./source/shaders/pointShadowDepthMap/geometry.geo");
	createShader(&(shadowCM.render_shader), "./source/shaders/rendPointShadow/vertext.vert", "./source/shaders/rendPointShadow/fragmentTest.frag");
	
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
	prepare_material(&(shadowCM.render_shader), 1, 32.0f);
	prepare_material_lum(&objectShader , 0, false, lightColor, 0.6f, 0.0028f, 0.000014f); /* 0.007  0.0002*/
	prepare_material_lum(&instanceShader, 0, false, lightColor, 0.3f, 0.007f, 0.0002f);
	prepare_material_lum(&(shadowCM.render_shader), 0, false, lightColor, 0.9f, 0.0014f, 0.000007f);
	
	setInt(&(shadowCM.render_shader), "depthMap", 10);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	prepare_uniformblockData(&(shadowCM.render_shader), "Matrices");
	
	prepare_uniformblockMatrices(&uboMatrices, camera);
	prepare_renderer(&render, config);
	prepare_shadow_cubemap(&shadowCM, 1024, 1024, 0.01f, 256.0f);
	
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
	
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		camera.currentFrame = (float)glfwGetTime();
		camera.deltaTime = camera.currentFrame - camera.lastFrame;
		camera.lastFrame = camera.currentFrame;
		
		/*render normal scene*/
		run_uniformblockMatrices(uboMatrices, camera);
		run_camera(&camera, window);
		run_renderer(&render, camera);
		draw_skybox(skybox, camera);
		processInput(window, &config);
		
		light.pos[0] = sin(glfwGetTime()/2.0f)*9.0f;
		light.pos[1] = sin(glfwGetTime()*2.0f)-3;
		light.pos[2] = cos(glfwGetTime()/2.0f)*9.0f;
		
		glm_mat4_identity(light.model);
		glm_translate(light.model, light.pos);
		glm_scale(light.model, light.scale);
		
		instance_draw(light, &lightShader, camera);
		useShader(&(shadowCM.render_shader));
		
		bind_texture(floorDiffuse, 0);
		bind_texture(floorSpecular, 1);
		instanced_object_draw(tile_floor, tilesAmount);
		
		bind_texture(crateDiffuse, 0);
		bind_texture(crateSpecular, 1);
		instanced_object_draw(crate, cratesAmount);
		
		/*render shadow scene*/
		calculate_shadow_cubemap_light(&shadowCM, light, camera);
		
		setVec3(&(shadowCM.depth_shader), "lightPos", light.pos);
		setVec3(&(shadowCM.render_shader), "light[0].position", light.pos);
		useShader(&(shadowCM.depth_shader));
		
		instanced_object_draw(tile_floor, tilesAmount);
		instanced_object_draw(crate, tilesAmount);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
