#include "elements.h"
#include "textures.h"

int instance_create_quad(Tobject *ID, float x, float y, float z, int width, int height, float scale, int nAttributes)
{
	int attributes[] = {3, 3, 2, 3, 3};
	int i, nVertices = 0, n = 0;
	float hwidth, hheihght;
	/*
	GLfloat vertices[32] =
	{
		0.0f, 0.0f, 0.0f,	-0.025f, -0.025f, 5.0f,	0, 0,
		0.0f, 0.0f, 0.0f,	 0.025f, -0.025f, 5.0f,	1, 0,
		0.0f, 0.0f, 0.0f,	 0.025f,  0.025f, 5.0f,	1, 1,
		0.0f, 0.0f, 0.0f,	-0.025f,  0.025f, 5.0f,	0, 1
	};
	GLfloat vertices[32] =
	{
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0, 0,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1, 0,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1, 1,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0, 1,
	};
	*/
	GLfloat vertices[56] =
	{
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0, 0,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1, 0,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1, 1,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0, 1,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	};
	int indices[6] = 
	{
		0, 2, 3,
		0, 1, 2,
	};
	vec3 posInd[4];
	vec2 uvInd[4];
	vec3 nmInd;
	vec3 edge[2];
	vec3 deltaUv[2];
	
	vec3 tangent[2];
	vec3 bitangent[2];
	float f;

	ID->max_meshes = 1;
	/*nAttributes += 1;*/
	(ID->indices_n) = sizeof(indices)/sizeof(indices[0]);
	(ID->type) = nAttributes;
	(ID->width) = width;
	(ID->height) = height;
	(ID->position[0]) = x;
	(ID->position[1]) = y;
	(ID->position[2]) = z;
	glm_vec3_fill(ID->scale, scale);
	hwidth = (float) width/200;
	hheihght = (float) height/200;
	
	for (i = 0; i < nAttributes; i++)
	{
		nVertices += attributes[i];
	}
	
	vertices[(nVertices*0)] = -hwidth;	vertices[1+(nVertices*0)] = -hheihght;	vertices[2+(nVertices*0)] = 0.0f;
	vertices[(nVertices*1)] =  hwidth;	vertices[1+(nVertices*1)] = -hheihght;	vertices[2+(nVertices*1)] = 0.0f;
	vertices[(nVertices*2)] =  hwidth;	vertices[1+(nVertices*2)] =  hheihght;	vertices[2+(nVertices*2)] = 0.0f;
	vertices[(nVertices*3)] = -hwidth;	vertices[1+(nVertices*3)] =  hheihght;	vertices[2+(nVertices*3)] = 0.0f;
	
	/*Normal Map tangent space*/
	posInd[0][0] = -hwidth;	posInd[0][1] = -hheihght;	posInd[0][2] = 0.0f;
	posInd[1][0] =  hwidth;	posInd[1][1] = -hheihght;	posInd[1][2] = 0.0f;
	posInd[2][0] =  hwidth;	posInd[2][1] =  hheihght;	posInd[2][2] = 0.0f;
	posInd[3][0] = -hwidth;	posInd[3][1] =  hheihght;	posInd[3][2] = 0.0f;
	
	uvInd[0][0] = 0;	uvInd[0][1] = 0;
	uvInd[1][0] = 1;	uvInd[1][1] = 0;
	uvInd[2][0] = 1;	uvInd[2][1] = 1;
	uvInd[3][0] = 0;	uvInd[3][1] = 1;
	
	nmInd[0] = 0.0f; nmInd[1] = 0.0f; nmInd[2] = 1.0f; 
	
	/*first triangle*/
	glm_vec3_sub(posInd[1], posInd[0], edge[0]);
	glm_vec3_sub(posInd[2], posInd[0], edge[1]);
	
	glm_vec2_sub(uvInd[1], uvInd[0], deltaUv[0]);
	glm_vec2_sub(uvInd[2], uvInd[0], deltaUv[1]);

	f = 1.0f / (deltaUv[0][0] * deltaUv[1][1] - deltaUv[1][0] * deltaUv[0][1]);
	
	tangent[0][0] = f * (deltaUv[1][1] * edge[0][0] - deltaUv[0][1] * edge[1][0]);
	tangent[0][1] = f * (deltaUv[1][1] * edge[0][1] - deltaUv[0][1] * edge[1][1]);
	tangent[0][2] = f * (deltaUv[1][1] * edge[0][2] - deltaUv[0][1] * edge[1][2]);
	
	bitangent[0][0] = f * (-deltaUv[1][0] * edge[0][0] + deltaUv[0][0] * edge[1][0]);
	bitangent[0][1] = f * (-deltaUv[1][0] * edge[0][1] + deltaUv[0][0] * edge[1][1]);
	bitangent[0][2] = f * (-deltaUv[1][0] * edge[0][2] + deltaUv[0][0] * edge[1][2]);
	/*second triangle*/
	glm_vec3_sub(posInd[2], posInd[0], edge[0]);
	glm_vec3_sub(posInd[3], posInd[0], edge[1]);
	
	glm_vec2_sub(uvInd[2], uvInd[0], deltaUv[0]);
	glm_vec2_sub(uvInd[3], uvInd[0], deltaUv[1]);
	
	f = 1.0f / (deltaUv[0][0] * deltaUv[1][1] - deltaUv[1][0] * deltaUv[0][1]);
	
	tangent[1][0] = f * (deltaUv[1][1] * edge[0][0] - deltaUv[0][1] * edge[1][0]);
	tangent[1][1] = f * (deltaUv[1][1] * edge[0][1] - deltaUv[0][1] * edge[1][1]);
	tangent[1][2] = f * (deltaUv[1][1] * edge[0][2] - deltaUv[0][1] * edge[1][2]);
	
	bitangent[1][0] = f * (-deltaUv[1][0] * edge[0][0] + deltaUv[0][0] * edge[1][0]);
	bitangent[1][1] = f * (-deltaUv[1][0] * edge[0][1] + deltaUv[0][0] * edge[1][1]);
	bitangent[1][2] = f * (-deltaUv[1][0] * edge[0][2] + deltaUv[0][0] * edge[1][2]);
	
	/*adding tangent and bitangent to vector*/
	vertices[8+(nVertices*0)] = tangent[1][0]; vertices[9+(nVertices*0)] = tangent[1][1]; vertices[10+(nVertices*0)] = tangent[1][2]; 
	vertices[8+(nVertices*1)] = tangent[1][0]; vertices[9+(nVertices*1)] = tangent[1][1]; vertices[10+(nVertices*1)] = tangent[1][2]; 
	vertices[8+(nVertices*2)] = tangent[1][0]; vertices[9+(nVertices*2)] = tangent[1][1]; vertices[10+(nVertices*2)] = tangent[1][2]; 
	vertices[8+(nVertices*3)] = tangent[1][0]; vertices[9+(nVertices*3)] = tangent[1][1]; vertices[10+(nVertices*3)] = tangent[1][2]; 
	
	vertices[11+(nVertices*0)] = bitangent[1][0]; vertices[12+(nVertices*0)] = bitangent[1][1]; vertices[13+(nVertices*0)] = bitangent[1][2]; 
	vertices[11+(nVertices*1)] = bitangent[1][0]; vertices[12+(nVertices*1)] = bitangent[1][1]; vertices[13+(nVertices*1)] = bitangent[1][2]; 
	vertices[11+(nVertices*2)] = bitangent[1][0]; vertices[12+(nVertices*2)] = bitangent[1][1]; vertices[13+(nVertices*2)] = bitangent[1][2]; 
	vertices[11+(nVertices*3)] = bitangent[1][0]; vertices[12+(nVertices*3)] = bitangent[1][1]; vertices[13+(nVertices*3)] = bitangent[1][2]; 
	
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
	glm_translate(ID->model, ID->position);
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
		0, 3, 2,
		0, 2, 1,

		1, 5, 6,
		1, 6, 0,

		2, 4, 5,
		2, 5, 1,

		2, 7, 4,
		2, 3, 7,

		6, 3, 0,
		6, 7, 3,
		
		4, 6, 5,
		4, 7, 6,
	};
	ID->max_meshes = 1;
	nAttributes += 1;
	(ID->indices_n) = sizeof(indices)/sizeof(indices[0]);
	(ID->type) = nAttributes;
	(ID->width) = width;
	(ID->height) = height;
	(ID->thickness) = thickness;
	(ID->position[0]) = x;
	(ID->position[1]) = y;
	(ID->position[2]) = z;
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
	glm_translate(ID->model, ID->position);
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

