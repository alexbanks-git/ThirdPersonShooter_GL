#version 400 core
in vec3 frag_position;
in vec2 tex_coords;

out vec4 frag_color;
uniform sampler2D sampler;

void main()
{
	frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}