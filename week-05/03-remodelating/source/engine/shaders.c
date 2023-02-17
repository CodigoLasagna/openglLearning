#include "shaders.h"

char *readShaderFromFile(char *directory)
{
	FILE *file;
	long size = 0;
	char *shader_source;
	
	file = fopen(directory, "rb");
	if(shader_source == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen(directory, "r");
	shader_source = memset(malloc(size), '\0', size);
	fread(shader_source, 1, size-1, file);
	fclose(file);
	return shader_source;
}

void createShader(unsigned int *ID, char *vertexPath, char *fragmentPath)
{
	const char *vertexCode, *fragmentCode;
	unsigned int vertex, fragment;
	char infoLog[512];
	int succes;
	
	vertexCode = readShaderFromFile(vertexPath);
	fragmentCode = readShaderFromFile(fragmentPath);
	
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR EN EL FRAGMENT SHADER ::\n%s\n", infoLog);
	}
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR EN EL VERTEX SHADER ::\n%s\n", infoLog);
	}
	
	*ID = glCreateProgram();
	glAttachShader(*ID, vertex);
	glAttachShader(*ID, fragment);
	glLinkProgram(*ID);
	glGetProgramiv(*ID, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		glGetProgramInfoLog(*ID, 512, NULL, infoLog);
		printf("ERROR EN EL PROGRAM SHADER ::\n%s\n", infoLog);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
}

void createShader_geometry(unsigned int *ID, char *vertexPath, char *fragmentPath, char *geometryPath)
{
	const char *vertexCode, *fragmentCode, *geometryCode;
	unsigned int vertex, fragment, geometry;
	char infoLog[512];
	int succes;
	
	vertexCode = readShaderFromFile(vertexPath);
	fragmentCode = readShaderFromFile(fragmentPath);
	geometryCode = readShaderFromFile(geometryPath);
	
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR EN EL FRAGMENT SHADER ::\n%s\n", infoLog);
	}
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR EN EL VERTEX SHADER ::\n%s\n", infoLog);
	}
	
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &geometryCode, NULL);
	glCompileShader(geometry);
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(geometry, 512, NULL, infoLog);
		printf("ERROR EN EL GEOMETRY SHADER ::\n%s\n", infoLog);
	}
	
	*ID = glCreateProgram();
	glAttachShader(*ID, vertex);
	glAttachShader(*ID, fragment);
	glAttachShader(*ID, geometry);
	glLinkProgram(*ID);
	glGetProgramiv(*ID, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		glGetProgramInfoLog(*ID, 512, NULL, infoLog);
		printf("ERROR EN EL PROGRAM SHADER ::\n%s\n", infoLog);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

void useShader(unsigned int *ID)
{
	glUseProgram(0);
	glUseProgram(*ID);
}

void setBool(unsigned int *ID, char *name, bool value)
{
	useShader(ID);
	glUniform1f(glGetUniformLocation(*ID, name), (int) value);
}
void setInt(unsigned int *ID, char *name, int value)
{
	useShader(ID);
	glUniform1i(glGetUniformLocation(*ID, name), value);
}
void setFloat(unsigned int *ID, char *name, float value)
{
	useShader(ID);
	glUniform1f(glGetUniformLocation(*ID, name), value);
}

void setVec3(unsigned int *ID, char *name, vec3 vect)
{
	useShader(ID);
	glUniform3f(glGetUniformLocation(*ID, name), vect[0], vect[1], vect[2]);
}

void setVec4(unsigned int *ID, char *name, float v1, float v2, float v3, float v4)
{
	useShader(ID);
	glUniform4f(glGetUniformLocation(*ID, name), v1, v2, v3, v4);
}
void setMat4(unsigned int *ID, char *name, mat4 mat)
{
	useShader(ID);
	glUniformMatrix4fv(glGetUniformLocation(*ID, name), 1, GL_FALSE, (float*)(mat));
}
