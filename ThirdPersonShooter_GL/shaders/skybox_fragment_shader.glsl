#version 400 core

in vec3 vert;
out vec4 frag_color;
uniform samplerCube cube_texture;

void main()
{
	frag_color = texture(cube_texture, vert);
}