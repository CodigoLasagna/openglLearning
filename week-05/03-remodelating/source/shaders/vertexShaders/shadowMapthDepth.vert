#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 instancedMatrix;

uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * instancedMatrix * vec4(aPos, 1.0f);
}
