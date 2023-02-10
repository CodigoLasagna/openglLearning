#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emmision;
	float emmisionStr;
	float shininess;
	
};
uniform samplerCube depthMap;

struct Light{
	vec4 lightVector;
	
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
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 result = vec3(0.0f);
	for (i = 0; i < n_lights; i++)
	{
		result += CalcPointLight(light[i], norm, fs_in.FragPos, viewDir);
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
	
	if (light.lightVector.w == 1.0f)
	{
		lightDir = normalize(light.lightVector.xyz);
	}
	else if (light.lightVector.w == 0.0f)
	{
		lightDir = normalize(light.lightVector.xyz - fs_in.FragPos);
	}
	distance = length(light.lightVector.xyz - fs_in.FragPos);
	
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
		specular = light.specular * spec * texture(material.specular, fs_in.TexCoords).rgb;
	}
	color = texture(material.diffuse, fs_in.TexCoords).rgb;
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords)) ;
	vec3 emmision = texture(material.emmision, fs_in.TexCoords).rgb;
	
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
	vec3 fragToLight = fragPos - light.lightVector.xyz;
	float currentDepth = length(fragToLight);
	float shadow = 0.0f;
	float bias = 0.15;
	int samples = 30;
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
