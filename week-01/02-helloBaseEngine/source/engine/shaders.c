#include "shaders.h"

char *readVertexShader(char *directory)
{
	FILE *file;
	long size = 0;
	char *vertexShaderSource;
	
	file = fopen(directory, "rb");
	if(vertexShaderSource == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen(directory, "r");
	vertexShaderSource = memset(malloc(size), '\0', size);
	fread(vertexShaderSource, 1, size-1, file);
	fclose(file);
	return vertexShaderSource;
}

char *readFragmentShader(char *directory)
{
	FILE *file;
	long size = 0;
	char *fragmentShaderSource;
	
	file = fopen(directory, "rb");
	if(fragmentShaderSource == NULL) {
		return "";
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file)+1;
	fclose(file);
	
	file = fopen(directory, "r");
	fragmentShaderSource = memset(malloc(size), '\0', size);
	fread(fragmentShaderSource, 1, size-1, file);
	fclose(file);
	return fragmentShaderSource;
}

void createShader(unsigned int *ID, char *vertexPath, char *fragmentPath)
{
	const char *vertexCode, *fragmentCode;
	unsigned int vertex, fragment;
	char infoLog[512];
	int succes;
	
	vertexCode = readVertexShader(vertexPath);
	fragmentCode = readFragmentShader(fragmentPath);
	
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
void useShader(unsigned int *ID)
{
	glUseProgram(*ID);
}

void setBool(unsigned int *ID, char *name, bool value)
{
	glUniform1f(glGetUniformLocation(*ID, name), (int) value);
}
void setInt(unsigned int *ID, char *name, int value)
{
	glUniform1i(glGetUniformLocation(*ID, name), value);
}
void setFloat(unsigned int *ID, char *name, float value)
{
	glUniform1f(glGetUniformLocation(*ID, name), value);
}
