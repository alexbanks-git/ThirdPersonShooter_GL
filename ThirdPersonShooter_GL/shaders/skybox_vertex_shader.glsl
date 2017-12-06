#version 400 core
layout (location = 0) in vec3 position;

out vec3 vert;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vert = position;
	gl_Position = (projection * view * vec4(position, 1.0f)).xyww;
}