#include "./source/engine/engine.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main()
{
	Tconfig config;
	GLFWwindow *window;
	int i, j;
	Tobject cube;
	Tobject light;
	Tcamera camera;
	float delta, lastFrame, currentFrame, angle;
	unsigned int objsShader, lightShader, texture, specularTexture, emmisionTexture;
	unsigned int modShader;
	unsigned int lightVAO;
	vec3 lightPos = {1.5f, 1.5f, 0.0f};
	vec3 lightSize = {0.2f, 0.2f, 0.2f};
	vec3 modelSize = {1.0f, 1.0f, 1.0f};
	vec3 lightColor = {0.0f, 2.0f, 1.5f};
	vec3 cubeAngle = {0.0f, 1.0f, 0.0f};
	vec3 globalAmbience = {0.6f, 0.0f, 0.6f};
	mat4 model;
	
	vec3 cubePositions[] = {
		{ 0.0f,  0.0f,  0.0f}, 
	};
	
	const struct aiScene* scene = aiImportFile("./source/models/apple2.obj", aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	unsigned int numMeshes = scene->mNumMeshes;
	const struct aiMesh* mesh = scene->mMeshes[0];
	unsigned int numVertices = mesh->mNumVertices;
	unsigned int numFaces = mesh->mNumFaces;
	float *vertices = (float*) malloc(numVertices * 3 * sizeof(float));
	float *normals = (float*) malloc(numVertices * 3 * sizeof(float));
	unsigned int *indices = (unsigned int *) malloc(numFaces * 3 * sizeof(unsigned int));
	unsigned int index = 0;
	unsigned int VAO, VBO, EBO, NBO;
	if (!scene)
	{
		printf("No se ha podido importar el model\n");
	}
	for (i = 0; i < numVertices; i++)
	{
		vertices[i * 3] 	= mesh->mVertices[i].x;
		vertices[i * 3 + 1] = mesh->mVertices[i].y;
		vertices[i * 3 + 2] = mesh->mVertices[i].z;
		
		normals[i * 3]		= mesh->mNormals[i].x;
		normals[i * 3 + 1]  = mesh->mNormals[i].y;
		normals[i * 3 + 2]  = mesh->mNormals[i].z;
	}
	for (i = 0; i < numFaces; i++)
	{
		for (j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices[index++] = mesh->mFaces[i].mIndices[j];
		}
	}
	
	window = prepareGLFW(400, 400);
	
	prepareGLEW();
	init_camera(&camera, 1366, 768, 2);
	
	createShader(&objsShader, "./source/shaders/perspective.vert", "./source/shaders/lightmapEmmisiveMultipleLights.frag");
	createShader(&modShader, "./source/shaders/modPer.vert", "./source/shaders/lightmapEmmisiveMultipleLights.frag");
	createShader(&lightShader, "./source/shaders/perspective.vert", "./source/shaders/light.frag");
	createTexture(&texture, "./source/images/crate_03.png");
	createTexture(&specularTexture, "./source/images/container2_specular.png");
	createTexture(&emmisionTexture, "./source/images/crate_03_emmision.png");
	
	bind_texture(&objsShader, texture, "material.diffuse", 0);
	bind_texture(&objsShader, specularTexture, "material.specular", 1);
	bind_texture(&objsShader, emmisionTexture, "material.emmision", 2);
	setFloat(&objsShader, "material.shininess", 32.0f);
	setVec4(&objsShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	
	bind_texture(&modShader, texture, "material.diffuse", 0);
	bind_texture(&modShader, specularTexture, "material.specular", 1);
	bind_texture(&modShader, emmisionTexture, "material.emmision", 2);
	setFloat(&modShader, "material.shininess", 32.0f);
	setVec4(&modShader, "light[0].lightVector", lightPos[0], lightPos[1], lightPos[2], 0.0f);
	
	prepare_light(&objsShader, &lightShader, 0, lightColor);
	prepare_light(&modShader, &lightShader, 0, lightColor);
	setVec3(&modShader, "light[0].ambient", globalAmbience[0], globalAmbience[1], globalAmbience[2]);
	
	setInt(&objsShader, "n_lights", 1);
	setInt(&modShader, "n_lights", 1);
	
	instance_create_cube(&cube, 0.0f, 0.0f, 0.0f, 100, 100, 100, 1);
	instance_create_cube(&light, lightPos[0], lightPos[1], lightPos[2], 100, 100, 100, 1);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
	
	glm_scale(light.model, lightSize);
	
	
	glm_mat4_identity(model);
	glm_scale(model, modelSize);
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		
		/*
		glClearColor(0.050f, 0.046f, 0.120f, 1.0f);
		*/
		glClearColor(0.05f, 0.2f, 0.2f, 1.0f);
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, camera.frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		run_camera(&camera, window, delta);
		instance_draw(light, &lightShader, camera);
		
		setVec3(&objsShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		setVec3(&modShader, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
		
		/*
		instance_draw(cube, &objsShader, camera);
		*/
		
		glm_rotate(model, glm_rad(0.5f), cubeAngle);
		useShader(&modShader);
		glUniformMatrix4fv(glGetUniformLocation(modShader, "view"), 1, GL_FALSE, (float*)(camera.view));
		glUniformMatrix4fv(glGetUniformLocation(modShader, "projection"), 1, GL_FALSE, (float*)(camera.projection));
		glUniformMatrix4fv(glGetUniformLocation(modShader, "model"), 1, GL_FALSE, (float*)(model));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
		


		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	aiReleaseImport(scene);
	return 0;
}
