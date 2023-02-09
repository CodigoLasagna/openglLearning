#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instancedMatrix;

layout (std140) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};
uniform mat4 model;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * instancedMatrix * vec4(aPos, 1.0f);
	FragPos = vec3(instancedMatrix * vec4(aPos, 1.0f));
	TexCoords = aTexCoord;
	Normal = mat3(transpose(inverse(instancedMatrix))) * aNormal;
}
