#include "elements.h"
#include "textures.h"

int instance_create_quad(Tobject *ID, float x, float y, float z, int width, int height, float scale, int nAttributes)
{
	int attributes[] = {3, 3, 2};
	int i, nVertices = 0, n = 0;
	float hwidth, hheihght;
	/*
	GLfloat vertices[20] =
	{
		0.0f, 0.0f, 0.0f, 		0, 0,
		0.0f, 0.0f, 0.0f, 		1, 0,
		0.0f, 0.0f, 0.0f, 		1, 1,
		0.0f, 0.0f, 0.0f, 		0, 1
	};
	*/
	GLfloat vertices[32] =
	{
		0.0f, 0.0f, 0.0f,	-0.025f, -0.025f, 5.0f,	0, 0,
		0.0f, 0.0f, 0.0f,	 0.025f, -0.025f, 5.0f,	1, 0,
		0.0f, 0.0f, 0.0f,	 0.025f,  0.025f, 5.0f,	1, 1,
		0.0f, 0.0f, 0.0f,	-0.025f,  0.025f, 5.0f,	0, 1
	};
	int indices[6] = 
	{
		0, 3, 2,
		0, 2, 1,
	};
	nAttributes += 1;
	(ID->indices_n) = sizeof(indices)/sizeof(indices[0]);
	(ID->type) = nAttributes;
	(ID->width) = width;
	(ID->height) = height;
	(ID->pos[0]) = x;
	(ID->pos[1]) = y;
	(ID->pos[2]) = z;
	glm_vec3_fill(ID->scale, scale);
	hwidth = (float) width/200;
	hheihght = (float) height/200;
	
	for (i = 0; i < 3; i++)
	{
		nVertices += attributes[i];
	}
	
	vertices[0] =				-hwidth;	vertices[1] =				-hheihght;	vertices[2]					= 0.0f;
	vertices[nVertices] =		 hwidth;	vertices[1+nVertices] =		-hheihght;	vertices[2+nVertices]		= 0.0f;
	vertices[(nVertices*2)] =	 hwidth;	vertices[1+(nVertices*2)] =	 hheihght;	vertices[2+(nVertices*2)]	= 0.0f;
		vertices[(nVertices*3)] =	-hwidth;	vertices[1+(nVertices*3)] =	 hheihght;	vertices[2+(nVertices*3)]	= 0.0f;
	
	
	glGenVertexArrays(1, &(ID->VAO));
	glGenBuffers(1, &(ID->VBO));
	glGenBuffers(1, &(ID->EBO));
	
	glBindVertexArray(ID->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ID->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	for (i = 0; i < nAttributes; i++)
	{
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, nVertices*sizeof(GLfloat), (void*)(n*sizeof(GLfloat)));
		glEnableVertexAttribArray(i);
		n += attributes[i];
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glm_mat4_identity(ID->model);
	glm_translate(ID->model, ID->pos);
	glm_scale(ID->model, (ID->scale));
	
	glBindVertexArray(0);
	return 1;
}
int instance_create_cube(Tobject *ID, float x, float y, float z, int width, int height, int thickness, float scale, int nAttributes)
{
	int attributes[] = {3, 3, 2};
	int i, nVertices = 0, n = 0;
	float hwidth, hheihght, hthickens;
	GLfloat vertices[64] =
	{
		0.0f, 0.0f, 0.0f,	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	-1.0f,  1.0f, -1.0f,	1.0f, 0.0f,
		
		0.0f, 0.0f, 0.0f,	-1.0f,  1.0f,  1.0f,	1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,	-1.0f, -1.0f,  1.0f,	0.0f, 1.0f,
		
		0.0f, 0.0f, 0.0f,	 1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,	 1.0f,  1.0f, -1.0f,	0.0f, 0.0f,
		
		0.0f, 0.0f, 0.0f,	 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	 1.0f, -1.0f,  1.0f,	1.0f, 1.0f,
	};
	int indices[36] = 
	{
		0, 2, 3,
		0, 1, 2,

		1, 6, 5,
		1, 0, 6,

		2, 5, 4,
		2, 1, 5,

		2, 7, 4,
		2, 3, 7,

		6, 3, 0,
		6, 7, 3,
		
		4, 6, 5,
		4, 7, 6,
	};
	nAttributes += 1;
	(ID->indices_n) = sizeof(indices)/sizeof(indices[0]);
	(ID->type) = nAttributes;
	(ID->width) = width;
	(ID->height) = height;
	(ID->thickness) = thickness;
	(ID->pos[0]) = x;
	(ID->pos[1]) = y;
	(ID->pos[2]) = z;
	glm_vec3_fill(ID->scale, scale);
	hwidth = (float) width/200;
	hheihght = (float) height/200;
	hthickens = (float) thickness/200;
	
	for (i = 0; i < 3; i++)
	{
		nVertices += attributes[i];
	}
	
	/*left to behind*/
	vertices[0] 			= -hwidth;	vertices[1]					= -hheihght;	vertices[2]					= -hthickens; 
	vertices[nVertices] 	= -hwidth;	vertices[1+nVertices]		=  hheihght;	vertices[2+nVertices]		= -hthickens;

	/*behind to right*/
	vertices[(nVertices*2)]	= -hwidth;	vertices[1+(nVertices*2)]	=  hheihght;	vertices[2+(nVertices*2)]	=  hthickens;
	vertices[(nVertices*3)]	= -hwidth;	vertices[1+(nVertices*3)]	= -hheihght;	vertices[2+(nVertices*3)]	=  hthickens;


	/*right to front*/
	vertices[(nVertices*4)]	=  hwidth;	vertices[1+(nVertices*4)]	=  hheihght;	vertices[2+(nVertices*4)]	=  hthickens;
	vertices[(nVertices*5)]	=  hwidth;	vertices[1+(nVertices*5)]	=  hheihght;	vertices[2+(nVertices*5)]	= -hthickens;


	/*front to left*/
	vertices[(nVertices*6)]	=  hwidth;	vertices[1+(nVertices*6)]	= -hheihght;	vertices[2+(nVertices*6)]	= -hthickens;
	vertices[(nVertices*7)]	=  hwidth;	vertices[1+(nVertices*7)]	= -hheihght;	vertices[2+(nVertices*7)]	=  hthickens;

	glGenVertexArrays(1, &(ID->VAO));
	glGenBuffers(1, &(ID->VBO));
	glGenBuffers(1, &(ID->EBO));
	
	glBindVertexArray(ID->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ID->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	for (i = 0; i < nAttributes; i++)
	{
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, nVertices*sizeof(GLfloat), (void*)(n*sizeof(GLfloat)));
		glEnableVertexAttribArray(i);
		n += attributes[i];
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glm_mat4_identity(ID->model);
	glm_translate(ID->model, ID->pos);
	glm_scale(ID->model, (ID->scale));
	glBindVertexArray(0);
	return 1;
}

int instance_draw(Tobject ID, unsigned int *shader, Tcamera camera)
{
	useShader(shader);
	glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (float*)(ID.model));
	glBindVertexArray(ID.VAO);
	glDrawElements(GL_TRIANGLES, ID.indices_n, GL_UNSIGNED_INT, 0);
	return 1;
}

int instance_move(Tobject *ID)
{
	vec3 pos;
	glm_translate(ID->model, ID->pos);
	return 1;
}

int init_camera(Tcamera *camera, Tconfig config, float scale)
{
	(camera->allowMouse = 1);
	(camera->firstMouse) = true;
	(camera->pos[0])	= 0.0f; (camera->pos[1])	= 0.0f; (camera->pos[2])	=  3.0f;
	(camera->front[0])	= 0.0f; (camera->front[1])	= 0.0f; (camera->front[2])	= -1.0f;
	(camera->up[0])	= 0.0f; (camera->up[1])	= 1.0f; (camera->up[2])	=  0.0f;
	
	(camera->width) = (config.resWidth);
	(camera->height) = (config.resHeight);
	(camera->scale) = scale;
	(camera->yaw) = -90.0f;
	(camera->pitch) = 0.0f;
	(camera->lastX) = (float)(camera->width)/2;
	(camera->lastY) = (float)(camera->height)/2;
	(camera->fov) = 45;
	(camera->pos[2]) = 0.378339;
;
	
	glm_mat4_identity(camera->view);
	glm_mat4_identity(camera->projection);
	
	
	glGenFramebuffers(1, &(camera->frameBuffer));
	glGenRenderbuffers(1, &(camera->renderBufferColor));
	glGenRenderbuffers(1, &(camera->renderBufferDepth));
	glGenRenderbuffers(1, &(camera->renderBufferStencil));
	
	glBindRenderbuffer(GL_RENDERBUFFER, (camera->renderBufferColor));
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, (camera->width), (camera->height));

	glBindRenderbuffer(GL_RENDERBUFFER, (camera->renderBufferDepth));
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (camera->width), (camera->height));

	glBindRenderbuffer(GL_RENDERBUFFER, (camera->renderBufferStencil));
	
	glBindFramebuffer(GL_FRAMEBUFFER, (camera->frameBuffer));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
	                          GL_COLOR_ATTACHMENT0,
	                          GL_RENDERBUFFER,
	                          (camera->renderBufferColor));

	glBindFramebuffer(GL_FRAMEBUFFER, (camera->frameBuffer));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
	                          GL_DEPTH_STENCIL_ATTACHMENT,
	                          GL_RENDERBUFFER,
	                          (camera->renderBufferDepth));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 1;
}

