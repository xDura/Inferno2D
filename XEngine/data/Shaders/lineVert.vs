#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 v_color;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	v_color = aColor;
}