#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emmision;
	float emmisionStr;
	float shininess;
	
};
uniform sampler2D shadowMap;

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


uniform vec3 viewPos;

/*
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
*/

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculations(vec4 FragPosLightSpace, Light light);

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
	
	/*
	float gamma = 2.2f;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0f/gamma));
	*/
	
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
	
	shadow = ShadowCalculations(fs_in.FragPosLightSpace, light);
	
	/*
	vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;
	vec3 lightColor = vec3(0.3f);
	// ambient
	vec3 ambient = 0.3 * lightColor;
	// diffuse
	vec3 lightDir = normalize(light.lightVector.xyz - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0f;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0);
	vec3 specular = spec * lightColor;
	// shadow
	float shadow = ShadowCalculations(fs_in.FragPosLightSpace);
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;
	*/
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular + (emmision * material.emmisionStr) )) * color;

	
	
	return lighting;
	/*
	return (ambient + (1.0f - shadow) * (diffuse + specular + (emmision * material.emmisionStr))) * color ;
	*/
}

float ShadowCalculations(vec4 FragPosLightSpace, Light light)
{
    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.lightVector.xyz - fs_in.FragPos);
    float bias = max(0.01f * (1.0f - dot(normal, lightDir)), 0.005f);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
