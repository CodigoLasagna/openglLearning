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
	Tmodel grass_blade;
	unsigned int floorDiffuse, floorSpecular, crateDiffuse, crateSpecular, crateNormal, crateDepth, baseNormal, grass_diffuse;
	unsigned int lightShader, objectShader, instanceShader, grass_shader;
	unsigned int uboMatrices;
	unsigned int ibCrates, ibTiles, ibGrassblades, ibBrick;
	unsigned int tilesAmount = 100;
	unsigned int cratesAmount = 4;
	unsigned int gbladesAmount = 100000;
	unsigned int brickwallAmount = 1;
	unsigned int i, j;
	float tilesAmountSquared = 0;
	float objectsHeight = -2.0f;
	int x, y, z;
	mat4 tilesMatrices[100];
	mat4 cratesMatrices[4];
	mat4 bladesMatrices[100000];
	mat4 brickwMatrices[1];
	vec3 lightColor = {0.8f, 0.95f, 1.0f};
	vec3 adjustRot = {1.0f, 0.0f, 0.0f};
	vec3 crateRot = {0.0f, 0.0f, 1.0f};
	vec3 bladeRot = {0.0f, 1.0f, 0.0f};
	unsigned int seed;
	
	window = prepareGLFW(800, 600);
	prepareGLEW();
	
	init_config(&config, 1920, 1080);
	init_camera(&camera, config, 1);
	
	createShader(&lightShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/light.frag");
	createShader(&instanceShader, "./source/shaders/vertexShaders/newInstancedPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&grass_shader, "./source/shaders/vertexShaders/instanced_shaderGrass.vert", "./source/shaders/rendPointShadow/fragmentTest.frag");
	createShader(&objectShader, "./source/shaders/vertexShaders/newPers.vert", "./source/shaders/fragmentShaders/surfaceLight.frag");
	createShader(&(render.shader), "./source/shaders/vertexShaders/rendertoquad.vert", "./source/shaders/fragmentShaders/sampleScreen.frag");
	createShader(&(skybox.shader), "./source/shaders/vertexShaders/cubeMap.vert", "./source/shaders/fragmentShaders/showcubeMap.frag");
	createShader_geometry(&(shadowCM.depth_shader), "./source/shaders/pointShadowDepthMap/vertex.vert", "./source/shaders/pointShadowDepthMap/fragment.frag", "./source/shaders/pointShadowDepthMap/geometry.geo");
	createShader(&(shadowCM.render_shader), "./source/shaders/rendPointShadow/vertextNormal.vert", "./source/shaders/rendPointShadow/fragmentNormal.frag");
	
	create_skybox(&skybox, "./source/images/cubemaps/skybox_00/", 1);
	createTexture(&floorDiffuse, "./source/images/grass_02.png", 1, 1);
	createTexture(&floorSpecular, "./source/images/grass_02_specular.png", 1, 1);
	createTexture(&crateDiffuse, "./source/images/crate_03.png", 1, 1);
	createTexture(&crateSpecular, "./source/images/container2_specular.png", 1, 1);
	createTexture(&crateNormal, "./source/images/crate_03_new_normal.png", 1, 1);
	createTexture(&crateDepth, "./source/images/crate_03_depthmap.png", 1, 1);
	createTexture(&baseNormal, "./source/images/base_normal.png", 1, 1);
	createTexture(&grass_diffuse, "./source/images/grass_pink.png", 0, 1);
	
	instance_create_cube(&light, 0.0f, 1.0f, 0.35f, 100, 100, 100, 0.5f, 5);
	instance_create_quad(&tile_floor, 0.0f, 0.0f, 0.0f, 100, 100, 1.0f, 5);
	instance_create_cube(&crate, 0.0f, 0.0f, 0.0f, 100, 100, 100, 0.5f, 5);
	
	load_model(&grass_blade, "./source/models/grass blades/blade_00.obj", 0.0f, -5.41f, 0.0f, 0.1f, 0);
	
	prepare_lightobj(&lightShader, lightColor);
	prepare_material(&objectShader, 1, 32.0f);
	prepare_material(&instanceShader, 1, 32.0f);
	prepare_material(&grass_shader, 1, 32.0f);
	prepare_material(&(shadowCM.render_shader), 1, 32.0f);
	prepare_material_lum(&objectShader , 0, false, lightColor, 0.6f, 0.0028f, 0.000014f); /* 0.007  0.0002*/
	prepare_material_lum(&instanceShader, 0, false, lightColor, 0.2f, 0.007f, 0.0002f);
	prepare_material_lum(&grass_shader, 0, false, lightColor, 0.8f, 0.0014f, 0.000007f);
	prepare_material_lum(&(shadowCM.render_shader), 0, false, lightColor, 0.9f, 0.0014f, 0.000007f);
	
	setInt(&(shadowCM.render_shader), "depthMap", 10);
	setInt(&grass_shader, "depthMap", 10);
	
	prepare_uniformblockData(&lightShader, "Matrices");
	prepare_uniformblockData(&(skybox.shader), "Matrices");
	prepare_uniformblockData(&objectShader, "Matrices");
	prepare_uniformblockData(&instanceShader, "Matrices");
	prepare_uniformblockData(&grass_shader, "Matrices");
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
			tile_floor.position[0] = x;
			tile_floor.position[1] = objectsHeight-0.001f;
			tile_floor.position[2] = y;
			glm_translate(tile_floor.model, tile_floor.position);
			glm_rotate(tile_floor.model, glm_rad(-90.0f), adjustRot);
			glm_mat4_copy(tile_floor.model, tilesMatrices[i]);
			i++;
		}
	}
	
	seed = time(NULL);
	srand(seed);
	for (i = 0; i < cratesAmount; i++)
	{
		glm_vec3_fill(crate.scale, ((random()%30)/30.0f)+0.5f);
		glm_mat4_identity(crate.model);
		crate.position[0] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		crate.position[1] = objectsHeight+crate.scale[0]/2;
		crate.position[2] = (random()%((int)tilesAmountSquared*2))-tilesAmountSquared;
		glm_translate(crate.model, crate.position);
		adjustRot[0] = 1;
		adjustRot[2] = 0;
		glm_rotate(crate.model, glm_rad(-90.0f), adjustRot);
		adjustRot[0] = 0;
		adjustRot[2] = 1;
		glm_rotate(crate.model, glm_rad(random()%360), adjustRot);
		glm_scale(crate.model, crate.scale);
		glm_mat4_copy(crate.model, cratesMatrices[i]);
	}
	for (i = 0; i < gbladesAmount; i++)
	{
		glm_mat4_identity(grass_blade.model);
		grass_blade.pos[0] = (random()%(long)(tilesAmountSquared*2.0f))-tilesAmountSquared;
		grass_blade.pos[1] = objectsHeight-0.005;
		grass_blade.pos[2] = (random()%(long)(tilesAmountSquared*2.0f))-tilesAmountSquared;
		grass_blade.pos[0] += ((float)(random()%10000)/10000.0f)-0.5f;
		grass_blade.pos[2] += ((float)(random()%10000)/10000.0f)-0.5f;
		grass_blade.scale[1] = 0.1;
		grass_blade.scale[1] += (float)(random()%5)/50.0f;
		grass_blade.pos[1] += grass_blade.scale[1]/2;
		glm_translate(grass_blade.model, grass_blade.pos);
		glm_scale(grass_blade.model, grass_blade.scale);
		glm_rotate(grass_blade.model, -glm_rad(random()%360), bladeRot);
		glm_mat4_copy(grass_blade.model, bladesMatrices[i]);
	}
	
	
	instanced_object_buffer(&ibTiles , &tile_floor, tilesAmount, tilesMatrices);
	instanced_object_buffer(&ibCrates , &crate, cratesAmount, cratesMatrices);
	instanced_model_buffer(&ibCrates , &grass_blade, gbladesAmount, bladesMatrices);
	
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
		
		light.position[0] = sin(glfwGetTime()/1.0f)*9.0f;
		light.position[1] = cos(glfwGetTime()*2.0f)+1;
		light.position[2] = cos(glfwGetTime()/1.0f)*9.0f;
		
		glm_mat4_identity(light.model);
		glm_translate(light.model, light.position);
		glm_scale(light.model, light.scale);
		
		instance_draw(light, &lightShader, camera);
		useShader(&grass_shader);
		bind_cubemap(shadowCM.cube_map_texture, 10);
		setVec3(&grass_shader, "viewPos", camera.pos);
		setFloat(&grass_shader, "far_plane", shadowCM.far_plane);
		setFloat(&grass_shader, "far_plane", shadowCM.far_plane);
		
		bind_texture(grass_diffuse, 0);
		instanced_model_draw(grass_blade, gbladesAmount);
		useShader(&(shadowCM.render_shader));
		
		bind_texture(floorDiffuse, 0);
		bind_texture(floorSpecular, 1);
		bind_texture(baseNormal, 2);
		instanced_object_draw(tile_floor, tilesAmount);
		
		bind_texture(crateDiffuse, 0);
		bind_texture(crateSpecular, 1);
		bind_texture(crateNormal, 2);
		bind_texture(crateDepth, 3);
		instanced_object_draw(crate, cratesAmount);
		
		/*render shadow scene*/
		calculate_shadow_cubemap_light(&shadowCM, light, camera);
		
		setFloat(&grass_shader, "time", sin(glfwGetTime()));
		setVec3(&(shadowCM.depth_shader), "lightPos", light.position);
		setVec3(&(shadowCM.render_shader), "light[0].position", light.position);
		setVec3(&grass_shader, "light[0].position", light.position);
		useShader(&(shadowCM.depth_shader));
		
		instanced_object_draw(tile_floor, tilesAmount);
		instanced_object_draw(crate, tilesAmount);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
