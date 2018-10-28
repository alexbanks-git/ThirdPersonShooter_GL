#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texture_coordinate;
layout (location = 5) in uvec4 bone_ids;
layout (location = 6) in vec4 bone_weights;

uniform mat4 model;
uniform mat4 bone_mats[100];
uniform int has_bones;
uniform mat4 light_matrix;

void main()
{
	vec4 final_position = vec4(0);
	if (has_bones == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			final_position += bone_mats[bone_ids[i]] * vec4(position, 1.0f) * bone_weights[i];
		}
	}
	else
	{
		final_position = vec4(position, 1.0f);
		
	}

	gl_Position = light_matrix * model * final_position;
}