int run_camera(Tcamera *camera, GLFWwindow *window)
{
	float x, y;
	double xpos, ypos;
	float xoffset, yoffset;
	float sensitivity = 0.1f;
	int perspective = 1;
	vec3 helper = {0.0f, 0.0f, -3.0f};
	vec3 front;
	
	(camera->nWidth) = (camera->width)/(camera->scale);
	(camera->nHeight) = (camera->height)/(camera->scale);
	
	(camera->speed) = (5.0f * camera->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm_vec3_muladds(camera->up, (camera->speed), camera->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		glm_vec3_muladds(camera->up, -(camera->speed), camera->pos);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm_vec3_muladds(camera->front, (camera->speed), camera->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm_vec3_muladds(camera->front, -(camera->speed), camera->pos);
	}
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm_vec3_cross(camera->up, camera->front, helper);
		glm_normalize(helper);
		glm_vec3_muladds(helper, (camera->speed), camera->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm_vec3_cross(camera->up, camera->front, helper);
		glm_normalize(helper);
		glm_vec3_muladds(helper, -(camera->speed), camera->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (camera->fov > 1)
		{
			(camera->fov) -= 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (camera->fov < 170)
		{
			(camera->fov) += 1;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (camera->scale < 2)
		{
			(camera->scale) += 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (camera->scale > 1)
		{
			(camera->scale) -= 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera->allowMouse = 0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		camera->allowMouse = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	}
	
	glfwGetCursorPos(window, &xpos, &ypos);
	
	if (camera->allowMouse)
	{
		if (camera->firstMouse)
		{
			(camera->lastX) = xpos;
			(camera->lastY) = ypos;
			(camera->firstMouse) = false;
		}
		
		xoffset = xpos - (camera->lastX);
		yoffset = (camera->lastY) - ypos;
		(camera->lastX) = xpos;
		(camera->lastY) = ypos;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		
		(camera->yaw) += xoffset;
		(camera->pitch) += yoffset;
		
		if ((camera->pitch) > 89.0f)
		{
			(camera->pitch) = 89.0f;
		}
		if ((camera->pitch) < -89.0f)
		{
			(camera->pitch) = -89.0f;
		}
		
	}
	front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	front[1] = sin(glm_rad(camera->pitch));
	front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	
	
	glm_normalize_to(front, camera->front);
	glm_vec3_add(camera->front, camera->pos, helper);
	
	glm_mat4_identity(camera->model);
	glm_mat4_identity(camera->view);
	glm_mat4_identity(camera->projection);
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		perspective = !perspective;
	}
	if (perspective)
	{
		glm_perspective(glm_rad(camera->fov), (float)(camera->width) / (float)(camera->height), 0.01f, 2000.0f, camera->projection);
	}
	else
	{
		glm_ortho(-((float)camera->width/160)/2, ((float)camera->width/160)/2, -((float)camera->height/160)/2, ((float)camera->height/160)/2, 0.01f, 2000.0f, camera->projection);
	}
	glm_lookat(camera->pos, helper, camera->up, camera->view);
	
	return 1;
}

int apply_camera(unsigned int *shader, Tcamera camera)
{
	return 0;
}

int prepare_light(unsigned int *surface_shader, unsigned int *light_shader, int n_light, vec3 color, float ambient_mag, float linear, float quadratic)
{
	char helper[24];
	char name[24];
	char num[5];
	strcpy(name, "light[");
	sprintf(num, "%d", n_light);
	strcat(name, num);
	strcat(name, "].");
	
	strcpy(helper, name);
	strcat(helper, "ambient");
	setVec3(surface_shader, helper, color[0]*ambient_mag, color[1]*ambient_mag, color[2]*ambient_mag);
	
	strcpy(helper, name);
	strcat(helper, "diffuse");
	setVec3(surface_shader, helper, color[0], color[1], color[2]);
	
	strcpy(helper, name);
	strcat(helper, "specular");
	setVec3(surface_shader, helper, color[0]*2.0f, color[1]*2.0f, color[2]*2.0f);
	
	strcpy(helper, name);
	strcat(helper, "constant");
	setFloat(surface_shader, helper, 1.0f);
	
	strcpy(helper, name);
	strcat(helper, "linear");
	/*
	setFloat(surface_shader, helper, 0.09f);
	*/
	setFloat(surface_shader, helper, linear);
	
	strcpy(helper, name);
	strcat(helper, "quadratic");
	/*
	setFloat(surface_shader, helper, 0.032f);
	*/
	setFloat(surface_shader, helper, quadratic);
	
	setVec3(light_shader, "color", color[0]*0.65f, color[1]*0.65f, color[2]*0.65f);
	return 0;
}

int load_model(Tmodel *model, char *directory, float x, float y, float z, int type)
{
	const struct aiScene *scene;
	const struct aiMesh *mesh;
	unsigned int numMeshes;
	unsigned int numVertices;
	unsigned int numFaces;
	float *vertices;
	float *normals;
	float *textures;
	unsigned int *indices;
	unsigned int index;
	unsigned int i, j, k;
	(model->pos[0]) = x;
	(model->pos[1]) = y;
	(model->pos[2]) = z;
	
	scene = aiImportFile(directory, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	(model->max_meshes) = scene->mNumMeshes;
	
	(model->VAO) = (unsigned int*) malloc(model->max_meshes * sizeof(unsigned int));
	(model->VBO) = (unsigned int*) malloc(model->max_meshes * sizeof(unsigned int));
	(model->EBO) = (unsigned int*) malloc(model->max_meshes * sizeof(unsigned int));
	(model->NBO) = (unsigned int*) malloc(model->max_meshes * sizeof(unsigned int));
	(model->indices) = (unsigned int*) malloc(model->max_meshes * sizeof(unsigned int));

	if (type)
	{
		(model->TBO) = (unsigned int*) malloc(model->max_meshes * sizeof(unsigned int));
	}
	(model->type) = type;
	
	
	if (!scene)
	{
		printf("No se ha podido importar el model\n");
	}
	
	for (i = 0; i < model->max_meshes; i++)
	{
		index = 0;
		mesh = scene->mMeshes[i];
		numVertices = mesh->mNumVertices;
		numFaces = mesh->mNumFaces;
		vertices = (float*) malloc(numVertices * 3 * sizeof(float));
		normals = (float*) malloc(numVertices * 3 * sizeof(float));
		indices = (unsigned int *) malloc(numFaces * 3 * sizeof(unsigned int));
		if (type)
		{
			textures = (float*) malloc(numVertices * 2 * sizeof(float));
			for (j = 0; j < numVertices; j++)
			{
				textures[j * 2]		= mesh->mTextureCoords[0][j].x;
				textures[j * 2 + 1] = mesh->mTextureCoords[0][j].y;
			}
		}
		
		for (j = 0; j < numVertices; j++)
		{
			vertices[j * 3] 	= mesh->mVertices[j].x;
			vertices[j * 3 + 1] = mesh->mVertices[j].y;
			vertices[j * 3 + 2] = mesh->mVertices[j].z;
			
			normals[j * 3]		= mesh->mNormals[j].x;
			normals[j * 3 + 1]  = mesh->mNormals[j].y;
			normals[j * 3 + 2]  = mesh->mNormals[j].z;
			
			
		}
		for (j = 0; j < numFaces; j++)
		{
			for (k = 0; k < mesh->mFaces[j].mNumIndices; k++)
			{
				indices[index++] = mesh->mFaces[j].mIndices[k];
			}
		}
	model->indices[i] = index;
	glGenBuffers(1, &(model->VBO[i]));
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &(model->NBO[i]));
	glBindBuffer(GL_ARRAY_BUFFER, model->NBO[i]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	
	glGenBuffers(1, &(model->EBO[i]));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	if (type)
	{
		glGenBuffers(1, &(model->TBO[i]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->TBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertices * 2 * sizeof(unsigned int), textures, GL_STATIC_DRAW);
	}
	
	
	glGenVertexArrays(1, &(model->VAO[i]));
	glBindVertexArray(model->VAO[i]);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO[i]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->NBO[i]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	if (type)
	{
		glBindBuffer(GL_ARRAY_BUFFER, model->TBO[i]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO[i]);
	glBindVertexArray(0);
	
	free(vertices);
	free(normals);
	free(indices);
	}
	
	aiReleaseImport(scene);
	glm_mat4_identity(model->model);
	glm_translate(model->model, model->pos);
	return 0;
}

int draw_model(Tmodel *model, unsigned int *shader, Tcamera camera)
{
	int i;
	useShader(shader);
	glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (float*)(model->model));
	for (i = 0; i < model->max_meshes; i++)
	{
		glBindVertexArray(model->VAO[i]);
		glDrawElements(GL_TRIANGLES, (model->indices[i] * 3), GL_UNSIGNED_INT, NULL);
	}
	return 0;
}

int prepare_renderer(Trenderer *renderer, Tconfig config)
{
	instance_create_quad(&(renderer->screen), 0, 0, 0, 200.0f, 200.0f, 1.0f, 2);
	glGenFramebuffers(1, &(renderer->frameBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->frameBuffer);
	
	glGenTextures(1, &(renderer->texcolBuffer));
	glBindTexture(GL_TEXTURE_2D, (renderer->texcolBuffer));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (config.resWidth), (config.resHeight), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->texcolBuffer, 0);
	
	glGenRenderbuffers(1, &(renderer->renderBuffer));
	glBindRenderbuffer(GL_RENDERBUFFER, renderer->renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (config.resWidth), (config.resHeight));
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->renderBuffer);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR EL FRAMEBUFFER NO ESTA COMPLETO\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

int run_rederer(Trenderer *renderer, Tcamera camera)
{
	bind_texture(&(renderer->shader), renderer->texcolBuffer, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.nWidth, camera.nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, camera.width, camera.height);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	instance_draw(renderer->screen, &(renderer->shader), camera);
	
	
	/*render scene*/
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, camera.nWidth, camera.nHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->frameBuffer);
	return 0;
}

int prepare_uniformblockData(unsigned int *shader, char *name)
{
	glUniformBlockBinding(*shader, glGetUniformBlockIndex(*shader, name), 0);
	return 0;
}

int prepare_uniformblockMatrices(unsigned int *uboBuffer, Tcamera camera)
{
	glGenBuffers(1, uboBuffer);
	
	glBindBuffer(GL_UNIFORM_BUFFER, *uboBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, *uboBuffer, 0, 2 * sizeof(mat4));
	
	glBindBuffer(GL_UNIFORM_BUFFER, *uboBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), (float*)(camera.projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, *uboBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), (float*)(camera.projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), (float*)(camera.view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	return 0;
}

int run_uniformblockMatrices(unsigned int uboBuffer, Tcamera camera)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), (float*)(camera.projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), (float*)(camera.view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return 0;
}

int init_config(Tconfig *config, int width, int height)
{
	(config->resWidth) = width;
	(config->resHeight) = height;
	(config->wireframe) = 0;
	return 0;
}
