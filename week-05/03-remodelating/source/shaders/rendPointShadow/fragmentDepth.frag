#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	mat3 TBN;
} fs_in;

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emmisionMap;
	sampler2D normalMap;
	sampler2D depthMap;
	float emmisionStr;
	float shininess;
	
};
uniform samplerCube depthMap;

struct Light{
	vec3 position;
	bool type;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 8


uniform Material material;
uniform Light light[NR_POINT_LIGHTS];
uniform int n_lights;
uniform float far_plane;


uniform vec3 viewPos;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculations(vec3 fragPos, Light light);
vec2 ParallaxMapping(vec2 TexCoords, vec3 viewDir);
vec2 texCoordsHeightMap;

vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
	vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
	vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
	vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main()
{
	int i;
	vec3 normal;
	vec3 viewDir;
	
	viewDir = normalize(viewPos - fs_in.FragPos);
	texCoordsHeightMap = ParallaxMapping(fs_in.TexCoords, viewDir);
	normal = texture(material.normalMap, texCoordsHeightMap).rgb;
	normal = normal * 2.0f - 1.0f;
	normal = normalize(fs_in.TBN * normal);
	vec3 result = vec3(0.0f);
	for (i = 0; i < n_lights; i++)
	{
		result += CalcPointLight(light[i], normal, fs_in.FragPos, viewDir);
	}
	
	FragColor = vec4(result, 1.0f);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float distance;
	vec3 color;
	float spec = 0;
	vec3 specular = vec3(0.0f);
	vec3 lightDir, halfwayDir, reflectDir;
	int blinn = 0;
	float shadow;
	
	if (light.type)
	{
		lightDir = normalize(light.position);
	}
	else
	{
		lightDir = normalize(light.position - fs_in.FragPos);
	}
	distance = length(light.position - fs_in.FragPos);
	
	float diff = max(dot(normal, lightDir), 0.0f);
	
	halfwayDir = normalize(lightDir + viewDir);
	reflectDir = reflect(-lightDir, normal);

	if (material.shininess > 0)
	{
		if (blinn == 1)
		{
			spec = pow(max(dot(viewDir, halfwayDir), 0.0f), material.shininess);
		}
		else
		{
			spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		}
		specular = light.specular * spec * texture(material.specularMap, fs_in.TexCoords).rgb;
	}
	color = texture(material.diffuseMap, fs_in.TexCoords).rgb;
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, texCoordsHeightMap)) ;
	vec3 emmision = texture(material.emmisionMap, fs_in.TexCoords).rgb;
	
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	shadow = ShadowCalculations(fragPos, light);
	
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular + (emmision * material.emmisionStr) )) * color;
	
	return lighting;
}

float ShadowCalculations(vec3 fragPos, Light light)
{
	int i;
	vec3 fragToLight = fragPos - light.position;
	float currentDepth = length(fragToLight);
	float shadow = 0.0f;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = (1.0f + (viewDistance / far_plane)) / 25.0f;
	for (i = 0; i < samples; ++i)
	{
		float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= far_plane;
		if (currentDepth - bias > closestDepth)
		{
			shadow += 1.0f;
		}
	}
	shadow /= float(samples);
	
	return shadow;
}

vec2 ParallaxMapping(vec2 TexCoords, vec3 viewDir)
{
	float height_scale = 0.01;
	float height =  texture(material.depthMap, TexCoords).r;
	vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
	return TexCoords - p;
}
