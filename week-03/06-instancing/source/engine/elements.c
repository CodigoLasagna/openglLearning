#include "elements.h"

int instance_create_quad(Tobject *ID, float x, float y, float z, int width, int height, int nAttributes)
{
	int attributes[] = {3, 2};
	int i, nVertices = 0, n = 0;
	float hwidth, hheihght;
	GLfloat vertices[20] =
	{
		0.0f, 0.0f, 0.0f,   0, 0,
		0.0f, 0.0f, 0.0f,   1, 0,
		0.0f, 0.0f, 0.0f,   1, 1,
		0.0f, 0.0f, 0.0f,   0, 1
	};
	int indices[6] = 
	{
		0, 1, 2,
		0, 2, 3
	};
	nAttributes += 1;
	(ID->indices_n) = sizeof(indices);
	(ID->type) = nAttributes;
	(ID->width) = width;
	(ID->height) = height;
	(ID->pos[0]) = x;
	(ID->pos[1]) = y;
	(ID->pos[2]) = z;
	hwidth = (float) width/200;
	hheihght = (float) height/200;
	
	for (i = 0; i < nAttributes; i++)
	{
		nVertices += attributes[i];
	}
	
	vertices[0] = -hwidth;				vertices[1] = -hheihght;				vertices[2] = 0.0f;
	vertices[nVertices] = hwidth;		vertices[1+nVertices] = -hheihght;		vertices[2+nVertices] = 0.0f;
	vertices[(nVertices*2)] = hwidth;	vertices[1+(nVertices*2)] = hheihght;	vertices[2+(nVertices*2)] = 0.0f;
	vertices[(nVertices*3)] = -hwidth;	vertices[1+(nVertices*3)] = hheihght;	vertices[2+(nVertices*3)] = 0.0f;
	
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
		n = attributes[i];
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glm_mat4_identity(ID->model);
	glm_translate(ID->model, ID->pos);
	glBindVertexArray(0);
	return 1;
}
int instance_create_cube(Tobject *ID, float x, float y, float z, int width, int height, int thickness, int nAttributes)
{
	int attributes[] = {3, 2};
	int i, nVertices = 0, n = 0;
	float hwidth, hheihght, hthickens;
	GLfloat vertices[64] =
	{
		0.0f, 0.0f, 0.0f,		0, 0,
		0.0f, 0.0f, 0.0f,		1, 0,

		0.0f, 0.0f, 0.0f,		1, 1,
		0.0f, 0.0f, 0.0f,		0, 1,

		0.0f, 0.0f, 0.0f,		0, 1,
		0.0f, 0.0f, 0.0f,		0, 0,

		0.0f, 0.0f, 0.0f,		1, 0,
		0.0f, 0.0f, 0.0f,		1, 1,
	};
	int indices[36] = 
	{
		0, 2, 3,
		0, 1, 2,

		2, 4, 5,
		2, 5, 1,

		4, 6, 7,
		4, 5, 6,

		0, 3, 6,
		3, 6, 7,
			
		1, 5, 6,
		1, 6, 0,

		2, 4, 7,
		2, 7, 3

	};
	nAttributes += 1;
	(ID->indices_n) = sizeof(indices);
	(ID->type) = nAttributes;
	(ID->width) = width;
	(ID->height) = height;
	(ID->thickness) = thickness;
	(ID->pos[0]) = x;
	(ID->pos[1]) = y;
	(ID->pos[2]) = z;
	hwidth = (float) width/200;
	hheihght = (float) height/200;
	hthickens = (float) thickness/200;
	
	for (i = 0; i < nAttributes; i++)
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
		n = attributes[i];
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glm_mat4_identity(ID->model);
	glm_translate(ID->model, ID->pos);
	glBindVertexArray(0);
	return 1;
}

int instance_draw(Tobject ID, unsigned int *shader, Tcamera camera)
{
	useShader(shader);
	/*
	deprecated
	glUniformMatrix4fv(glGetUniformLocation(*shader, "view"), 1, GL_FALSE, (float*)(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float*)(camera.projection));
	*/
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

int init_camera(Tcamera *camera, int width, int height, float scale)
{
	camera->firstMouse = true;
	(camera->pos[0])	= 0.0f; (camera->pos[1])	= 0.0f; (camera->pos[2])	=  3.0f;
	(camera->front[0])	= 0.0f; (camera->front[1])	= 0.0f; (camera->front[2])	= -1.0f;
	(camera->up[0])	= 0.0f; (camera->up[1])	= 1.0f; (camera->up[2])	=  0.0f;
	
	(camera->width) = width;
	(camera->height) = height;
	(camera->scale) = scale;
	(camera->yaw) = -90.0f;
	(camera->pitch) = 0.0f;
	(camera->lastX) = (float)width/2;
	(camera->lastY) = (float)height/2;
	(camera->fov) = 45;
	
	glm_mat4_identity(camera->view);
	glm_mat4_identity(camera->projection);
	
	
	glGenFramebuffers(1, &(camera->frameBuffer));
	glGenRenderbuffers(1, &(camera->renderBufferColor));
	glGenRenderbuffers(1, &(camera->renderBufferDepth));
	glGenRenderbuffers(1, &(camera->renderBufferStencil));
	
	glBindRenderbuffer(GL_RENDERBUFFER, (camera->renderBufferColor));
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA , width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, (camera->renderBufferDepth));
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

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

int run_camera(Tcamera *camera, GLFWwindow *window, float delta)
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
	
	(camera->speed) = (2.5f * delta);
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
	
	glfwGetCursorPos(window, &xpos, &ypos);
	
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
		glm_ortho(-((float)camera->width/100)/2, ((float)camera->width/100)/2, -((float)camera->height/100)/2, ((float)camera->height/100)/2, 0.01f, 2000.0f, camera->projection);
	}
	glm_lookat(camera->pos, helper, camera->up, camera->view);
	
	return 1;
}

int apply_camera(unsigned int *shader, Tcamera camera)
{
	return 0;
}