int instanced_object_buffer(unsigned int *iBuffer, Tobject *ID, unsigned int amount, mat4 matrices[])
{
	size_t vec4size = sizeof(vec4);
	glGenBuffers(1, iBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *iBuffer);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int) (amount * sizeof(mat4)), (float*)matrices, GL_STATIC_DRAW);
	glBindVertexArray(ID->VAO);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (0 * vec4size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (1 * vec4size));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (2 * vec4size));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*) (3 * vec4size));
	
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return 1;
}

int instanced_object_draw(Tobject ID, unsigned int amount)
{
	glBindVertexArray(ID.VAO);
	glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) (ID.indices_n), GL_UNSIGNED_INT, 0, amount);
	return 0;
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

int prepare_lightobj(unsigned int *light_shader, vec3 color)
{
	glm_vec3_muladds(color, 0.65, color);
	setVec3(light_shader, "color", color);
	return 0;
}

int prepare_material(unsigned int *surface_shader, int lights_number, float specular_str)
{
	setInt(surface_shader, "n_lights", lights_number);
	setFloat(surface_shader, "material.shininess", specular_str);
	setInt(surface_shader, "material.diffuse", 0);
	setInt(surface_shader, "material.specular", 1);
	setInt(surface_shader, "material.normal", 2);
	
	return 0;
}

