#include "myobjs.h"

int prepare_light(unsigned int *surface_shader, unsigned int *light_shader, int n_light, vec3 color)
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
	setVec3(surface_shader, helper, 0.2f, 0.2f, 0.2f);
	
	strcpy(helper, name);
	strcat(helper, "diffuse");
	setVec3(surface_shader, helper, color[0], color[1], color[2]);
	
	strcpy(helper, name);
	strcat(helper, "specular");
	setVec3(surface_shader, helper, color[0], color[1], color[2]);
	
	strcpy(helper, name);
	strcat(helper, "constant");
	setFloat(surface_shader, helper, 1.0f);
	
	strcpy(helper, name);
	strcat(helper, "linear");
	setFloat(surface_shader, helper, 0.09f);
	
	strcpy(helper, name);
	strcat(helper, "quadratic");
	setFloat(surface_shader, helper, 0.032f);
	
	setVec3(light_shader, "color", color[0], color[1], color[2]);
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
	return 0;
}

int draw_model(Tmodel *model, unsigned int *shader, Tcamera camera)
{
	int i;
	useShader(shader);
	glUniformMatrix4fv(glGetUniformLocation(*shader, "view"), 1, GL_FALSE, (float*)(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float*)(camera.projection));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (float*)(model->model));
	for (i = 0; i < model->max_meshes; i++)
	{
		glBindVertexArray(model->VAO[i]);
		glDrawElements(GL_TRIANGLES, (model->indices[i] * 3), GL_UNSIGNED_INT, NULL);
	}
	return 0;
}
