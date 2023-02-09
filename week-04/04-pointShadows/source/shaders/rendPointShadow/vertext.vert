#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instancedMatrix;

out vec2 TexCoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

layout (std140) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};


void main()
{
	vs_out.FragPos = vec3(instancedMatrix * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(instancedMatrix))) * normalize(aNormal);
	vs_out.TexCoords = aTexCoords;
	gl_Position = projection * view * instancedMatrix * vec4(aPos, 1.0f);
	/*
	gl_Position = lightSpaceMatrix * instancedMatrix * vec4(aPos, 1.0f);
	*/
}
