#include "myobjs.h"

int prepare_light(unsigned int *surface_shader, unsigned int *light_shader, int n_light, vec3 color)
{
	char helper[24];
	char name[24];
	char num[5];
	strcpy(name, "light[");
	sprintf(num, "%d", n_light);
	strcat(name, num);
	strcat(name, "].");
	
	strcpy(helper, name);
	strcat(helper, "ambient");
	setVec3(surface_shader, helper, 0.2f, 0.2f, 0.2f);
	
	strcpy(helper, name);
	strcat(helper, "diffuse");
	setVec3(surface_shader, helper, color[0], color[1], color[2]);
	
	strcpy(helper, name);
	strcat(helper, "specular");
	setVec3(surface_shader, helper, color[0], color[1], color[2]);
	
	strcpy(helper, name);
	strcat(helper, "constant");
	setFloat(surface_shader, helper, 1.0f);
	
	strcpy(helper, name);
	strcat(helper, "linear");
	setFloat(surface_shader, helper, 0.09f);
	
	strcpy(helper, name);
	strcat(helper, "quadratic");
	setFloat(surface_shader, helper, 0.032f);
	
	setVec3(light_shader, "color", color[0], color[1], color[2]);
	return 0;
}
