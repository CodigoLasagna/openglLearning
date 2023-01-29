#version 330 core
out vec4 FragColor;
in vec3 custColor;

void main()
{
	FragColor = vec4(custColor, 1.0f);
}
