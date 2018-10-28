#version 400 core
in vec3 frag_position;
in vec2 tex_coords;

out vec4 frag_color;
uniform sampler2D sampler;
uniform vec4 color;

void main()
{
	frag_color = color;
}