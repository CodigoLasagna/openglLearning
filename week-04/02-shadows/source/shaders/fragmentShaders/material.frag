#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	/*
	float ambientStrenght = 0.1f;
	float specularStrenght = 0.5;
	*/
	vec3 ambient = light.ambient * material.ambient;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * material.diffuse) ;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	
	vec3 result = (ambient + diffuse + specular);
	
	FragColor = vec4(result, 1.0f);
}
