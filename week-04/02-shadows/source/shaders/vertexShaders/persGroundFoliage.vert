#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 0) in vec3 aNormal;

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
	float amount;
	vec3 newPos = aPos;
	newPos = aPos * 2.0f;
	newPos.y += 1.0f;
	
	gl_Position = projection * view * model * vec4(newPos, 1.0f);
	FragPos = vec3(model * vec4(newPos, 1.0f));
	TexCoords = (aTexCoord - 0.5f) * 2.0f + 0.5f;
	TexCoords.y += 0.5f;
	Normal = mat3(transpose(inverse(model))) * aNormal;
}
