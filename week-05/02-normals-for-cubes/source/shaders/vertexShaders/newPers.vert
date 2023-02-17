#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
layout (std140) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normalize(aNormal);
	TexCoords = aTexCoord;
}