int prepare_material_lum(unsigned int *surface_shader, int light_n, bool light_type, vec3 color, float ambient_mag, float linear, float quadratic)
{
	char helper[24];
	char name[24];
	char num[5];
	vec3 vecHelper;
	glm_vec3_fill(vecHelper, 0);
	strcpy(name, "light[");
	sprintf(num, "%d", light_n);
	strcat(name, num);
	strcat(name, "].");
	
	strcpy(helper, name);
	strcat(helper, "ambient");
	glm_vec3_muladds(color, ambient_mag, vecHelper);
	setVec3(surface_shader, helper, vecHelper);
	
	strcpy(helper, name);
	strcat(helper, "diffuse");
	setVec3(surface_shader, helper, color);
	
	strcpy(helper, name);
	strcat(helper, "specular");
	setVec3(surface_shader, helper, color);
	
	strcpy(helper, name);
	strcat(helper, "constant");
	setFloat(surface_shader, helper, 1.0f);
	
	strcpy(helper, name);
	strcat(helper, "linear");
	setFloat(surface_shader, helper, linear);
	
	strcpy(helper, name);
	strcat(helper, "quadratic");
	setFloat(surface_shader, helper, quadratic);

	strcpy(helper, name);
	strcat(helper, "type");
	setBool(surface_shader, helper, light_type);
	
	return 0;
}

int load_model(Tmodel *model, char *directory, float x, float y, float z, float scale, int type)
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
	glm_vec3_fill(model->scale, scale);
	
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
	glm_scale(model->model, model->scale);
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
		glDrawElements(GL_TRIANGLES, model->indices[i], GL_UNSIGNED_INT, NULL);
	}
	return 0;
}

int instanced_model_buffer(unsigned int *iBuffer, Tmodel *ID, unsigned int amount, mat4 matrices[])
{
	size_t vec4size = sizeof(vec4);
	int i;
	glGenBuffers(1, iBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *iBuffer);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int) (amount * sizeof(mat4)), (float*)matrices, GL_STATIC_DRAW);
	for (i = 0; i < ID->max_meshes; i++)
	{
		glBindVertexArray(ID->VAO[i]);
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return 1;
}

int instanced_model_draw(Tmodel ID, unsigned int amount)
{
	int i;
	for (i = 0; i < ID.max_meshes; i++)
	{
		glBindVertexArray(ID.VAO[i]);
		glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) (ID.indices[i]), GL_UNSIGNED_INT, 0, amount);
	}
	return 0;
}

