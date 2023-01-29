#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec4 vecColor;
out vec3 custColor;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	ourColor = aColor;
	custColor = aColor;
	vecColor = vec4(aPos, 1.0f);
}
