#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emmision;
	float emmisionStr;
	float shininess;
};
in vec2 TexCoords;

struct Light{
	vec4 lightVector;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	float distance;
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 lightDir;
	
	vec3 norm = normalize(Normal);
	if (light.lightVector.w == 0.0f)
	{
		lightDir = normalize(light.lightVector.xyz);
	}
	else if (light.lightVector.w == 1.0f)
	{
		lightDir = normalize(light.lightVector.xyz - FragPos);
	}
	distance = length(light.lightVector.xyz - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) ;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	vec3 emmision = texture(material.emmision, TexCoords).rgb;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = (ambient + diffuse + specular + (emmision * material.emmisionStr));
	
	FragColor = vec4(result, 1.0f);
}
