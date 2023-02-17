#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 instancedMatrix;

out vec2 TexCoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	/*
	vec3 TangentLighPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	*/
	mat3 TBN;
} vs_out;

layout (std140) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};


void main()
{
	vec3 T, B, N;
	mat3 TBN;
	T = normalize(vec3(instancedMatrix * vec4(aTangent, 0.0f)));
	N = normalize(vec3(instancedMatrix * vec4(aNormal, 0.0f)));
	//B = normalize(vec3(instancedMatrix * vec4(aBitangent, 0.0f)));
	T = normalize(T - dot(T, N) * N);
	B = cross(N, T);
	TBN = mat3(T, B, N);
	
	vs_out.TBN = TBN;
	vs_out.FragPos = vec3(instancedMatrix * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(instancedMatrix))) * normalize(aNormal);
	vs_out.TexCoords = aTexCoords;
	gl_Position = projection * view * instancedMatrix * vec4(aPos, 1.0f);
}
