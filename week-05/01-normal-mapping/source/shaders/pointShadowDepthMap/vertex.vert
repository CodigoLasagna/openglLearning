#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 5) in mat4 instancedMatrix;

void main()
{
	gl_Position = instancedMatrix * vec4(aPos, 1.0f);
}