int prepare_renderer(Trenderer *renderer, Tconfig config)
{
	instance_create_quad(&(renderer->screen), 0, 0, 0, 200.0f, 200.0f, 1.0f, 5);
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

int run_renderer(Trenderer *renderer, Tcamera camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bind_texture(renderer->texcolBuffer, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.nWidth, camera.nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, camera.width, camera.height);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	instance_draw(renderer->screen, &(renderer->shader), camera);
	
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, camera.nWidth, camera.nHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->frameBuffer);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return 0;
}
int run_rendererShader(Trenderer *renderer, unsigned int *shader, Tcamera camera)
{
	bind_texture(renderer->texcolBuffer, 0);
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	instance_draw(renderer->screen, shader, camera);
	
	
	/*render scene*/
	/*
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, camera.nWidth, camera.nHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->frameBuffer);
	*/
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

int prepare_shadow_cubemap(TshadowCM *shadowCM, unsigned int shadow_width, unsigned int shadow_height, float near_plane, float far_plane)
{
	unsigned int i;
	vec3 lav[6] =
	{
		{ 1.0f, 0.0f,  0.0f},
		{-1.0f, 0.0f,  0.0f},
		{ 0.0f, 1.0f,  0.0f},
		{ 0.0f,-1.0f,  0.0f},
		{ 0.0f, 0.0f,  1.0f},
		{ 0.0f, 0.0f, -1.0f},
	};
	shadowCM->SHADOW_WIDTH = shadow_width;
	shadowCM->SHADOW_HEIGHT = shadow_height;
	shadowCM->near_plane = near_plane;
	shadowCM->far_plane = far_plane;
	shadowCM->aspect_ratio = (float) shadow_width / (float) shadow_height;
	glm_vec3_copy(lav[0], shadowCM->lookatVecs[0]);
	glm_vec3_copy(lav[1], shadowCM->lookatVecs[1]);
	glm_vec3_copy(lav[2], shadowCM->lookatVecs[2]);
	glm_vec3_copy(lav[3], shadowCM->lookatVecs[3]);
	glm_vec3_copy(lav[4], shadowCM->lookatVecs[4]);
	glm_vec3_copy(lav[5], shadowCM->lookatVecs[5]);
	
	glGenTextures(1, &(shadowCM->cube_map_texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCM->cube_map_texture);
	for (i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
	
	glGenFramebuffers(1, &(shadowCM->frameBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, shadowCM->frameBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCM->cube_map_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

int calculate_shadow_cubemap_light(TshadowCM *shadowCM, Tobject light, Tcamera camera)
{
	glm_perspective(glm_rad(90.0f), shadowCM->aspect_ratio, shadowCM->near_plane, shadowCM->far_plane, shadowCM->shadowProj);
	
	glm_vec3_add(light.position, shadowCM->lookatVecs[0], shadowCM->vecHelper);
	glm_lookat(light.position, shadowCM->vecHelper, shadowCM->lookatVecs[3], shadowCM->matHelper);
	glm_mul(shadowCM->shadowProj, shadowCM->matHelper, shadowCM->shadowTransforms[0]);
	
	glm_vec3_add(light.position, shadowCM->lookatVecs[1], shadowCM->vecHelper);
	glm_lookat(light.position, shadowCM->vecHelper, shadowCM->lookatVecs[3], shadowCM->matHelper);
	glm_mul(shadowCM->shadowProj, shadowCM->matHelper, shadowCM->shadowTransforms[1]);
	
	glm_vec3_add(light.position, shadowCM->lookatVecs[2], shadowCM->vecHelper);
	glm_lookat(light.position, shadowCM->vecHelper, shadowCM->lookatVecs[4], shadowCM->matHelper);
	glm_mul(shadowCM->shadowProj, shadowCM->matHelper, shadowCM->shadowTransforms[2]);
	
	glm_vec3_add(light.position, shadowCM->lookatVecs[3], shadowCM->vecHelper);
	glm_lookat(light.position, shadowCM->vecHelper, shadowCM->lookatVecs[5], shadowCM->matHelper);
	glm_mul(shadowCM->shadowProj, shadowCM->matHelper, shadowCM->shadowTransforms[3]);
	
	glm_vec3_add(light.position, shadowCM->lookatVecs[4], shadowCM->vecHelper);
	glm_lookat(light.position, shadowCM->vecHelper, shadowCM->lookatVecs[3], shadowCM->matHelper);
	glm_mul(shadowCM->shadowProj, shadowCM->matHelper, shadowCM->shadowTransforms[4]);
	
	glm_vec3_add(light.position, shadowCM->lookatVecs[5], shadowCM->vecHelper);
	glm_lookat(light.position, shadowCM->vecHelper, shadowCM->lookatVecs[3], shadowCM->matHelper);
	glm_mul(shadowCM->shadowProj, shadowCM->matHelper, shadowCM->shadowTransforms[5]);
	
	useShader(&(shadowCM->render_shader));
	bind_cubemap(shadowCM->cube_map_texture, 10);
	setVec3(&(shadowCM->render_shader), "viewPos", camera.pos);
	setFloat(&(shadowCM->render_shader), "far_plane", shadowCM->far_plane);
	setFloat(&(shadowCM->depth_shader), "far_plane", shadowCM->far_plane);
	useShader(&(shadowCM->depth_shader));
	setMat4(&(shadowCM->depth_shader), "shadowMatrices[0]", shadowCM->shadowTransforms[0]);
	setMat4(&(shadowCM->depth_shader), "shadowMatrices[1]", shadowCM->shadowTransforms[1]);
	setMat4(&(shadowCM->depth_shader), "shadowMatrices[2]", shadowCM->shadowTransforms[2]);
	setMat4(&(shadowCM->depth_shader), "shadowMatrices[3]", shadowCM->shadowTransforms[3]);
	setMat4(&(shadowCM->depth_shader), "shadowMatrices[4]", shadowCM->shadowTransforms[4]);
	setMat4(&(shadowCM->depth_shader), "shadowMatrices[5]", shadowCM->shadowTransforms[5]);
	
	glViewport(0, 0, shadowCM->SHADOW_WIDTH, shadowCM->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowCM->frameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	
	return 0;
}
